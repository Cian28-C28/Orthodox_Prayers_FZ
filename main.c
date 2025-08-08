#include <furi.h>
#include <gui/gui.h>
#include <gui/view_port.h>
#include <gui/menu.h>
#include <input/input.h>
#include <storage/storage.h>
#include <stdio.h>

/*
 * Orthodox Prayers – simple text reader for Flipper Zero
 * ------------------------------------------------------
 * This example demonstrates a minimal application that loads
 * plain text files and displays them inside a scrolling view.
 *
 * Layout overview
 * 1.  Main menu lists high level categories (morning/evening/etc).
 * 2.  Selecting a category opens a sub‑menu with individual prayers.
 * 3.  When a prayer is chosen the corresponding .txt file is read
 *     from the SD card (see APP_BASE_PATH below) and the text is
 *     drawn on screen.  UP/DOWN scroll, BACK returns to the menu.
 *
 * All code is heavily commented so new developers can understand
 * the structure of a typical Flipper Zero application.
 */

// ---------------------------------------------------------------------
// Location of prayer text files on the device
// During build every file inside the `assets` directory of the project
// is copied to `/ext/apps_data/<appid>/` on the SD card.  The app id is
// defined in application.fam as `orthodox_prayers`.
// ---------------------------------------------------------------------
#define APP_BASE_PATH "/ext/apps_data/orthodox_prayers"

// ---------------------------------------------------------------------
// Data structures describing prayers and categories
// ---------------------------------------------------------------------
typedef struct {
    const char* name;     // Display name shown in the menu
    const char* file;     // Relative path to the text file
} PrayerEntry;

typedef struct {
    const char* name;             // Category title
    const PrayerEntry* prayers;   // Array of prayers inside the category
    size_t count;                 // Number of elements in the array
} PrayerCategory;

// ---------------------------------------------------------------------
// Prayer lists for each category.  Only a few examples are provided but
// adding more prayers is as simple as dropping a text file into the
// assets folder and listing it here.
// ---------------------------------------------------------------------
static const PrayerEntry morning_prayers[] = {
    {"Morning Prayer", "morning/opening.txt"},
};

static const PrayerEntry evening_prayers[] = {
    {"Evening Prayer", "evening/thanksgiving.txt"},
};

static const PrayerEntry occasional_prayers[] = {
    {"Prayer for Travelers", "occasional/travel.txt"},
};

static const PrayerCategory categories[] = {
    {"Morning Prayers", morning_prayers, sizeof(morning_prayers) / sizeof(PrayerEntry)},
    {"Evening Prayers", evening_prayers, sizeof(evening_prayers) / sizeof(PrayerEntry)},
    {"Occasional Prayers", occasional_prayers, sizeof(occasional_prayers) / sizeof(PrayerEntry)},
};

// ---------------------------------------------------------------------
// Global UI state
// ---------------------------------------------------------------------
static ViewPort* view_port;          // Area where everything is drawn
static bool show_text = false;       // Are we currently showing a prayer?
static FuriString* prayer_buffer;    // Holds the loaded text
static const char* current_text;     // Pointer inside `prayer_buffer`
static int16_t scroll_y = 0;         // Vertical scroll offset in pixels

// ---------------------------------------------------------------------
// Draw callback – called by the GUI system when a redraw is required.
// ---------------------------------------------------------------------
static void draw_callback(Canvas* canvas, void* ctx) {
    canvas_clear(canvas);
    canvas_set_font(canvas, FontPrimary);

    if(show_text && current_text) {
        // Draw the loaded text starting at negative scroll offset.
        canvas_draw_text(canvas, 0, -scroll_y, current_text);
    } else {
        // Help screen shown when no prayer is selected.
        canvas_draw_str(canvas, 0, 10, "Use menu to choose");
        canvas_draw_str(canvas, 0, 24, "BACK to return");
    }
}

// ---------------------------------------------------------------------
// Input callback – handles button presses while displaying a prayer.
// ---------------------------------------------------------------------
static void input_callback(InputEvent* event, void* ctx) {
    if(!show_text) return; // Menu handles input when not in text mode

    if(event->type == InputTypePress) {
        switch(event->key) {
        case InputKeyUp:
            if(scroll_y > 0) scroll_y -= 10; // Scroll text up
            furi_hal_os_view_port_send_view_invalidate(view_port);
            break;
        case InputKeyDown:
            scroll_y += 10; // Scroll text down
            furi_hal_os_view_port_send_view_invalidate(view_port);
            break;
        case InputKeyBack:
        case InputKeyOk:
            // Exit the prayer view and free the loaded text
            show_text = false;
            scroll_y = 0;
            current_text = NULL;
            if(prayer_buffer) {
                furi_string_free(prayer_buffer);
                prayer_buffer = NULL;
            }
            furi_hal_os_view_port_send_view_invalidate(view_port);
            break;
        default:
            break;
        }
    }
}

// ---------------------------------------------------------------------
// Helper: read a text file into `prayer_buffer` and display it.
// ---------------------------------------------------------------------
static void show_prayer_from_file(const char* file_name) {
    // Build the absolute path to the text file
    char path[256];
    snprintf(path, sizeof(path), "%s/%s", APP_BASE_PATH, file_name);

    // Prepare storage objects and buffer
    Storage* storage = furi_record_open("storage");
    File* file = storage_file_alloc(storage);
    prayer_buffer = furi_string_alloc();

    if(storage_file_open(file, path, FSAM_READ, FSOM_OPEN_EXISTING)) {
        // Read the file in chunks and append to the string
        char chunk[128];
        while(true) {
            uint16_t read = storage_file_read(file, chunk, sizeof(chunk) - 1);
            if(read == 0) break; // EOF
            chunk[read] = '\0';
            furi_string_cat(prayer_buffer, chunk);
        }
        storage_file_close(file);
    } else {
        // In case of error show a simple message
        furi_string_set(prayer_buffer, "Unable to open file.");
    }

    // Clean up storage objects
    storage_file_free(file);
    furi_record_close("storage");

    // Update global pointers so draw_callback can use the new text
    current_text = furi_string_get_cstr(prayer_buffer);
    show_text = true;
    scroll_y = 0;
    furi_hal_os_view_port_send_view_invalidate(view_port);
}

// ---------------------------------------------------------------------
// Menu callbacks
// ---------------------------------------------------------------------
static void prayer_menu_callback(Menu* menu, MenuItem* item) {
    // `menu_item_get_user_data` contains the file path provided when
    // constructing the menu.  We simply pass it to the loader function.
    const char* file = menu_item_get_user_data(item);
    show_prayer_from_file(file);
}

static void category_menu_callback(Menu* menu, MenuItem* item) {
    // Retrieve index of the selected category and build a child menu.
    uint32_t index = (uint32_t)menu_item_get_user_data(item);
    const PrayerCategory* cat = &categories[index];

    Menu* child = menu_alloc();
    menu_set_title(child, cat->name);

    for(size_t i = 0; i < cat->count; i++) {
        menu_add_item(
            child,
            cat->prayers[i].name,
            NULL,
            prayer_menu_callback,
            (void*)cat->prayers[i].file);
    }

    view_port_remove_menu(view_port);
    view_port_attach_menu(view_port, child);
    menu_free(child); // Menu is copied into the view port; local copy can go
}

// ---------------------------------------------------------------------
// Entry point – called by the firmware when the application starts.
// ---------------------------------------------------------------------
int32_t fap_orthodox_prayers(void* p) {
    Gui* gui = furi_record_open("gui");
    view_port = view_port_alloc();

    // Register our draw and input handlers
    view_port_draw_callback_set(view_port, draw_callback, NULL);
    view_port_input_callback_set(view_port, input_callback, NULL);

    // Build the root menu listing all categories
    Menu* root = menu_alloc();
    menu_set_title(root, "Orthodox Prayers");
    for(size_t i = 0; i < sizeof(categories) / sizeof(categories[0]); i++) {
        menu_add_item(
            root,
            categories[i].name,
            NULL,
            category_menu_callback,
            (void*)i);
    }

    // Attach the menu to the viewport and display it
    view_port_attach_menu(view_port, root);
    gui_add_view_port(gui, view_port, GuiLayerFullscreen);
    menu_show(root); // Blocks until the user backs out completely

    // Cleanup
    gui_remove_view_port(gui, view_port);
    view_port_free(view_port);
    menu_free(root);
    furi_record_close("gui");
    return 0;
}

