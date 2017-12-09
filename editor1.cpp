// Xcom2edi
// Copyright (c) 1997, Jeff Lawson <jeff@bovine.net>
// All rights reserved.
// See LICENSE for distribution information.

#include "editor.h"


// []-----------------------[]
// |  all buildings in game  |
// []-----------------------[]
char building_names[][19] = {"Empty","Access Lift","Living Quarters","Laboratory",\
    "Workshop","Standard Sonar","Wide Array Sonar","Torpedo Defences","General Stores",\
    "Alien Containment","Gauss Defences","Sonic Defences","P.W.T. Defences","Bombardment Shield",\
    "M.C. Generator","M.C. Laboratory","Trans. Resolver","Sub Pen"};


// []---------------------------------------------[]
// |  lookup table of base identification numbers  |
// []---------------------------------------------[]
struct baselisting_type baselisting[8];
unsigned numbases = 0;


// []---------------------[]
// |  all objects in game  |
// []---------------------[]
struct object_type objects[] = {{"Ajax Launcher",1},{"D.U.P. Head Launcher",2},{"Craft Gas Cannon",3},\
    {"P.W.T. Cannon",4},{"Gauss Cannon",5},{"Sonic Oscillator",6},{"Ajax Torpedoes",7},\
    {"D.U.P. Head Torpedoes",8},{"Gas Rounds (x50)",9},{"P.W.T. Ammo",10},{"Coelacanth/G.Cannon",11},\
    {"Coelacanth/Aqua jet",12},{"Coelacanth/Gauss",13},{"Displacer/Sonic",14},{"Displacer/P.W.T.",15},\
    {"Dart Gun",16},{"Dart Clip",17},{"Jet Harpoon",18},{"Harpoon Clip",19},{"Gas Cannon",20},\
    {"GC-AP Bolts",21},{"GC-HE Bolts",22},{"GC-Phosphorous Bolts",23},{"Hydro-Jet Cannon",24},\
    {"HJ-AP Ammo",25},{"HJ-HE Ammo",26},{"HJ-P Ammo",27},{"Torpedo Launcher",28},{"Small Torpedo",29},\
    {"Large Torpedo",30},{"Phosphor Torpedo",31},{"Gauss Pistol",32},{"Gauss Pistol Clip",63},\
    {"Gauss Rifle",33},{"Gauss Rifle Clip",64},{"Heavy Gauss",34},{"Heavy Gauss Clip",65},\
    {"Magna-Blast Grenade",35},{"Dye Grenade",36},{"Particle Disturbance Grenade",37},{"Magna-Pack Explosive",38},\
    {"Particle Disturbance Sensor",39},{"Medi-Kit",40},{"M.C. Disruptor",41},{"Thermal Tazer",42},\
    {"Chemical-Flare",43},{"Vibro Blade",44},{"Thermic Lance",45},{"Heavy Thermic Lance",46},\
    {"Sonic Cannon",50},{"Cannon Power Clip",51},{"Sonic-Blasto Rifle",52},{"Blasta Power Clip",53},\
    {"Sonic Pistol",54},{"Pistol Power Clip",55},{"Disruptor Pulse Launcher",56},\
    {"Disruptor Ammo",57},{"Thermal Shok Launcher",58},{"Thermal Shok Bomb",59},{"Sonic Pulser",60},\
    {"Zrbite",61},{"M.C. Reader",62},{"Aquatoid Corpse",66},{"Gill Man Corpse",67},\
    {"Lobster Man Corpse",68},{"Tasoth Corpse",69},{"Calcinite Corpse",70},{"Deep One Corpse",71},\
    {"Bio-Drone Corpse",72},{"Tentacular Corpse",73},{"Triscene Corpse",74},{"Hallucinoid Corpse",75},\
    {"Xarquid Corpse",76},{" Technician",77},{" Squad Leader",78},{" Soldier",79},{" Terrorist",80},\
    {"Ion Beam Accelerator",81},{"Magnetic Navigation",82},{"Alien Sub Construction",83},{"Alien Cryogenics",84},\
    {"Alien Cloning",85},{"Alien Learning Arrays",86},{"Alien Implanter",87},{"Examination Room",88},\
    {"Aqua Plastics",89},{"Alien Re-animation Zone",90},{"Plastic Aqua-Armour",91},\
    {"Ion Armour",92},{"Magnetic Ion Armour",93}};
char damagetype[][15] = {"none","armor piercing","phosphorous","high explosive",\
    "gauss beam","sonic beam","freeze"};


// []-------------------------[]
// |  prototype all functions  |
// []-------------------------[]
screentype screen;
mousetype mouse;
static int main_screen (void);
static void check_saved_games(void);
static int select_function(int savedgame);
static void read_base_names(int savedgame);
static int select_base(void);


//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
#pragma argsused
void main (int argc, char *argv[])
{
    // make sure that there are saved games here
    check_saved_games();

    activescreenclass->registerfarewell(Farewell);

    // main loop
    while (TRUE) {
        // select which saved game to edit
        int savedgame_number = main_screen();
        if (savedgame_number == 0) break;
        read_base_names(savedgame_number);


        // select what to modify
        int option, base;
        while ((option = select_function(savedgame_number)) != 0) {
            switch (option) {
                case 1: // edit money
                    money_edit(savedgame_number);
                    break;
                case 2: // edit base inventory
                    base = select_base();
                    if (base) edit_inventory(savedgame_number, base);
                    break;
                case 3: // edit base buildings
                    base = select_base();
                    if (base) edit_buildings(savedgame_number, base);
                    break;
                case 4: // edit soldiers
                    edit_soldiers(savedgame_number);
                    break;
                case 5: // import soldiers
                    import_soldiers(savedgame_number);
                    break;
                case 6: { // backup
                    activescreenclass->save();
                    activemouseclass->hide();
                    clrscr();
                    char filename[MAXPATH], filemask[MAXPATH];
                    sprintf(filename, "BACKUP%i.ZIP", savedgame_number);
                    sprintf(filemask, "GAME_%i\\*.*", savedgame_number);
                    spawnlpo(P_WAIT, "PKZIP.EXE", "", filename, filemask, "/ex", NULL);
                    printf("\nPress any key...");
                    if (!getch()) getch();
                    activemouseclass->show();
                    activescreenclass->restore();
                    break;
                    }
                case 7: { // restore
                    activescreenclass->save();
                    activemouseclass->hide();
                    clrscr();
                    char filename[MAXPATH], filemask[MAXPATH];
                    sprintf(filename, "BACKUP%i.ZIP", savedgame_number);
                    sprintf(filemask, "GAME_%i", savedgame_number);
                    spawnlpo(P_WAIT, "PKUNZIP.EXE", "", filename, filemask, "/o", NULL);
                    printf("\nPress any key...");
                    if (!getch()) getch();
                    activemouseclass->show();
                    activescreenclass->restore();
                    break;
                    }
                case 8: // know all
                    activemouseclass->setcursor(MOUSE_WAIT);
                    FILE *fp;
                    char filename[25];
                    sprintf(filename, "GAME_%i\\FACIL.DAT", savedgame_number);
                    if ((fp = fopen(filename, "wb")) == NULL) file_error();
                    fwrite(FacilDat, sizeof(char), 272, fp);
                    fclose(fp);
                    sprintf(filename, "GAME_%i\\PRODUCT.DAT", savedgame_number);
                    if ((fp = fopen(filename, "wb")) == NULL) file_error();
                    fwrite(ProductDat, sizeof(char), 774, fp);
                    fclose(fp);
                    sprintf(filename, "GAME_%i\\RESEARCH.DAT", savedgame_number);
                    if ((fp = fopen(filename, "wb")) == NULL) file_error();
                    fwrite(ResearchDat, sizeof(char), 2068, fp);
                    fclose(fp);
                    sprintf(filename, "GAME_%i\\UP.DAT", savedgame_number);
                    if ((fp = fopen(filename, "wb")) == NULL) file_error();
                    fwrite(UpDat, sizeof(char), 1464, fp);
                    fclose(fp);
                    activemouseclass->setcursor(MOUSE_ARROW);
                    ok_box("\nChanges successfully made.\n");
                    break;
                case 9: // tactical editor
                    tactical_editor(savedgame_number);
                    break;
                case 10: // alien containment editor
                    base = select_base();
                    if (base) do_edit_astore(savedgame_number, base);
                    break;
                case 11: // edit diplomacy
                    edit_diplomacy(savedgame_number);
                    break;
                case 12: // submarine editor
                    submarine_editor(savedgame_number);
                    break;
                default: break;
            }
        }
    }

    exit(0);
}
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
static void check_saved_games(void)
{
    for (int i = 1; i <= 10; i++) {
        FILE *fp;
        char filename[MAXPATH];
        sprintf(filename, "GAME_%i\\SAVEINFO.DAT", i);
        if ((fp = fopen(filename, "rb")) != NULL) {
            fclose(fp);
            return;
        }
    }
    no_games();
}
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
static int main_screen (void)
{
    int i, j;

    // []------------------[]
    // |  clear the screen  |
    // []------------------[]
    activescreenclass->drawrepeat('°', 80*25, 1, 1, COLOR(LIGHT_WHITE, LIGHT_BLUE));
    activescreenclass->refresh();


    // []--------------------------------------[]
    // |  read in all of the saved games names  |
    // []--------------------------------------[]
    activemouseclass->setcursor(MOUSE_WAIT);
    unsigned buffersize = 300, bufferused = 0; char *buffer;
    if ((buffer = new char[buffersize]) == NULL) no_memory();
    for (i = 1; i <= 10; i++) {
        FILE *fp;
        char filename[MAXPATH];
        sprintf(filename, "GAME_%i\\SAVEINFO.DAT", i);
        if ((fp = fopen(filename, "rb")) != NULL) {
            if (buffersize - bufferused < 80) {
                buffersize += 300;
                char *newbuffer = new char[buffersize];
                if (newbuffer == NULL) no_memory();
                memmove(newbuffer, buffer, bufferused);
                delete buffer; buffer = newbuffer;
            }
            bufferused += sprintf(buffer + bufferused, "%2i. ", i);
            fseek(fp, 2, SEEK_SET);
            fread(buffer + bufferused, sizeof(char), 80, fp);
            fclose(fp);
            bufferused += strlen(buffer + bufferused);
            bufferused += sprintf(buffer + bufferused, "\n");
        }
    }
    if (bufferused == 0) no_games();
    activemouseclass->setcursor(MOUSE_ARROW);


    // []------------------------------------[]
    // |  create all controls that we'll use  |
    // []------------------------------------[]
    buttontype quit_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY),\
        3, 60, "Quit editor");
    buttontype about_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY),\
        9, 60, "About editor");
    buttontype edit_objects(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY),\
        15,  60, "Edit Objects");
    menuboxtype saved_games(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, BLACK), \
        COLOR(GRAY, LIGHT_WHITE), 10, 23, buffer);
    activescreenclass->drawstring("version " VERSION
        #ifdef DEBUG
            " áeta -- please do not distribute!"
        #endif
        , 25, 1, COLOR(LIGHT_WHITE, LIGHT_BLUE));
    delete buffer;


    // []-----------------[]
    // |  main input loop  |
    // []-----------------[]
    while (TRUE) {
        // []-------------[]
        // |  quit button  |
        // []-------------[]
        quit_button.handle();
        if (quit_button.pressed) if (ask_yes_no("\nDo you really want to quit?\n")) return(0);


        // []--------------[]
        // |  about button  |
        // []--------------[]
        about_button.handle();
        if (about_button.pressed) {
            activescreenclass->save();
            #define ABOUT_ROW 2
            #define ABOUT_COL 20
            textboxtype message(COLOR(LIGHT_WHITE, BLUE), COLOR(LIGHT_YELLOW, BLUE), \
                ABOUT_ROW, ABOUT_COL, 42, 23, "\n"
                "This program was written in Borland\n"
                "C++ 4.52 using my own user interface,\n"
                "screen, and mouse handling classes.\n"
                "This specific version was compiled on\n"
                __DATE__ " at " __TIME__ ".  If you\n"
                "want to talk to the author, you may\n"
                "write me at \2jeff@bovine.net\1\n"
                "I can also be reached via the WWW at\n"
                "\2http://www.bovine.net/\1\n"
                );
            buttontype ok_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY),\
                ABOUT_ROW + 19, ABOUT_COL + 17, "OK");
            while (!ok_button.pressed) {
                message.handle();
                ok_button.handle();
            }
            activescreenclass->restore();
        }

        // []----------------------------------------------[]
        // |  handle the edit weapon and edit ammo buttons  |
        // []----------------------------------------------[]
        edit_objects.handle();
        if (edit_objects.pressed) do_edit_objects();


        // []---------------------[]
        // |  handle the menu box  |
        // []---------------------[]
        saved_games.handle();
        if (saved_games.currentchoice) {
            while (activemouseclass->button) saved_games.handle();
            break;
        }
    }


    // []-------------------------------------[]
    // |  return the correct save game number  |
    // []-------------------------------------[]
    for (i = 1, j = 0; i <= 10; i++) {
        char filename[MAXPATH];
        sprintf(filename, "GAME_%i\\SAVEINFO.DAT", i);
        if (file_exists(filename)) if (++j == saved_games.currentchoice) return(i);
    }
    return(0);
}
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
static int select_function(int savedgame)
{
    // []------------------[]
    // |  clear the screen  |
    // []------------------[]
    activescreenclass->drawrepeat('°', 80*25, 1, 1, COLOR(LIGHT_WHITE, LIGHT_BLUE));
    activescreenclass->refresh();


    // []------------------------------------------------[]
    // |  display info about the current game at the top  |
    // []------------------------------------------------[]
    {
        char *tempbuffer = new char[200];
        if (tempbuffer == NULL) no_memory();
        sprintf(tempbuffer, "save #%i -- ", savedgame);

        char filename[MAXPATH];
        sprintf(filename, "GAME_%i\\SAVEINFO.DAT", savedgame);
        FILE *fp = fopen(filename, "rb");
        if (fp) {
            fseek(fp, 2, SEEK_SET);
            fread(strchr(tempbuffer, 0), sizeof(char), 80, fp);
            fclose(fp);
        } else strcat(tempbuffer, "unknown description");
        activescreenclass->drawstring(tempbuffer, 1, \
            (80 - strlen(tempbuffer)) / 2, COLOR(LIGHT_WHITE, LIGHT_BLUE));
        delete tempbuffer;
    }

    // []------------------------------------[]
    // |  create all controls that we'll use  |
    // []------------------------------------[]
    buttontype money_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY),\
        3, 5, "Edit money");
    buttontype inventory_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY),\
        7, 5, "Edit base inventory");
    buttontype buildings_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY),\
        11, 5, "Edit base buildings");
    buttontype soldier_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY),\
        15, 5, "Edit soldiers");
    buttontype import_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY),\
        19, 5, "Import soldiers");

    buttontype submarine_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY),\
        3, 30, "Submarine Editor");
    buttontype diplomacy_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY),\
        7, 30, "Diplomacy Editor");
    buttontype acontainment_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY),\
        11, 30, "Alien Containment");
    buttontype tactical_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY),\
        15, 30, "Tactical Editor");
    buttontype knowledge_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY),\
        19, 30, "Increased knowledge");

    buttontype ok_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY),\
        3, 70, "OK");
    buttontype backup_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY),\
        15, 60, "Make backup");
    buttontype restore_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY),\
        19, 60, "Restore backup");


    // []-----------------[]
    // |  main input loop  |
    // []-----------------[]
    while (TRUE) {
        // ok button
        ok_button.handle();
        if (ok_button.pressed) return(0);

        // edit money button
        money_button.handle();
        if (money_button.pressed) return(1);

        // edit base inventory button
        inventory_button.handle();
        if (inventory_button.pressed) return(2);

        // edit base buildings button
        buildings_button.handle();
        if (buildings_button.pressed) return(3);

        // edit soldier button
        soldier_button.handle();
        if (soldier_button.pressed) return(4);

        // import soldiers button
        import_button.handle();
        if (import_button.pressed) return(5);

        // backup button
        backup_button.handle();
        if (backup_button.pressed) {
            if (ask_yes_no("\nDo you wish to make a backup now?\n(requires PKZIP.EXE to be in path)\n")) return(6);
        }

        // restore button
        restore_button.handle();
        if (restore_button.pressed) {
            if (ask_yes_no("\nDo you wish to restore your backup?\n(requires PKUNZIP.EXE to be in path)\n")) return(7);
        }

        // increased knowledge button
        knowledge_button.handle();
        if (knowledge_button.pressed) {
            if (ask_yes_no("Do you wish to have researched all\n"
                           "items, have all Ufopedia entires,\n"
                           "be able to produce all items?\n")) return(8);
        }

        // tactical editor button
        tactical_button.handle();
        if (tactical_button.pressed) return(9);

        // alien containment button
        acontainment_button.handle();
        if (acontainment_button.pressed) return(10);

        // diplomacy_button
        diplomacy_button.handle();
        if (diplomacy_button.pressed) return(11);

        // submarine button
        submarine_button.handle();
        if (submarine_button.pressed) return(12);
    }
}
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
static void read_base_names(int savedgame)
{
    int i;
    FILE *fp;
    char filename[MAXPATH];
    activemouseclass->setcursor(MOUSE_WAIT);


    // []--------------------------[]
    // |  process the LOC.DAT file  |
    // []--------------------------[]
    sprintf(filename, "GAME_%i\\LOC.DAT", savedgame);
    if ((fp = fopen(filename, "rb")) == NULL) file_error();
    int numlocs = filelength(fileno(fp)) / sizeof(struct locdat);
    for (i = numbases = 0; i < numlocs && numbases < 8; i++) {
        struct locdat loc;
        fread(&loc, sizeof(struct locdat), 1, fp);
        if (loc.activeif3 == 3) {
            baselisting[numbases].location = i;
            baselisting[numbases].basedatidx = loc.basedatidx;
            numbases++;
        }
    }
    fclose(fp);
    if (numbases == 0) return;


    // []------------------------------------------[]
    // |  now read in the base names from BASE.DAT  |
    // []------------------------------------------[]
    sprintf(filename, "GAME_%i\\BASE.DAT", savedgame);
    if ((fp = fopen(filename, "rb")) == NULL) file_error();
    for (i = 0; i < numbases; i++) {
        struct basedat base;
        fseek(fp, baselisting[i].basedatidx * sizeof(struct basedat), SEEK_SET);
        fread(&base, sizeof(struct basedat), 1, fp);
        memmove(&baselisting[i].name, &base.name, 16);
        baselisting[i].name[16] = 0;
    }
    fclose(fp);
    activemouseclass->setcursor(MOUSE_ARROW);
}
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// returns a number from 1 to 8, indicating the selected base
//
#define BASE_ROW 5
#define BASE_COL 20
static int select_base(void)
{
    // []------------------[]
    // |  clear the screen  |
    // []------------------[]
    activescreenclass->drawrepeat('°', 80*25, 1, 1, COLOR(LIGHT_WHITE, LIGHT_BLUE));
    activescreenclass->refresh();


    // []------------------------------------[]
    // |  create all controls that we'll use  |
    // []------------------------------------[]
    unsigned buffersize = 100, bufferused = 0; char *buffer;
    if ((buffer = new char[buffersize]) == NULL) no_memory();
    for (int i = 0; i < numbases; i++) {
        if (buffersize - bufferused < 20) {
            buffersize += 100;
            char *newbuffer = new char[buffersize];
            if (newbuffer == NULL) no_memory();
            memmove(newbuffer, buffer, bufferused);
            delete buffer; buffer = newbuffer;
        }
        bufferused += sprintf(buffer + bufferused, "%s\n", baselisting[i].name);
    }
    textboxtype message(COLOR(LIGHT_WHITE, BLUE), COLOR(LIGHT_YELLOW, BLUE), \
        BASE_ROW, BASE_COL, 30, 15, "\nSelect base to edit:\n");
    menuboxtype base_names(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, BLACK), \
        COLOR(GRAY, LIGHT_WHITE), BASE_ROW + 4, BASE_COL + 6, buffer);
    delete buffer;


    // []-----------------[]
    // |  main input loop  |
    // []-----------------[]
    while (TRUE) {
        message.handle();
        base_names.handle();
        if (base_names.currentchoice && !activemouseclass->button) break;
    }


    // []--------------------------[]
    // |  return the selected base  |
    // []--------------------------[]
    return(base_names.currentchoice);
}
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
