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
void edit_diplomacy(int savedgame)
{
    FILE *fp;
    char filename[MAXPATH];
    int i, j, k, modified = FALSE;
    struct diplom zone[16];
    char *countries[] = {"USA","Alaska","Euro-Syndicate","Arabian Bloc","Egyptian Cartel","Africa Corp",
        "Brazilian Union","New Mexico","Asian Coalition","Scandinavia","Neo-Japan","Free China","Australiasia",
        "Feo Korea","Eurasia","Icelandic Union"};


    // []------------------[]
    // |  clear the screen  |
    // []------------------[]
    activescreenclass->drawrepeat('�', 80*25, 1, 1, COLOR(LIGHT_WHITE, LIGHT_BLUE));
    activescreenclass->refresh();


    // []----------------------------[]
    // |  read in all of the entries  |
    // []----------------------------[]
    sprintf(filename, "GAME_%i\\DIPLOM.DAT", savedgame);
    if ((fp = fopen(filename, "rb")) == NULL) file_error();
    fread(zone, sizeof(struct diplom), 16, fp);
    fclose(fp);


    // []-------------------------------------[]
    // |  Create the controls that we'll need  |
    // []-------------------------------------[]
    buttontype ok_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY), 2, 73, "OK");
    menuboxtype *relations = NULL;


    // []-----------[]
    // |  Main loop  |
    // []-----------[]
    while (TRUE) {
        // *********************************************
        // ***  Regenerate the listing if necessary  ***
        // *********************************************
        if (!relations) {
            unsigned buffersize = 300, bufferused = 0;
            char *buffer = new char[buffersize];
            if (buffer == NULL) no_memory();
            bufferused = sprintf(buffer, "Country         Zindex Status Traitor Incr. Funds Next.\n");
            for (i = 0; i < 16; i++) {
                if (buffersize - bufferused < 100) {
                    buffersize += 300;
                    char *newbuffer = new char[buffersize];
                    if (newbuffer == NULL) no_memory();
                    memmove(newbuffer, buffer, buffersize);
                    delete buffer; buffer = newbuffer;
                }
                bufferused += sprintf(buffer + bufferused, "%-15.15s %6u %6u "
                    "%7u %5u %5u %5u\n", countries[i], zone[i].zindex, zone[i].status,
                    zone[i].traitor, zone[i].incfunding, zone[i].funding,
                    zone[i].funds_next_factor);
            }
            relations = new menuboxtype(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, BLACK), \
                COLOR(GRAY, LIGHT_WHITE), 6, 5, 17, buffer);
            relations->setchoiceinfo(1, TRUE, FALSE);
            delete buffer;
        }


        // ****************************
        // ***  Handle the listing  ***
        // ****************************
        relations->handle();
        if (relations->currentchoice && !activemouseclass->button) {
            int country = relations->currentchoice - 2;
            int col = activemouseclass->col - 7;
            if (col > 15 && col < 22) { // zindex
                stat_edit(zone[country].zindex, 0, 0x7FFF, "\nZindex?\n", 0);
                delete relations; relations = NULL;
                modified = TRUE;
            } else if (col > 22 && col < 29) { // status
                stat_edit(zone[country].status, 0, 0xFFFF, "\nStatus? (3=happy)\n", 3);
                delete relations; relations = NULL;
                modified = TRUE;
            } else if (col > 29 && col < 37) { // traitor
                stat_edit(zone[country].traitor, 0, 0xFFFF, "\nTraitor? (1=turning)\n", 3);
                delete relations; relations = NULL;
                modified = TRUE;
            } else if (col > 37 && col < 43) { // increase
                stat_edit(zone[country].incfunding, 0, 0xFFFF, "\nIncrease rate?\n", 0x27F);
                delete relations; relations = NULL;
                modified = TRUE;
            } else if (col > 43 && col < 49) { // funds
                stat_edit(zone[country].funding, 0, 0x7FFF, "\nFunding amount?\n", 0x3F3F);
                delete relations; relations = NULL;
                modified = TRUE;
            } else if (col > 49 && col < 55) { // next
                stat_edit(zone[country].funds_next_factor, 0, 0x7FFF, "\nNext funding amount?\n", 0x3F3F);
                delete relations; relations = NULL;
                modified = TRUE;
            } else {
                relations->currentchoice = 0;
                relations->needupdate = TRUE;
            }
        }


        // *****************************
        // ***  Handle the ok button ***
        // *****************************
        ok_button.handle();
        if (ok_button.pressed) break;
    }


    // []-----------------------------------[]
    // |  Ask if we should save the changes  |
    // []-----------------------------------[]
    if (modified) {
        if (ask_yes_no("\nDo you wish to save these changes?\n")) {
            if ((fp = fopen(filename, "rb+")) == NULL) file_error();
            fwrite(zone, sizeof(struct diplom), 16, fp);
            fclose(fp);
        }
    }
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
