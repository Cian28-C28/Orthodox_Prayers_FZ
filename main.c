#include <furi.h>
#include <gui/gui.h>
#include <gui/view_port.h>
#include <input/input.h>
#include <storage/storage.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <strings.h>

// C28_TXT_VWR – browse nested folders of plain text files on Flipper Zero.
//
// The application exposes a lightweight file browser rooted at APP_BASE_PATH.
// Users can create any directory hierarchy on the SD card and drop .txt files
// inside.  The browser view supports navigating into folders, jumping back to
// parent directories, and opening text files.  When a file is opened the view
// switches to a scrolling reader; pressing OK or BACK returns to the browser.

#define APP_BASE_PATH "/ext/apps_data/c28_txt_vwr"
#define MAX_BROWSER_ENTRIES 128
#define MAX_LABEL_LEN 64
#define MAX_PATH_LEN 256
#define BROWSER_VISIBLE_LINES 4
#define BROWSER_LINE_HEIGHT 10
#define BROWSER_START_Y 26
#define TEXT_VISIBLE_LINES 4
#define TEXT_LINE_HEIGHT 10
#define TEXT_LINE_MAX_CHARS 21
#define TEXT_MAX_LINES 512

typedef enum {
    BrowserEntryTypeParent,
    BrowserEntryTypeDirectory,
    BrowserEntryTypeFile,
    BrowserEntryTypeMessage,
} BrowserEntryType;

typedef struct {
    BrowserEntryType type;
    char label[MAX_LABEL_LEN];
    char path[MAX_PATH_LEN];
    char sort_key[MAX_LABEL_LEN];
} BrowserEntry;

typedef struct {
    InputEvent event;
} AppEvent;

static ViewPort* view_port;
static Gui* gui;
static FuriMessageQueue* event_queue;

static FuriString* current_path;
static FuriString* current_file_path;

static BrowserEntry browser_entries[MAX_BROWSER_ENTRIES];
static size_t browser_entry_count = 0;
static size_t browser_selection = 0;
static size_t browser_scroll = 0;

static bool showing_text = false;
static FuriString* text_buffer;
static const char* current_text;
static FuriString* text_lines[TEXT_MAX_LINES];
static size_t text_line_count = 0;
static size_t text_scroll_line = 0;

static bool running = true;

static void safe_copy(char* dest, size_t size, const char* src) {
    if(size == 0) return;
    if(!src) src = "";
    snprintf(dest, size, "%s", src);
}

static const char* relative_path(const char* full_path) {
    size_t base_len = strlen(APP_BASE_PATH);
    if(strncmp(full_path, APP_BASE_PATH, base_len) == 0) {
        const char* relative = full_path + base_len;
        if(*relative == '/') relative++;
        if(*relative == '\0') return "/";
        return relative;
    }
    return full_path;
}

static void compute_parent_path(const char* path, char* buffer, size_t buffer_size) {
    if(buffer_size == 0) return;
    safe_copy(buffer, buffer_size, path);
    size_t len = strlen(buffer);
    while(len > 0 && buffer[len - 1] == '/') {
        buffer[len - 1] = '\0';
        len--;
    }
    char* slash = strrchr(buffer, '/');
    if(slash) {
        if(slash == buffer) {
            slash[1] = '\0';
        } else {
            *slash = '\0';
        }
    }
    if(strlen(buffer) == 0) {
        safe_copy(buffer, buffer_size, APP_BASE_PATH);
    }
}

static bool has_txt_extension(const char* name) {
    size_t len = strlen(name);
    if(len < 4) return false;
    return strcasecmp(name + len - 4, ".txt") == 0;
}

static void free_text_lines(void) {
    for(size_t i = 0; i < text_line_count; i++) {
        if(text_lines[i]) {
            furi_string_free(text_lines[i]);
            text_lines[i] = NULL;
        }
    }
    text_line_count = 0;
    text_scroll_line = 0;
}

static bool add_text_line_segment(const char* start, size_t length) {
    if(text_line_count >= TEXT_MAX_LINES) {
        return false;
    }

    while(length > 0 && (*start == ' ' || *start == '\t')) {
        start++;
        length--;
    }

    while(length > 0 && (start[length - 1] == ' ' || start[length - 1] == '\t')) {
        length--;
    }

    FuriString* line = furi_string_alloc();
    if(length > 0) {
        furi_string_set_n(line, start, length);
    } else {
        furi_string_set(line, "");
    }
    text_lines[text_line_count++] = line;
    return true;
}

static void wrap_text_to_lines(const char* text) {
    free_text_lines();

    if(!text) {
        return;
    }

    const char* cursor = text;
    bool truncated = false;

    while(*cursor && text_line_count < TEXT_MAX_LINES) {
        if(*cursor == '\r') {
            cursor++;
            continue;
        }

        if(*cursor == '\n') {
            add_text_line_segment("", 0);
            cursor++;
            continue;
        }

        const char* line_start = cursor;
        size_t line_length = 0;
        int32_t last_space = -1;

        while(cursor[line_length] && cursor[line_length] != '\n' && line_length < TEXT_LINE_MAX_CHARS) {
            char c = cursor[line_length];
            if(c == '\r') {
                break;
            }
            if(c == ' ' || c == '\t') {
                last_space = (int32_t)line_length;
            }
            line_length++;
        }

        const char* after = cursor + line_length;
        size_t emit_length = line_length;

        if(cursor[line_length] && cursor[line_length] != '\n' && cursor[line_length] != '\r') {
            if(line_length >= TEXT_LINE_MAX_CHARS && last_space >= 0) {
                emit_length = (size_t)last_space;
                after = cursor + last_space + 1;
            }
        }

        if(emit_length == 0) {
            if(cursor[line_length] == '\n') {
                add_text_line_segment("", 0);
                cursor += line_length + 1;
                continue;
            } else if(cursor[line_length] == '\0') {
                add_text_line_segment("", 0);
                break;
            } else {
                emit_length = line_length;
            }
        }

        if(!add_text_line_segment(line_start, emit_length)) {
            truncated = true;
            break;
        }

        cursor = after;
        while(*cursor == ' ' || *cursor == '\t') {
            cursor++;
        }
        if(*cursor == '\r') {
            cursor++;
        }
        if(*cursor == '\n') {
            cursor++;
        }
    }

    if(text_line_count == 0) {
        add_text_line_segment("", 0);
    }

    if(truncated && text_line_count > 0) {
        furi_string_set(text_lines[text_line_count - 1], "(Text truncated)");
    }

    text_scroll_line = 0;
}

static bool ensure_directory_exists(Storage* storage, const char* path) {
    FileInfo info;
    if(storage_common_stat(storage, path, &info) == FSE_OK) {
        return (info.attributes & FSF_ATTR_DIR) != 0;
    }
    return storage_common_mkdir(storage, path) == FSE_OK;
}

static bool ensure_base_path(void) {
    Storage* storage = furi_record_open("storage");
    bool ok = ensure_directory_exists(storage, APP_BASE_PATH);
    furi_record_close("storage");
    return ok;
}

static int browser_entry_compare(const void* a, const void* b) {
    const BrowserEntry* ea = a;
    const BrowserEntry* eb = b;

    if(ea->type == BrowserEntryTypeParent && eb->type != BrowserEntryTypeParent) return -1;
    if(eb->type == BrowserEntryTypeParent && ea->type != BrowserEntryTypeParent) return 1;
    if(ea->type == BrowserEntryTypeMessage && eb->type != BrowserEntryTypeMessage) return 1;
    if(eb->type == BrowserEntryTypeMessage && ea->type != BrowserEntryTypeMessage) return -1;

    if(ea->type != eb->type) {
        return (ea->type == BrowserEntryTypeDirectory) ? -1 : 1;
    }

    return strcasecmp(ea->sort_key, eb->sort_key);
}

static void browser_set_message(const char* message) {
    browser_entry_count = 0;
    BrowserEntry* entry = &browser_entries[browser_entry_count++];
    entry->type = BrowserEntryTypeMessage;
    safe_copy(entry->label, sizeof(entry->label), message);
    entry->path[0] = '\0';
    entry->sort_key[0] = '\0';
    browser_selection = 0;
    browser_scroll = 0;
}

static void browser_refresh_entries(void) {
    browser_entry_count = 0;

    Storage* storage = furi_record_open("storage");
    Directory* dir = storage_dir_alloc(storage);
    const char* path = furi_string_get_cstr(current_path);

    if(!storage_dir_open(dir, path)) {
        if(!ensure_directory_exists(storage, path) || !storage_dir_open(dir, path)) {
            browser_set_message("Unable to open folder");
            storage_dir_free(dir);
            furi_record_close("storage");
            return;
        }
    }

    {
        if(strcmp(path, APP_BASE_PATH) != 0) {
            BrowserEntry* parent = &browser_entries[browser_entry_count++];
            parent->type = BrowserEntryTypeParent;
            safe_copy(parent->label, sizeof(parent->label), ".. (Up)");
            compute_parent_path(path, parent->path, sizeof(parent->path));
            safe_copy(parent->sort_key, sizeof(parent->sort_key), "..");
        }

        FileInfo file_info;
        char name_buffer[MAX_PATH_LEN];

        while(storage_dir_read(dir, &file_info, name_buffer, sizeof(name_buffer))) {
            if(name_buffer[0] == '.') continue;

            if(file_info.attributes & FSF_ATTR_DIR) {
                if(browser_entry_count >= MAX_BROWSER_ENTRIES) break;
                BrowserEntry* entry = &browser_entries[browser_entry_count++];
                entry->type = BrowserEntryTypeDirectory;
                snprintf(entry->label, sizeof(entry->label), "%s/", name_buffer);
                snprintf(entry->path, sizeof(entry->path), "%s/%s", path, name_buffer);
                safe_copy(entry->sort_key, sizeof(entry->sort_key), name_buffer);
            } else if(has_txt_extension(name_buffer)) {
                if(browser_entry_count >= MAX_BROWSER_ENTRIES) break;
                BrowserEntry* entry = &browser_entries[browser_entry_count++];
                entry->type = BrowserEntryTypeFile;
                safe_copy(entry->label, sizeof(entry->label), name_buffer);
                snprintf(entry->path, sizeof(entry->path), "%s/%s", path, name_buffer);
                safe_copy(entry->sort_key, sizeof(entry->sort_key), name_buffer);
            }
        }

        storage_dir_close(dir);

        if(browser_entry_count == 0) {
            if(strcmp(path, APP_BASE_PATH) == 0) {
                browser_set_message("Place TXT files here");
            } else {
                browser_set_message("(Empty directory)");
            }
        } else {
            qsort(browser_entries, browser_entry_count, sizeof(BrowserEntry), browser_entry_compare);
        }
    }

    storage_dir_free(dir);
    furi_record_close("storage");

    if(browser_selection >= browser_entry_count) {
        browser_selection = browser_entry_count ? browser_entry_count - 1 : 0;
    }
    if(browser_selection < browser_scroll) {
        browser_scroll = browser_selection;
    }
}

static void draw_browser(Canvas* canvas) {
    const char* full_path = furi_string_get_cstr(current_path);
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 0, 12, "C28_TXT_VWR");

    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str(canvas, 0, 22, relative_path(full_path));

    uint8_t start_y = BROWSER_START_Y;
    uint8_t line_height = BROWSER_LINE_HEIGHT;
    size_t visible_lines = BROWSER_VISIBLE_LINES;

    for(size_t i = 0; i < visible_lines; i++) {
        size_t index = browser_scroll + i;
        if(index >= browser_entry_count) break;
        const BrowserEntry* entry = &browser_entries[index];
        char line[MAX_LABEL_LEN + 4];
        snprintf(line, sizeof(line), "%c %s", index == browser_selection ? '>' : ' ', entry->label);
        canvas_draw_str(canvas, 0, start_y + (i * line_height), line);
    }

    if(browser_entry_count == 0) {
        canvas_draw_str(canvas, 0, start_y, "No items");
    }
}

static void draw_text(Canvas* canvas) {
    const char* file_path = current_file_path ? furi_string_get_cstr(current_file_path) : "";

    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str(canvas, 0, 12, relative_path(file_path));

    if(text_line_count > TEXT_VISIBLE_LINES) {
        char indicator[16];
        size_t current_line = text_scroll_line + 1;
        if(current_line > text_line_count) {
            current_line = text_line_count;
        }
        snprintf(indicator, sizeof(indicator), "%zu/%zu", current_line, text_line_count);
        canvas_draw_str_aligned(canvas, 124, 12, AlignRight, AlignTop, indicator);
    }

    uint8_t start_y = 26;

    if(text_line_count == 0) {
        canvas_draw_str(canvas, 0, start_y, "(Empty file)");
        return;
    }

    for(size_t i = 0; i < TEXT_VISIBLE_LINES; i++) {
        size_t index = text_scroll_line + i;
        if(index >= text_line_count) break;
        const char* line = furi_string_get_cstr(text_lines[index]);
        canvas_draw_str(canvas, 0, start_y + (i * TEXT_LINE_HEIGHT), line);
    }
}

static void draw_callback(Canvas* canvas, void* ctx) {
    UNUSED(ctx);
    canvas_clear(canvas);
    if(showing_text) {
        draw_text(canvas);
    } else {
        draw_browser(canvas);
    }
}

static void open_text_file(const char* path) {
    Storage* storage = furi_record_open("storage");
    File* file = storage_file_alloc(storage);

    if(!text_buffer) {
        text_buffer = furi_string_alloc();
    } else {
        furi_string_reset(text_buffer);
    }

    if(storage_file_open(file, path, FSAM_READ, FSOM_OPEN_EXISTING)) {
        char chunk[128];
        while(true) {
            uint16_t read = storage_file_read(file, chunk, sizeof(chunk) - 1);
            if(read == 0) break;
            chunk[read] = '\0';
            furi_string_cat(text_buffer, chunk);
        }
        storage_file_close(file);
    } else {
        furi_string_set(text_buffer, "Unable to open file.");
    }

    storage_file_free(file);
    furi_record_close("storage");

    if(!current_file_path) {
        current_file_path = furi_string_alloc();
    }
    furi_string_set(current_file_path, path);

    current_text = furi_string_get_cstr(text_buffer);
    wrap_text_to_lines(current_text);
    showing_text = true;

    view_port_update(view_port);
}

static void close_text_view(void) {
    showing_text = false;
    current_text = NULL;
    text_scroll_line = 0;
    free_text_lines();
    if(text_buffer) {
        furi_string_free(text_buffer);
        text_buffer = NULL;
    }
    browser_refresh_entries();
    view_port_update(view_port);
}

static void handle_browser_ok(void) {
    if(browser_entry_count == 0) return;
    BrowserEntry* entry = &browser_entries[browser_selection];
    switch(entry->type) {
    case BrowserEntryTypeDirectory:
        furi_string_set(current_path, entry->path);
        browser_selection = 0;
        browser_scroll = 0;
        browser_refresh_entries();
        view_port_update(view_port);
        break;
    case BrowserEntryTypeParent:
        furi_string_set(current_path, entry->path);
        browser_selection = 0;
        browser_scroll = 0;
        browser_refresh_entries();
        view_port_update(view_port);
        break;
    case BrowserEntryTypeFile:
        open_text_file(entry->path);
        break;
    case BrowserEntryTypeMessage:
    default:
        break;
    }
}

static void handle_browser_back(void) {
    const char* path = furi_string_get_cstr(current_path);
    if(strcmp(path, APP_BASE_PATH) == 0) {
        running = false;
    } else {
        char parent[MAX_PATH_LEN];
        compute_parent_path(path, parent, sizeof(parent));
        furi_string_set(current_path, parent);
        browser_selection = 0;
        browser_scroll = 0;
        browser_refresh_entries();
        view_port_update(view_port);
    }
}

static void handle_browser_navigation(InputKey key) {
    if(browser_entry_count == 0) return;
    size_t visible_lines = BROWSER_VISIBLE_LINES;
    size_t max_scroll = (browser_entry_count > visible_lines) ? browser_entry_count - visible_lines : 0;

    if(key == InputKeyUp) {
        if(browser_selection > 0) {
            browser_selection--;
            if(browser_selection < browser_scroll) {
                browser_scroll = browser_selection;
            }
            if(browser_scroll > max_scroll) {
                browser_scroll = max_scroll;
            }
            view_port_update(view_port);
        }
    } else if(key == InputKeyDown) {
        if(browser_selection + 1 < browser_entry_count) {
            browser_selection++;
            if(browser_selection >= browser_scroll + visible_lines) {
                browser_scroll = browser_selection - visible_lines + 1;
            }
            if(browser_scroll > max_scroll) {
                browser_scroll = max_scroll;
            }
            view_port_update(view_port);
        }
    }
}

static void process_event(const AppEvent* event) {
    if(
        event->event.type == InputTypePress || event->event.type == InputTypeRepeat ||
        event->event.type == InputTypeShort || event->event.type == InputTypeLong) {
        if(showing_text) {
            switch(event->event.key) {
            case InputKeyUp:
                if(text_scroll_line > 0) {
                    text_scroll_line--;
                    view_port_update(view_port);
                }
                break;
            case InputKeyDown:
                if(text_scroll_line + TEXT_VISIBLE_LINES < text_line_count) {
                    text_scroll_line++;
                    view_port_update(view_port);
                }
                break;
            case InputKeyBack:
            case InputKeyOk:
                close_text_view();
                break;
            default:
                break;
            }
        } else {
            switch(event->event.key) {
            case InputKeyUp:
            case InputKeyDown:
                handle_browser_navigation(event->event.key);
                break;
            case InputKeyOk:
                handle_browser_ok();
                break;
            case InputKeyBack:
                handle_browser_back();
                break;
            default:
                break;
            }
        }
    }
}

static void input_callback(InputEvent* input_event, void* ctx) {
    FuriMessageQueue* queue = ctx;
    AppEvent event = {.event = *input_event};
    furi_message_queue_put(queue, &event, 0);
}

int32_t c28_txt_vwr(void* p) {
    UNUSED(p);

    running = true;
    showing_text = false;
    text_scroll_line = 0;
    free_text_lines();
    if(text_buffer) {
        furi_string_free(text_buffer);
        text_buffer = NULL;
    }

    if(current_path) {
        furi_string_free(current_path);
    }
    current_path = furi_string_alloc_set(APP_BASE_PATH);
    browser_entry_count = 0;
    browser_selection = 0;
    browser_scroll = 0;
    current_text = NULL;

    gui = furi_record_open("gui");
    view_port = view_port_alloc();
    event_queue = furi_message_queue_alloc(8, sizeof(AppEvent));

    view_port_draw_callback_set(view_port, draw_callback, NULL);
    view_port_input_callback_set(view_port, input_callback, event_queue);

    if(!ensure_base_path()) {
        browser_set_message("SD card unavailable");
    } else {
        browser_refresh_entries();
    }

    gui_add_view_port(gui, view_port, GuiLayerFullscreen);
    view_port_update(view_port);

    while(running) {
        AppEvent event;
        if(furi_message_queue_get(event_queue, &event, 100) == FuriStatusOk) {
            process_event(&event);
        }
    }

    gui_remove_view_port(gui, view_port);

    furi_message_queue_free(event_queue);
    view_port_free(view_port);
    furi_record_close("gui");

    free_text_lines();
    if(text_buffer) {
        furi_string_free(text_buffer);
        text_buffer = NULL;
    }
    if(current_path) {
        furi_string_free(current_path);
        current_path = NULL;
    }
    if(current_file_path) {
        furi_string_free(current_file_path);
        current_file_path = NULL;
    }

    return 0;
}
