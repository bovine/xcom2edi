// Xcom2edi
// Copyright (c) 1997, Jeff Lawson <jeff@bovine.net>
// All rights reserved.
// See LICENSE for distribution information.

#include "editor.h"


static void edit_all_soldiers(int &rank, int &location, int &armour, int &missions, \
    int &kills, int &recovery, int &timeunits, int &stamina, int &health, \
    int &bravery, int &reactions, int &firing, int &throwing, int &strength, \
    int &mcskill, int &mcstrength);

//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
// savedgame is a number from 1 to 10 and is the saved game number to edit
//
void edit_soldiers(int savedgame)
{
    char filename[MAXPATH]; FILE *fp; int i, j, modified = FALSE;
    struct soldier_type *soldiers;
    activemouseclass->setcursor(MOUSE_WAIT);


    // []------------------[]
    // |  clear the screen  |
    // []------------------[]
    activescreenclass->drawrepeat('�', 80*25, 1, 1, COLOR(LIGHT_WHITE, LIGHT_BLUE));
    activescreenclass->refresh();


    // []------------------------------------[]
    // |  create all controls that we'll use  |
    // []------------------------------------[]
    buttontype ok_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY), 2, 50, "OK");
    buttontype changeall_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY), 7, 31, "Change all at\nchecked bases");
    menuboxtype *soldier_names = NULL, *statistics = NULL;
    checkboxtype *base_box[8];
    dropdowntype *name = NULL;
    dropdowntype rank(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, BLACK),\
        3, 13, 15, "dead\nseaman\nable seaman\nensign\nlieutenant\ncommander\ncaptain\n");
    dropdowntype armour(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, BLACK),\
        5, 13, 20, "none\nplastic aqua armour\nion armour\nmagnetic ion armour\n");


    // []--------------------------------------[]
    // |  create as many base boxes as we need  |
    // []--------------------------------------[]
    unsigned buffersize = 100, bufferused = 0; char *buffer;
    if ((buffer = new char[buffersize]) == NULL) no_memory();
    for (i = 0; i < numbases; i++) {
        base_box[i] = new checkboxtype(COLOR(BLACK, LIGHT_WHITE), \
            16 + i, 31, (char *)baselisting[i].name);
        if (buffersize - bufferused < 20) {
            buffersize += 100;
            char *newbuffer = new char[buffersize];
            if (newbuffer == NULL) no_memory();
            memmove(newbuffer, buffer, bufferused);
            delete buffer; buffer = newbuffer;
        }
        bufferused += sprintf(buffer + bufferused, "%s\n", baselisting[i].name);
    }
    dropdowntype location(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, BLACK),\
        4, 13, 15, buffer);
    delete buffer;


    // []----------------------[]
    // |  read in the soldiers  |
    // []----------------------[]
    if ((soldiers = new struct soldier_type[250]) == NULL) no_memory();
    sprintf(filename, "GAME_%i\\SOLDIER.DAT", savedgame);
    if ((fp = fopen(filename, "rb")) == NULL) file_error();
    fread(soldiers, sizeof(struct soldier_type), 250, fp);
    fclose(fp);


    // []-----------------------------[]
    // |  validate the soldier fields  |
    // []-----------------------------[]
    for (i = 0; i < 250; i++) {
        soldiers[i].name[25] = 0;
        for (j = 0; j < strlen(soldiers[i].name); j++) \
            if (!isalpha(soldiers[i].name[j]) && soldiers[i].name[j] != ' ') soldiers[i].name[0] = 0;
        if (soldiers[i].rank > 6) soldiers[i].rank = 0xFFFF;
        for (int x = 0, flag = FALSE; x < numbases; x++) {
            if (soldiers[i].location == baselisting[x].location) flag = TRUE;
        }
        if (flag == FALSE) soldiers[i].location = baselisting[0].location;
        soldiers[i].time += soldiers[i].inctime; soldiers[i].inctime = 0;
        soldiers[i].health += soldiers[i].inchealth; soldiers[i].inchealth = 0;
        soldiers[i].stamina += soldiers[i].incstamina; soldiers[i].incstamina = 0;
        soldiers[i].reactions += soldiers[i].increactions; soldiers[i].increactions = 0;
        soldiers[i].strength += soldiers[i].incstrength; soldiers[i].incstrength = 0;
        soldiers[i].firing += soldiers[i].incfiring; soldiers[i].incfiring = 0;
        soldiers[i].throwing += soldiers[i].incthrowing; soldiers[i].incthrowing = 0;
        soldiers[i].incbravery = 0;
        if (soldiers[i].bravery > 11) soldiers[i].bravery = 11;
        if (soldiers[i].bravery == 0) soldiers[i].bravery = 1;
        if (soldiers[i].armour > 3) soldiers[i].armour = 0;
    }


    // []-----------------[]
    // |  main input loop  |
    // []-----------------[]
    activemouseclass->setcursor(MOUSE_ARROW);
    int regenerate_list = TRUE, regenerate_soldier = TRUE, soldier_number = -1;
    while (TRUE) {
        // []------------------------------------------[]
        // |  build new page of soldiers, if necessary  |
        // []------------------------------------------[]
        if (regenerate_list) {
            // delete old list, if necessary
            if (soldier_names) {delete soldier_names; soldier_names = NULL;}
            // erase screen
            for (i = 6; i <= 24; i++) activescreenclass->drawrepeat('�', 30, \
                i, 50, COLOR(LIGHT_WHITE, LIGHT_BLUE));
            activescreenclass->refresh();
            // generate new list buffer
            unsigned buffersize = 100, bufferused = 0; char *buffer;
            char dead[250]; unsigned numactive = 0;
            if ((buffer = new char[buffersize]) == NULL) no_memory();
            soldier_number = -1;
            for (i = 0; i < 250; i++) {
                if (!soldiers[i].name[0]) continue;
                for (j = 0; j < numbases; j++) {
                    if (soldiers[i].location == baselisting[j].location && base_box[j]->checked) {
                        if (buffersize - bufferused < 30) {
                            buffersize += 200;
                            char *newbuffer = new char[buffersize];
                            if (newbuffer == NULL) no_memory();
                            memmove(newbuffer, buffer, bufferused);
                            delete buffer; buffer = newbuffer;
                        }
                        if (soldier_number == -1) soldier_number = i;
                        bufferused += sprintf(buffer + bufferused, "%-25.25s\n", soldiers[i].name);
                        dead[numactive++] = soldiers[i].rank == 0xFF;
                        break;
                    }
                }
            }
            // create new list
            if (bufferused > 0) {
                soldier_names = new menuboxtype(COLOR(BLACK, LIGHT_WHITE), \
                    COLOR(LIGHT_WHITE, BLACK), COLOR(GRAY, LIGHT_WHITE), 6, 50, 17, buffer);
                for (i = 0; i < numactive; i++) {
                    soldier_names->setchoiceinfo(i + 1, FALSE, dead[i]);
                }
                soldier_names->currentchoice = 1;
            }
            regenerate_list = FALSE; delete buffer;
            regenerate_soldier = TRUE;
        }


        // []---------------------------------------------[]
        // |  regenerate soldier statistics, if necessary  |
        // []---------------------------------------------[]
        if (regenerate_soldier) {
            // delete the old statistics, if necessary
            if (statistics) {delete statistics; statistics = NULL;}
            // erase the screen
            for (i = 1; i < 7; i++) activescreenclass->drawrepeat('�', 45, \
                i, 1, COLOR(LIGHT_WHITE, LIGHT_BLUE));
            for (i = 7; i <= 25; i++) activescreenclass->drawrepeat('�', 30, \
                i, 1, COLOR(LIGHT_WHITE, LIGHT_BLUE));
            activescreenclass->refresh();
            // create the new statistics
            if (soldier_number >= 0) {
                // update name
                activescreenclass->drawstring("name:", 2, 6, COLOR(BLACK, LIGHT_WHITE));
                char namebuffer[60]; sprintf(namebuffer, "%s\nModify name...\n", soldiers[soldier_number].name);
                if (name) delete name;
                if ((name = new dropdowntype(COLOR(BLACK, LIGHT_WHITE), \
                    COLOR(LIGHT_WHITE, BLACK), 2, 13, 25, namebuffer)) == NULL) no_memory();
                // update rank
                activescreenclass->drawstring("rank:", 3, 6, COLOR(BLACK, LIGHT_WHITE));
                rank.currentchoice = soldiers[soldier_number].rank + 2;
                rank.needupdate = TRUE;
                // update location
                activescreenclass->drawstring("location:", 4, 2, COLOR(BLACK, LIGHT_WHITE));
                for (i = 0; i < numbases; i++) {
                    if (soldiers[soldier_number].location == baselisting[i].location) {
                        location.currentchoice = i + 1;
                        break;
                    }
                }
                location.needupdate = TRUE;
                // update armour
                activescreenclass->drawstring("armour:", 5, 4, COLOR(BLACK, LIGHT_WHITE));
                armour.currentchoice = soldiers[soldier_number].armour + 1;
                armour.needupdate = TRUE;
                // update statistics box
                char *buffer = new char[350];
                if (buffer == NULL) no_memory();
                if (sprintf(buffer, "missions: %u\n"
                                "kills: %u\n"
                                "recovery: %u days\n\n"
                                "time units: %hu\n"
                                "stamina: %hu\n"
                                "health: %hu\n"
                                "bravery: %hu\n"
                                "reactions: %hu\n"
                                "firing accuracy: %hu\n"
                                "throwing accuracy: %hu\n"
                                "strength: %hu\n"
                                "mc skill: %hu\n"
                                "mc strength: %hu\n",
                    soldiers[soldier_number].missions, soldiers[soldier_number].kills,
                    soldiers[soldier_number].recovery, soldiers[soldier_number].time,
                    soldiers[soldier_number].stamina, soldiers[soldier_number].health,
                    110 - 10 * soldiers[soldier_number].bravery,
                    soldiers[soldier_number].reactions, soldiers[soldier_number].firing,
                    soldiers[soldier_number].throwing, soldiers[soldier_number].strength,
                    soldiers[soldier_number].mcskill, soldiers[soldier_number].mcstrength) >= 350) memory_corrupt();
                if ((statistics = new menuboxtype(COLOR(BLACK, LIGHT_WHITE), \
                    COLOR(LIGHT_WHITE, BLACK), COLOR(GRAY, LIGHT_WHITE), \
                    8, 2, buffer)) == NULL) no_memory();
                statistics->setchoiceinfo(4, TRUE, FALSE);
                delete buffer;
            }
            regenerate_soldier = FALSE;
        }


        // []-----------[]
        // |  ok button  |
        // []-----------[]
        ok_button.handle();
        if (ok_button.pressed) break;


        // []------------------[]
        // |  base check boxes  |
        // []------------------[]
        for (i = 0; i < numbases; i++) {
            int k = base_box[i]->checked;
            base_box[i]->handle();
            if (k != base_box[i]->checked) {regenerate_list = TRUE; break;}
        }


        // []--------------------------------------------[]
        // |  statistics list and other editing controls  |
        // []--------------------------------------------[]
        if (statistics) {
            // update name
            name->handle();
            if (name->currentchoice != 1) {
                #define NEWNAME_ROW 9
                #define NEWNAME_COL 20
                activescreenclass->save();
                textboxtype message(COLOR(LIGHT_WHITE, BLUE), COLOR(LIGHT_YELLOW, BLUE), \
                    NEWNAME_ROW, NEWNAME_COL, 35, 10, "Enter the new name:");
                textinputtype stat_text(COLOR(BLACK, LIGHT_WHITE), \
                    NEWNAME_ROW + 3, NEWNAME_COL + 3, 25, NULL);
                strcpy(stat_text.message, soldiers[soldier_number].name);
                buttontype ok_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY), \
                    NEWNAME_ROW + 5, NEWNAME_COL + 8, "OK");
                buttontype cancel_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY), \
                    NEWNAME_ROW + 5, NEWNAME_COL + 18, "Cancel");
                while (TRUE) {
                    message.handle();
                    stat_text.handle();

                    // ok button
                    ok_button.handle();
                    if (ok_button.pressed) {
                        strcpy(soldiers[soldier_number].name, stat_text.message);
                        regenerate_soldier = regenerate_list = TRUE;
                        break;
                    }

                    // cancel button
                    cancel_button.handle();
                    if (cancel_button.pressed) break;
                }
                activescreenclass->restore();
            }

            // update rank
            i = rank.currentchoice;
            rank.handle();
            if (i != rank.currentchoice) {
                if (rank.currentchoice != 1) {
                    soldiers[soldier_number].rank = rank.currentchoice - 2;
                } else {
                    soldiers[soldier_number].rank = 0xFFFF;
                }
                soldier_names->setchoiceinfo(soldier_names->currentchoice, \
                    FALSE, soldiers[soldier_number].rank == 0xFFFF);
                soldier_names->needupdate = TRUE;
            }

            // update location
            i = location.currentchoice;
            location.handle();
            if (i != location.currentchoice & location.currentchoice >= 1) {
                soldiers[soldier_number].location = baselisting[location.currentchoice - 1].location;
                regenerate_list = TRUE;
            }

            // update armour
            armour.handle();
            soldiers[soldier_number].armour = armour.currentchoice - 1;

            // update statistics box
            statistics->handle();
            if (statistics->currentchoice && !activemouseclass->button) {
                switch (statistics->currentchoice) {
                    case 1: // missions
                        stat_edit(soldiers[soldier_number].missions, 0, 0x7FFF, "\nNumber of missions?\n", 32000);
                        break;
                    case 2: // kills
                        stat_edit(soldiers[soldier_number].kills, 0, 0x7FFF, "\nNumber of kills?\n", 32000);
                        break;
                    case 3: // recovery
                        stat_edit(soldiers[soldier_number].recovery, 0, 0x7FFF, "\nDays until recovery?\n", 0);
                        break;
                    case 5: // time units
                        stat_edit(soldiers[soldier_number].time, 0, 0xFF, "\nNumber of time units?\n", 180);
                        break;
                    case 6: // stamina
                        stat_edit(soldiers[soldier_number].stamina, 0, 0xFF, "\nAmount of stamina?\n", 180);
                        break;
                    case 7: // health
                        stat_edit(soldiers[soldier_number].health, 0, 0xFF, "\nAmount of health?\n", 180);
                        break;
                    case 8: // bravery
                        i = 110 - 10 * soldiers[soldier_number].bravery;
                        stat_edit((unsigned) i, 20, 100, "\nAmount for bravery?\n", 100);
                        soldiers[soldier_number].bravery = 11 - i / 10;
                        break;
                    case 9: // reactions
                        stat_edit(soldiers[soldier_number].reactions, 0, 0xFF, "\nAmount for reactions?\n", 180);
                        break;
                    case 10: // firing accuracy
                        stat_edit(soldiers[soldier_number].firing, 0, 0xFF, "\nFiring accuracy?\n", 180);
                        break;
                    case 11: // throwing accuracy
                        stat_edit(soldiers[soldier_number].throwing, 0, 0xFF, "\nThrowing accuracy?\n", 180);
                        break;
                    case 12: // strength
                        stat_edit(soldiers[soldier_number].strength, 0, 0xFF, "\nAmount of strength?\n", 50);
                        break;
                    case 13: // mc skill
                        stat_edit(soldiers[soldier_number].mcskill, 0, 0xFF, "\nAmount of mc skill?\n", 180);
                        break;
                    case 14: // mc strength
                        stat_edit(soldiers[soldier_number].mcstrength, 0, 0xFF, "\nAmount of mc strength?\n", 180);
                        break;
                    default: break;
                }
                modified = regenerate_soldier = TRUE;
            }
        }


        // []-------------------[]
        // |  soldier name list  |
        // []-------------------[]
        if (soldier_names) {
            i = soldier_names->currentchoice;
            soldier_names->handle();
            if (soldier_names->currentchoice < 1) soldier_names->currentchoice = i;
            if (i != soldier_names->currentchoice) {
                int k = 1; soldier_number = -1;
                for (i = 0; i < 250, soldier_number == -1; i++) {
                    if (!soldiers[i].name[0]) continue;
                    for (j = 0; j < numbases; j++) {
                        if (soldiers[i].location == baselisting[j].location && base_box[j]->checked) {
                            if (k++ == soldier_names->currentchoice) soldier_number = i;
                            break;
                        }
                    }
                }
                regenerate_soldier = TRUE;
            }
        }


        // []------------------------------[]
        // |  handle the change all button  |
        // []------------------------------[]
        changeall_button.handle();
        if (changeall_button.pressed && soldier_names) {
            int rank, location, armour, missions, kills, recovery, \
                timeunits, stamina, health, bravery, reactions, firing, \
                throwing, strength, mcskill, mcstrength;
            edit_all_soldiers(rank, location, armour, missions, kills, \
                recovery, timeunits, stamina, health, bravery, reactions, \
                firing, throwing, strength, mcskill, mcstrength);

            for (i = 0; i < 250; i++) {
                if (!soldiers[i].name[0]) continue;
                for (j = 0; j < numbases; j++) {
                    if (soldiers[i].location == baselisting[j].location && base_box[j]->checked) {
                        if (rank != -1) soldiers[i].rank = rank - 1;
                        if (location != -1) soldiers[i].location = baselisting[location].location;
                        if (armour != -1) soldiers[i].armour = armour;
                        if (missions != -1) soldiers[i].missions = missions;
                        if (kills != -1) soldiers[i].kills = kills;
                        if (recovery != -1) soldiers[i].recovery = recovery;
                        if (timeunits != -1) soldiers[i].time = timeunits;
                        if (stamina != -1) soldiers[i].stamina = stamina;
                        if (health != -1) soldiers[i].health = health;
                        if (bravery != -1) soldiers[i].bravery = 11 - bravery / 10;
                        if (reactions != -1) soldiers[i].reactions = reactions;
                        if (firing != -1) soldiers[i].firing = firing;
                        if (throwing != -1) soldiers[i].throwing = throwing;
                        if (strength != -1) soldiers[i].strength = strength;
                        if (mcskill != -1) soldiers[i].mcskill = mcskill;
                        if (mcstrength != -1) soldiers[i].mcstrength = mcstrength;
                        break;
                    }
                }
            }
            modified = regenerate_soldier = regenerate_list = TRUE;
        }
    }


    // []-----------------------------------[]
    // |  Ask if we should save the changes  |
    // []-----------------------------------[]
    if (modified) {
        if (ask_yes_no("\nDo you wish to save these changes?\n")) {
            if ((fp = fopen(filename, "rb+")) == NULL) file_error();
            fwrite(soldiers, sizeof(struct soldier_type), 250, fp);
            fclose(fp);
        }
    }


    // []-----------------[]
    // |  free everything  |
    // []-----------------[]
    delete soldiers;
    if (name) delete name;
    if (statistics) delete statistics;
    if (soldier_names) delete soldier_names;
    for (i = 0; i < numbases; i++) delete base_box[i];
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
static void edit_all_soldiers(int &rank, int &location, int &armour, int &missions, \
    int &kills, int &recovery, int &timeunits, int &stamina, int &health, \
    int &bravery, int &reactions, int &firing, int &throwing, int &strength, \
    int &mcskill, int &mcstrength)
{
    // []-----------------[]
    // |  Save the screen  |
    // []-----------------[]
    char *savebuffer = new char[activescreenclass->buffersize];
    if (!savebuffer) no_memory();
    activescreenclass->save(savebuffer);
    activescreenclass->drawrepeat('�', 80*25, 1, 1, COLOR(LIGHT_WHITE, LIGHT_BLUE));
    activescreenclass->refresh();


    // []---------------------------------------[]
    // |  Reset all of the stats to "unchanged"  |
    // []---------------------------------------[]
    rank = location = armour = missions = kills = recovery = timeunits = \
        stamina = health = bravery = reactions = firing = throwing = \
        strength = mcskill = mcstrength = -1;


    // []-------------------------------------------[]
    // |  Create all of the controls that we'll use  |
    // []-------------------------------------------[]
    menuboxtype *statistics = NULL;
    buttontype ok_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY), 5, 65, "OK");
    buttontype cancel_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY), 9, 65, "Cancel");


    // []-----------[]
    // |  Main loop  |
    // []-----------[]
    while (TRUE) {
        // []-----------------------------------------[]
        // |  Generate a new statistics box if needed  |
        // []-----------------------------------------[]
        if (statistics == NULL) {
            char rankpick[][15] = {"unchanged", "dead", "seaman", "able seaman", "ensign", \
                "lieutenant", "commander", "captain"};
            char armourpick[][25] = {"unchanged", "none", "plastic aqua armour", \
                "ion armour", "magnetic ion armour"};
            // erase the screen behind it
            for (int i = 1; i <= 25; i++) activescreenclass->drawrepeat('�', 60, \
                i, 1, COLOR(LIGHT_WHITE, LIGHT_BLUE));
            activescreenclass->refresh();
            // update statistics box
            char *buffer = new char[500];
            if (buffer == NULL) no_memory();
            buffer[0] = 0;
            strcat(buffer, "rank: ");
            strcat(buffer, rankpick[rank + 1]);
            statcat(buffer, "\nlocation: base %s\n", (location < 0 ? location : location + 1));
            strcat(buffer, "armour: ");
            strcat(buffer, armourpick[armour + 1]);
            statcat(buffer, "\nmissions: %s\n", missions);
            statcat(buffer, "kills: %s\n", kills);
            statcat(buffer, "recovery: %s days\n\n", recovery);
            statcat(buffer, "time units: %s\n", timeunits);
            statcat(buffer, "stamina: %s\n", stamina);
            statcat(buffer, "health: %s\n", health);
            statcat(buffer, "bravery: %s\n", bravery);
            statcat(buffer, "reactions: %s\n", reactions);
            statcat(buffer, "firing accuracy: %s\n", firing);
            statcat(buffer, "throwing accuracy: %s\n", throwing);
            statcat(buffer, "strength: %s\n", strength);
            statcat(buffer, "mc skill: %s\n", mcskill);
            statcat(buffer, "mc strength: %s\n", mcstrength);
            if (strlen(buffer) > 500) memory_corrupt();
            if ((statistics = new menuboxtype(COLOR(BLACK, LIGHT_WHITE), \
                COLOR(LIGHT_WHITE, BLACK), COLOR(GRAY, LIGHT_WHITE), \
                4, 10, buffer)) == NULL) no_memory();
            statistics->setchoiceinfo(7, TRUE, FALSE);
            delete buffer;
        }

        // []---------------------------------[]
        // |  Handle the editing of the stats  |
        // []---------------------------------[]
        if (statistics) {
            statistics->handle();
            if (statistics->currentchoice && !activemouseclass->button) {
                switch (statistics->currentchoice) {
                    case 1: // rank
                        rank = generic_button_pick("Select a rank:", "Unch.", \
                            "dead", "seaman", "able", "ensign", "lt", "cmdr", "capn");
                        break;
                    case 2: // location
                        location = generic_button_pick("Select a base:", "Unchanged", \
                            (numbases >= 1 ? "1" : NULL), (numbases >= 2 ? "2" : NULL), \
                            (numbases >= 3 ? "3" : NULL), (numbases >= 4 ? "4" : NULL), \
                            (numbases >= 5 ? "5" : NULL), (numbases >= 6 ? "6" : NULL), \
                            (numbases >= 7 ? "7" : NULL), (numbases >= 8 ? "8" : NULL));
                        break;
                    case 3: // armour
                        armour = generic_button_pick("Select an armour:", "Unchanged", \
                            "None", "Plastic", "Ion", "Magnetic Ion");
                        break;
                    case 4: // missions
                        if (stat_edit((unsigned)missions, 0, 0x7FFF, "\nNumber of missions?\n", -1)) missions = -1;
                        break;
                    case 5: // kills
                        if (stat_edit((unsigned)kills, 0, 0x7FFF, "\nNumber of kills?\n", -1)) kills = -1;
                        break;
                    case 6: // recovery
                        if (stat_edit((unsigned)recovery, 0, 0x7FFF, "\nDays until recovery?\n", -1)) recovery = -1;
                        break;
                    case 8: // time units
                        if (stat_edit((unsigned)timeunits, 0, 0xFF, "\nNumber of time units?\n", -1)) timeunits = -1;
                        break;
                    case 9: // stamina
                        if (stat_edit((unsigned)stamina, 0, 0xFF, "\nAmount of stamina?\n", -1)) stamina = -1;
                        break;
                    case 10: // health
                        if (stat_edit((unsigned)health, 0, 0xFF, "\nAmount of health?\n", -1)) health = -1;
                        break;
                    case 11: // bravery
                        if (stat_edit((unsigned)bravery, 20, 100, "\nAmount for bravery?\n", -1)) {
                            bravery = -1;
                        } else {
                            bravery = (bravery / 10) * 10;
                        }
                        break;
                    case 12: // reactions
                        if (stat_edit((unsigned)reactions, 0, 0xFF, "\nAmount for reactions?\n", -1)) reactions = -1;
                        break;
                    case 13: // firing accuracy
                        if (stat_edit((unsigned)firing, 0, 0xFF, "\nFiring accuracy?\n", -1)) firing = -1;
                        break;
                    case 14: // throwing accuracy
                        if (stat_edit((unsigned)throwing, 0, 0xFF, "\nThrowing accuracy?\n", -1)) throwing = -1;
                        break;
                    case 15: // strength
                        if (stat_edit((unsigned)strength, 0, 0xFF, "\nAmount of strength?\n", -1)) strength = -1;
                        break;
                    case 16: // mc skill
                        if (stat_edit((unsigned)mcskill, 0, 0xFF, "\nAmount of mc skill?\n", -1)) mcskill = -1;
                        break;
                    case 17: // mc strength
                        if (stat_edit((unsigned)mcstrength, 0, 0xFF, "\nAmount of mc strength?\n", -1)) mcstrength = -1;
                        break;
                    default: break;
                }
                delete statistics;
                statistics = NULL;
            }
        }


        // []----------------------[]
        // |  Handle the ok button  |
        // []----------------------[]
        ok_button.handle();
        if (ok_button.pressed) break;


        // []--------------------------[]
        // |  Handle the cancel button  |
        // []--------------------------[]
        cancel_button.handle();
        if (cancel_button.pressed) {
            rank = location = armour = missions = kills = recovery = \
                timeunits = stamina = health = bravery = reactions = \
                firing = throwing = strength = mcskill = mcstrength = -1;
            break;
        }
    }

    // []--------------------[]
    // |  Restore the screen  |
    // []--------------------[]
    activescreenclass->restore(savebuffer);
    delete savebuffer;
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
