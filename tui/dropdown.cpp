//          C++ Text User Interface Kit
//
//              by Jeff Lawson of JL Enterprises, 1994
//
#include <string.h>
#include <stdlib.h>
#include <iostream.h>
#include <stdarg.h>
#include <dos.h>
#include <conio.h>
#include "mouse.hpp"
#include "screen.hpp"
#include "dropdown.hpp"

#ifndef __LARGE__
    #error must be compiled in large model
#endif

// define any external error message routines we'll use
extern void no_memory (void);

//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
dropdowntype::dropdowntype(int mynormal, int myhilite, int myrow, int mycol, \
        int mywidth, char *mychoices)
{
    // general initialization
    row = myrow;
    col = mycol;
    width = mywidth;
    currentchoice = 0;
    normalattr = mynormal;
    hiliteattr = myhilite;
    droppeddown = 0;
    needupdate = 1;

    // count the number of choices, allocate memory, and make a copy of them
    char *p = mychoices; int longest = 0;
    for (numchoices = 0; *p; numchoices++) {
        char *start = p;
        while (*p++ != '\n');
        if (p - start > longest) longest = p - start;
    }
    choices = new char[strlen(mychoices) + 1];
    if (choices == NULL) no_memory();
    strcpy(choices, mychoices);
    currentchoicetext = new char[longest];
    if (currentchoicetext == NULL) no_memory();
    strcpy(currentchoicetext, "");
}
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
dropdowntype::~dropdowntype(void)
{
    delete choices;
    delete currentchoicetext;
}
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
void dropdowntype::draw(void)
{
    int i, count, curcol;
    char *p, *s, *t;

    // figure out the text for the choice that we're on
    if (currentchoice < 1 || currentchoice > numchoices) currentchoice = 1;
    s = choices; t = currentchoicetext;
    for (i = 1; i < currentchoice; i++) while(*s++ != '\n');
    while ((*t++ = *s++) != '\n');
    *(t-1) = 0;

    // display the text
    count = width; curcol = col; p = currentchoicetext;
    activescreenclass->drawrepeat(' ', width, row, col, normalattr);
    while (*p && count--) activescreenclass->drawchar(*p++, row, curcol++, normalattr);

    if (droppeddown) {
        // draw top row
        activescreenclass->drawchar('Ú', row + 1, col - 2, normalattr);
        activescreenclass->drawrepeat('Ä', width + 2, row + 1, col - 1, normalattr);
        activescreenclass->drawchar('¿', row + 1, col + width + 1, normalattr);

        // draw middle row(s)
        for (i = 0, p = choices; i < numchoices; i++, p++) {
            // display left border and wipe text area
            int attr = (currentchoice == i + 1 ? hiliteattr : normalattr);
            activescreenclass->drawchar('³', row + 2 + i, col - 2, normalattr);
            activescreenclass->drawrepeat(' ', width + 2, row + 2 + i, col - 1, attr);

            // display the text
            count = width; curcol = col;
            while (*p != '\n') {
                if (count-- > 0) activescreenclass->drawchar(*p, row + 2 + i, curcol++, attr);
                p++;
            }

            // display right border
            activescreenclass->drawchar('³', row + 2 + i, col + width + 1, normalattr);
        }

        // draw bottom row
        activescreenclass->drawchar('À', row + 2 + numchoices, col - 2, normalattr);
        activescreenclass->drawrepeat('Ä', width + 2, row + 2 + numchoices, col - 1, normalattr);
        activescreenclass->drawchar('Ù', row + 2 + numchoices, col + width + 1, normalattr);
    }

    // say the menu has just been redrawn
    needupdate = 0;
}
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
void dropdowntype::handle(void)
{
    int oldx, oldy;
    char *savebuffer;
    
    // make sure that the mouse button is down and is on the drop down box
    if (droppeddown) {droppeddown = 0; needupdate = 1;}
    if (activemouseclass->row == row && activemouseclass->col >= col && \
            activemouseclass->col <= col + width - 1 && activemouseclass->button) {
        savebuffer = new char[activescreenclass->buffersize];
        if (savebuffer == NULL) no_memory();
        activescreenclass->save(savebuffer);
        oldx = activemouseclass->col;
        oldy = activemouseclass->row;
        activemouseclass->locatemouse(currentchoice + row + 1, activemouseclass->col);
        droppeddown = 1; needupdate = 1;
    } else {
        if (needupdate) {draw(); activescreenclass->refresh();}
        return;
    }

    // actually handle the drop down box
    while (1) {
        if (needupdate) {draw(); activescreenclass->refresh();}
        if (droppeddown && activemouseclass->row > row + 1 && activemouseclass->row < row + 2 + numchoices && \
                activemouseclass->col >= col && activemouseclass->col <= col + width - 1) {
            if (activemouseclass->button) {
                // mouse is on a choice and button is down
                if (currentchoice != activemouseclass->row - (row + 1)) {
                    currentchoice = activemouseclass->row - (row + 1);
                    needupdate = 1;
                }
            } else {
                // mouse is on a choice, but button is up
                break;
            }
        } else {
            if (!activemouseclass->button) {
                // mouse is not on a choice and the button is up
                break;
            }
        }
    }
    activescreenclass->restore(savebuffer);
    delete savebuffer;
    activemouseclass->locatemouse(oldy, oldx);

    // Drain the keyboard if nothing has read it for awhile
    static passesby = 0, kbstart = 0;
    if (peek(0x40, 0x1A) != peek(0x40, 0x1C) && kbstart == peek(0x40, 0x1A)) {
        if (passesby++ > 5) while(kbhit()) getch();
    } else {passesby = 0; kbstart = peek(0x40, 0x1A);}
}
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
