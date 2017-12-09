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
#include "button.hpp"
#include "colors.hpp"

// define any external error message routines we'll use
extern void no_memory (void);

//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
buttontype::buttontype (color mynormal, color myhilite, int myrow, int mycol, \
        char *mymessage)
{
    row = myrow;
    col = mycol;
    pressed = 0;
    normalattr = mynormal;
    hiliteattr = myhilite;
    needupdate = 1;

    // compute the width and height of the button
    message = new char[strlen(mymessage) + 2];
    if (message == NULL) no_memory();
    char *s = mymessage, *t = message, last = NULL;
    buttonwidth = 0; buttonheight = 0; int colcount = 0;
    while (1) {
        if (*s == NULL) {
            if (last != 10) {
                buttonheight++;
                *t++ = '\n';
            }
            *t++ = NULL;
            break;
        } else if (*s == 10) {
            buttonheight++; colcount = 0;
            last = *t++ = *s++;
        } else if (*s == 13) {
            s++;
        } else {
            last = *t++ = *s++;
            colcount++; if (colcount > buttonwidth) buttonwidth = colcount;
        }
    }
    buttonwidth += 4;
    buttonheight += 2;
}
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
buttontype::~buttontype (void)
{
    delete message;
}
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
void buttontype::draw (void)
{
    int attr = (pressed ? hiliteattr : normalattr);

    // draw top row
    activescreenclass->drawchar('É', row, col, attr);
    activescreenclass->drawrepeat('Í', buttonwidth - 2, row, col + 1, attr);
    activescreenclass->drawchar('»', row, col + buttonwidth - 1, attr);

    // draw middle row(s)
    char *p = message; int i, j;
    for (i = 0; i < buttonheight - 2; i++) {
        activescreenclass->drawchar('º', row + 1 + i, col, attr);
        activescreenclass->drawrepeat(' ', buttonwidth - 2, row + 1 + i, col + 1, attr);
        activescreenclass->drawchar('º', row + 1 + i, col + buttonwidth - 1, attr);
        for (j = 0; *p != '\n'; p++, j++) {
            activescreenclass->drawchar(*p, row + 1 + i, col + 2 + j, attr);
        }
        if (*p == NULL) break;
        p++;
    }

    // draw bottom row
    activescreenclass->drawchar('È', row + buttonheight - 1, col, attr);
    activescreenclass->drawrepeat('Í', buttonwidth - 2, row + buttonheight - 1, col + 1, attr);
    activescreenclass->drawchar('¼', row + buttonheight - 1, col + buttonwidth - 1, attr);

    needupdate = 0;
}
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
void buttontype::handle (void)
{
    if (pressed) {pressed = 0; needupdate = 1;}

    while (1) {
        if (needupdate) {draw(); activescreenclass->refresh();}

        if (activemouseclass->row >= row && activemouseclass->row <= row + buttonheight - 1 && \
                activemouseclass->col >= col && activemouseclass->col <= col + buttonwidth - 1) {
            if (activemouseclass->button) {
                // mouse is positioned inside and button is currently pressed
                if (!pressed) {pressed = activemouseclass->button; needupdate = 1;}
            } else {
                // mouse is positioned inside but button is currently released
                break;
            }
        } else {
            // not within range
            if (pressed) {pressed = 0; needupdate = 1;} else break;
        }
    }

    // Drain the keyboard if nothing has read it for awhile
    static passesby = 0, kbstart = 0;
    if (peek(0x40, 0x1A) != peek(0x40, 0x1C) && kbstart == peek(0x40, 0x1A)) {
        if (passesby++ > 5) while(kbhit()) getch();
    } else {passesby = 0; kbstart = peek(0x40, 0x1A);}
}
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
