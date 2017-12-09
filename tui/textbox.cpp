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
#include "textbox.hpp"
#include "colors.hpp"

// some defines
#define TRUE 1
#define FALSE 0

// define any external error message routines we'll use
extern void no_memory (void);

//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
textboxtype::textboxtype(color mynormal, color myhilite, int myrow, int mycol, \
        int mywidth, int myheight, char *mymessage)
{
    // set the dimensions of the window
    row = myrow;
    col = mycol;
    width = mywidth;
    height = myheight;
    if (width < 5 && mymessage) width = 5;
    if (height < 5 && mymessage) height = 5;
    normalattr = mynormal;
    hiliteattr = myhilite;

    // initialize the information relating to the text
    lasttoprow = 1;
    toprow = 1;
    if (mymessage) {
        message = new char[strlen(mymessage) + 2];
        if (message == NULL) no_memory();
    } else message = NULL;
    curptr = message;

    // compute the furthest we can scroll down and copy the buffer
    char *s = mymessage, *t = message, last = NULL;
    linecount = 0;
    while (TRUE) {
        if (*s == NULL) {
            if (last != 10) {
                linecount++;
                *t++ = '\n';
            }
            *t++ = NULL;
            break;
        } else if (*s == 10) {
            linecount++;
            last = *t++ = *s++;
        } else if (*s == 13) {
            s++;
        } else {
            last = *t++ = *s++;
        }
    }
    maxtoprow = linecount - height + 3;
    if (maxtoprow < 1) maxtoprow = 1;

    // say we need to be updated
    needupdate = TRUE;
}
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
textboxtype::~textboxtype (void)
{
    if (message) delete message;
}
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
void textboxtype::draw (void)
{
    // recalculate the message ptr, if necessary
    if (lasttoprow != toprow && message) {
        if (toprow < 1) toprow = 1;
        if (toprow > maxtoprow) toprow = maxtoprow;
        if (toprow == 1) {
            curptr = message;
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
    }
    lasttoprow = toprow;

    // draw top row
    activescreenclass->drawchar('Ú', row, col, normalattr);
    activescreenclass->drawrepeat('Ä', width - 2, row, col + 1, normalattr);
    activescreenclass->drawchar('¿', row, col + width - 1, normalattr);

    // draw middle rows
    int i, j, currow = row + 1, color = normalattr;
    int firstline = 1 + (height - 4)*(toprow - 1)/linecount;
    int lastline = firstline + (height - 4)*(height - 2)/linecount;
    char *p = curptr;
    for (i = 0; i < height - 2; i++, currow++) {
        // draw left border
        activescreenclass->drawchar('³', currow, col, normalattr);

        // wipe out the text area
        activescreenclass->drawrepeat(' ', width - 2, currow, col + 1, normalattr);

        // display the text
        if (i + toprow <= linecount && *p && p) {
            for (j = 0; *p != '\n'; p++) {
                if (*p == '\1') {color = normalattr; continue;}
                if (*p == '\2') {color = hiliteattr; continue;}
                if (*p == '\t') {
                    int k, newj = (j + 8 + 7) & 0xFFF8;
                    for (k = j; k < newj; k++) {
                        if (k < width - 4) activescreenclass->drawchar(' ', \
                            currow, col + 2 + k, color);
                    }
                    j = newj;
                    continue;
                }
                if (j < width - 4) activescreenclass->drawchar(*p, currow, \
                    col + 2 + j++, color);
            }
            p++;
        }

        // display the right side of the box
        if (maxtoprow == 1) j = '³';
        else if (i == 0) j = 24;
        else if (i == height - 3) j = 25;
        else j = (i >= firstline && i <= lastline ? 'Û' : '°');
        activescreenclass->drawchar(j, currow, col + width - 1, normalattr);
    }

    // draw bottom row
    activescreenclass->drawchar('À', row + height - 1, col, normalattr);
    activescreenclass->drawrepeat('Ä', width - 2, row + height - 1, col + 1, normalattr);
    activescreenclass->drawchar('Ù', row + height - 1, col + width - 1, normalattr);

    // say we've just been updated
    needupdate = FALSE;
}
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
void textboxtype::handle (void)
{
    clock_t myclock = clock();
    int delay = 0, pressed = FALSE;

    while (TRUE) {
        if (needupdate) {draw(); activescreenclass->refresh();}

        if (activemouseclass->button) {
            if (delay) {
                clock_t tempclock = clock();
                if (tempclock != myclock) {myclock = tempclock; delay--;}
            }
            if ((activemouseclass->col == col + width - 1 && activemouseclass->row > row + 1 && activemouseclass->row < row + height - 2) || pressed) {
                // on slider bar
                int newtoprow = 1 + (8 * activemouseclass->row + activemouseclass->rowfrac - 8*(row + 2))*linecount/(8*(height-4));
                if (newtoprow < 1) newtoprow = 1;
                if (newtoprow > maxtoprow) newtoprow = maxtoprow;
                if (toprow != newtoprow) {
                    toprow = newtoprow;
                    needupdate = TRUE;
                }
                pressed = TRUE;
            } else if (activemouseclass->col == col + width - 1 && activemouseclass->row == row + 1) {
                // up-arrow
                if (toprow > 1 && !delay) {
                    delay = 4;
                    toprow--; needupdate = TRUE;
                }
            } else if (activemouseclass->col == col + width - 1 && activemouseclass->row == row + height - 2) {
                // down-arrow
                if (toprow < maxtoprow && !delay) {
                    delay = 4;
                    toprow++; needupdate = TRUE;
                }
            } else if (!pressed) {
                break;
            }
        } else {
            break;
        }
    }

    // Drain the keyboard if nothing has read it for awhile
    static passesby = 0, kbstart = 0;
    if (peek(0x40, 0x1A) != peek(0x40, 0x1C) && kbstart == peek(0x40, 0x1A)) {
        if (passesby++ > 5) while(kbhit()) getch();
    } else {passesby = 0; kbstart = peek(0x40, 0x1A);}
}
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
