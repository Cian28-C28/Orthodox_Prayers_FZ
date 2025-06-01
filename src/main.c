#include <furi.h>
#include <gui/gui.h>
#include <gui/view_port.h>
#include <gui/menu.h>
#include <input/input.h>

// ─────────────────────────────
// STEP 1: Define your prayer data
// ─────────────────────────────

// Top‐level categories
static const char* prayer_categories[] = {
    "Symbol of Faith",
    "Morning Prayers",
    "Prayers At Mealtime",
    "Before Sleep",
    "The Six Psalms",
    "Small Compline",
    "Holy Communion Prep",
    "Thanksgiving After Communion"
};

// Sub‐menu items for each category
static const char* symbol_of_faith_list[] = {
    "The Symbol of Faith"
};

static const char* morning_prayers_list[] = {
    "Morning Prayers (Opening)",
    "Prayer of Saint Basil (Daily)",
    "Another Prayer (Saturday)",
    "Prayer of Saint Eustratios (Saturday)"
};

static const char* mealtime_prayers_list[] = {
    "Before Breakfast",
    "Thanksgiving After Breakfast",
    "Before Lunch",
    "Thanksgiving After Lunch",
    "Before Dinner",
    "Thanksgiving After Dinner",
    "Another Mealtime Prayer"
};

static const char* bedtime_prayers_list[] = {
    "Before Sleep (General)",
    "A Prayer For Children",
    "A Prayer For Adults",
    "I Believe in One God (Bedtime Creed)",
    "At All Times (Bedtime)"
};

static const char* six_psalms_list[] = {
    "Opening Prayers (Six Psalms)",
    "Psalm 3",
    "Psalm 37 (38)",
    "Psalm 62 (63)",
    "Psalm 87 (88)",
    "Psalm 102 (103)",
    "Psalm 142 (143)"
};

static const char* small_compline_list[] = {
    "Opening (Small Compline)",
    "Psalm 50 (51)",
    "Psalm 69 (70)",
    "Psalm 142 (143)",
    "Doxology"
};

static const char* communion_prep_list[] = {
    "Opening (Prep for Communion)",
    "Psalm 50 (51)",
    "Psalm 69 (70)",
    "Psalm 142 (143)",
    "Doxology"
};

static const char* thanksgiving_comm_list[] = {
    "Opening Thanksgiving",
    "Anonymous #1",
    "Prayer of Saint Basil",
    "Prayer of Symeon Metaphrastes",
    "Anonymous #2",
    "All holy Lady, Theotokos"
};

// Now for each specific prayer, include the full text as a C string.
// Replace any internal double‐quotes with \" if needed. Newlines use \n.

// Symbol of Faith
static const char* text_symbol_of_faith =
    "I believe in one God,\n"
    "Father Almighty,\n"
    "Creator of heaven and earth, \n"
    "and of all things visible and invisible.\n"
    "\n"
    "And in one Lord Jesus Christ,\n"
    "the only-begotten Son of God,\n"
    "begotten of the Father\n"
    "before all ages;\n"
    "\n"
    "Light of Light,\n"
    "true God of true God,\n"
    "begotten, not created,\n"
    "of one essence with the Father\n"
    "through Whom all things were made.\n"
    "\n"
    "Who for us men\n"
    "and for our salvation\n"
    "came down from heaven\n"
    "and was incarnate of the Holy Spirit\n"
    "and the Virgin Mary\n"
    "and became man.\n"
    "\n"
    "He was crucified for us\n"
    "under Pontius Pilate,\n"
    "and he suffered and was buried; \n"
    "\n"
    "And he rose on the third day,\n"
    "according to the Scriptures.\n"
    "\n"
    "He ascended into heaven\n"
    "and is seated at the right hand of the Father;\n"
    "\n"
    "And He will come again with glory\n"
    "to judge the living and dead.\n"
    "His kingdom will have no end.\n"
    "\n"
    "And in the Holy Spirit,\n"
    "the Lord, the Creator of life,\n"
    "Who proceeds from the Father,\n"
    "Who together with the Father and the Son\n"
    "is worshiped and glorified,\n"
    "who spoke through the prophets. \n"
    "\n"
    "In one, holy, catholic,\n"
    "and apostolic Church.\n"
    "\n"
    "I confess one baptism\n"
    "for the forgiveness of sins.\n"
    "\n"
    "I look for the resurrection of the dead.\n"
    "and the life of the age to come. Amen.\n";

// Morning Prayers – Opening
static const char* text_morning_opening =
    "After rising from sleep, stand with reverence and fear of God, make the sign of the cross, and say:\n"
    "\n"
    "In the name of the Father and the Son and the Holy Spirit. Amen.\n"
    "\n"
    "Glory to you, our God, glory to you.\n"
    "\n"
    "Heavenly King, Comforter, the Spirit of truth, who are present everywhere filling all things, Treasury of good things and Giver of life, come and dwell in us. Cleanse us of every stain, and save our souls, gracious Lord.\n"
    "\n"
    "Holy God, Holy Mighty, Holy Immortal, have mercy on us (3).\n"
    "\n"
    "Glory to the Father and the Son and the Holy Spirit, now and forever and to the ages of ages. Amen.\n"
    "\n"
    "All holy Trinity, have mercy on us. Lord, forgive our sins. Master, pardon our transgressions. Holy One, visit and heal our infirmities for your name’s sake.\n"
    "\n"
    "Lord, have mercy. Lord, have mercy. Lord, have mercy.\n"
    "\n"
    "Glory to the Father and the Son and the Holy Spirit, now and forever and to the ages of ages. Amen.\n"
    "\n"
    "Our Father,\n"
    "who are in heaven,\n"
    "hallowed be your name.\n"
    "Your kingdom come,\n"
    "your will be done,\n"
    "on earth as it is in heaven.\n"
    "Give us this day our daily bread;\n"
    "and forgive us our trespasses,\n"
    "as we forgive those who trespass against us.\n"
    "And lead us not into temptation,\n"
    "but deliver us from evil.\n"
    "\n"
    "For yours is the kingdom and the power and the glory,\n"
    "of the Father and the Son and the Holy Spirit,\n"
    "now and forever and to the ages of ages. Amen.\n"
    "\n"
    "Then the following prayers to the Holy Trinity:\n"
    "\n"
    "Having risen from sleep, we fall before you, O good One, and sing to you, mighty One, the angelic hymn: Holy, holy, holy are you, O God. Through the prayers of the Theotokos, have mercy on us.\n"
    "\n"
    "Glory to the Father and the Son and the Holy Spirit.\n"
    "\n"
    "Having aroused me from sleep and bed, O Lord, enlighten my mind and open my heart and lips that I may sing to you, Holy Trinity: Holy, holy, holy are you, O God. Through the Theotokos, have mercy on us.\n"
    "\n"
    "Now and forever and to the ages of ages. Amen.\n"
    "\n"
    "Suddenly the Judge will come and everyone’s deeds will be revealed. But with awe we cry out in the middle of the night: Holy, holy, holy are you, O God. Through the prayers of the Theotokos, have mercy on us.\n"
    "\n"
    "Lord, have mercy (12).\n"
    "\n"
    "Psalm 120 (121)\n"
    "\n"
    "I will lift, up my eyes to the hills. From where comes my help? My help comes from the Lord, who made heaven and earth. He will not allow your foot to be moved; he who keeps you will not slumber. Behold, he who watches over Israel shall neither slumber nor sleep. The Lord shall protect you; the Lord is your shelter at your right hand. The sun shall not burn you by day, nor the moon by night. The Lord shall preserve you from all evil; the Lord shall preserve your soul. The Lord shall preserve your going out and your coming in from this time forth, and forevermore.\n"
    "\n"
    "A Prayer Of Thanksgiving\n"
    "\n"
    "As I rise from sleep, I thank you, Holy Trinity, for because of your great goodness and patience, you were not angry with me, an idler and sinner, nor have you destroyed me in my sins, but have shown your usual love for me. And when I was prostrate in despair, you raised me to glorify with your power. Enlighten now my mind’s eye, open my mouth to study your words and understand your commandments, to do your will and sing to you in heartfelt adoration, and praise your most holy name, of the Father and the Son and the Holy Spirit, now and forever and to the ages of ages. Amen.\n"
    "\n"
    "Another Prayer\n"
    "\n"
    "Glory to you, King, God almighty, who through your divine and loving providence have consented that I, an unworthy sinner, should rise from sleep and obtain entrance into your holy house. Accept, Lord, the voice of my prayer as you accept those of your holy and spiritual powers; and that not through my defiled lips, but from a pure heart and humble spirit, praise may be offered to you so that I also, with the bright lamp of my soul, may become a companion of the wise virgins and glorify you, God the Word, who is glorified in the Father and the Spirit. Amen.\n"
    "\n"
    "Glory to the Father and the Son and the Holy Spirit, now and forever and to ages of ages. Amen.\n"
    "\n"
    "I believe in one God, \n"
    "Father Almighty, \n"
    "Creator of heaven and earth, \n"
    "and of all things visible and invisible.\n"
    "\n"
    "And in one Lord Jesus Christ, \n"
    "the only-begotten Son of God, \n"
    "begotten of the Father \n"
    "before all ages;\n"
    "\n"
    "Light of Light, \n"
    "true God of true God, \n"
    "begotten, not created, \n"
    "of one essence with the Father \n"
    "through Whom all things were made. \n"
    "\n"
    "Who for us men\n"
    "and for our salvation \n"
    "came down from heaven \n"
    "and was incarnate of the Holy Spirit \n"
    "and the Virgin Mary \n"
    "and became man.\n"
    "\n"
    "He was crucified for us \n"
    "under Pontius Pilate, \n"
    "and he suffered and was buried; \n"
    "\n"
    "And he rose on the third day,\n"
    "according to the Scriptures. \n"
    "\n"
    "He ascended into heaven \n"
    "and is seated at the right hand of the Father;\n"
    "\n"
    "And He will come again with glory \n"
    "to judge the living and dead. \n"
    "His kingdom will have no end.\n"
    "\n"
    "And in the Holy Spirit, \n"
    "the Lord, the Creator of life, \n"
    "Who proceeds from the Father, \n"
    "Who together with the Father and the Son \n"
    "is worshiped and glorified, \n"
    "who spoke through the prophets. \n"
    "\n"
    "In one, holy, catholic, \n"
    "and apostolic Church. \n"
    "\n"
    "I confess one baptism \n"
    "for the forgiveness of sins. \n"
    "\n"
    "I look for the resurrection of the dead. \n"
    "and the life of the age to come. Amen.\n";

// Prayer of Saint Basil (Daily)
static const char* text_prayer_basil_daily =
    "Prayer of Saint Basil\n"
    "\n"
    "Read daily except on Saturday\n"
    "\n"
    "We bless you, O God, most high and Lord of mercy. You are always doing great and inscrutable things with us, glorious and wonderful, and without number. You grant us sleep for rest from our infirmities, and repose from the burdens of our much toiling flesh. We thank you, for you have not destroyed us with our sins, but have continued to love us; and though we were sunk in despair, you have raised us up to glorify your power. Therefore, we implore your incomparable goodness. Enlighten the eyes of our understanding and raise up our minds from the heavy sleep of indolence. Open our mouth and fill it with your praise, that we may be able without distraction to sing and confess that you are God, glorified in all and by all, the eternal Father, with your only begotten Son, and your all holy, good, and life giving Spirit, now and forever and to the ages of ages. Amen.\n";

// Another Prayer (Saturday)
static const char* text_prayer_basil_saturday =
    "Another Prayer\n"
    "\n"
    "Read on Saturday\n"
    "\n"
    "Remember, Lord, our fathers and brethren who have fallen asleep in the hope of the resurrection to eternal life, and all those who ended this life in piety and faith. Pardon their every transgression, committed voluntarily or involuntarily, in word, or deed, or thought. Place them in a place of light, a place of refreshment, a place of peace from which every ailment, sorrow, and lamentation are banished, and from which the light of your countenance shines and gladdens all your saints from all ages. Bestow on them and on us your kingdom. Grant to them the participation of your ineffable and everlasting blessings, and the enjoyment of your endless and blissful life. For you are the life, the resurrection, and the peace of your departed servants, Christ our God, and to you we give glory, together with your all holy, good, and life giving Spirit, now and forever and to the ages of ages. Amen.\n";

// Prayer Of Saint Eustratios (Saturday)
static const char* text_prayer_eustratios =
    "Prayer Of Saint Eustratios\n"
    "\n"
    "Read on Saturday\n"
    "\n"
    "Most highly do I praise you, Lord, for you have taken notice of my lowliness and have not delivered me into the hands of my enemies but have relieved my soul of its needs. Now, Master, let your hand shelter me, and let your grace descend upon me, for my soul is distracted and pained at the departure from this, my miserable and sordid body, that the end design of the adversary may not overtake her and make her stumble into the darkness for the unknown and known sins amassed by me in this life. Be gracious to me, Master, and do not let my soul see the dark countenance of the evil spirits, but let her be received by your angels bright and shining. Glorify your holy name, and by your might set me before your divine judgment seat. When I am being judged, do not allow the hand of the prince of this world to take hold of me, to throw me, a sinner, into the depths of hell, but stand by me and be a savior and mediator to me. Have mercy, Lord, on my soul, defiled through the passions of this life, and receive her cleansed by penitence and confession, for you are blessed to the ages of ages. Amen.\n";

// Prayers At Mealtime – Before Breakfast
static const char* text_before_breakfast =
    "Prayers At Mealtime\n"
    "\n"
    "Before Breakfast\n"
    "\n"
    "Glory to the Father and the Son and the Holy Spirit, now and forever and to the ages of ages. Amen.\n"
    "Lord, have mercy. Lord, have mercy. Lord, have mercy.\n"
    "Our Father, who are in heaven, hallowed be your name. Your kingdom come, your will be done, on earth as it is in heaven. Give us this day our daily bread; and forgive us our trespasses, as we forgive those who trespass against us. And lead us not into temptation, but deliver us from evil. For yours is the kingdom and the power and the glory, of the Father and the Son and the Holy Spirit, now and forever and to the ages of ages. Amen.\n"
    "Christ our God, bless the food and drink of your servants, for you are holy always, now and forever and to the ages of ages. Amen.\n";

// Prayers At Mealtime – Thanksgiving After Breakfast
static const char* text_after_breakfast =
    "Thanksgiving After Breakfast\n"
    "\n"
    "We thank you, Christ our God, for you have satisfied us with earthly gifts. Do not deprive us of your heavenly kingdom, but as you, O Savior, came among your disciples and gave them peace, come among us also and save us.\n";

// Prayers At Mealtime – Before Lunch
static const char* text_before_lunch =
    "Before Lunch\n"
    "\n"
    "Glory to the Father and the Son and the Holy Spirit, now and forever and to the ages of ages. Amen.\n"
    "Lord, have mercy. Lord, have mercy. Lord, have mercy.\n"
    "Our Father, who are in heaven, hallowed be your name. Your kingdom come, your will be done, on earth as it is in heaven. Give us this day our daily bread; and forgive us our trespasses, as we forgive those who trespass against us. And lead us not into temptation, but deliver us from evil. For yours is the kingdom and the power and the glory, of the Father and the Son and the Holy Spirit, now and forever and to the ages of ages. Amen.\n"
    "Christ our God, bless the food and drink of your servants, for you are holy always, now and forever and to the ages of ages. Amen.\n";

// Prayers At Mealtime – Thanksgiving After Lunch
static const char* text_after_lunch =
    "Thanksgiving After Lunch\n"
    "\n"
    "We thank you, Christ our God, for you have satisfied us with earthly gifts. Do not deprive us of your heavenly kingdom, but as you, O Savior, came among your disciples and gave them peace, come among us also and save us.\n";

// Prayers At Mealtime – Before Dinner
static const char* text_before_dinner =
    "Prayers Before Dinner\n"
    "\n"
    "The poor shall eat and be satisfied, and they who seek the Lord shall praise him; their hearts shall live to the ages of ages.\n"
    "Glory to the Father and the Son and the Holy Spirit, now and forever and to the ages of ages. Amen.\n"
    "Lord, have mercy. Lord, have mercy. Lord, have mercy.\n"
    "Christ our God, bless the food and drink of your servants, for you are holy always, now and forever and to the ages of ages. Amen.\n";

// Prayers At Mealtime – Thanksgiving After Dinner
static const char* text_after_dinner =
    "Thanksgiving After Dinner\n"
    "\n"
    "Glory to the Father and to the Son and to the Holy Spirit, now and forever and to the ages of ages. Amen.\n"
    "Lord, have mercy. Lord, have mercy. Lord, have mercy.\n"
    "Lord, you have gladdened our hearts in your creation, and we have rejoiced in the work of your hands. The light of your countenance has shined upon us, Lord. You have gladdened our hearts. We have been satisfied with the good things of the earth. We shall sleep in peace and repose in you, for you alone, Lord, have sustained us in hope.\n";

// Prayers At Mealtime – Another Mealtime Prayer
static const char* text_another_mealtime =
    "Another Prayer\n"
    "\n"
    "Blessed is God, who has mercy upon us and nourishes us from his bountiful gifts by his grace and love always, now and forever and to the ages of ages. Amen.\n";

// Before Sleep – General
static const char* text_before_sleep_general =
    "Prayers Before Sleep\n"
    "\n"
    "In the name of the Father and the Son and the Holy Spirit. Amen.\n"
    "\n"
    "Glory to you, our God, glory to you.\n"
    "Heavenly King, Comforter, the Spirit of truth, who are present everywhere filling all things, Treasury of good things and Giver of life, come and dwell in us. Cleanse us of every stain, and save our souls, gracious Lord.\n"
    "\n"
    "Holy God, Holy and Mighty, Holy Immortal, have mercy on us (3).\n"
    "\n"
    "Glory to the Father and the Son and the Holy Spirit, now and forever and to the ages of ages. Amen.\n"
    "\n"
    "All holy Trinity, have mercy on us. Lord, forgive our sins. Master, pardon our transgressions. Holy One, visit and heal our infirmities for your name’s sake.\n"
    "\n"
    "Lord, have mercy. Lord, have mercy. Lord, have mercy.\n"
    "\n"
    "Glory to the Father and the Son and the Holy Spirit, now and forever and to the ages of ages. Amen.\n"
    "\n"
    "Our Father, who are in heaven, hallowed be your name. Your kingdom come, your will be done, on earth as it is in heaven. Give us this day our daily bread; and forgive us our trespasses, as we forgive those who trespass against us. And lead us not into temptation, but deliver us from evil. For yours is the kingdom and the power and the glory, of the Father and the Son and the Holy Spirit, now and forever and to the ages of ages. Amen.\n"
    "\n"
    "Now that the day has come to a close, I thank you, Lord, and entreat that the evening and the night be sinless. Grant this to me, Savior, and save me. Glory to the Father and the Son and the Holy Spirit. Now that the day has passed, I glorify you, Master, and I entreat that the evening with the night be without offense. Grant this to me, Savior, and save me. Now and forever and to the ages of ages. Amen.\n"
    "\n"
    "Now that the day has ended, I praise you, Holy One, and entreat that the evening and the night be undisturbed. Grant this to me, Savior, and save me.\n"
    "\n"
    "Lord, have mercy (12).\n";

// Before Sleep – A Prayer For Children
static const char* text_prayer_children =
    "A Prayer For Children\n"
    "\n"
    "Lord Jesus Christ, you received the children who came to you, receive also from the lips of your child this evening prayer. Shelter me under the protection of your wings that I may lie down in peace and sleep. Awaken me in due time that I may glorify you, for you alone are good and love all people.\n";

// Before Sleep – A Prayer For Adults
static const char* text_prayer_adults =
    "A Prayer For Adults\n"
    "\n"
    "Lord our God, whatever sins I have committed this day, in word, deed or thought, forgive me, for you are good and love all people. Grant me a peaceful and undisturbed sleep. Protect me from every abuse and plot of the evil one. Raise me up in due time that I may glorify you, for you are blessed, together with your only begotten Son, and your all holy Spirit, now and forever and to the ages of ages. Amen.\n"
    "\n"
    "I believe in one God, the Father, the Almighty, Creator of heaven and earth and of all things visible and invisible. And in one Lord Jesus Christ, the only begotten Son of God, begotten of the Father before all ages. Light of Light, true God of true God, begotten not created, of one essence with the Father through whom all things were made. For us and for our salvation he came down from heaven and was incarnate by the Holy Spirit and the Virgin Mary and became man. He was crucified for us under Pontius Pilate, and he suffered and was buried. On the third day he rose according to the Scriptures. He ascended into heaven and is seated at the right hand of the Father. He will come again in glory to judge the living and dead. His kingdom will have no end. And in the Holy Spirit, the Lord, the Giver of life, who proceeds from the Father, who together with the Father and the Son is worshiped and glorified, who spoke through the prophets. In one, holy, catholic, and apostolic Church. I acknowledge one baptism for the forgiveness of sins. I expect the resurrection of the dead. And the life of the ages to come. Amen.\n"
    "\n"
    "At all times and in every hour, you are worshiped and glorified in heaven and on earth, Christ our God. Long in patience, great in mercy and compassion, you love the righteous and show mercy to sinners. You call all to salvation through the promise of good things to come. Lord, receive our prayers at the present time. Direct our lives according to your commandments. Sanctify our souls. Purify our bodies. Set our minds aright. Cleanse our thoughts and deliver us from all sorrow, evil, and distress. Surround us with your holy angels that, guarded and guided by their host, we may arrive at the unity of faith and the understanding of your ineffable glory. For you are blessed to the ages of ages. Amen.\n";

// Before Sleep – I Believe in One God (Bedtime Creed)
static const char* text_believe_one_god_bedtime =
    "I believe in one God, \n"
    "the Father, the Almighty, \n"
    "Creator of heaven and earth, \n"
    "and of all things visible and invisible. \n"
    "\n"
    "And in one Lord Jesus Christ, \n"
    "the only begotten Son of God, \n"
    "begotten of the Father before all ages; \n"
    "\n"
    "Light of Light, true God of true God, \n"
    "begotten, not created, of one essence with the Father through whom all things were made. \n"
    "\n"
    "Who for us men and for our salvation came down from heaven and was incarnate of the Holy Spirit and the Virgin Mary and became man. \n"
    "\n"
    "He was crucified for us under Pontius Pilate, and he suffered and was buried. \n"
    "\n"
    "And he rose on the third day, according to the Scriptures. He ascended into heaven and is seated at the right hand of the Father. \n"
    "\n"
    "And He will come again with glory to judge the living and dead. His kingdom will have no end. \n"
    "\n"
    "And in the Holy Spirit, the Lord, the Creator of life, Who proceeds from the Father, Who together with the Father and the Son is worshiped and glorified, who spoke through the prophets. \n"
    "\n"
    "In one, holy, catholic, and apostolic Church. I confess one baptism for the forgiveness of sins. I look for the resurrection of the dead and the life of the age to come. Amen.\n";

// Before Sleep – At All Times (Bedtime)
static const char* text_at_all_times_bedtime =
    "At all times and in every hour, you are worshiped and glorified in heaven and on earth, Christ our God. Long in patience, great in mercy and compassion, you love the righteous and show mercy to sinners. You call all to salvation through the promise of good things to come. Lord, receive our prayers at the present time. Direct our lives according to your commandments. Sanctify our souls. Purify our bodies. Set our minds aright. Cleanse our thoughts and deliver us from all sorrow, evil, and distress. Surround us with your holy angels that, guarded and guided by their host, we may arrive at the unity of faith and the understanding of your ineffable glory. For you are blessed to the ages of ages. Amen.\n";

// The Six Psalms – Opening
static const char* text_six_opening =
    "Service Of The Six Psalms\n"
    "\n"
    "Holy God, Holy Mighty, Holy Immortal, have mercy on us (3).\n"
    "Glory to the Father and the Son and the Holy Spirit, now and forever and to the ages of ages. Amen.\n"
    "\n"
    "All holy Trinity, have mercy on us. Lord, forgive our sins. Master, pardon our transgressions. Holy One, visit and heal our infirmities for your name’s sake. Glory to the Father and the Son and the Holy Spirit, now and forever and to the ages of ages. Amen.\n"
    "\n"
    "Our Father, who are in heaven, hallowed be your name. Your kingdom come, your will be done, on earth as it is in heaven. Give us this day our daily bread; and forgive us our trespasses, as we forgive those who trespass against us. And lead us not into temptation, but deliver us from evil. For yours is the kingdom and the power and the glory, of the Father and the Son and the Holy Spirit, now and forever and to the ages of ages. Amen.\n"
    "\n"
    "Lord, save your people and bless your inheritance. Grant victory to the Orthodox over their adversaries and guard your commonwealth with your cross. Glory to the Father and the Son and the Holy Spirit. You were lifted up upon the cross of your own will, Christ our God. Grant your mercy upon the new commonwealth that bears your name. In your strength make glad the Orthodox, giving them victory over their adversaries. May they have your alliance as a weapon of peace, and an invincible trophy. Now and forever and to the ages of ages. Amen.\n"
    "\n"
    "O awesome and ever present protection, do not overlook, O gracious one, our supplications. Most praised Theotokos, establish the Orthodox people, save those whom you have called to govern and grant them victory from above, for you have given birth to God, only blessed one.\n"
    "\n"
    "Lord, have mercy. Lord, have mercy. Lord, have mercy.\n"
    "\n"
    "Glory to God in the highest, and on earth peace, good will among all people (3).\n"
    "\n"
    "Lord, open my lips and my mouth shall proclaim your praise (3).\n";

// The Six Psalms – Psalm 3
static const char* text_psalm_3 =
    "Psalm 3\n"
    "\n"
    "Lord, why have they multiplied who afflict me? Many rise up against me. Many say to my soul, there is no salvation for him in his God. But you, Lord, are my helper, my glory, and the one who lifts up my head. I cried to the Lord with my voice, and he heard me from his holy mountain. I lay down and slept. I awoke, for the Lord will help me. I will not be afraid of ten thousands of people who have set themselves against me all around. Arise, Lord, save me, my God. For you have struck all those who without cause are my enemies; you have broken the teeth of sinners. Salvation belongs to the Lord, and your blessing is upon your people.\n";

// The Six Psalms – Psalm 37 (38)
static const char* text_psalm_37 =
    "Psalm 37 (38)\n"
    "\n"
    "Lord, rebuke me not in your anger, nor chasten me in your wrath. For your arrows pierce me deeply, and your hand presses me down. There is no health in my flesh because of your anger, nor is there any health in my bones because of my sins. For my iniquities have risen higher than my head; like a heavy burden they have pressed heavily upon me. My wounds are foul and festering because of my foolishness. I am troubled and bowed down greatly; I go mourning all the day long. For my loins are filled with mockings, and there is no healing in my flesh. I am afflicted and exceedingly humbled; I have roared from the groaning of my heart. Lord, all my desire is before you; and my sighing is not hidden from you. My heart is troubled, and my strength has failed me; and the light of my eyes, it also has gone from me. My friends and my neighbors drew near me and stood against me. And my kinsmen stood afar off. Those who sought my soul used violence; those who sought evil for me spoke vain things and meditated deception all day long. But I became like a deaf man who does not hear; and I am like a mute who does not open his mouth. Thus I am like a man who does not hear, and in whose mouth there is no response. For in you, Lord, have I hope; you will hear me, Lord my God. For I said, never let my enemies rejoice over me, and when my feet are shaken, they magnify themselves against me. For I am ready for scourges, and my sorrow is continually before me. For I will declare my iniquity, and I will be in anguish over my sin. But my enemies live and become stronger than me. And those who hate me unjustly have multiplied. Those who render evil for good slandered me because I pursued goodness. Do not forsake me, Lord, my God, be not far from me. Make haste to help me, Lord of my salvation.\n";

// The Six Psalms – Psalm 62 (63)
static const char* text_psalm_62 =
    "Psalm 62 (63)\n"
    "\n"
    "O God, my God, early at dawn I rise to you. My soul thirsts for you; my flesh longs for you in a barren, untrodden and unwatered land. So I have appeared before you in the sanctuary to see your power and your glory. For your mercy is better than life; my lips shall praise you. Thus I shall bless you while I live, and I will lift up my hands in your name. Let my soul be filled with marrow and fatness, and my mouth shall praise you with joyful lips. When I remember you on my bed, I meditate on you at dawn. For you have become my helper; I shall rejoice in the shelter of your wings. My soul follows close behind you; your right hand has been quick to help me. But those who seek my life, to destroy it, shall go into the lower parts of the earth. They shall fall by the sword; they shall be a portion for jackals. But the king shall rejoice in God; everyone who swears by him shall be praised; but the mouths of those who speak lies shall be stopped.\n"
    "\n"
    "Glory to the Father and the Son and the Holy Spirit, now and forever and to the ages of ages. Amen.\n"
    "Alleluia. Alleluia. Alleluia.\n"
    "Lord, have mercy. Lord, have mercy. Lord, have mercy.\n"
    "Glory to the Father and the Son and the Holy Spirit, now and forever and to the ages of ages. Amen.\n";

// The Six Psalms – Psalm 87 (88)
static const char* text_psalm_87 =
    "Psalm 87 (88)\n"
    "\n"
    "O Lord, God of my salvation, I have cried out day and night before you. Let my prayer come before you; incline your ear to my cry. For my soul is filled with evil, And my life draws near to Hades. I am counted with those who go down into the pit; I am like a man without help, adrift among the dead. Like the bodies of the slain who sleep in the grave, whom you remember no more, and who are cut off from your hand. They have laid me in the lowest pit, in darkness, and the shadow of death. Your wrath lies heavy upon me, and you have afflicted me with all your waves. You have made me an abomination to them; I am shut up, and I cannot get out; my eyes have grown weak from poverty. I have cried to you, Lord, the entire day. I have stretched out my hands to you. Will you work wonders for the dead? Or shall physicians raise them up so that they might thank you? Shall any in the grave speak of your mercy and your truth in the place of destruction? Shall your wonders be known in the dark, and your righteousness in the land of forgetfulness? But as for me, I have cried out to you, Lord, and in the morning my prayer shall come before you. Lord, why do you cast off my soul, and turn away from me? I am a poor man and in trouble. From my youth, having been exalted, I was humbled and brought to distress. Your fierce wrath has gone over me, and your terrors have sorely troubled me. They came around me all day long like water; they engulfed me altogether. You have put far away from me friend and neighbor, and my acquaintances because of my misery.\n";

// The Six Psalms – Psalm 102 (103)
static const char* text_psalm_102 =
    "Psalm 102 (103)\n"
    "\n"
    "Bless the Lord, my soul; and all that is within me, bless his holy name! Bless the Lord, my soul, and forget not all that he has done for you. He forgives all your iniquities. He heals all your infirmities. He redeems your life from corruption. He crowns you with mercy and compassion. He satisfies your desire with good things, so that your youth is renewed like the eagle’s. The Lord performs deeds of mercy, and justice for all who are wronged. He made known his ways to Moses, his acts to the children of Israel. The Lord is merciful and compassionate, slow to anger, and abounding in mercy. He will not always strive with us, nor will he keep his anger forever. He has not dealt with us according to our iniquities, nor has he rewarded us according to our sins. For as the heavens are high above the earth, so great is his mercy toward those who fear him; As far as the east is from the west, so far has he removed our iniquities from us. As a father has compassion upon his son, so has the Lord compassion upon those who fear him. For he knows what we are made of, he remembers that we are dust. As for man, his days are like grass; as a flower of the field, so shall he blossom. For the wind passes over it, then it shall be gone. And its place remembers it no more. But the mercy of the Lord is from eternity to eternity upon those who fear him, and his righteousness is upon sons of sons, upon those who keep his covenant and remember to do his commandments. The Lord has established his throne in heaven, and his kingdom rules over all. Bless the Lord, all of his angels, who excel in strength, who do his word, heeding the voice of his word. Bless the Lord, all you his hosts, you ministers of his, who do his pleasure. Bless the Lord, all his works, in all places of his dominion. Bless the Lord, my soul.\n";

// The Six Psalms – Psalm 142 (143)
static const char* text_psalm_142 =
    "Psalm 142 (143)\n"
    "\n"
    "Lord, hear my prayer. In your truth, give ear to my supplications; in your righteousness, hear me. And enter not into judgment with your servant, for no one living is justified in your sight. For the enemy has pursued my soul; he has crushed my life to the ground; he has made me dwell in darkness, like those who have long been dead, and my spirit is overwhelmed within me; my heart within me is distressed. I remembered the days of old; I meditated on all your works; I pondered on the work of your hands. I spread out my hands to you; my soul longs for you like a thirsty land. Lord, hear me quickly; my spirit fails. Turn not your face away from me, lest I be like those who go down into the pit. Let me to hear your mercy in the morning, for in you I have put my trust. Lord, teach me to know the way in which I should walk, for I lift up my soul to you. Rescue me, Lord, from my enemies; to you have I fled for refuge. Teach me to do your will, for you are my God. Your good Spirit shall lead me on a level path; Lord, for your name’s sake, you shall preserve my life. In your righteousness, you shall bring my soul out of trouble, and in your mercy, you shall utterly destroy my enemies. And you shall destroy all those who afflict my soul, for I am your servant. Hear me, Lord, in your righteousness, And enter not into judgment with your servant (2). Your good Spirit shall lead me on a level path. Glory to the Father and the Son and the Holy Spirit, now and forever and to the ages of ages. Amen. Alleluia. Alleluia. Alleluia. Glory to you, God (3). Lord, you are our hope, glory to you.\n";

// Small Compline – Opening
static const char* text_small_compline_opening =
    "Small Compline\n"
    "\n"
    "In the name of the Father and the Son and the Holy Spirit, now and forever and to the ages of ages. Amen.\n"
    "\n"
    "Glory to you, our God, glory to you.\n"
    "\n"
    "Heavenly King, Comforter, the Spirit of truth who are present everywhere filling all things, Treasury of good things and Giver of life, come and dwell in us. Cleanse us of every stain, and save our souls, gracious Lord.\n"
    "\n"
    "Holy God, Holy Mighty, Holy Immortal, have mercy on us (3).\n"
    "\n"
    "Glory to the Father and the Son and the Holy Spirit. Now and forever and to the ages of ages. Amen.\n"
    "\n"
    "All holy Trinity, have mercy on us. Lord, forgive our sins. Master, pardon our transgressions. Holy One, visit and heal our infirmities, for your name’s sake.\n"
    "\n"
    "Lord, have mercy. Lord, have mercy. Lord, have mercy.\n"
    "\n"
    "Glory to the Father and the Son and the Holy Spirit. Now and forever and unto the ages of ages. Amen.\n"
    "\n"
    "Our Father,\n"
    "who are in heaven,\n"
    "hallowed be your name.\n"
    "Your kingdom come,\n"
    "your will be done,\n"
    "on earth as it is in heaven.\n"
    "Give us this day our daily bread;\n"
    "and forgive us our trespasses,\n"
    "as we forgive those who trespass against us.\n"
    "And lead us not into temptation,\n"
    "but deliver us from evil.\n"
    "\n"
    "For yours is the kingdom and the power and the glory, of the Father and the Son and the Holy Spirit, now and forever and unto the ages of ages. Amen.\n"
    "\n"
    "Lord, have mercy. Lord, have mercy. Lord, have mercy.\n"
    "\n"
    "Glory to the Father and the Son and the Holy Spirit, now and forever and to the ages of ages. Amen.\n";

// Small Compline – Psalm 50 (51)
static const char* text_small_psalm_50 =
    "Psalm 50 (51)\n"
    "\n"
    "Have mercy upon me, O God, according to your great mercy; and according to the multitude of your compassion, blot out my transgression. Wash me thoroughly from my iniquity, and cleanse me from my sin. For I acknowledge my iniquity, and my sin is ever before me. Against you, you only, have I sinned, and done evil in your sight, that you may be found just when you speak, and victorious when you are judged. For behold, I was conceived in iniquity, and in sin my mother bore me. For behold, you have loved truth; you have made known to me the secret and hidden things of your wisdom. You shall sprinkle me with hyssop, and I shall be made clean; you shall wash me, and I shall be whiter than snow. Make me to hear joy and gladness, that the afflicted bones may rejoice. Turn your face away from my sins, and blot out all my iniquities. Create in me a clean heart, O God, and renew a steadfast spirit within me. Cast me not away from your presence, and take not your Holy Spirit from me. Restore to me the joy of your salvation, and establish me with your governing Spirit. I shall teach transgressors your ways, and the ungodly shall turn back to you. Deliver me from bloodguiltiness, O God, the God of my salvation, my tongue shall joyfully declare your righteousness. Lord, open my lips, and my mouth shall declare your praise. For if you had desired sacrifice, I would give it; you do not delight in burnt offerings. A sacrifice to God is a broken spirit; God will not despise a broken and a humbled heart. Do good in your good pleasure to Sion; and let the walls of Jerusalem be built. Then you shall be pleased with a sacrifice of righteousness, with oblation and whole burnt offerings. Then they shall offer bulls on your altar.\n";

// Small Compline – Psalm 69 (70)
static const char* text_small_psalm_69 =
    "Psalm 69 (70)\n"
    "\n"
    "O God, be attentive to help me. Lord, make haste to help me. Let them be ashamed and confounded who seek my life. Let them be turned back and be ashamed who desire evil against me. Let them be turned back because of their shame, who say to me, Well done! Well done! Let all those who seek you rejoice and be glad in you; and let those who love your salvation say continually, Let God be magnified! But as for me, I am poor and needy; O God, help me! You are my help and my deliverer; Lord, do not delay.\n";

// Small Compline – Psalm 142 (143)
static const char* text_small_psalm_142 =
    "Psalm 142 (143)\n"
    "\n"
    "Lord, hear my prayer. In your truth, give ear to my supplications; in your righteousness, hear me. And enter not into judgment with your servant, for no one living is justified in your sight. For the enemy has pursued my soul; he has crushed my life to the ground; he has made me dwell in darkness, like those who have long been dead, and my spirit is overwhelmed within me; my heart within me is distressed. I remembered the days of old; I meditated on all your works; I pondered on the work of your hands. I spread out my hands to you; my soul longs for you like a thirsty land. Lord, hear me quickly; my spirit fails. Turn not your face away from me, lest I be like those who go down into the pit. Let me to hear your mercy in the morning, for in you I have put my trust. Lord, teach me to know the way in which I should walk, for I lift up my soul to you. Rescue me, Lord, from my enemies; to you have I fled for refuge. Teach me to do your will, for you are my God. Your good Spirit shall lead me on a level path; Lord, for your name’s sake, you shall preserve my life. In your righteousness, you shall bring my soul out of trouble, and in your mercy, you shall utterly destroy my enemies. And you shall destroy all those who afflict my soul, for I am your servant.\n";

// Small Compline – Doxology
static const char* text_small_doxology =
    "Doxology\n"
    "\n"
    "Glory to God in the highest and on earth peace, good will to all people. We praise you, we bless you, we worship you, we glorify you, we give thanks to you for your great glory. Lord God, heavenly King, God the Father, almighty Lord, the only begotten Son, Jesus Christ, and Holy Spirit. Lord God, Lamb of God, Son of the Father who take away the sin of the world, have mercy upon us, you who take away the sins of the world. Receive our prayer, you who sit at the right hand of the Father, and have mercy upon us. For you only are holy, only you are Lord, Jesus Christ, to the glory of God the Father. Amen. Each evening we bless you, and we praise your name forever and to the ages of ages. Lord, you have been our refuge from generation to generation. I said: Lord, have mercy upon me; heal my soul, for I have sinned against you. Lord, to you have I fled; teach me to do your will, for you are my God. For you are the source of life, and in your light we shall see light. Continue your mercy to those who know you. Lord, grant to keep us this night without sin. Blessed are you, Lord, God of our fathers. Your name is praised and glorified from all ages. Amen. Let your mercy, Lord, lighten upon us, as our trust is in you. Blessed are you, Lord, teach me your commandments. Blessed are you, Master, teach me your commandments. Blessed are you, Holy One, enlighten me in your commandments. Your mercy, Lord, endures forever; turn not away from the works of your own hands. To you belongs praise, to you belongs worship, to you belongs glory, to the Father and the Son and the Holy Spirit, now and forever and to the ages of ages. Amen.\n";

// Holy Communion Prep – Opening
static const char* text_comm_prep_opening =
    "Service Of Preparation For Holy Communion\n"
    "\n"
    "Glory to you, our God, glory to you. Heavenly King, Comforter, the Spirit of truth, who are present everywhere filling all things, Treasury of good things and Giver of life, come and dwell in us. Cleanse us of every stain, and save our souls, gracious Lord.\n"
    "\n"
    "Holy God, Holy Mighty, Holy Immortal, have mercy on us (3).\n"
    "\n"
    "Glory to the Father and the Son and the Holy Spirit, now and forever and to the ages of ages. Amen.\n"
    "\n"
    "All holy Trinity, have mercy on us. Lord, forgive our sins. Master, pardon our transgressions. Holy One, visit and heal our infirmities for your name’s sake.\n"
    "\n"
    "Lord, have mercy. Lord, have mercy. Lord, have mercy.\n"
    "\n"
    "Glory to the Father and the Son and the Holy Spirit, now and forever and to the ages of ages. Amen.\n"
    "\n"
    "Our Father,\n"
    "who are in heaven,\n"
    "hallowed be your name.\n"
    "Your kingdom come,\n"
    "your will be done,\n"
    "on earth as it is in heaven.\n"
    "Give us this day our daily bread;\n"
    "and forgive us our trespasses,\n"
    "as we forgive those who trespass against us.\n"
    "And lead us not into temptation,\n"
    "but deliver us from evil.\n"
    "\n"
    "For yours is the kingdom and the power and the glory, of the Father and the Son and the Holy Spirit, now and forever and to the ages of ages. Amen.\n"
    "\n"
    "Lord, have mercy. Lord, have mercy. Lord, have mercy.\n"
    "\n"
    "Glory to the Father and the Son and the Holy Spirit, now and forever and to the ages of ages. Amen.\n";

// Holy Communion Prep – Psalm 50 (51)
static const char* text_comm_prep_psalm_50 =
    "Psalm 50 (51)\n"
    "\n"
    "Have mercy upon me, O God, according to your great mercy; and according to the multitude of your compassion, blot out my transgression. Wash me thoroughly from my iniquity, and cleanse me from my sin. For I acknowledge my iniquity, and my sin is ever before me. Against you, you only, have I sinned, and done evil in your sight, that you may be found just when you speak, and victorious when you are judged. For behold, I was conceived in iniquity, and in sin my mother bore me. For behold, you have loved truth; you have made known to me the secret and hidden things of your wisdom. You shall sprinkle me with hyssop, and I shall be made clean; you shall wash me, and I shall be whiter than snow. Make me to hear joy and gladness, that the afflicted bones may rejoice. Turn your face away from my sins, and blot out all my iniquities. Create in me a clean heart, O God, and renew a steadfast spirit within me. Cast me not away from your presence, and take not your Holy Spirit from me. Restore to me the joy of your salvation, and establish me with your governing Spirit. I shall teach transgressors your ways, and the ungodly shall turn back to you. Deliver me from bloodguiltiness, O God, the God of my salvation, my tongue shall joyfully declare your righteousness. Lord, open my lips, and my mouth shall declare your praise. For if you had desired sacrifice, I would give it; you do not delight in burnt offerings. A sacrifice to God is a broken spirit; God will not despise a broken and a humbled heart. Do good in your good pleasure to Sion; and let the walls of Jerusalem be built. Then you shall be pleased with a sacrifice of righteousness, with oblation and whole burnt offerings. Then they shall offer bulls on your altar.\n";

// Holy Communion Prep – Psalm 69 (70)
static const char* text_comm_prep_psalm_69 =
    "Psalm 69 (70)\n"
    "\n"
    "O God, be attentive to help me. Lord, make haste to help me. Let them be ashamed and confounded who seek my life. Let them be turned back and be ashamed who desire evil against me. Let them be turned back because of their shame, who say to me, Well done! Well done! Let all those who seek you rejoice and be glad in you; and let those who love your salvation say continually, Let God be magnified! But as for me, I am poor and needy; O God, help me! You are my help and my deliverer; Lord, do not delay.\n";

// Holy Communion Prep – Psalm 142 (143)
static const char* text_comm_prep_psalm_142 =
    "Psalm 142 (143)\n"
    "\n"
    "Lord, hear my prayer. In your truth, give ear to my supplications; in your righteousness, hear me. And enter not into judgment with your servant, for no one living is justified in your sight. For the enemy has pursued my soul; he has crushed my life to the ground; he has made me dwell in darkness, like those who have long been dead, and my spirit is overwhelmed within me; my heart within me is distressed. I remembered the days of old; I meditated on all your works; I pondered on the work of your hands. I spread out my hands to you; my soul longs for you like a thirsty land. Lord, hear me quickly; my spirit fails. Turn not your face away from me, lest I be like those who go down into the pit. Let me to hear your mercy in the morning, for in you I have put my trust. Lord, teach me to know the way in which I should walk, for I lift up my soul to you. Rescue me, Lord, from my enemies; to you have I fled for refuge. Teach me to do your will, for you are my God. Your good Spirit shall lead me on a level path; Lord, for your name’s sake, you shall preserve my life. In your righteousness, you shall bring my soul out of trouble, and in your mercy, you shall utterly destroy my enemies. And you shall destroy all those who afflict my soul, for I am your servant.\n";

// Holy Communion Prep – Doxology
static const char* text_comm_prep_doxology =
    "Doxology\n"
    "\n"
    "Glory to God in the highest and on earth peace, good will to all people. We praise you, we bless you, we worship you, we glorify you, we give thanks to you for your great glory. Lord God, heavenly King, God the Father, almighty Lord, the only begotten Son, Jesus Christ, and Holy Spirit. Lord God, Lamb of God, Son of the Father who take away the sin of the world, have mercy upon us, you who take away the sins of the world. Receive our prayer, you who sit at the right hand of the Father, and have mercy upon us. For you only are holy, only you are Lord, Jesus Christ, to the glory of God the Father. Amen.\n"
    "\n"
    "Each evening we bless you, and we praise your name forever and to the ages of ages. Lord, you have been our refuge from generation to generation. I said: Lord, have mercy upon me; heal my soul, for I have sinned against you. Lord, to you have I fled; teach me to do your will, for you are my God. For you are the source of life, and in your light we shall see light. Continue your mercy to those who know you. Lord, grant to keep us this night without sin. Blessed are you, Lord, God of our fathers. Your name is praised and glorified from all ages. Amen.\n"
    "\n"
    "Let your mercy, Lord, lighten upon us, as our trust is in you. Blessed are you, Lord, teach me your commandments. Blessed are you, Master, teach me your commandments. Blessed are you, Holy One, enlighten me in your commandments. Your mercy, Lord, endures forever; turn not away from the works of your own hands. To you belongs praise, to you belongs worship, to you belongs glory, to the Father and the Son and the Holy Spirit, now and forever and to the ages of ages. Amen.\n";

// Thanksgiving After Communion – Opening Thanksgiving
static const char* text_thank_comm_opening =
    "Thanksgiving After Holy Communion\n"
    "\n"
    "When you have had your due and rightful part in these life giving and mysterious gifts, give immediate praise and great thanks to God, and say the following with fervent soul:\n"
    "\n"
    "Glory to you, O God (3).\n"
    "\n"
    "Then the following thanksgiving prayers:\n";

// Thanksgiving After Communion – Anonymous #1
static const char* text_thank_anon1 =
    "Anonymous\n"
    "\n"
    "I thank you, Lord, my God, that you have not rejected me, a sinner, but have made me worthy to partake of your holy Mysteries. I thank you that you have permitted me, although I am unworthy, to receive your pure and heavenly gifts. O loving Master, who died and rose for our sake, and granted to us these awesome and life-giving Mysteries for the well-being and sanctification of our souls and bodies, let these gifts be for healing of my own soul and body, the averting of every evil, the illumination of the eyes of my heart, the peace of my spiritual powers, a faith unashamed, a love unfeigned, the fulfilling of wisdom, the observing of your commandments, the receiving of your divine grace, and the inheritance of your kingdom. Preserved by them in your holiness, may I always be mindful of your grace and no longer live for myself, but for you, our Master and Benefactor. May I pass from this life in the hope of eternal life, and attain to the everlasting rest, where the voices of your saints who feast are unceasing, and their joy, beholding the ineffable beauty of your countenance, is unending. For you, Christ our God, are the true joy and the inexpressible gladness of those who love you, and all creation praises you forever. Amen.\n";

// Thanksgiving After Communion – Prayer of Saint Basil
static const char* text_thank_prayer_basil =
    "Prayer Of Saint Basil\n"
    "\n"
    "I thank you, Christ and Master our God, King of the ages and Creator of all things, for all the good gifts you have given me, and especially for the participation in your pure and life-giving mysteries. I, therefore, pray to you, good and loving Lord: keep me under your protection and under the shadow of your wings. Grant that to my last breath I may with a pure conscience partake worthily of your gifts for the forgiveness of sins and for eternal life. For you are the bread of life, the source of holiness, the giver of all good things, and to you we give glory, with the Father and the Holy Spirit, now and forever and to the ages of ages. Amen.\n";

// Thanksgiving After Communion – Prayer of Symeon Metaphrastes
static const char* text_thank_symeon =
    "Prayer Of Symeon Metaphrastes\n"
    "\n"
    "You who have voluntarily given me your flesh as food, who are a burning fire to the unworthy, do not consume me. No, my Creator. Rather, penetrate into my members, all my joints, my organs, my heart and burn like thorns all my iniquities. Cleanse my soul, hallow my thoughts, make firm my knees and my bones as well. Illumine my five senses and make my entire being vigilant with the fear of you. Watch over me always, shield and protect me from every deed and word that corrupts the soul. Cleanse me, purify and put me in order. Adorn me, give me understanding, and illumine me. Mark me as your dwelling place, of the Spirit only and no longer a place of sin, so that when you enter as if into your own home of communion, every evil doer and every passion will flee from me as from fire. As advocates, I bring to you all the saints, the captains of the incorporeal hosts, your Forerunner, your wise Apostles, and more than these, your spotless, pure Mother, whose prayers accept, my compassionate Christ. Make your servant a child of light. For in your goodness, you alone sanctify and enlighten our souls, and to you, our God and Master, do we give, as it is right, glory every day.\n";

// Thanksgiving After Communion – Anonymous #2
static const char* text_thank_anon2 =
    "Anonymous\n"
    "\n"
    "Let your sacred body, Lord Jesus Christ our God, lead me to eternal life, and your precious blood to remission of sins. Let this Eucharist be to me joy, health, and gladness. And at your fearful Coming deem me a sinner worthy to stand at the right hand of your glory, by the intercession of your most pure Mother, and of all your saints. Amen.\n";

// Thanksgiving After Communion – All holy Lady, Theotokos
static const char* text_thank_theotokos =
    "All holy Lady, Theotokos\n"
    "\n"
    "All holy Lady, Theotokos, light of my darkened soul, my hope, shelter, refuge, comfort, and joy, I thank you, for you have deemed me, the unworthy one, worthy to partake of the most pure body and of the precious blood of your Son. But you who gave birth to the true Light, enlighten the spiritual eyes of my heart. You conceived the source of immortality. Now give life to me who am dead in sin. You, the compassionate Mother of the merciful God, have mercy on me and give me penitence and contrition of heart and meekness in my thoughts and an awakening of my thoughts from captivity. And grant me, until my last breath, to receive without condemnation the sanctification of your sacred Mysteries for the healing of soul and body. Grant me tears of repentance and confession that I may praise and glorify you all the days of my life. For you are blessed and glorified to the ages of ages. Amen.\n";

// ─────────────────────────────
// STEP 2: Forward‐declare UI callbacks
// ─────────────────────────────

static void main_menu_callback(Menu* m, MenuItem* item);
static void sub_menu_callback(Menu* m, MenuItem* item);
static void show_prayer_callback(Menu* m, MenuItem* item);

// ─────────────────────────────
// STEP 3: Global UI handles
// ─────────────────────────────

static ViewPort* view_port;                 // The viewport we’ll draw on
static bool show_text = false;              // Are we in “display prayer text” state?
static const char* current_prayer_text = NULL;

// ─────────────────────────────
// STEP 4: Draw callback – shows text or “help” info
// ─────────────────────────────

static void draw_callback(Canvas* canvas, void* ctx) {
    if(show_text && current_prayer_text) {
        static int16_t scroll_y = 0;
        canvas_clear(canvas);
        canvas_set_font(canvas, FontPrimary);
        // Draw the text starting at y = –scroll_y
        canvas_draw_text(canvas, 0, -scroll_y, current_prayer_text);
    } else {
        canvas_clear(canvas);
        canvas_set_font(canvas, FontPrimary);
        canvas_draw_str(canvas, 0, 0, "Use menu to pick a prayer");
        canvas_draw_str(canvas, 0, 16, "BACK to return");
    }
}

// ─────────────────────────────
// STEP 5: Input callback – handle scroll and BACK
// ─────────────────────────────

static void input_callback(InputEvent* event, void* ctx) {
    static int16_t scroll_y = 0;
    const int16_t scroll_step = 10; // pixels per key press
    if(!show_text) {
        return;
    }
    if(event->type == InputTypePress) {
        switch(event->key) {
            case InputKeyUp:
                scroll_y -= scroll_step;
                if(scroll_y < 0) scroll_y = 0;
                furi_hal_os_view_port_send_view_invalidate(view_port);
                break;
            case InputKeyDown:
                scroll_y += scroll_step;
                if(scroll_y > 2000) scroll_y = 2000; // large clamp
                furi_hal_os_view_port_send_view_invalidate(view_port);
                break;
            case InputKeyBack:
            case InputKeyOk:
                show_text = false;
                scroll_y = 0;
                furi_hal_os_view_port_send_view_invalidate(view_port);
                break;
            default:
                break;
        }
    }
}

// ─────────────────────────────
// STEP 6: Build the “show prayer” screen
// ─────────────────────────────

static void show_prayer_screen(const char* prayer_text) {
    current_prayer_text = prayer_text;
    show_text = true;
    furi_hal_os_view_port_send_view_invalidate(view_port);
}

// ─────────────────────────────
// STEP 7: “Show prayer” menu callback
//      (each specific‐prayer item hits this)
// ─────────────────────────────

static void show_prayer_callback(Menu* m, MenuItem* item) {
    const char* text = (const char*)item->context;
    show_prayer_screen(text);
}

// ─────────────────────────────
// STEP 8: Sub‐menu callback
//      (when you select a top‐level category)
// ─────────────────────────────

static void sub_menu_callback(Menu* m, MenuItem* item) {
    intptr_t idx = (intptr_t)item->context;
    Menu* child_menu = menu_alloc();
    menu_set_title(child_menu, prayer_categories[idx]);

    switch(idx) {
        case 0: // Symbol of Faith
            menu_add_item(
                child_menu,
                symbol_of_faith_list[0],
                NULL,
                show_prayer_callback,
                (void*)text_symbol_of_faith
            );
            break;

        case 1: // Morning Prayers
            menu_add_item(
                child_menu,
                morning_prayers_list[0],
                NULL,
                show_prayer_callback,
                (void*)text_morning_opening
            );
            menu_add_item(
                child_menu,
                morning_prayers_list[1],
                NULL,
                show_prayer_callback,
                (void*)text_prayer_basil_daily
            );
            menu_add_item(
                child_menu,
                morning_prayers_list[2],
                NULL,
                show_prayer_callback,
                (void*)text_prayer_basil_saturday
            );
            menu_add_item(
                child_menu,
                morning_prayers_list[3],
                NULL,
                show_prayer_callback,
                (void*)text_prayer_eustratios
            );
            break;

        case 2: // Prayers At Mealtime
            menu_add_item(
                child_menu,
                mealtime_prayers_list[0],
                NULL,
                show_prayer_callback,
                (void*)text_before_breakfast
            );
            menu_add_item(
                child_menu,
                mealtime_prayers_list[1],
                NULL,
                show_prayer_callback,
                (void*)text_after_breakfast
            );
            menu_add_item(
                child_menu,
                mealtime_prayers_list[2],
                NULL,
                show_prayer_callback,
                (void*)text_before_lunch
            );
            menu_add_item(
                child_menu,
                mealtime_prayers_list[3],
                NULL,
                show_prayer_callback,
                (void*)text_after_lunch
            );
            menu_add_item(
                child_menu,
                mealtime_prayers_list[4],
                NULL,
                show_prayer_callback,
                (void*)text_before_dinner
            );
            menu_add_item(
                child_menu,
                mealtime_prayers_list[5],
                NULL,
                show_prayer_callback,
                (void*)text_after_dinner
            );
            menu_add_item(
                child_menu,
                mealtime_prayers_list[6],
                NULL,
                show_prayer_callback,
                (void*)text_another_mealtime
            );
            break;

        case 3: // Before Sleep
            menu_add_item(
                child_menu,
                bedtime_prayers_list[0],
                NULL,
                show_prayer_callback,
                (void*)text_before_sleep_general
            );
            menu_add_item(
                child_menu,
                bedtime_prayers_list[1],
                NULL,
                show_prayer_callback,
                (void*)text_prayer_children
            );
            menu_add_item(
                child_menu,
                bedtime_prayers_list[2],
                NULL,
                show_prayer_callback,
                (void*)text_prayer_adults
            );
            menu_add_item(
                child_menu,
                bedtime_prayers_list[3],
                NULL,
                show_prayer_callback,
                (void*)text_believe_one_god_bedtime
            );
            menu_add_item(
                child_menu,
                bedtime_prayers_list[4],
                NULL,
                show_prayer_callback,
                (void*)text_at_all_times_bedtime
            );
            break;

        case 4: // The Six Psalms
            menu_add_item(
                child_menu,
                six_psalms_list[0],
                NULL,
                show_prayer_callback,
                (void*)text_six_opening
            );
            menu_add_item(
                child_menu,
                six_psalms_list[1],
                NULL,
                show_prayer_callback,
                (void*)text_psalm_3
            );
            menu_add_item(
                child_menu,
                six_psalms_list[2],
                NULL,
                show_prayer_callback,
                (void*)text_psalm_37
            );
            menu_add_item(
                child_menu,
                six_psalms_list[3],
                NULL,
                show_prayer_callback,
                (void*)text_psalm_62
            );
            menu_add_item(
                child_menu,
                six_psalms_list[4],
                NULL,
                show_prayer_callback,
                (void*)text_psalm_87
            );
            menu_add_item(
                child_menu,
                six_psalms_list[5],
                NULL,
                show_prayer_callback,
                (void*)text_psalm_102
            );
            menu_add_item(
                child_menu,
                six_psalms_list[6],
                NULL,
                show_prayer_callback,
                (void*)text_psalm_142
            );
            break;

        case 5: // Small Compline
            menu_add_item(
                child_menu,
                small_compline_list[0],
                NULL,
                show_prayer_callback,
                (void*)text_small_compline_opening
            );
            menu_add_item(
                child_menu,
                small_compline_list[1],
                NULL,
                show_prayer_callback,
                (void*)text_small_psalm_50
            );
            menu_add_item(
                child_menu,
                small_compline_list[2],
                NULL,
                show_prayer_callback,
                (void*)text_small_psalm_69
            );
            menu_add_item(
                child_menu,
                small_compline_list[3],
                NULL,
                show_prayer_callback,
                (void*)text_small_psalm_142
            );
            menu_add_item(
                child_menu,
                small_compline_list[4],
                NULL,
                show_prayer_callback,
                (void*)text_small_doxology
            );
            break;

        case 6: // Holy Communion Prep
            menu_add_item(
                child_menu,
                communion_prep_list[0],
                NULL,
                show_prayer_callback,
                (void*)text_comm_prep_opening
            );
            menu_add_item(
                child_menu,
                communion_prep_list[1],
                NULL,
                show_prayer_callback,
                (void*)text_comm_prep_psalm_50
            );
            menu_add_item(
                child_menu,
                communion_prep_list[2],
                NULL,
                show_prayer_callback,
                (void*)text_comm_prep_psalm_69
            );
            menu_add_item(
                child_menu,
                communion_prep_list[3],
                NULL,
                show_prayer_callback,
                (void*)text_comm_prep_psalm_142
            );
            menu_add_item(
                child_menu,
                communion_prep_list[4],
                NULL,
                show_prayer_callback,
                (void*)text_comm_prep_doxology
            );
            break;

        case 7: // Thanksgiving After Communion
            menu_add_item(
                child_menu,
                thanksgiving_comm_list[0],
                NULL,
                show_prayer_callback,
                (void*)text_thank_comm_opening
            );
            menu_add_item(
                child_menu,
                thanksgiving_comm_list[1],
                NULL,
                show_prayer_callback,
                (void*)text_thank_anon1
            );
            menu_add_item(
                child_menu,
                thanksgiving_comm_list[2],
                NULL,
                show_prayer_callback,
                (void*)text_thank_prayer_basil
            );
            menu_add_item(
                child_menu,
                thanksgiving_comm_list[3],
                NULL,
                show_prayer_callback,
                (void*)text_thank_symeon
            );
            menu_add_item(
                child_menu,
                thanksgiving_comm_list[4],
                NULL,
                show_prayer_callback,
                (void*)text_thank_anon2
            );
            menu_add_item(
                child_menu,
                thanksgiving_comm_list[5],
                NULL,
                show_prayer_callback,
                (void*)text_thank_theotokos
            );
            break;

        default:
            break;
    }

    view_port_remove_menu(view_port);
    view_port_attach_menu(view_port, child_menu);
    menu_free(child_menu);
}

// ─────────────────────────────
// STEP 9: Main menu callback (level 1)
//      (shows categories)
// ─────────────────────────────

static void main_menu_callback(Menu* m, MenuItem* item) {
    sub_menu_callback(m, item);
}

// ─────────────────────────────
// STEP 10: Entry point: fap_orthodox_prayers_init()
// ─────────────────────────────

int32_t fap_orthodox_prayers(void* p) {
    Gui* gui = furi_record_open("gui");
    view_port = view_port_alloc();

    view_port_draw_callback_set(view_port, draw_callback, NULL);
    view_port_input_callback_set(view_port, input_callback, NULL);

    Menu* root_menu = menu_alloc();
    menu_set_title(root_menu, "Orthodox Prayers");

    for(uint32_t i = 0; i < sizeof(prayer_categories)/sizeof(prayer_categories[0]); i++) {
        menu_add_item(
            root_menu,
            prayer_categories[i],
            NULL,
            main_menu_callback,
            (void*)(intptr_t)i
        );
    }

    view_port_attach_menu(view_port, root_menu);
    gui_add_view_port(gui, view_port, GuiLayerFullscreen);

    // Blocks until user backs out of the root menu completely
    menu_show(root_menu);

    gui_remove_view_port(gui, view_port);
    view_port_free(view_port);
    menu_free(root_menu);
    furi_record_close("gui");
    return 0;
}