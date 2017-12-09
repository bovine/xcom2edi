//          C++ Text User Interface Kit
//
//              by Jeff Lawson of JL Enterprises, 1994
//
#include <string.h>
#include <stdlib.h>
#include <iostream.h>
#include <dos.h>
#include <conio.h>
#include <time.h>
#include "mouse.hpp"
#include "screen.hpp"
#include "menubox.hpp"
#include "colors.hpp"

// Defining this will allow menubox to keep its selection hilighted even
// after the user has released the button.  Useful for menuboxes that will
// remain on screen
#define STICKYCHOICES       

// some defines
#define TRUE 1
#define FALSE 0

// define any external error message routines we'll use
extern void no_memory (void);

//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
menuboxtype::menuboxtype (color mynormal, color myhilite, color mygray, \
        int myrow, int mycol, int maxchoices, char *mychoices)
{
    row = myrow;
    col = mycol;
    currentchoice = 0;
    lasttoprow = toprow = 1;
    if ((menuheight = maxchoices + 2) < 4) menuheight = 4;
    normalattr = mynormal;
    hiliteattr = myhilite;
    grayattr = mygray;
    needupdate = TRUE;

    // initialize the information relating to the text
    if (mychoices && strlen(mychoices)) {
        if ((curptr = choices = new char[strlen(mychoices) + 2]) == NULL) no_memory();
    } else choices = NULL;

    // compute the furthest we can scroll down and copy the buffer
    char *s = mychoices, *t = choices, *start = choices, last = NULL;
    numchoices = menuwidth = 0;
    while (TRUE) {
        if (*s == NULL) {
            if (last != '\n') {
                numchoices++;
                *t++ = '\n';
            }
            if (t - start + 1> menuwidth) menuwidth = t - start + 1;
            *t++ = NULL;
            break;
        } else if (*s == '\n') {
            if (t - start + 1 > menuwidth) menuwidth = t - start + 1;
            numchoices++;
            s++;
            last = *t++ = '\n';
            start = t;
        } else if (*s == '\r') {
            s++;
        } else {
            last = *t++ = *s++;
        }
    }
    menuwidth += 3;
    maxtoprow = numchoices - menuheight + 3;
    if (maxtoprow < 1) maxtoprow = 1;

    // allocate memory and say that all choices are good and not checked
    if ((choiceinfo = new struct choiceinfotype[numchoices]) == NULL) no_memory();
    for (int i = 0; i < numchoices; i++) \
    	choiceinfo[i].notvalid = choiceinfo[i].checked = FALSE;
}
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
menuboxtype::menuboxtype (color mynormal, color myhilite, color mygray, \
        int myrow, int mycol, char *mychoices)
{
    row = myrow;
    col = mycol;
    currentchoice = 0;
    lasttoprow = toprow = 1;
    normalattr = mynormal;
    hiliteattr = myhilite;
    grayattr = mygray;
    needupdate = TRUE;

    // count the number of choices and find the longest one
    char *p = mychoices;
    for (numchoices = 0, menuwidth = 0; *p; numchoices++) {
        char *start = p;
        while (*p++ != '\n');
        if (p - start > menuwidth) menuwidth = p - start;
    }
    menuwidth += 3;
    menuheight = (row + numchoices + 2 > 25 ? 25 - row - 2 : numchoices) + 2;
    maxtoprow = numchoices - menuheight + 3;
    if (maxtoprow < 1) maxtoprow = 1;

    // allocate memory, and make a copy of the choice strings
    curptr = choices = new char[p - mychoices + 1];
    if (choices == NULL) no_memory();
    strcpy(choices, mychoices);

    // allocate memory and say that all choices are good and not checked
    choiceinfo = new struct choiceinfotype[numchoices];
    if (choiceinfo == NULL) no_memory();
    int i;
    for (i = 0; i < numchoices; i++) {
        choiceinfo[i].notvalid = FALSE;
        choiceinfo[i].checked = FALSE;
    }
}
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
menuboxtype::~menuboxtype (void)
{
    delete choices;
    delete choiceinfo;
}
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
void menuboxtype::setchoiceinfo(int choice, int notvalid, int checked)
{
    if (choice > 0 && choice <= numchoices) {
        choiceinfo[choice - 1].notvalid = (notvalid != FALSE);
        choiceinfo[choice - 1].checked = (checked != FALSE);
    }
    needupdate = TRUE;
}
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
void menuboxtype::draw (void)
{
    // make the selected item be displayed in the window
    if (currentchoice) {
        if (toprow > currentchoice) toprow = currentchoice;
        if (toprow + menuheight - 3 < currentchoice) toprow = currentchoice - (menuheight - 3);
    }

    // recalculate the choice ptr, if necessary
    if (lasttoprow != toprow) {
        if (toprow < 1) toprow = 1;
        if (toprow > maxtoprow) toprow = maxtoprow;
        if (toprow == 1) {
            curptr = choices;
        } else {
            int count = toprow - lasttoprow;
            if (count < 0) count = -count;

            for (; count > 0; count--) {
                if (toprow > lasttoprow) {  // seek forward
                    while (*curptr++ != '\n');
                } else {                    // seek backward
                    curptr -= 2;
                    while (*curptr-- != '\n');
                    curptr += 2;
                }
            }
        }
        lasttoprow = toprow;
    }

    // draw top row
    activescreenclass->drawchar('Ú', row, col, normalattr);
    activescreenclass->drawrepeat('Ä', menuwidth - 2, row, col + 1, normalattr);
    activescreenclass->drawchar('¿', row, col + menuwidth - 1, normalattr);

    // draw middle row(s)
    int i, j, currow = row + 1; char *p = curptr;
    int firstline = 1 + (menuheight - 4)*(toprow - 1)/(numchoices ? numchoices : 1);
    int lastline = firstline + (menuheight - 4)*(menuheight - 2)/(numchoices ? numchoices : 1);
    for (i = 0; i < menuheight - 2; i++, currow++) {
        // display left border
        activescreenclass->drawchar('³', currow, col, normalattr);

        // display the text
        if (toprow + i <= numchoices) {
            int attr = (currentchoice == i + toprow ? hiliteattr : normalattr);
            if (choiceinfo[toprow + i - 1].notvalid) attr = grayattr;
            activescreenclass->drawrepeat(' ', menuwidth - 2, currow, col + 1, attr);
            if (choiceinfo[toprow + i - 1].checked) activescreenclass->drawchar('û', currow, col + 1, attr);

            for (j = 0; *p != '\n'; p++) {
                if (j < menuwidth - 4) activescreenclass->drawchar(*p, currow, \
                    col + 2 + j++, attr);
            }
            p++;
        } else {
            activescreenclass->drawrepeat(' ', menuwidth - 2, currow, col + 1, normalattr);
        }

        // display right border
        if (maxtoprow == 1) j = '³';
        else if (i == 0) j = 24;
        else if (i == menuheight - 3) j = 25;
        else j = (i >= firstline && i <= lastline ? '±' : '°');
        activescreenclass->drawchar(j, currow, col + menuwidth - 1, normalattr);
    }

    // draw bottom row
    activescreenclass->drawchar('À', row + menuheight - 1, col, normalattr);
    activescreenclass->drawrepeat('Ä', menuwidth - 2, row + menuheight - 1, col + 1, normalattr);
    activescreenclass->drawchar('Ù', row + menuheight - 1, col + menuwidth - 1, normalattr);

    needupdate = FALSE;
}
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
void menuboxtype::handle (void)
{
    clock_t myclock = clock();
    int delay = 0, inside = FALSE, pressed = FALSE;

    #ifndef STICKYCHOICES
        if (currentchoice) {currentchoice = 0; needupdate = TRUE;}
    #endif

    // Drain the keyboard if nothing has read it for awhile
    static passesby = 0, kbstart = 0;
    if (peek(0x40, 0x1A) != peek(0x40, 0x1C) && kbstart == peek(0x40, 0x1A)) {
        if (passesby++ > 5) while(kbhit()) getch();
    } else {passesby = 0; kbstart = peek(0x40, 0x1A);}

    // Get to work
    while (TRUE) {
        if (needupdate) {draw(); activescreenclass->refresh(); }

        if (activemouseclass->button) {
            if (clock() != myclock) {myclock = clock(); if (delay) delay--;}

            if ((activemouseclass->col == col + menuwidth - 1 && activemouseclass->row > row + 1 && activemouseclass->row < row + menuheight - 2) || pressed) {
                // on slider bar
                int newtoprow = 1 + (8 * activemouseclass->row + activemouseclass->rowfrac - 8*(row + 2))*numchoices/(8*(menuheight-4));
                if (newtoprow < 1) newtoprow = 1;
                if (newtoprow > maxtoprow) newtoprow = maxtoprow;
                if (toprow != newtoprow) {
                    toprow = newtoprow;
                    if (currentchoice < toprow || currentchoice > toprow + menuheight - 3) currentchoice = 0;
                    needupdate = TRUE;
                }
                pressed = TRUE;
            } else if (numchoices > menuheight - 2 && ((activemouseclass->row == row + 1 && activemouseclass->col == col + menuwidth - 1) || (activemouseclass->row < row + 1 && inside))) {
                // up-arrow or above menu
                if (delay == 0) {
                    if (currentchoice > 1) {
                        delay = 3; currentchoice--; needupdate = TRUE;
                    } else if (toprow > 1) {
                        delay = 3; toprow--; needupdate = TRUE;
                    }
                }
            } else if (numchoices > menuheight - 2 && ((activemouseclass->row == row + menuheight - 2 && activemouseclass->col == col + menuwidth - 1) || (activemouseclass->row > row + menuheight - 2 && inside))) {
                // down-arrow or below menu
                if (delay == 0) {
                    if (currentchoice < numchoices && currentchoice > 0) {
                        delay = 3; currentchoice++; needupdate = TRUE;
                    } else if (toprow < maxtoprow) {
                        delay = 3; toprow++; needupdate = TRUE;
                    }
                }
            } else if (activemouseclass->col > col && activemouseclass->col < col + menuwidth - 1) {
                // inside the menu box
                int newchoice = toprow + activemouseclass->row - row - 1;
                if (newchoice >= toprow && newchoice <= toprow + menuheight - 3 && newchoice <= numchoices) {
                    if (choiceinfo[newchoice - 1].notvalid) {
                        #ifdef STICKYCHOICES
                            newchoice = currentchoice;
                        #else
                            newchoice = 0;
                        #endif
                    }
                    if (currentchoice != newchoice) {
                        currentchoice = newchoice;
                        needupdate = TRUE;
                    }
                    inside = TRUE;
                } else {
                    #ifndef STICKYCHOICES
                        if (currentchoice) {currentchoice = 0; needupdate = TRUE; continue;}
                    #endif
                    return;
                }
            } else {
                // button is pressed, but is not located where we want it to be
                #ifndef STICKYCHOICES
                    if (currentchoice) {currentchoice = 0; needupdate = TRUE; continue;}
                #endif
                return;
            }
        } else {
            // button is not pressed
            return;
        }
    }
}
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
int menuboxtype::ischecked(int choice)
{
    if (choice > 0 && choice <= numchoices) {
        return(choiceinfo[choice - 1].checked);
    }
    return(0);
}
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
