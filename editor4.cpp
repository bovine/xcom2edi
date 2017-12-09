// Xcom2edi
// Copyright (c) 1997, Jeff Lawson <jeff@bovine.net>
// All rights reserved.
// See LICENSE for distribution information.

#include "editor.h"


static char *old_directory(char *path);
static int choose_old_game(char *oldpath);

//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
// savedgame is a number from 1 to 10 and is the save game number to edit
//
void import_soldiers(int savedgame)
{
    char filename[MAXPATH], oldpath[MAXPATH]; FILE *fp; int i, j;
    struct old_soldier_type *soldiers;
    if ((soldiers = new struct old_soldier_type[250]) == NULL) no_memory();
//    char basenames[8][17];

    // []--------------------------------[]
    // |  prompt for the old XCOM I path  |
    // []--------------------------------[]
    if (strlen(old_directory(oldpath)) == 0) return;
    int oldsavedgame = choose_old_game(oldpath);
    if (oldsavedgame == 0) {
        ok_box("The path you entered does not\n" \
            "seem to have any XCOM I saves.");
        return;
    }


    // []------------------[]
    // |  clear the screen  |
    // []------------------[]
    activescreenclass->drawrepeat('�', 80*25, 1, 1, COLOR(LIGHT_WHITE, LIGHT_BLUE));
    activescreenclass->refresh();


    // []------------------------------------[]
    // |  create all controls that we'll use  |
    // []------------------------------------[]
    buttontype ok_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY), 2, 50, "OK");
    buttontype cancel_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY), 2, 58, "Cancel");
    buttontype mark_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY), 2, 70, "Mark");
    menuboxtype *soldier_names = NULL, *statistics = NULL;


    // []---------------------------------------[]
    // |  read in the names of all of the bases  |
    // []---------------------------------------[]
    activemouseclass->setcursor(MOUSE_WAIT);
/*
    sprintf(filename, "%s\\GAME_%i\\BASE.DAT", oldpath, oldsavedgame);
    if ((fp = fopen(filename, "rb")) == NULL) file_error();
    for (i = 0; i < 8; i++) {
        fseek(fp, i * 0x124, SEEK_SET);
        fread(basenames[i], sizeof(char), 16, fp); basenames[i][16] = 0;
        if (strlen(basenames[i]) == 0) sprintf(basenames[i], "(base %i)", i + 1);
    }
    fclose(fp);
*/

    // []----------------------[]
    // |  read in the soldiers  |
    // []----------------------[]
    sprintf(filename, "%s\\GAME_%i\\SOLDIER.DAT", oldpath, oldsavedgame);
    if ((fp = fopen(filename, "rb")) == NULL) file_error();
    fread(soldiers, sizeof(struct old_soldier_type), 250, fp);
    fclose(fp);


    // []---------------------------------------------------------------[]
    // |  generate the soldier name list buffer and validate the fields  |
    // []---------------------------------------------------------------[]
    int total_matching = 0, buffersize = 100, bufferused = 0, active[250];
    char *buffer = new char[buffersize]; buffer[0] = 0;
    for (i = 0; i < 250; i++) {
        soldiers[i].name[25] = 0;
        for (j = 0; j < strlen(soldiers[i].name); j++) if (!isalpha((unsigned char)soldiers[i].name[j]) && soldiers[i].name[j] != ' ') soldiers[i].name[0] = 0;
        if (soldiers[i].name[0] && soldiers[i].rank <= 6) {
            if (buffersize - bufferused < 30) {
                buffersize += 100;
                char *newbuffer = new char[buffersize];
                if (newbuffer == NULL) no_memory();
                memmove(newbuffer, buffer, bufferused);
                delete buffer; buffer = newbuffer;
            }
            bufferused += sprintf(buffer + bufferused, "%-25.25s\n", soldiers[i].name);
            active[total_matching] = i;
            total_matching++;
        } else {
            soldiers[i].name[0] = 0;
        }
        if (soldiers[i].rank > 6) soldiers[i].rank = 0xFFFF;
        soldiers[i].location = 0;
        soldiers[i].time += soldiers[i].inctime; soldiers[i].inctime = 0;
        soldiers[i].health += soldiers[i].inchealth; soldiers[i].inchealth = 0;
        soldiers[i].stamina += soldiers[i].incstamina; soldiers[i].incstamina = 0;
        soldiers[i].reactions += soldiers[i].increactions; soldiers[i].increactions = 0;
        soldiers[i].strength += soldiers[i].incstrength; soldiers[i].incstrength = 0;
        soldiers[i].firing += soldiers[i].incfiring; soldiers[i].incfiring = 0;
        soldiers[i].throwing += soldiers[i].incthrowing; soldiers[i].incthrowing = 0;
        soldiers[i].incpsionicsk = 0; soldiers[i].incbravery = 0;
        if (soldiers[i].armour > 3) soldiers[i].armour = 0;
    }


    // []-----------------------------[]
    // |  create new list of soldiers  |
    // []-----------------------------[]
    if (total_matching > 0) soldier_names = new menuboxtype(COLOR(BLACK, \
        LIGHT_WHITE), COLOR(LIGHT_WHITE, BLACK), COLOR(GRAY, LIGHT_WHITE), \
        6, 50, 17, buffer);
    for (i = 0; i < total_matching; i++) soldier_names->setchoiceinfo(i + 1, \
        FALSE, soldiers[active[i]].rank > 6);
    soldier_names->currentchoice = 1;
    delete buffer;
    activemouseclass->setcursor(MOUSE_ARROW);


    // []-----------------[]
    // |  main input loop  |
    // []-----------------[]
    int regenerate_soldier = TRUE, import_soldiers = FALSE;
    while (TRUE) {
        // regenerate soldier statistics, if necessary
        if (regenerate_soldier) {
            int soldier_number = active[soldier_names->currentchoice - 1];
            char buffer[500], rankname[25], armourname[25];
            rankname[0] = armourname[0] = 0;

            // delete the old statistics, if necessary
            if (statistics) {delete statistics; statistics = NULL;}
            // erase the screen
            for (i = 1; i <= 25; i++) activescreenclass->drawrepeat('�', 49, \
                i, 1, COLOR(LIGHT_WHITE, LIGHT_BLUE));
            // update statistics box
            if (soldier_names->currentchoice < 1) {
                soldier_names->currentchoice = soldier_names->toprow;
                soldier_names->needupdate = TRUE;
            }
            switch (soldiers[soldier_number].rank) {
                case 0: strcpy(rankname, "seaman"); break;
                case 1: strcpy(rankname, "able seaman"); break;
                case 2: strcpy(rankname, "ensign"); break;
                case 3: strcpy(rankname, "lieutenant"); break;
                case 4: strcpy(rankname, "commander"); break;
                case 5: strcpy(rankname, "captain"); break;
                default:strcpy(rankname, "dead"); break;
            }
            switch (soldiers[soldier_number].armour) {
                case 1: strcpy(armourname, "plastic aqua armour"); break;
                case 2: strcpy(armourname, "ion armour"); break;
                case 3: strcpy(armourname, "magnetic ion armour"); break;
                default: strcpy(armourname, "none"); break;
            }
            if (sprintf(buffer, "name: %s\n"
                            "rank: %s\n"
                            "armour: %s\n\n"
                            "missions: %u\n"
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
                soldiers[soldier_number].name, rankname, armourname,
                soldiers[soldier_number].missions, soldiers[soldier_number].kills,
                soldiers[soldier_number].recovery, soldiers[soldier_number].time,
                soldiers[soldier_number].stamina, soldiers[soldier_number].health,
                100 - 10 * (soldiers[soldier_number].bravery - 1),
                soldiers[soldier_number].reactions, soldiers[soldier_number].firing,
                soldiers[soldier_number].throwing, soldiers[soldier_number].strength,
                soldiers[soldier_number].psionicsk, soldiers[soldier_number].psionicstr) >= 500) memory_corrupt();
            if ((statistics = new menuboxtype(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, BLACK), \
                COLOR(GRAY, LIGHT_WHITE), 4, 2, buffer)) == NULL) no_memory();
            statistics->setchoiceinfo(5, TRUE, FALSE);
            statistics->setchoiceinfo(9, TRUE, FALSE);
            regenerate_soldier = FALSE;
        }

        // []-----------[]
        // |  ok button  |
        // []-----------[]
        ok_button.handle();
        if (ok_button.pressed) {import_soldiers = TRUE; break;}


        // []---------------[]
        // |  cancel button  |
        // []---------------[]
        cancel_button.handle();
        if (cancel_button.pressed) {import_soldiers = FALSE; break;}


        // []-------------[]
        // |  mark button  |
        // []-------------[]
        mark_button.handle();
        if (mark_button.pressed) {
            soldier_names->setchoiceinfo(soldier_names->currentchoice, FALSE, \
                !soldier_names->ischecked(soldier_names->currentchoice));
        }


        // []-----------------------[]
        // |  update statistics box  |
        // []-----------------------[]
        statistics->handle();


        // []-------------------[]
        // |  soldier name list  |
        // []-------------------[]
        int oldnum = soldier_names->currentchoice;
        soldier_names->handle();
        if (oldnum != soldier_names->currentchoice) regenerate_soldier = TRUE;
    }


    // []--------------------------------------------[]
    // |  actually import the soldiers if we need to  |
    // []--------------------------------------------[]
    if (import_soldiers) {
        int convertnumber = 0;
        for (i = 0; i < total_matching; i++) {
            if (soldier_names->ischecked(i + 1)) convertnumber++;
        }
        if (convertnumber == 0) {
            ok_box("No soldiers were selected to\nbe imported into XCOM II.");
        } else {
            char buffer[250];
            sprintf(buffer, "\nImport %i soldiers?", convertnumber);
            if (ask_yes_no(buffer)) {
                // create all controls that we'll use
                #define IMPORTING_ROW 9
                #define IMPORTING_COL 19
                activescreenclass->save();
                textboxtype message(COLOR(LIGHT_WHITE, BLUE), COLOR(\
                    LIGHT_YELLOW, BLUE), IMPORTING_ROW, IMPORTING_COL, 40, \
                    3, "\nImporting the selected soldiers");
                message.handle();
                activemouseclass->setcursor(MOUSE_WAIT);
                delay(3000);

                // actually do the import
                sprintf(filename, "GAME_%i\\SOLDIER.DAT", savedgame);
                if ((fp = fopen(filename, "rb+")) == NULL) file_error();
                struct soldier_type soldier;
                for (i = j = 0; i < 250, j < convertnumber; i++) {
                    fseek(fp, i * sizeof(struct soldier_type), SEEK_SET);
                    fread (&soldier, sizeof(struct soldier_type), 1, fp);
                    if (soldier.rank > 5) {
                        // we've found an empty slot
                        // now find a soldier to put here
                        int index, found = 0, actualnumber = -1;
                        for (index = 0; index < total_matching; index++) {
                            if (soldier_names->ischecked(index + 1)) {
                                if (found++ == j) {
                                    actualnumber = active[index];
                                    j++;
                                    break;
                                }
                            }
                        }
                        if (actualnumber == -1) break;

                        // now transfer over all of the data
                        strcpy(soldier.name, soldiers[actualnumber].name);
                        soldier.inctime = 0;
                        soldier.implantation = 0;
                        soldier.inchealth = 0;
                        soldier.incstrength = 0;
                        soldier.location = baselisting[0].location;
                        soldier.craft = 0xFFFF;
                        soldier.rank = soldiers[actualnumber].rank;
                        soldier.kills = soldiers[actualnumber].kills;
                        soldier.recovery = 0;
                        soldier.missions = soldiers[actualnumber].missions;
                        soldier.incstamina = 0;
                        soldier.increactions = 0;
                        soldier.incfiring = 0;
                        soldier.time = soldiers[actualnumber].time;
                        soldier.health = soldiers[actualnumber].health;
                        soldier.stamina = soldiers[actualnumber].stamina;
                        soldier.throwing = soldiers[actualnumber].throwing;
                        soldier.armour = soldiers[actualnumber].armour;
                        soldier.firing = soldiers[actualnumber].firing;
                        soldier.strength = soldiers[actualnumber].strength;
                        soldier.incbravery = 0;
                        soldier.incthrowing = 0;
                        soldier.bravery = soldiers[actualnumber].bravery;
                        soldier.mcskill = soldiers[actualnumber].psionicsk;
                        soldier.mcstrength = soldiers[actualnumber].psionicstr;
                        soldier.reactions = soldiers[actualnumber].reactions;
                        soldier.UNK0 = 0;
                        soldier.UNK1 = 0;
                        soldier.UNK2 = 0xFFFF;
                        soldier.UNK3 = 0x14;
                        soldier.UNK4 = 0;
                        soldier.UNK5 = 0;
                        soldier.UNK6 = 0;
                        soldier.UNK7 = 1;
                        soldier.UNK8 = 0;
                        soldier.UNK9 = 0;
                        soldier.UNKA = 0x21;
                        fseek(fp, i * sizeof(struct soldier_type), SEEK_SET);
                        fwrite(&soldier, sizeof(struct soldier_type), 1, fp);
                    }
                }
                fclose(fp);

                // restore the screen
                activemouseclass->setcursor(MOUSE_ARROW);
                activescreenclass->restore();

                // display the results
                char resultmsg[250];
                if (j != convertnumber) {
                    if (sprintf(resultmsg, "Only %i soldier%s transferred\n" \
                        "into your XCOM II game.  No more room?", j, \
                        (j != 1 ? "s were" : " was")) >= 250) memory_corrupt();
                } else {
                    if (sprintf(resultmsg, "%i soldier%s successfully\n" \
                        "transferred into your XCOM II game.", j, (j != 1 ? \
                        "s were" : " was")) >= 250) memory_corrupt();
                }
                ok_box(resultmsg);
            }
        }
    }


    // []-----------------[]
    // |  free everything  |
    // []-----------------[]
    delete soldiers;
    if (statistics) delete statistics;
    if (soldier_names) delete soldier_names;
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
static char *old_directory(char *path)
{
    // save the screen
    activescreenclass->save();

    // create all controls that we'll use
    #define ASKUFO_ROW 10
    #define ASKUFO_COL 7
    textboxtype message(COLOR(LIGHT_WHITE, BLUE), COLOR(LIGHT_YELLOW, BLUE), \
        ASKUFO_ROW, ASKUFO_COL, MAXPATH - 20 + 6, 10, "Enter the full path " \
        "to your main XCOM I directory.");
    textinputtype old_directory(COLOR(BLACK, LIGHT_WHITE), \
        ASKUFO_ROW + 3, ASKUFO_COL + 2, MAXPATH - 20, NULL);
    buttontype ok_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY), \
        ASKUFO_ROW + 5, ASKUFO_COL + 15, "OK");
    buttontype cancel_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY), \
        ASKUFO_ROW + 5, ASKUFO_COL + 23, "Cancel");

    // main input loop
    while (TRUE) {
        message.handle();
        old_directory.handle();

        // ok button
        ok_button.handle();
        if (ok_button.pressed) {
            // return the new amount
            strcpy(path, old_directory.message);
            break;
        }

        // cancel button
        cancel_button.handle();
        if (cancel_button.pressed) {
            path[0] = 0;
            break;
        }
    }

    // restore the screen
    activescreenclass->restore();

    // quickly parse the pathname
    char *p;
    strupr(path);
    for (p = path; *p; p++) if (!isspace(*p)) {strcpy (path, p); break;}
    for (p = path; *p; p++) if (isspace(*p)) {*p = 0; break;}
    if (path[strlen(path) - 1] == '\\') path[strlen(path) - 1] = 0;

    // return the value
    return(path);
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
static int choose_old_game(char *oldpath)
{
    // read in all of the saved games names
    int length = 1, i, j;
    char *buffer = new char[length], filename[MAXPATH];
    if (buffer == NULL) no_memory();
    strcpy(buffer, "");
    for (i = 1; i <= 10; i++) {
        sprintf(filename, "%s\\GAME_%i\\SAVEINFO.DAT", oldpath, i);
        if (file_exists(filename)) {
            char title[81], *temp;
            FILE *fp;

            if ((fp = fopen(filename, "rb")) == NULL) file_error();
            memset(title, 0, 80);
            fseek(fp, 2, SEEK_SET);
            fread(title, sizeof(char), 80, fp);
            fclose(fp);

            length += 4 + strlen(title) + 1; temp = new char[length];
            if (temp == NULL) no_memory();
            sprintf(temp, "%s%2i. %s\n", buffer, i, title);
            delete buffer; buffer = temp;
        }
    }
    if (length == 1) return(0);

    // clear the screen
    activescreenclass->drawrepeat('�', 80*25, 1, 1, COLOR(LIGHT_WHITE, LIGHT_BLUE));
    activescreenclass->refresh();

    // create the controls that we need
    menuboxtype saved_games(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, BLACK), \
        COLOR(GRAY, LIGHT_WHITE), 10, 23, buffer);
    delete buffer;

    // main input loop
    while (TRUE) {
        // handle the menu box
        saved_games.handle();
        if (saved_games.currentchoice) {
            while (activemouseclass->button) saved_games.handle();
            break;
        }
    }

    // return the correct save game number
    for (i = 1, j = 0; i <= 10; i++) {
        sprintf(filename, "%s\\GAME_%i\\SAVEINFO.DAT", oldpath, i);
        if (file_exists(filename)) {
            if (++j == saved_games.currentchoice) return(i);
        }
    }
    return(0);
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
