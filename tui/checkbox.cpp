//          C++ Text User Interface Kit
//
//              by Jeff Lawson of JL Enterprises, 1994
//
#include <string.h>
#include <stdlib.h>
#include <iostream.h>
#include <dos.h>
#include <conio.h>
#include "mouse.hpp"
#include "screen.hpp"
#include "checkbox.hpp"

#ifndef __LARGE__
    #error must be compiled in large model
#endif

// define any external error message routines we'll use
extern void no_memory (void);

//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
checkboxtype::checkboxtype (int mynormal, int myrow, int mycol, char *mymessage)
{
    row = myrow;
    col = mycol;
    checked = 0;
    normalattr = mynormal;
    needupdate = 1;

    messagelength = strlen(mymessage) + 4;
    message = new char[strlen(mymessage) + 1];
    if (message == NULL) no_memory();
    strcpy(message, mymessage);
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
checkboxtype::~checkboxtype (void)
{
    delete message;
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void checkboxtype::draw (void)
{
    activescreenclass->drawchar('[', row, col, normalattr);
    activescreenclass->drawchar((checked ? 'X' : ' '), row, col + 1, normalattr);
    activescreenclass->drawstring("] ", row, col + 2, normalattr);
    activescreenclass->drawstring(message, row, col + 4, normalattr);

    needupdate = 0;
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void checkboxtype::handle (void)
{
    int pressed = 0;

    while (1) {
        if (needupdate) {draw(); activescreenclass->refresh();}

        if (activemouseclass->row == row && activemouseclass->col >= col && \
                activemouseclass->col <= col + messagelength - 1) {
            if (activemouseclass->button) {
                // mouse is positioned inside and button is currently pressed
                pressed = 1;
            } else {
                // mouse is positioned inside but button is currently released
                if (pressed) {      // if it was ever clicked on us
                    checked = !checked;
                    pressed = 0;
                    needupdate = 1;
                } else {
                    break;
                }
            }
        } else {
            // not within range
            break;
        }
    }

    // Drain the keyboard if nothing has read it for awhile
    static passesby = 0, kbstart = 0;
    if (peek(0x40, 0x1A) != peek(0x40, 0x1C) && kbstart == peek(0x40, 0x1A)) {
        if (passesby++ > 5) while(kbhit()) getch();
    } else {passesby = 0; kbstart = peek(0x40, 0x1A);}
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
