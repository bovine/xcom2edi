// Xcom2edi
// Copyright (c) 1997, Jeff Lawson <jeff@bovine.net>
// All rights reserved.
// See LICENSE for distribution information.

#include "editor.h"


//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
// savedgame is a number from 1 to 10 and is the save game number to edit
//
#define MONEY_ROW 9
#define MONEY_COL 20
void money_edit (int savedgame)
{
    // []------------------[]
    // |  clear the screen  |
    // []------------------[]
    activescreenclass->drawrepeat('�', 80*25, 1, 1, COLOR(LIGHT_WHITE, LIGHT_BLUE));
    activescreenclass->refresh();


    // []------------------------------------[]
    // |  create all controls that we'll use  |
    // []------------------------------------[]
    textboxtype message(COLOR(LIGHT_WHITE, BLUE), COLOR(LIGHT_YELLOW, BLUE), \
        MONEY_ROW, MONEY_COL, 35, 11, "\nHow much money do you want?\n");
    textinputtype money_text(COLOR(BLACK, LIGHT_WHITE), \
        MONEY_ROW + 4, MONEY_COL + 7, 15, NULL);
    buttontype max_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY), \
        MONEY_ROW + 6, MONEY_COL + 2, "  Lots ");
    buttontype ok_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY), \
        MONEY_ROW + 6, MONEY_COL + 15, "OK");
    buttontype cancel_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY), \
        MONEY_ROW + 6, MONEY_COL + 23, "Cancel");


    // []-----------------------------------------------[]
    // |  read in how much money the user currently has  |
    // []-----------------------------------------------[]
    char filename[MAXPATH]; FILE *fp; long money;
    sprintf(filename, "GAME_%i\\LIGLOB.DAT", savedgame);
    if ((fp = fopen(filename, "rb")) == NULL) file_error();
    fread(&money, sizeof(long), 1, fp);
    fclose(fp);
    if (money < 0) money = 0;
    sprintf(money_text.message, "%lu", money);


    // []-----------------[]
    // |  main input loop  |
    // []-----------------[]
    while (TRUE) {
        message.handle();
        money_text.handle();

        // maximum button
        max_button.handle();
        if (max_button.pressed) {
            sprintf(money_text.message, "%lu", 0x7FF00000);
            money_text.needupdate = TRUE;
        }

        // ok button
        ok_button.handle();
        if (ok_button.pressed) {
            // save out the changes
            sscanf(money_text.message, "%lu", &money);
            if (money < 0) money = -money;
            if ((fp = fopen(filename, "rb+")) == NULL) file_error();
            fwrite(&money, sizeof(long), 1, fp);
            fclose(fp);
            break;
        }

        // cancel button
        cancel_button.handle();
        if (cancel_button.pressed) break;
    }
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
// savedgame is a number from 1 to 10 and is the saved game number to edit
// base is a number from 1 to 8 and is the base number to edit
//
void edit_inventory(int savedgame, int basenum)
{
    FILE *fp;
    int i, modified = FALSE;
    char filename[MAXPATH];
    struct basedat *base;


    // []------------------[]
    // |  clear the screen  |
    // []------------------[]
    activescreenclass->drawrepeat('�', 80*25, 1, 1, COLOR(LIGHT_WHITE, LIGHT_BLUE));
    activescreenclass->refresh();


    // []-------------------------------[]
    // |  read in the current inventory  |
    // []-------------------------------[]
    if ((base = new struct basedat) == NULL) no_memory();
    sprintf(filename, "GAME_%i\\BASE.DAT", savedgame);
    if ((fp = fopen(filename, "rb")) == NULL) file_error();
    fseek(fp, (basenum - 1) * sizeof(struct basedat), SEEK_SET);
    fread(base, sizeof(struct basedat), 1, fp);
    fclose(fp);


    // []------------------------------------[]
    // |  create all controls that we'll use  |
    // []------------------------------------[]
    menuboxtype *item_names = NULL;
    buttontype ok_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY), 3, 50, "OK");
    buttontype all_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY), 22, 50, "Change all quantities");


    // []-----------------[]
    // |  main input loop  |
    // []-----------------[]
    int regenerate_list = TRUE;
    while (TRUE) {
        // []---------------------------------------[]
        // |  build new page of items, if necessary  |
        // []---------------------------------------[]
        if (regenerate_list) {
            // clear screen
            for (i = 2; i <= 24; i++) activescreenclass->drawrepeat('�', 44, \
                i, 3, COLOR(LIGHT_WHITE, LIGHT_BLUE));
            // delete old list, if present
            int oldtoprow = 1;
            if (item_names) {
                oldtoprow = item_names->toprow;
                delete item_names; item_names = NULL;
            }
            // generate new list
            int buffersize = 200, bufferused = 0; char *buffer;
            if ((buffer = new char[buffersize]) == NULL) no_memory();
            for (i = 0;; i++) {
                if (buffersize - bufferused < 45) {
                    buffersize += 200;
                    char *newbuffer = new char[buffersize];
                    if (newbuffer == NULL) no_memory();
                    memmove(newbuffer, buffer, bufferused);
                    delete buffer; buffer = newbuffer;
                }
                bufferused += sprintf(buffer + bufferused, "%-34.34s%6u\n", \
                    objects[i].name, base->inventory[objects[i].id - 1]);
                if (objects[i].id >= 93) break;
            }
            if ((item_names = new menuboxtype(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, BLACK), \
                COLOR(GRAY, LIGHT_WHITE), 2, 3, 21, buffer)) == NULL) no_memory();
            item_names->toprow = oldtoprow;
            regenerate_list = FALSE; delete buffer;
        }

        // []----------------------[]
        // |  update the controls   |
        // []----------------------[]
        item_names->handle();
        if (item_names->currentchoice && !activemouseclass->button) {
            stat_edit(base->inventory[objects[item_names->currentchoice - 1].id - 1], \
                0, 0x7FFF, "\nHow many do you want?\n", 1000);
            modified = regenerate_list = TRUE;
        }


        // []------------[]
        // |  all button  |
        // []------------[]
        all_button.handle();
        if (all_button.pressed) {
            unsigned count = 5;
            if (!stat_edit(count, 0, 0x7FFF, "\nHow many do you want?\n", 1000)) {
                for (i = 0; i < sizeof(base->inventory) / sizeof(unsigned); i++)
                    base->inventory[objects[i].id - 1] = count;
                modified = regenerate_list = TRUE;
            }
        }


        // []-----------[]
        // |  ok button  |
        // []-----------[]
        ok_button.handle();
        if (ok_button.pressed) break;
    }


    // []----------------------------------------[]
    // |  Ask if we should write out the changes  |
    // []----------------------------------------[]
    if (modified) {
        if (ask_yes_no("\nDo you wish to save these changes?\n")) {
            if ((fp = fopen(filename, "rb+")) == NULL) file_error();
            fseek(fp, (basenum - 1) * sizeof(struct basedat), SEEK_SET);
            fwrite(base, sizeof(struct basedat), 1, fp);
            fclose(fp);
        }
    }


    // []-----------------[]
    // |  free everything  |
    // []-----------------[]
    if (item_names) delete item_names;
    delete base;
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
// savedgame is a number from 1 to 10 and is the save game number to edit
// base is a number from 1 to 8 and is the base number to edit
//
void edit_buildings(int savedgame, int basenum)
{
    FILE *fp;
    int i, j, modified = FALSE;
    char basefilename[MAXPATH], facilfilename[MAXPATH];
    struct basedat *base;
    struct facildat *facil;


    // []------------------[]
    // |  clear the screen  |
    // []------------------[]
    activescreenclass->drawrepeat('�', 80*25, 1, 1, COLOR(LIGHT_WHITE, LIGHT_BLUE));
    activescreenclass->refresh();


    // []---------------------------------[]
    // |  read in the current base layout  |
    // []---------------------------------[]
    if ((base = new struct basedat) == NULL) no_memory();
    sprintf(basefilename, "GAME_%i\\BASE.DAT", savedgame);
    if ((fp = fopen(basefilename, "rb")) == NULL) file_error();
    fseek(fp, (basenum - 1) * sizeof(struct basedat), SEEK_SET);
    fread(base, sizeof(struct basedat), 1, fp);
    fclose(fp);

    if ((facil = new struct facildat[17]) == NULL) no_memory();
    sprintf(facilfilename, "GAME_%i\\FACIL.DAT", savedgame);
    if ((fp = fopen(facilfilename, "rb")) == NULL) file_error();
    fread(facil, sizeof(struct facildat), 17, fp);
    fclose(fp);



    // []------------------------------------[]
    // |  create all controls that we'll use  |
    // []------------------------------------[]
    buttontype ok_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY), 2, 74, "OK");
    checkboxtype set_box(COLOR(BLACK, LIGHT_WHITE), 6, 57, "Set");
    buttontype *building_buttons[6][6];
    for (i = 0; i < 6; i++)
        for (j = 0; j < 6; j++)
            building_buttons[i][j] = NULL;
    buttontype details_box(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY), 2, 57, "Details...");


    // []--------------------------------------------[]
    // |  generate the menubox of all building types  |
    // []--------------------------------------------[]
    unsigned buffersize = 100, bufferused = 0; char *buffer;
    if ((buffer = new char[buffersize]) == NULL) no_memory();
    for (i = 0; i < TOTAL_BUILDINGS; i++) {
        if (buffersize - bufferused < 25) {
            buffersize += 100;
            char *newbuffer = new char[buffersize];
            if (newbuffer == NULL) no_memory();
            memmove(newbuffer, buffer, bufferused);
            delete buffer; buffer = newbuffer;
        }
        bufferused += sprintf(buffer + bufferused, "%-19.19s\n", building_names[i]);
    }
    menuboxtype building_list(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, BLACK), \
        COLOR(GRAY, LIGHT_WHITE), 8, 57, 15, buffer);
    delete buffer;


    // []-----------------[]
    // |  main input loop  |
    // []-----------------[]
    int regenerate_buildings = TRUE;
    while (TRUE) {
        // build screenful of buttons, if necessary
        if (regenerate_buildings) {
            // clear the screen
            for (i = 1; i <= 25; i++) activescreenclass->drawrepeat('�', 55, \
                i, 1, COLOR(LIGHT_WHITE, LIGHT_BLUE));
            // generate new buttons to replace the old ones
            for (i = 0; i < 6; i++) {
                for (j = 0; j < 6; j++) {
                    // delete this button if one already exists
                    if (building_buttons[i][j]) {
                        delete building_buttons[i][j];
                        building_buttons[i][j] = NULL;
                    }
                    // now create a new updated one
                    char buffer[30];
                    if (base->buildings[i][j] == 0x10) {
                        strcpy(buffer, "\n\n   Sub Pen    \n\n\n");
                    } else if (base->buildings[i][j] == 0x11 || base->buildings[i][j] == 0x12 || base->buildings[i][j] == 0x13) {
                        continue;
                    } else if (base->buildings[i][j] == 0xFF) {
                        sprintf(buffer, "%-5.5s", building_names[0]);
                    } else {
                        sprintf(buffer, "%-5.5s", building_names[base->buildings[i][j] + 1]);
                    }
                    if (base->completion[i][j] == 0xFF) {
                        // nothing
                        if ((building_buttons[i][j] = new buttontype(COLOR(BLACK, LIGHT_YELLOW), \
                            COLOR(LIGHT_YELLOW, GRAY), i * 4 + 2, j * 9 + 2, buffer)) == NULL) no_memory();
                    } else if (base->completion[i][j] == 0) {
                        // already completed building
                        if ((building_buttons[i][j] = new buttontype(COLOR(BLACK, LIGHT_GREEN), \
                            COLOR(LIGHT_GREEN, GRAY), i * 4 + 2, j * 9 + 2, buffer)) == NULL) no_memory();
                    } else {
                        // not completed building
                        if ((building_buttons[i][j] = new buttontype(COLOR(BLACK, LIGHT_RED), \
                            COLOR(LIGHT_RED, GRAY), i * 4 + 2, j * 9 + 2, buffer)) == NULL) no_memory();
                    }
                }
            }
            regenerate_buildings = FALSE;
        }

        // update the controls
        building_list.handle();
        set_box.handle();

        // update the building buttons
        for (i = 0; i < 6; i++) {
            for (j = 0; j < 6; j++) {
                if (!building_buttons[i][j]) continue;
                building_buttons[i][j]->handle();
                if (!building_buttons[i][j]->pressed) continue;


                // []----------------------------------[]
                // |  Handle creation of new buildings  |
                // []----------------------------------[]
                if (set_box.checked && building_list.currentchoice) {
                    // the user is trying to change a building
                    modified = TRUE;
                    if (base->buildings[i][j] == 0x10) {
                        // if it was a hangar, destroy all 4 blocks
                        base->buildings[i][j+1] = base->completion[i][j+1] = 0xFF;
                        base->buildings[i+1][j] = base->completion[i+1][j] = 0xFF;
                        base->buildings[i+1][j+1] = base->completion[i+1][j+1] = 0xFF;
                    }

                    // determine the default construction time
                    unsigned completion = facil[building_list.currentchoice - \
                        2].construction_time;

                    // actually create the building
                    if (building_list.currentchoice - 2 == 0x10)
                    {
                        // if we're changing it to a hangar...
                        // make sure it will fit
                        if (i >= 5 || j >= 5) continue;

                        // prompt for the completion time
                        if (stat_edit(completion, 0, 255,
                            "\nHow many days until completion?\n", 0)) continue;

                        // destroy any hangars that we may partially cover
                        if (base->buildings[i][j+1] == 0x10 && j < 4) {  // left side
                            base->buildings[i][j+1] = base->completion[i][j+1] = 0xFF;
                            base->buildings[i][j+2] = base->completion[i][j+2] = 0xFF;
                            base->buildings[i+1][j+1] = base->completion[i+1][j+1] = 0xFF;
                            base->buildings[i+1][j+2] = base->completion[i+1][j+2] = 0xFF;
                        }
                        if (base->buildings[i-1][j+1] == 0x10 && j < 4 && i > 0) { // upper-left
                            base->buildings[i-1][j+1] = base->completion[i-1][j+1] = 0xFF;
                            base->buildings[i-1][j+2] = base->completion[i-1][j+2] = 0xFF;
                            base->buildings[i][j+1] = base->completion[i][j+1] = 0xFF;
                            base->buildings[i][j+2] = base->completion[i][j+2] = 0xFF;
                        }
                        if (base->buildings[i+1][j+1] == 0x10 && j < 4 && i < 4) { // lower-left
                            base->buildings[i+1][j+1] = base->completion[i+1][j+1] = 0xFF;
                            base->buildings[i+1][j+2] = base->completion[i+1][j+2] = 0xFF;
                            base->buildings[i+2][j+1] = base->completion[i+2][j+1] = 0xFF;
                            base->buildings[i+2][j+2] = base->completion[i+2][j+2] = 0xFF;
                        }
                        if (base->buildings[i+1][j] == 0x10 && i < 4) { // top
                            base->buildings[i+1][j] = base->completion[i+1][j] = 0xFF;
                            base->buildings[i+1][j+1] = base->completion[i+1][j+1] = 0xFF;
                            base->buildings[i+2][j] = base->completion[i+2][j] = 0xFF;
                            base->buildings[i+2][j+1] = base->completion[i+2][j+1] = 0xFF;
                        }
                        if (base->buildings[i+1][j-1] == 0x10 && i < 4 && j > 0) { // upper-right
                            base->buildings[i+1][j-1] = base->completion[i+1][j-1] = 0xFF;
                            base->buildings[i+1][j] = base->completion[i+1][j] = 0xFF;
                            base->buildings[i+2][j-1] = base->completion[i+2][j-1] = 0xFF;
                            base->buildings[i+2][j] = base->completion[i+2][j] = 0xFF;
                        }

                        // create the hangar
                        base->buildings[i][j] = 0x10; base->completion[i][j] = completion;
                        base->buildings[i][j+1] = 0x11; base->completion[i][j+1] = completion;
                        base->buildings[i+1][j] = 0x12; base->completion[i+1][j] = completion;
                        base->buildings[i+1][j+1] = 0x13; base->completion[i+1][j+1] = completion;
                    } else if (building_list.currentchoice - 2 == -1) {
                        // if we're changing it to nothing...
                        base->buildings[i][j] = base->completion[i][j] = 0xFF;
                    } else {
                        // else we're changing it to something else...
                        // prompt for completion time.
                        if (stat_edit(completion, 0, 255,
                            "\nHow many days until completion?\n", 0)) continue;

                        // create the building
                        base->buildings[i][j] = building_list.currentchoice - 2;
                        base->completion[i][j] = completion;
                    }
                    regenerate_buildings = TRUE;
                    continue;
                }


                // []---------------------------------------[]
                // |  Handle querying of existing buildings  |
                // []---------------------------------------[]
                // otherwise the user is querying us about a building
                building_list.currentchoice = 
                    (base->buildings[i][j] == 0xFF ?
                    1 : base->buildings[i][j] + 2);
                if (base->completion[i][j] != 0 && base->buildings[i][j] != 0xFF) {
                    char buffer[80];
                    sprintf(buffer, "%i days until completion.",
                        base->completion[i][j]);
                    ok_box(buffer);
                }
                building_list.needupdate = TRUE;
            }
        }

        // ok button
        ok_button.handle();
        if (ok_button.pressed) break;

        // details button
        details_box.handle();
        if (details_box.pressed && building_list.currentchoice > 1)
        {
            #define BASEDETAIL_ROW  6
            #define BASEDETAIL_COL  10
            activescreenclass->save();
            char buffer[300];
            sprintf(buffer, "\n\2Details for %s\1\n\n"
                "  Construction cost ($K):       (0-65535)\n"
                "   Maintenance cost ($K):       (0-255)\n"
                "Construction time (days):       (0-255)\n"
                "  Building Defense value:       (0-65535)\n"
                "     Hit ratio (percent):       (0-255)\n",
                building_names[building_list.currentchoice - 1]);
            textboxtype message(COLOR(LIGHT_WHITE, BLUE), COLOR(LIGHT_YELLOW, BLUE), \
                BASEDETAIL_ROW, BASEDETAIL_COL, 45, 14, buffer);
            textinputtype conscost(COLOR(BLACK, LIGHT_WHITE), \
                BASEDETAIL_ROW + 4, BASEDETAIL_COL + 28, 5, NULL);
            textinputtype maincost(COLOR(BLACK, LIGHT_WHITE), \
                BASEDETAIL_ROW + 5, BASEDETAIL_COL + 28, 5, NULL);
            textinputtype constime(COLOR(BLACK, LIGHT_WHITE), \
                BASEDETAIL_ROW + 6, BASEDETAIL_COL + 28, 5, NULL);
            textinputtype defenseval(COLOR(BLACK, LIGHT_WHITE), \
                BASEDETAIL_ROW + 7, BASEDETAIL_COL + 28, 5, NULL);
            textinputtype hitratio(COLOR(BLACK, LIGHT_WHITE), \
                BASEDETAIL_ROW + 8, BASEDETAIL_COL + 28, 5, NULL);
            buttontype ok_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY), \
                BASEDETAIL_ROW + 10, BASEDETAIL_COL + 20, "OK");

            sprintf(conscost.message, "%u", facil[building_list.currentchoice - 1].construction_cost);
            sprintf(maincost.message, "%u", facil[building_list.currentchoice - 1].maintenance_cost);
            sprintf(constime.message, "%u", facil[building_list.currentchoice - 1].construction_time);
            sprintf(defenseval.message, "%u", facil[building_list.currentchoice - 1].defense_value);
            sprintf(hitratio.message, "%u", facil[building_list.currentchoice - 1].hit_ratio);

            while (!ok_button.pressed) {
                message.handle();
                conscost.handle();
                maincost.handle();
                constime.handle();
                defenseval.handle();
                hitratio.handle();
                ok_button.handle();
            }

            facil[building_list.currentchoice - 1].construction_cost = atol(conscost.message);
            facil[building_list.currentchoice - 1].maintenance_cost = atol(maincost.message);
            facil[building_list.currentchoice - 1].construction_time = atol(constime.message);
            facil[building_list.currentchoice - 1].defense_value = atol(defenseval.message);
            facil[building_list.currentchoice - 1].hit_ratio = atol(hitratio.message);

            activescreenclass->restore();
        }
    }


    // []----------------------------------------[]
    // |  Ask if we should write out the changes  |
    // []----------------------------------------[]
    if (modified) {
        if (ask_yes_no("\nDo you wish to save these changes?\n")) {
            if ((fp = fopen(basefilename, "rb+")) == NULL) file_error();
            fseek(fp, (basenum - 1) * sizeof(struct basedat), SEEK_SET);
            fwrite(base, sizeof(struct basedat), 1, fp);
            fclose(fp);

            if ((fp = fopen(facilfilename, "rb+")) == NULL) file_error();
            fwrite(facil, sizeof(struct facildat), 17, fp);
            fclose(fp);
        }
    }


    // []-----------------[]
    // |  free everything  |
    // []-----------------[]
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 6; j++) {
            if (building_buttons[i][j]) delete building_buttons[i][j];
        }
    }
    delete base;
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
static char alientypes[][15] = {"Aquatoid","Gill Man","Lobster Man","Tasoth","Calcinite",
    "Deep One","Bio-Drone","Tentaculat","Triscene","Hallucinoid","Xarquid"};
static char alienranks[][15] = {"Commander","Navigator","Medic","Technician",
    "Squad Leader","Soldier","Terrorist"};
#define ASTORE_ROW 3
#define ASTORE_COL 20
void do_edit_astore(int savedgame, int basenum)
{
    FILE *fp;
    char filename[MAXPATH];
    struct astoredat *astore;
    int i, j, numaliens, modified = FALSE, *table;

    // []-------------------[]
    // |  set up the screen  |
    // []-------------------[]
    activescreenclass->save();
    textboxtype message(COLOR(LIGHT_WHITE, BLUE), COLOR(LIGHT_YELLOW, \
        BLUE), ASTORE_ROW, ASTORE_COL, 40, 21, "");
    buttontype add_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY),\
        ASTORE_ROW + 17, ASTORE_COL + 10, "Add...");
    buttontype ok_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY),\
        ASTORE_ROW + 17, ASTORE_COL + 23, "OK");
    menuboxtype *choices = NULL;


    // []--------------------[]
    // |  read in the aliens  |
    // []--------------------[]
    sprintf(filename, "GAME_%i\\ASTORE.DAT", savedgame);
    if ((fp = fopen(filename, "rb")) == NULL) file_error();
    numaliens = filelength(fileno(fp)) / sizeof(struct astoredat);
    if ((astore = new struct astoredat[numaliens]) == NULL) no_memory();
    if ((table = new int[numaliens]) == NULL) no_memory();
    fread(astore, sizeof(struct astoredat), numaliens, fp);
    fclose(fp);


    // []-----------[]
    // |  main loop  |
    // []-----------[]
    while (TRUE) {
        message.handle();
        if (choices == NULL) {
            /*******************************************************/
            /***  Generate a new listing of aliens at this base  ***/
            /*******************************************************/
            unsigned buffersize = 100, bufferused = 0; char *buffer;
            if ((buffer = new char[buffersize]) == NULL) no_memory();
            for (i = j = 0; i < numaliens; i++) {
                if (astore[i].alientype) {
                    if (astore[i].location == baselisting[basenum-1].location) {
                        table[j++] = i;
                        if (buffersize - bufferused < 40) {
                            buffersize += 100;
                            char *newbuffer = new char[buffersize];
                            if (newbuffer == NULL) no_memory();
                            memmove(newbuffer, buffer, bufferused);
                            delete buffer; buffer = newbuffer;
                        }
                        if (astore[i].alientype > 11) astore[i].alientype = 1;
                        if (astore[i].alienrank > 7 || astore[i].alienrank == 0) astore[i].alienrank = 1;
                        char temp[40];
                        sprintf(temp, "%s %s", alientypes[astore[i].alientype - 1], alienranks[astore[i].alienrank - 1]);
                        bufferused += sprintf(buffer + bufferused, "%-30s\n", temp);
                    }
                }
            }
            if (bufferused == 0) sprintf(buffer, "%-30s\n", "no aliens in storage");
            if ((choices = new menuboxtype(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, BLACK), \
                COLOR(GRAY, LIGHT_WHITE), ASTORE_ROW + 2, ASTORE_COL + 3, 12, buffer)) == NULL) no_memory();
            if (bufferused == 0) choices->setchoiceinfo(1, TRUE, FALSE);
            delete buffer;
        } else {
            /***********************************/
            /***  Handle deleting of aliens  ***/
            /***********************************/
            choices->handle();
            if (choices->currentchoice && !activemouseclass->button) {
                if (ask_yes_no("\nDo you want to delete this alien?\n")) {
                    i = table[choices->currentchoice - 1];
                    astore[i].alientype = astore[i].location = astore[i].alienrank = 0;
                    delete choices; choices = NULL;
                    modified = TRUE;
                } else {
                    choices->currentchoice = 0;
                    choices->needupdate = TRUE;
                }
            }
        }


        /*************************************/
        /***  Handle adding of new aliens  ***/
        /*************************************/
        add_button.handle();
        if (add_button.pressed) {
            for (i = 0; i < numaliens; i++) if (astore[i].alientype == 0) break;
            if (i == numaliens) {
                ok_box("No room for any more aliens.\n" \
                    "Delete one from any base first.");
            } else {
                modified = TRUE;
                j = 1 + generic_button_pick("Choose alien type (page 1/2):",
                    "Others...", "Aqua","Gill","Lobstr","Tasoth","Calc");
                if (j == 0) j = 7 + generic_button_pick("Choose alien type (page 2/2):",
                    "DeepOne","BioDrone","Tentac","Triscene","Halluc","Xarq");
                astore[i].alientype = j;
                astore[i].alienrank = 2 + generic_button_pick("Choose alien rank:", \
                    "Cmndr","Navigtr","Medic","Tech","Leadr","Sold","Terist");
                astore[i].location = baselisting[basenum-1].location;
                if (choices) delete choices; choices = NULL;
            }
        }


        /*******************/
        /***  Ok button  ***/
        /*******************/
        ok_button.handle();
        if (ok_button.pressed) break;
    }


    // []-----------------[]
    // |  Free everything  |
    // []-----------------[]
    if (modified) {
        if (ask_yes_no("\nDo you wish to save these changes?\n")) {
            sprintf(filename, "GAME_%i\\ASTORE.DAT", savedgame);
            if ((fp = fopen(filename, "rb+")) == NULL) file_error();
            fwrite(astore, sizeof(struct astoredat), numaliens, fp);
            fclose(fp);
        }
    }
    if (choices) delete choices;
    activescreenclass->restore();
    delete astore;
    delete table;
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
#define EDITOBJECTS_ROW 2
#define EDITOBJECTS_COL 15
void do_edit_objects(void)
{
    FILE *fp;
    menuboxtype *stats = NULL;
    unsigned regenerate_stats = TRUE, modified = FALSE, curobject = 0, numobjects;
    struct obdata *myobjects;


    // []-------------------[]
    // |  Set up the screen  |
    // []-------------------[]
    char *savebuffer = new char[activescreenclass->buffersize];
    if (savebuffer == NULL) no_memory();
    activescreenclass->save(savebuffer);
    textboxtype message(COLOR(LIGHT_WHITE, BLUE), COLOR(LIGHT_YELLOW, \
        BLUE), EDITOBJECTS_ROW, EDITOBJECTS_COL, 46, 23, "");
    buttontype ok_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY),\
        EDITOBJECTS_ROW + 19, EDITOBJECTS_COL + 28, "OK");
    buttontype advanced_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY),\
        EDITOBJECTS_ROW + 19, EDITOBJECTS_COL + 8, "Advanced...");


    // []-----------------------------[]
    // |  Read in the data structures  |
    // []-----------------------------[]
    if ((fp = fopen("geodata\\obdata.dat", "rb")) == NULL) weapon_error();
    numobjects = filelength(fileno(fp)) / sizeof(struct obdata);
    if ((myobjects = new struct obdata[numobjects]) == NULL) no_memory;
    fread(myobjects, sizeof(struct obdata), numobjects, fp);
    fclose(fp);


    while (TRUE) {
        // []-----------------------------[]
        // |  Regenerate stats, if needed  |
        // []-----------------------------[]
        if (regenerate_stats) {
            char *buffer = new char[750];
            if (buffer == NULL) no_memory();
            if (stats) delete stats;
            if (myobjects[curobject].damagetype > 5) myobjects[curobject].damagetype = 0xFF;
            if (myobjects[curobject].is_grenade) {
                if (sprintf(buffer, "Grenade: %-29.29s\nDamage: %i\nDamage Type: %s\n"
                    "Detonation time: %i\n", myobjects[curobject].name, \
                    myobjects[curobject].damage, \
                    damagetype[char(myobjects[curobject].damagetype) + 1], \
                    myobjects[curobject].capacity / 2) >= 750) memory_corrupt();
            } else if (myobjects[curobject].is_ammo) {
                if (sprintf(buffer, "Ammo: %-32.32s\nDamage: %i\nDamage Type: %s\n"
                    "Magazine Capacity: %i\n", myobjects[curobject].name, \
                    myobjects[curobject].damage, \
                    damagetype[char(myobjects[curobject].damagetype) + 1], \
                    myobjects[curobject].capacity) >= 750) memory_corrupt();
            } else if (myobjects[curobject].is_gun) {
                if (sprintf(buffer, "Gun: %-33.33s\nDamage: %i\nDamage Type: %s\n\nAmmo 1: %s\nAmmo 2: %s\n" \
                    "Ammo 3: %s\n\nAccuracy (Auto): %i%%\nAccuracy (Snap): %i%%\n" \
                    "Accuracy (Aimed): %i%%\n\nTime Units (Auto): %i\nTime Units (Snap): %i\n" \
                    "Time Units (Aimed): %i\n", myobjects[curobject].name, \
                    myobjects[curobject].damage, \
                    damagetype[char(myobjects[curobject].damagetype) + 1], \
                    (myobjects[curobject].ammotype[0] != 255 ? myobjects[myobjects[curobject].ammotype[0]].name : "none"), \
                    (myobjects[curobject].ammotype[1] != 255 ? myobjects[myobjects[curobject].ammotype[1]].name : "none"), \
                    (myobjects[curobject].ammotype[2] != 255 ? myobjects[myobjects[curobject].ammotype[2]].name : "none"), \
                    myobjects[curobject].accuracy[0], myobjects[curobject].accuracy[1], \
                    myobjects[curobject].accuracy[2], myobjects[curobject].timeunits[0], \
                    myobjects[curobject].timeunits[1], myobjects[curobject].timeunits[2]) >= 750) memory_corrupt();
            } else {
                if (sprintf(buffer, "%-38s\n", "unknown object type.") >= 750) memory_corrupt();
            }
            if ((stats = new menuboxtype(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, \
                BLACK), COLOR(GRAY, LIGHT_WHITE), EDITOBJECTS_ROW + 1, EDITOBJECTS_COL + 2, 15, \
                buffer)) == NULL) no_memory();
            delete buffer;
            if (myobjects[curobject].is_gun) {
                stats->setchoiceinfo(4, TRUE, FALSE);
                stats->setchoiceinfo(8, TRUE, FALSE);
                stats->setchoiceinfo(12, TRUE, FALSE);
            }
            regenerate_stats = FALSE;
        }

        // []----------------------------[]
        // |  Handle all of the updating  |
        // []----------------------------[]
        message.handle();
        stats->handle();
        if (stats->currentchoice && !activemouseclass->button) {
            switch (stats->currentchoice) {
                case 1: { // current object
                    if (stats) delete stats;
                    int i, buffersize = 100, bufferused = 0;
                    char *buffer = new char[buffersize];
                    if (buffer == NULL) no_memory();
                    buffer[0] = 0;
                    for (i = 0; i < numobjects; i++) {
                        char indicator[4];
                        if (buffersize - bufferused < 40) {
                            buffersize += 100;
                            char *newbuffer = new char[buffersize];
                            if (newbuffer == NULL) no_memory();
                            memmove(newbuffer, buffer, bufferused);
                            delete buffer; buffer = newbuffer;
                        }
                        if (myobjects[i].is_grenade) {
                            strcpy(indicator, "gre");
                        } else if (myobjects[i].is_gun) {
                            strcpy(indicator, "gun");
                        } else if (myobjects[i].is_ammo) {
                            strcpy(indicator, "amm");
                        } else {
                            indicator[0] = 0;
                        }
                        bufferused += sprintf(buffer + bufferused, "%-35s%3.3s\n",
                             myobjects[i].name, indicator);
                    }
                    if ((stats = new menuboxtype(COLOR(BLACK, LIGHT_WHITE), \
                        COLOR(LIGHT_WHITE, BLACK), COLOR(GRAY, LIGHT_WHITE), \
                        EDITOBJECTS_ROW + 1, EDITOBJECTS_COL + 2, 15, buffer)) == NULL) no_memory();
                    delete buffer;
                    stats->setchoiceinfo(curobject + 1, FALSE, TRUE);
                    while (TRUE) {
                        stats->handle();
                        if (stats->currentchoice && !activemouseclass->button) {
                            curobject = stats->currentchoice - 1;
                            break;
                        }
                        ok_button.handle();
                        if (ok_button.pressed) break;
                    }
                    break;
                }
                case 2: // damage
                    stat_edit(myobjects[curobject].damage, 0, 255, "\nDamage (0-255)?\n", 100);
                    modified = TRUE;
                    break;
                case 3: // damage type
                    myobjects[curobject].damagetype = generic_button_pick("Choose damage type:", \
                        "none","piercing","phos.","high exp.","gauss", "sonic","freeze");
                    modified = TRUE;
                    break;
                case 4: // magazine capacity (ammo and grenades only)
                    if (myobjects[curobject].is_grenade) {
                        unsigned temp = myobjects[curobject].capacity / 2;
                        stat_edit(temp, 0, 127, "Grenade detonation time (0-127 turns)?\n", 0);
                        myobjects[curobject].capacity = temp * 2;
                    } else if (myobjects[curobject].is_ammo) {
                        stat_edit(myobjects[curobject].capacity, 0, 255, "Magazine capacity (0-254, \n255=infinite)?\n", 100);
                    }
                    modified = TRUE;
                    break;
                case 5: // ammo 1
                case 6: // ammo 2
                case 7: { // ammo 3
                    int i, j, buffersize = 100, bufferused = 0, found = 0;
                    int ammoslot = stats->currentchoice - 5;
                    char *buffer = new char[buffersize];
                    if (buffer == NULL) no_memory();
                    bufferused = strlen(strcpy(buffer, "none\n"));
                    for (i = 0, j = 2; i < numobjects; i++) {
                        if (!myobjects[i].is_ammo) continue;
                        if (buffersize - bufferused < 40) {
                            buffersize += 100;
                            char *newbuffer = new char[buffersize];
                            if (newbuffer == NULL) no_memory();
                            memmove(newbuffer, buffer, bufferused);
                            delete buffer; buffer = newbuffer;
                        }
                        bufferused += sprintf(buffer + bufferused, "%-38s\n", myobjects[i].name);
                        if (i == myobjects[curobject].ammotype[ammoslot]) found = j;
                        j++;
                    }
                    if (myobjects[curobject].ammotype[ammoslot] == 255) found = 1;
                    if (stats) delete stats;
                    if ((stats = new menuboxtype(COLOR(BLACK, LIGHT_WHITE), \
                        COLOR(LIGHT_WHITE, BLACK), COLOR(GRAY, LIGHT_WHITE), \
                        EDITOBJECTS_ROW + 1, EDITOBJECTS_COL + 2, 15, buffer)) == NULL) no_memory();
                    delete buffer;
                    stats->setchoiceinfo(found, FALSE, TRUE);
                    while (TRUE) {
                        stats->handle();
                        if (stats->currentchoice && !activemouseclass->button) {
                            if (stats->currentchoice == 1) {
                                myobjects[curobject].ammotype[ammoslot] = 255;
                            } else {
                                for (i = 0, j = 2; i < numobjects; i++) {
                                    if (!myobjects[i].is_ammo) continue;
                                    if (j++ == stats->currentchoice) myobjects[curobject].ammotype[ammoslot] = i;
                                }
                            }
                            break;
                        }
                        ok_button.handle();
                        if (ok_button.pressed) break;
                    }
                    modified = TRUE;
                    break;
                }
                case 9: // accuracy (auto)
                case 10: // accuracy (snap)
                case 11: // accuracy (aimed)
                    stat_edit(myobjects[curobject].accuracy[stats->currentchoice-9], 0, 255, "\nAccuracy (0-255%)?\n", 100);
                    modified = TRUE;
                    break;
                case 13: // time units (auto)
                case 14: // time units (snap)
                case 15: // time units (aimed)
                    stat_edit(myobjects[curobject].timeunits[stats->currentchoice-13], 0, 255, "\nTime Units (0-255)?\n", 0);
                    modified = TRUE;
                    break;
                default: break;
            }
            regenerate_stats = TRUE;
        }


        // []-----------------[]
        // |  Advanced Button  |
        // []-----------------[]
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
                    bufferused += sprintf(buffer+bufferused, "icons: %u/%u\n", myobjects[curobject].icons[0], myobjects[curobject].icons[1]);
                    bufferused += sprintf(buffer+bufferused, "width/height: %u/%u\n", myobjects[curobject].width, myobjects[curobject].height);
                    bufferused += sprintf(buffer+bufferused, "health: %u\n", myobjects[curobject].health);
                    bufferused += sprintf(buffer+bufferused, "weight: %u\n", myobjects[curobject].weight);
                    bufferused += sprintf(buffer+bufferused, "     is_gun: %u\n", myobjects[curobject].is_gun);
                    bufferused += sprintf(buffer+bufferused, "     is_ammo: %u\n", myobjects[curobject].is_ammo);
                    bufferused += sprintf(buffer+bufferused, "     is_grenade: %u\n", myobjects[curobject].is_grenade);
                    bufferused += sprintf(buffer+bufferused, "contact_weapon_only: %u\n", myobjects[curobject].contact_weapon_only);
                    bufferused += sprintf(buffer+bufferused, "two_handed: %u\n", myobjects[curobject].two_handed);
                    bufferused += sprintf(buffer+bufferused, "is_disruptor: %u\n", myobjects[curobject].is_disruptor);
                    bufferused += sprintf(buffer+bufferused, "is_alien: %u\n", myobjects[curobject].is_alien);
                    bufferused += sprintf(buffer+bufferused, "uses_ammo: %u\n", myobjects[curobject].uses_ammo);
                    bufferused += sprintf(buffer+bufferused, "unk2: %02X\n", myobjects[curobject].unk2);
                    bufferused += sprintf(buffer+bufferused, "unk3: %02X %02X\n", myobjects[curobject].unk3[0], myobjects[curobject].unk3[1]);
                    bufferused += sprintf(buffer+bufferused, "unk4: %02X %02X %02X\n", myobjects[curobject].unk4[0], myobjects[curobject].unk4[1], myobjects[curobject].unk4[2]);
                    bufferused += sprintf(buffer+bufferused, "unk5: %02X\n", myobjects[curobject].unk5);
                    bufferused += sprintf(buffer+bufferused, "unk6: %02X\n", myobjects[curobject].unk6);
                    if (bufferused >= buffersize) memory_corrupt();
                       if ((choices = new menuboxtype(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, BLACK), \
                           COLOR(GRAY, LIGHT_WHITE), 2, 20, buffer)) == NULL) no_memory();
                    delete buffer;
                } else {
                          choices->handle();
                    if (choices->currentchoice && !activemouseclass->button) {
                        switch (choices->currentchoice) {
                            case 1: // icons
                                stat_edit(myobjects[curobject].icons[0], 0, 0xFF, "\nAmount for icon 1?\n", 180);
                                stat_edit(myobjects[curobject].icons[1], 0, 0xFF, "\nAmount for icon 2?\n", 180);
                                break;
                            case 2: // width/height
                                stat_edit(myobjects[curobject].width, 0, 0xFF, "\nAmount for width?\n", 180);
                                stat_edit(myobjects[curobject].height, 0, 0xFF, "\nAmount for height?\n", 180);
                                break;
                            case 3: // health
                                stat_edit(myobjects[curobject].health, 0, 0xFF, "\nAmount for health?\n", 180);
                                break;
                            case 4: // weight
                                stat_edit(myobjects[curobject].weight, 0, 0xFF, "\nAmount for weight?\n", 180);
                                break;
                            case 5: // is_grenade
                                stat_edit(myobjects[curobject].is_grenade, 0, 0xFF, "\nAmount for is_grenade?\n", 180);
                                break;
                            case 6: // uses_ammo
                                stat_edit(myobjects[curobject].uses_ammo, 0, 0xFF, "\nAmount for uses_ammo?\n", 180);
                                break;
                            case 7: // contact_weapon_only
                                stat_edit(myobjects[curobject].contact_weapon_only, 0, 0xFF, "\nAmount for contact_weapon_only?\n", 180);
                                break;
                            case 8: // is_gun
                                stat_edit(myobjects[curobject].is_gun, 0, 0xFF, "\nAmount for is_gun?\n", 180);
                                break;
                            case 9: // is_ammo
                                stat_edit(myobjects[curobject].is_ammo, 0, 0xFF, "\nAmount for is_ammo?\n", 180);
                                break;
                            case 10: // two_handed
                                stat_edit(myobjects[curobject].two_handed, 0, 0xFF, "\nAmount for two_handed?\n", 180);
                                break;
                            case 11: // is_disruptor
                                stat_edit(myobjects[curobject].is_disruptor, 0, 0xFF, "\nAmount for is_disruptor?\n", 180);
                                break;
                            case 12: // is_alien
                                stat_edit(myobjects[curobject].is_alien, 0, 0xFF, "\nAmount for is_alien?\n", 180);
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

        // []-----------[]
        // |  OK Button  |
        // []-----------[]
        ok_button.handle();
        if (ok_button.pressed) break;
    }


    // []-----------------[]
    // |  Free everything  |
    // []-----------------[]
    delete stats;
    activescreenclass->restore(savebuffer);
    delete savebuffer;
    if (modified) {
        if (ask_yes_no("\nDo you wish to save these changes?\n")) {
            if ((fp = fopen("geodata\\obdata.dat", "rb+")) == NULL) weapon_error();
            fwrite(myobjects, sizeof(struct obdata), numobjects, fp);
            fclose(fp);
        }
    }
    delete myobjects;
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같

