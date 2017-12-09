// []-----------------------------------------------------------------------[]
// |                                                                         |
// |                                                                         |
// |  This source and its associated executable are copyright by Jeff Lawson |
// |  of JL Enterprises, however some portions of this code may or may not   |
// |  have been originally inspired by other authors and credit is given if  |
// |  due.  You are encouraged to redistribute this package, but you must    |
// |  include all files originally distributed in this archive in their      |
// |  original state and format and without modifications of any kind.       |
// |  Under no circumstances may you make modifications to this product and  |
// |  redistribute the resulting code or executables.  If you believe you    |
// |  have made some useful or otherwise important enhancements and you      |
// |  would like to share these changes with others, please contact the      |
// |  author through one of the methods listed below.  Additionally, no      |
// |  fees may be charged for the usage of this product by anyone other than |
// |  the author of this file, except for fees (not to exceed US $10) by     |
// |  disk distributors to cover disk duplication and handling.  If there    |
// |  is any need to question this policy, please contact the author.        |
// |  This source code and its executable are distributed without any kind   |
// |  of warranty and the author may not be held accountable for damages of  |
// |  any kind.                                                              |
// |                                                                         |
// |                                                                         |
// |  I can reached via postal mail at:                                      |
// |                                                                         |
// |          Jeff Lawson                                                    |
// |          1893 Kaweah Drive                                              |
// |          Pasadena, CA 91105-2174                                        |
// |          USA                                                            |
// |                                                                         |
// |  or through E-mail at:                                                  |
// |                                                                         |
// |          JeffLawson@Aol.Com                                             |
// |                                                                         |
// |  also via phone at:                                                     |
// |                                                                         |
// |          (213) 258-5604                                                 |
// []-----------------------------------------------------------------------[]

#include "editor.h"


//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void tactical_editor(int savedgame)
{
    FILE *fp;
    char filename[MAXPATH];
    int i, j, k, modified = FALSE;
    struct unitref *units;
    char *unit_active;


    // []------------------[]
    // |  clear the screen  |
    // []------------------[]
    activescreenclass->drawrepeat('�', 80*25, 1, 1, COLOR(LIGHT_WHITE, LIGHT_BLUE));
    activescreenclass->refresh();


    // []-------------------------------------------[]
    // |  read in the saved game location indicator  |
    // []-------------------------------------------[]
    sprintf(filename, "GAME_%i\\SAVEINFO.DAT", savedgame);
    if ((fp = fopen(filename, "rb")) == NULL) file_error();
    fseek(fp, 0x26, SEEK_SET);
    i = getc(fp);
    fclose(fp);


    // []--------------------------------------------------------------[]
    // |  if the indicator is wrong, but the file exists, ask the user  |
    // []--------------------------------------------------------------[]
    sprintf(filename, "GAME_%i\\UNITREF.DAT", savedgame);
    if (i == 0) {
        if (file_exists(filename)) {
            if (!ask_yes_no("This game doesn't seem to saved\n"
                            "at a site, but I might be wrong.\n"
                            "Do you want to edit anyways?")) return;
        } else {
            ok_box("\nGame not saved at a site\n");
            return;
        }
    }


    // []----------------------------[]
    // |  read in all of the entries  |
    // []----------------------------[]
    if ((fp = fopen(filename, "rb")) == NULL) file_error();
    int total_units = filelength(fileno(fp)) / sizeof(struct unitref);
    if ((units = new struct unitref[total_units]) == NULL) no_memory();
    if ((unit_active = new char[total_units]) == NULL) no_memory();
    fread(units, sizeof(struct unitref), total_units, fp);
    fclose(fp);


    // []-------------------------------------[]
    // |  Create the controls that we'll need  |
    // []-------------------------------------[]
    unsigned buffersize = 100, bufferused = 0;
    char *buffer = new char[buffersize];
    if (buffer == NULL) no_memory();
    buffer[0] = 0;
    for (i = 0; i < total_units; i++) {
        int unit_active = (strlen(units[i].name) > 3) && (units[i].type != 0xFF);
        for (j = 0; j < 26 && units[i].name[j]; j++)
            if (!isalpha(units[i].name[j]) && units[i].name[j] != ' ') {unit_active = FALSE; break;}
        if (unit_active == FALSE) {
            units[i].type = 0xFF;
        } else {
            if (buffersize - bufferused < 35) {
                buffersize += 100;
                char *newbuffer = new char[buffersize];
                if (newbuffer == NULL) no_memory();
                memmove(newbuffer, buffer, buffersize);
                delete buffer; buffer = newbuffer;
            }
            char identifier[5];
            if (units[i].type <= 2) strcpy(identifier, "sold");
            else if (units[i].type == 3) strcpy(identifier, "tank");
            else if (units[i].type <= 14) strcpy(identifier, "enmy");
            else if (units[i].type <= 16) strcpy(identifier, "civ");
            else identifier[0] = 0;
            bufferused += sprintf(buffer + bufferused, "%-26.26s%4.4s\n",
                units[i].name, identifier);
        }
    }
    buttontype ok_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY), 2, 73, "OK");
    buttontype advanced_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY), 2, 45, "Advanced...");
    buttontype reveal_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY), 2, 60, "Show Map");
    menuboxtype unit_names(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, BLACK), \
        COLOR(GRAY, LIGHT_WHITE), 6, 45, 17, buffer);
    unit_names.currentchoice = 1;
    menuboxtype *unit_stats = NULL;
    delete buffer;


    // []-----------------[]
    // |  main input loop  |
    // []-----------------[]
    unsigned currentunit;
    while (TRUE) {
        // create a new unit stats box if needed
        if (unit_stats == NULL && unit_names.currentchoice) {
            for (i = 1; i <= 25; i++) activescreenclass->drawrepeat('�', 44, i, 1, COLOR(LIGHT_WHITE, LIGHT_BLUE));
            for (i = j = 0; i < total_units; j++) {
                if (units[j].type == 0xFF) continue;
                i++;
                unit_names.setchoiceinfo(i, FALSE, units[j].health == 0);
                if (i == unit_names.currentchoice) {
                    currentunit = j;
                    if ((buffer = new char[1000]) == NULL) no_memory();
                    if (sprintf(buffer, "time units:  %i/%i\n"
                                    "energy:  %i/%i\n"
                                    "health:  %i/%i\n"
                                    "fatal wounds:  %i/%i/%i/%i/%i/%i\n"
                                    "bravery:  %i\n"
                                    "morale:  %i\n"
                                    "reactions:  %i/%i\n"
                                    "firing accuracy:  %i%%\n"
                                    "throwing accuracy:  %i%%\n"
                                    "strength:  %i\n"
                                    "mc skill:  %i\n"
                                    "mc strength:  %i\n"
                                    "shok:  %i\n"
                                    "armour (front):  %i/%i\n"
                                    "armour (left):  %i/%i\n"
                                    "armour (right):  %i/%i\n"
                                    "armour (rear):  %i/%i\n"
                                    "armour (under):  %i/%i\n",
                        units[currentunit].time, units[currentunit].timemax,
                        units[currentunit].energy, units[currentunit].energymax,
                        units[currentunit].health, units[currentunit].healthmax,
                        units[currentunit].fatal[0], units[currentunit].fatal[1], units[currentunit].fatal[2], units[currentunit].fatal[3], units[currentunit].fatal[4], units[currentunit].fatal[5],
                        110 - 10 * units[currentunit].bravery,
                        units[currentunit].morale,
                        units[currentunit].reactions, units[currentunit].reactionsmax,
                        units[currentunit].firing,
                        units[currentunit].throwing,
                        units[currentunit].strength,
                        units[currentunit].mcskill,
                        units[currentunit].mcstrength,
                        units[currentunit].shok,
                        units[currentunit].armour[0], units[currentunit].armourmax[0],
                        units[currentunit].armour[1], units[currentunit].armourmax[1],
                        units[currentunit].armour[2], units[currentunit].armourmax[2],
                        units[currentunit].armour[3], units[currentunit].armourmax[3],
                        units[currentunit].armour[4], units[currentunit].armourmax[4]) > 1000) memory_corrupt();
                    if ((unit_stats = new menuboxtype(COLOR(BLACK, LIGHT_WHITE), \
                        COLOR(LIGHT_WHITE, BLACK), COLOR(GRAY, LIGHT_WHITE), \
                        3, 5, buffer)) == NULL) no_memory();
                    delete buffer;
                } /* endif(i == unit_names.currentchoice) */
            }
        }

        // Update the list of unit names
        i = unit_names.currentchoice;
        unit_names.handle();
        if (unit_names.currentchoice != i && unit_stats) {
            if (unit_names.currentchoice > 0) {
                // user has selected a new one--we need to refresh now
                delete unit_stats;
                unit_stats = NULL;
            } else {
                // don't let the user unselect it
                unit_names.currentchoice = i;
                unit_names.draw();
                activescreenclass->refresh();
            }
        }

        // Handle the unit stats box
        if (unit_stats) {
            unit_stats->handle();
            switch (unit_stats->currentchoice) {
                case 1: // time
                    stat_edit(units[currentunit].time, 0, 0xFF, "\nNumber of time units?\n", 180);
                    stat_edit(units[currentunit].timemax, 0, 0xFF, "\nNumber of max time units?\n", 180);
                    delete unit_stats; unit_stats = NULL; modified = TRUE;
                    break;
                case 2: // energy
                    stat_edit(units[currentunit].energy, 0, 0xFF, "\nAmount for energy?\n", 180);
                    stat_edit(units[currentunit].energymax, 0, 0xFF, "\nAmount for max energy?\n", 180);
                    delete unit_stats; unit_stats = NULL; modified = TRUE;
                    break;
                case 3: // health
                    stat_edit(units[currentunit].health, 0, 0xFF, "\nAmount for health?\n", 180);
                    stat_edit(units[currentunit].healthmax, 0, 0xFF, "\nAmount for max health?\n", 180);
                    delete unit_stats; unit_stats = NULL; modified = TRUE;
                    break;
                case 4: // fatal wounds
                    i = generic_button_pick("Select armour region:", "Head","Torso","Rarm", "Larm", "Rleg", "Lleg") + 1;
                    stat_edit(units[currentunit].fatal[i], 0, 0xFF, "\nAmount for fatal wounds?\n", 0);
                    delete unit_stats; unit_stats = NULL; modified = TRUE;
                    break;
                case 5: // bravery
                    i = 110 - 10 * units[currentunit].bravery;
                    stat_edit((unsigned) i, 20, 100, "\nAmount for bravery?\n", 100);
                    units[currentunit].bravery = 11 - i / 10;
                    delete unit_stats; unit_stats = NULL; modified = TRUE;
                    break;
                case 6: // morale
                    stat_edit(units[currentunit].morale, 0, 0xFF, "\nAmount for morale?\n", 180);
                    delete unit_stats; unit_stats = NULL; modified = TRUE;
                    break;
                case 7: // reactions
                    stat_edit(units[currentunit].reactions, 0, 0xFF, "\nAmount for reactions?\n", 180);
                    stat_edit(units[currentunit].reactionsmax, 0, 0xFF, "\nAmount for max reactions?\n", 180);
                    delete unit_stats; unit_stats = NULL; modified = TRUE;
                    break;
                case 8: // firing accuracy
                    stat_edit(units[currentunit].firing, 0, 0xFF, "\nAmount for firing?\n", 180);
                    delete unit_stats; unit_stats = NULL; modified = TRUE;
                    break;
                case 9: // throwing accuracy
                    stat_edit(units[currentunit].throwing, 0, 0xFF, "\nAmount for throwing?\n", 180);
                    delete unit_stats; unit_stats = NULL; modified = TRUE;
                    break;
                case 10: // strength
                    stat_edit(units[currentunit].strength, 0, 0xFF, "\nAmount for strength?\n", 50);
                    delete unit_stats; unit_stats = NULL; modified = TRUE;
                    break;
                case 11: // mc skill
                    stat_edit(units[currentunit].mcskill, 0, 0xFF, "\nAmount for mc skill?\n", 180);
                    delete unit_stats; unit_stats = NULL; modified = TRUE;
                    break;
                case 12: // mc strength
                    stat_edit(units[currentunit].mcstrength, 0, 0xFF, "\nAmount for mc strength?\n", 180);
                    delete unit_stats; unit_stats = NULL; modified = TRUE;
                    break;
                case 13: // shok
                    stat_edit(units[currentunit].shok, 0, 0xFF, "\nAmount for shok?\n", 180);
                    delete unit_stats; unit_stats = NULL; modified = TRUE;
                    break;
                case 14: // armour front
                case 15: // armour left
                case 16: // armour right
                case 17: // armour rear
                case 18: // armour under
                    stat_edit(units[currentunit].armour[unit_stats->currentchoice-14], 0, 0xFF, "\nAmount for armour?\n", 0xFF);
                    stat_edit(units[currentunit].armourmax[unit_stats->currentchoice-14], 0, 0xFF, "\nAmount for max armour?\n", 0xFF);
                    delete unit_stats; unit_stats = NULL; modified = TRUE;
                    break;
                default: break;
            }
        }

        // Handle the buttons
        ok_button.handle();
        if (ok_button.pressed) break;

        // Handle the advanced button
        advanced_button.handle();
        if (advanced_button.pressed) {
            menuboxtype *choices = NULL;
            char *savebuffer = new char[activescreenclass->buffersize];
            if (savebuffer == NULL) no_memory();
            activescreenclass->save(savebuffer);
            activescreenclass->drawrepeat('�', 80*25, 1, 1, COLOR(LIGHT_WHITE, LIGHT_BLUE));
            ok_button.draw();
                  while (TRUE) {
                      if (choices == NULL) {
                    unsigned bufferused = 0, buffersize = 1000; char *buffer = new char[buffersize];
                    if (buffer == NULL) no_memory();
                    bufferused += sprintf(buffer+bufferused, "type: %u\n", units[currentunit].type);
                    bufferused += sprintf(buffer+bufferused, "heading: %u\n", units[currentunit].heading);
                    bufferused += sprintf(buffer+bufferused, "rank/alien rank: %u/%u\n", units[currentunit].rank, units[currentunit].alienrank);
                    bufferused += sprintf(buffer+bufferused, "corpse_icon: %u\n", units[currentunit].corpse_icon);
                    bufferused += sprintf(buffer+bufferused, "unitnum: %u\n", units[currentunit].unitnum);
                    bufferused += sprintf(buffer+bufferused, "left/right hand: %u/%u\n", units[currentunit].lefthand, units[currentunit].righthand);
                    bufferused += sprintf(buffer+bufferused, "unk0: ");
                        for (i = 0; i < 9; i++) bufferused += sprintf(buffer+bufferused, "%02X ", units[currentunit].unk0[i]);
                    bufferused += sprintf(buffer+bufferused, "\nunk1: ");
                        for (i = 0; i < 3; i++) bufferused += sprintf(buffer+bufferused, "%02X ", units[currentunit].unk1[i]);
                    bufferused += sprintf(buffer+bufferused, "\nunk2: %02X ", units[currentunit].unk2);
                    bufferused += sprintf(buffer+bufferused, "\nunk3: %02X ", units[currentunit].unk3);
                    bufferused += sprintf(buffer+bufferused, "\nunk4: ");
                        for (i = 0; i < 10; i++) bufferused += sprintf(buffer+bufferused, "%02X ", units[currentunit].unk4[i]);
                    bufferused += sprintf(buffer+bufferused, "\nunk5: ");
                        for (i = 0; i < 2; i++) bufferused += sprintf(buffer+bufferused, "%02X ", units[currentunit].unk5[i]);
                    bufferused += sprintf(buffer+bufferused, "\nunk6: ");
                        for (i = 0; i < 3; i++) bufferused += sprintf(buffer+bufferused, "%02X ", units[currentunit].unk6[i]);
                    bufferused += sprintf(buffer+bufferused, "\nunk7: ");
                        for (i = 0; i < 17; i++) bufferused += sprintf(buffer+bufferused, "%02X ", units[currentunit].unk7[i]);
                    bufferused += sprintf(buffer+bufferused, "\nunk8: ");
                        for (i = 0; i < 20; i++) bufferused += sprintf(buffer+bufferused, "%02X ", units[currentunit].unk8[i]);
                    bufferused += sprintf(buffer+bufferused, "\n");
                    if (bufferused >= buffersize) memory_corrupt();
                       if ((choices = new menuboxtype(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, BLACK), \
                           COLOR(GRAY, LIGHT_WHITE), 2, 1, buffer)) == NULL) no_memory();
                    delete buffer;
                } else {
                          choices->handle();
                    if (choices->currentchoice && !activemouseclass->button) {
                        switch (choices->currentchoice) {
                            case 1: // type
                                stat_edit(units[currentunit].type, 0, 0xFF, "\nAmount for type?\n", 180);
                                break;
                            case 2: // heading
                                stat_edit(units[currentunit].heading, 0, 0xFF, "\nAmount for heading?\n", 180);
                                break;
                            case 3: // rank/alien rank
                                units[currentunit].rank = generic_button_pick("Pick new human rank:", "dead", "seaman", "able", "ensign", "lt", "cmdr", "capn");
                                units[currentunit].alienrank = 2 + generic_button_pick("Pick new alien rank:", "Cmndr","Navigtr","Medic","Tech","Leadr","Sold","Terist");
                                break;
                            case 4: // corpse_icon
                                stat_edit(units[currentunit].corpse_icon, 0, 0xFF, "\nAmount for corpse_icon?\n", 180);
                                break;
                            case 5: // unitnum
                                stat_edit(units[currentunit].unitnum, 0, 0xFF, "Value for unitnum?\n(MAX to dissociate)\n", 0xFF);
                                break;
                            case 6: // left/right hand
                                stat_edit(units[currentunit].lefthand, 0, 0xFF, "Value for left hand?\n(MAX for nothing)\n", 0xFF);
                                stat_edit(units[currentunit].righthand, 0, 0xFF, "Value for right hand?\n(MAX for nothing)\n", 0xFF);
                                break;
                            default: break;
                        }
                        modified = TRUE; delete choices; choices = NULL;
                    }
                }
                ok_button.handle();
                if (ok_button.pressed) break;
            }
            if (choices) delete choices;
            activescreenclass->restore(savebuffer);
            delete savebuffer;
        }

        // handle the reveal map button
        reveal_button.handle();
        if (reveal_button.pressed) {
            if (ask_yes_no("\nShould I reveal the entire map?\n")) {
                FILE *fp;
                char filename[MAXPATH];
                long length;

                sprintf(filename, "GAME_%i\\SEEMAP.DAT", savedgame);
                if ((fp = fopen(filename, "r+b")) == NULL) file_error();
                length = filelength(fileno(fp));
                while (length--) putc(7, fp);
                fclose(fp);

                sprintf(filename, "GAME_%i\\SOURCEMP.DAT", savedgame);
                if ((fp = fopen(filename, "r+b")) == NULL) file_error();
                length = filelength(fileno(fp));
                while (length--) putc(1, fp);
                fclose(fp);

                sprintf(filename, "GAME_%i\\UNITPOS.DAT", savedgame);
                if ((fp = fopen(filename, "r+b")) == NULL) file_error();
                length = filelength(fileno(fp)) / sizeof(struct unitpos);
                struct unitpos unit_pos;
                for (i = 0; i < length; i++) {
                    fseek(fp, i * sizeof(struct unitpos), SEEK_SET);
                    fread(&unit_pos, sizeof(struct unitpos), 1, fp);
                    unit_pos.sighted |= 1;
                    unit_pos.visible |= 1;
                    fseek(fp, i * sizeof(struct unitpos), SEEK_SET);
                    fwrite(&unit_pos, sizeof(struct unitpos), 1, fp);
                }
                fclose(fp);
                ok_box("\nThe map has been revealed\n");
            }
        }
    }


    // []-----------------------------------[]
    // |  see if we should save the changes  |
    // []-----------------------------------[]
    if (modified) {
        if (ask_yes_no("\nDo you wish to save these changes?\n")) {
            if ((fp = fopen(filename, "wb")) == NULL) file_error();
            fwrite(units, sizeof(struct unitref), total_units, fp);
            fclose(fp);
        }
    }


    // []-----------------[]
    // |  free everything  |
    // []-----------------[]
    if (unit_stats) delete unit_stats;
    delete units;
    delete unit_active;
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
