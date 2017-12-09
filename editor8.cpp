// Xcom2edi
// Copyright (c) 1997, Jeff Lawson <jeff@bovine.net>
// All rights reserved.
// See LICENSE for distribution information.

#include "editor.h"

char *subitemdescription[] = {"Coelacanth/G.cannon", "Coelacanth/Aquajet",
    "Coelacanth/Gauss", "Displacer/Sonic", "Displacer/P.W.T.", "Dart gun",
    "Dart clip", "Jet Harpoon", "Jet clip", "Gas Cannon", "GC-AP Bolts",
    "GE-HE Bolts", "GC-Phosphorous Bolts", "Hydrojet Cannon", "HJ-AP ammo",
    "HJ-HE ammo", "HJ-P ammo", "Torpedo Launcher", "Small Torpedo",
    "Large Torpedo", "Phosphorous Torpedo", "Gauss Pistol", "Gauss Rifle",
    "Heavy gauss", "Magna-Blast Grenade", "Dye Grenade",
    "Particle Disturbance Grenade", "Magna-Pack Explosive",
    "Particle Disturbance Sensor", "Medi-Kit", "M.C. Disruptor",
    "Thermal Tazer", "Chemical-Flare", "Vibro Blade", "Thermic Lance",
    "Heavy Thermic Lance", "  unknown", "  unknown", "  unknown",
    "Sonic Cannon", "Cannon Powerclip", "Sonic-Blasta Rifle",
    "Blasta Power Clip", "Sonic Pistol", "Pistol Power Clip",
    "Disruptor Pulse Launcher", "Disruptor Ammo", "Thermal Shok Launcher",
    "Thermal Shok Bomb", "Sonic Pulser", "  unknown", "M.C. Reader",
    "Gauss Pistol Clip", "Gauss Rifle Clip", "Heavy Gauss Clip"};
#define NUMSUBITEMS 55

char *subtypenames[] = {"Triton", "Hammerhead", "Leviathan", "Baracuda",
    "Manta", "Alien Survey", "Alien Escort", "Alien Cruiser",
    "Alien H-Cruiser", "Alien Hunter", "Alien Battleship",
    "Alien Dreadnought", "Alien Fleet Supply"};
#define DESCRIBESHIP(i) (i < 13 ? subtypenames[i] : "unknown")

char *subweaponnames[] = {"Ajax", "DUP Head", "Craft Gas Cannon",
    "PWT Cannon", "Gauss Cannon", "Sonic Oscillator"};
#define DESCRIBEWEAPON(i) (i == 0xFF ? "none" : \
    (i < 6 ? subweaponnames[i] : "unknown") )

void edit_sub_manifest(struct craft *submarine);


//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void submarine_editor(int savedgame)
{
    FILE *fp;
    char filename[MAXPATH];
    int i, j, k, modified = FALSE;
    struct craft *submarine;


    // []------------------[]
    // |  clear the screen  |
    // []------------------[]
    activescreenclass->drawrepeat('�', 80*25, 1, 1, COLOR(LIGHT_WHITE, LIGHT_BLUE));
    activescreenclass->refresh();


    // []----------------------------[]
    // |  read in all of the entries  |
    // []----------------------------[]
    sprintf(filename, "GAME_%i\\CRAFT.DAT", savedgame);
    if ((fp = fopen(filename, "rb")) == NULL) file_error();
    int totalcrafts = filelength(fileno(fp)) / sizeof(struct craft);
    if ((submarine = new struct craft[totalcrafts]) == NULL) no_memory();
    fread(submarine, sizeof(struct craft), totalcrafts, fp);
    fclose(fp);


    // []---------------------------------------[]
    // |  Create all of the controls we'll need  |
    // []---------------------------------------[]
    buttontype ok_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY), 2, 73, "OK");
    buttontype manifest_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY), 2, 58, "Manifest");
    menuboxtype *subnames = NULL;
    menuboxtype *shipdetails = NULL;


    // []------------------[]
    // |  main editor loop  |
    // []------------------[]
    while (TRUE) {
        // []---------------------------------------[]
        // |  rebuild a new subnames box, if needed  |
        // []---------------------------------------[]
        if (subnames == NULL) {
            // generate the buffer containing the names
            unsigned buffersize = 100, bufferused = 0;
            char *buffer = new char[buffersize];
            if (buffer == NULL) no_memory();
            buffer[0] = 0;
            for (i = 0; i < totalcrafts; i++) {
                if (submarine[i].shiptype != 0xFF) {
                    if (buffersize - bufferused < 35) {
                        buffersize += 100;
                        char *newbuffer = new char[buffersize];
                        if (newbuffer == NULL) no_memory();
                        memmove(newbuffer, buffer, buffersize);
                        delete buffer; buffer = newbuffer;
                    }
                    bufferused += sprintf(buffer + bufferused,
                        "Ship %i (%s)\n", i + 1,
                        DESCRIBESHIP(submarine[i].shiptype));
                }
            }

            // generate the menubox containing the names
            if ((subnames = new menuboxtype(COLOR(BLACK, LIGHT_WHITE), \
                COLOR(LIGHT_WHITE, BLACK), COLOR(GRAY, LIGHT_WHITE), \
                6, 49, 17, buffer)) == NULL) no_memory();
            delete buffer;
        }


        // []-----------------------------------------------[]
        // |  generate a new ship details box, if necessary  |
        // []-----------------------------------------------[]
        if (shipdetails == NULL && subnames->currentchoice) {
            int buffersize = 300;
            char *buffer = new char[buffersize];
            if (buffer == NULL) no_memory();
            i = subnames->currentchoice - 1;
            if (sprintf(buffer, "ship type:  %-25s\n"
                            "left weapon type:  %s\n"
                            "left ammo count: %u\n"
                            "right weapon type:  %s\n"
                            "right ammo count: %u\n"
                            "fuel remaining: %u\n",
                    DESCRIBESHIP(submarine[i].shiptype),
                    DESCRIBEWEAPON(submarine[i].leftammotype),
                    unsigned(submarine[i].leftammocount),
                    DESCRIBEWEAPON(submarine[i].rightammotype),
                    unsigned(submarine[i].rightammocount),
                    submarine[i].fuelremaining) >= buffersize) no_memory();
            if ((shipdetails = new menuboxtype(COLOR(BLACK, LIGHT_WHITE), \
                COLOR(LIGHT_WHITE, BLACK), COLOR(GRAY, LIGHT_WHITE), \
                6, 3, 17, buffer)) == NULL) no_memory();
            delete buffer;
        }


        // []------------------------------[]
        // |  handle the subnames menu box  |
        // []------------------------------[]
        if (subnames) {
            int oldchoice = subnames->currentchoice;
            subnames->handle();
            if (oldchoice != subnames->currentchoice) {
                if (shipdetails) {
                    delete shipdetails;
                    shipdetails = NULL;
                }
            }
        }


        // []-----------------------------[]
        // |  handle the ship details box  |
        // []-----------------------------[]
        if (shipdetails) {
            shipdetails->handle();
            if (shipdetails->currentchoice && !activemouseclass->button) {
                i = subnames->currentchoice - 1;
                switch(shipdetails->currentchoice) {
                    case 1: // ship type
                        break;
                    case 2: // left weapon type
                        submarine[i].leftammotype = generic_button_pick \
                            ("Select new left weapon", "none", "Ajax", \
                            "DUP", "Gas", "PWT", "Gauss", "Sonic");
                        break;
                    case 3: // left ammo count
                        stat_edit(submarine[i].leftammocount, 0, 255, \
                            "Enter desired left ammo count:", 100);
                        break;
                    case 4: // right weapon type
                        submarine[i].rightammotype = generic_button_pick \
                            ("Select new right weapon", "none", "Ajax", \
                            "DUP", "Gas", "PWT", "Gauss", "Sonic");
                        break;
                    case 5: // right ammo count
                        stat_edit(submarine[i].rightammocount, 0, 255, \
                            "Enter desired right ammo count:", 100);
                        break;
                    case 6: // fuel remaining
                        stat_edit(submarine[i].fuelremaining, 0, 0x7FFF, \
                            "Enter desired fuel remaining:", 0x4000);
                        break;
                    default: break;
                }
                modified = TRUE;
                delete shipdetails; shipdetails = NULL;
            }
        }


        // []----------------------[]
        // |  handle the ok button  |
        // []----------------------[]
        ok_button.handle();
        if (ok_button.pressed) break;


        // []----------------------------[]
        // |  handle the manifest button  |
        // []----------------------------[]
        manifest_button.handle();
        if (subnames->currentchoice && manifest_button.pressed) {
            edit_sub_manifest(&submarine[subnames->currentchoice - 1]);
            modified = TRUE;
        }
    }



    // []-----------------------------------[]
    // |  see if we should save the changes  |
    // []-----------------------------------[]
    if (modified) {
        if (ask_yes_no("\nDo you wish to save these changes?\n")) {
            if ((fp = fopen(filename, "wb")) == NULL) file_error();
            fwrite(submarine, sizeof(struct craft), totalcrafts, fp);
            fclose(fp);
        }
    }


    // []-----------------[]
    // |  free everything  |
    // []-----------------[]
    delete submarine;
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void edit_sub_manifest(struct craft *submarine)
{
    int i;

    // []-------------------[]
    // |  Set up the screen  |
    // []-------------------[]
    char *savebuffer = new char[activescreenclass->buffersize];
    if (savebuffer == NULL) no_memory();
    activescreenclass->save(savebuffer);


    // []---------------------------------------[]
    // |  Create all of the controls we'll need  |
    // []---------------------------------------[]
    #define MANIFEST_ROW 2
    #define MANIFEST_COL 15
    textboxtype message(COLOR(LIGHT_WHITE, BLUE), COLOR(LIGHT_YELLOW, BLUE), \
        MANIFEST_ROW, MANIFEST_COL, 44, 23, "");
    buttontype ok_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY),
        MANIFEST_ROW + 19, MANIFEST_COL + 8, "OK");
    buttontype changeall_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY),
        MANIFEST_ROW + 19, MANIFEST_COL + 21, "Change All");
    menuboxtype *itemlist = NULL;
    int autotoprow = 0;


    // []-----------[]
    // |  Main loop  |
    // []-----------[]
    while (TRUE) {
        // []------------------------------------------[]
        // |  Regenerate a new item list, if necessary  |
        // []------------------------------------------[]
        if (itemlist == NULL) {
            // generate the buffer containing the names
            unsigned buffersize = 100, bufferused = 0;
            char *buffer = new char[buffersize];
            if (buffer == NULL) no_memory();
            buffer[0] = 0;
            for (i = 0; i < NUMSUBITEMS; i++) {
                if (buffersize - bufferused < 45) {
                    buffersize += 300;
                    char *newbuffer = new char[buffersize];
                    if (newbuffer == NULL) no_memory();
                    memmove(newbuffer, buffer, buffersize);
                    delete buffer; buffer = newbuffer;
                }
                bufferused += sprintf(buffer + bufferused,
                    "%-30.30s %3u\n", subitemdescription[i],
                    submarine->inventory[i]);
            }

            // generate the menubox containing the names
            if ((itemlist = new menuboxtype(COLOR(BLACK, LIGHT_WHITE), \
                COLOR(LIGHT_WHITE, BLACK), COLOR(GRAY, LIGHT_WHITE), \
                MANIFEST_ROW + 2, MANIFEST_COL + 3, 14, buffer)) == NULL) \
                no_memory();
            if (autotoprow) itemlist->toprow = autotoprow;
            delete buffer;
        }


        // []------------------------[]
        // |  Handle the message box  |
        // []------------------------[]
        message.handle();


        // []----------------------[]
        // |  Handle the item list  |
        // []----------------------[]
        if (itemlist) {
            itemlist->handle();
            if (itemlist->currentchoice && !activemouseclass->button) {
                autotoprow = itemlist->toprow;
                i = itemlist->currentchoice - 1;
                stat_edit(submarine->inventory[i], 0, 255, \
                    "Enter desired amount:", 15);
                delete itemlist;
                itemlist = NULL;
            }
        }


        // []----------------------[]
        // |  Handle the OK button  |
        // []----------------------[]
        ok_button.handle();
        if (ok_button.pressed) break;


        // []------------------------------[]
        // |  Handle the change all button  |
        // []------------------------------[]
        changeall_button.handle();
        if (changeall_button.pressed) {
            unsigned count = 0;
            if (!stat_edit(count, 0, 255, "Enter desired amount:", 15)) {
                autotoprow = itemlist->toprow;
                for (i = 0; i < NUMSUBITEMS; i++)
                    submarine->inventory[i] = count;
                delete itemlist;
                itemlist = NULL;
            }
        }
    }


    // []-----------------[]
    // |  Free everything  |
    // []-----------------[]
    if (itemlist) delete itemlist;
    activescreenclass->restore(savebuffer);
    delete savebuffer;
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
