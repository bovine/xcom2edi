//          C++ Text User Interface Kit
//
//              by Jeff Lawson of JL Enterprises, 1994
//
#include <string.h>
#include <stdlib.h>
#include <iostream.h>
#include <conio.h>
#include <dos.h>
#include "mouse.hpp"
#include "screen.hpp"
#include "txtinput.hpp"

#ifndef __LARGE__
    #error must be compiled in large model
#endif

// some defines
#define TRUE 1
#define FALSE 0

// define any external error message routines we'll use
extern void no_memory (void);

// define the flag indicating if we're the only active input box
static int active_input_id;
static int next_input_id = 0;
static int num_active_inputs = 0;

//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
textinputtype::textinputtype (color mynormal, int myrow, int mycol,
    int mywidth, int mymaskchar)
{
    row = myrow;
    col = mycol;
    curcol = 1;
    normalattr = mynormal;
    width = mywidth;
    maskchar = mymaskchar;
    needupdate = TRUE;
    ourid = next_input_id++;
    num_active_inputs++;
    if (ourid == 0) {
        // we're the first one, so make ourself the active input box
        active_input_id = ourid;
        activescreenclass->locate(1, 1, 0);
    } // otherwise we're not the first, so let someone else be active

    // drain the keyboard buffer
    while(kbhit()) getch();

    // allocate the input buffer
    message = new char[width + 1];
    if (message == NULL) no_memory();
    strcpy(message, "");
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
textinputtype::~textinputtype (void)
{
    if (!message) return;
    delete message;

    if (next_input_id - 1 == ourid) next_input_id--;
    if (--num_active_inputs == 0) next_input_id = 0;
    if (active_input_id == ourid || num_active_inputs == 0) {
        active_input_id = -1;
        activescreenclass->locate(1, 1, 0);
    }
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void textinputtype::destroy (void)
{
    if (!message) return;
    delete message; message = NULL;

    if (next_input_id - 1 == ourid) next_input_id--;
    if (--num_active_inputs == 0) next_input_id = 0;
    if (active_input_id == ourid) {
        active_input_id = -1;
        activescreenclass->locate(1, 1, 0);
    }
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void textinputtype::draw (void)
{
    if (!message) return;

    // handle updating the screen
    if (maskchar) {
        activescreenclass->drawrepeat(' ', width, row, col, normalattr);
        activescreenclass->drawrepeat(maskchar, strlen(message), row, col, \
            normalattr);
    } else {
        activescreenclass->padstring(message, ' ', width, row, col, \
            normalattr);
    }

    // handle moving the hardware cursor
    if (curcol > strlen(message) + 1) curcol = strlen(message) + 1;
    if (active_input_id == ourid) {
        activescreenclass->locate(row, col + curcol - 1);
    } else if (active_input_id == -1) {
        activescreenclass->locate(1, 1, 0);
    }

    needupdate = FALSE;
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void textinputtype::handle (void)
{
    if (!message) return;

    while (TRUE) {
        if (needupdate) {draw(); activescreenclass->refresh();}
        if (activemouseclass->button) {
            if (activemouseclass->row == row && activemouseclass->col >= col \
                    && activemouseclass->col <= col + width - 1) {
                // mouse is positioned inside and button is currently pressed
                int newcol = activemouseclass->col - col + 1;
                if (newcol > strlen(message) + 1) newcol = strlen(message) + 1;
                if (active_input_id != ourid) {
                    // attention is getting switched to us!
                    while(kbhit()) getch();
                    active_input_id = ourid;
                    newcol = curcol; needupdate = TRUE;
                }
                if (newcol != curcol) {
                    curcol = newcol;
                    needupdate = TRUE;
                }
                break;
            } else {
                // mouse is not within range and is currently pressed
                if (active_input_id == ourid) {
                    active_input_id = -1;
                    activescreenclass->locate(1, 1, 0);
                }
                break;
            }
        } else {
            if (active_input_id == ourid && kbhit()) {
                // a key has been pressed and we're currently active!
                int key = getch(); if (!key) key = getch() << 8;
                switch (key) {
                    case 0x0008: // backspace
                        if (curcol > 1) {
                            strcpy(message + curcol - 2, message + curcol - 1);
                            curcol--;
                            needupdate = TRUE;
                        }
                        break;
                    case 0x5300: // Delete
                        if (curcol <= strlen(message)) {
                            strcpy(message + curcol - 1, message + curcol);
                            needupdate = TRUE;
                        }
                        break;
                    case 0x4700: // Home
                        if (curcol != 1) {
                            curcol = 1; needupdate = TRUE;
                        }
                        break;
                    case 0x4F00: // End
                        if (curcol != strlen(message) + 1) {
                            curcol = strlen(message) + 1;
                            needupdate = TRUE;
                        }
                        break;
                    case 0x4D00: // cursor right
                        if (curcol <= strlen(message)) {
                            curcol++;
                            needupdate = TRUE;
                        }
                        break;
                    case 0x4B00: // cursor left
                        if (curcol > 1) {
                            curcol--;
                            needupdate = TRUE;
                        }
                        break;
                    default: // anything else
                        if (key >= ' ' && key < 127 && strlen(message) < width) {
                            char *temp = new char[strlen(message) - curcol + 2];
                            if (!temp) no_memory();
                            strcpy(temp, message + curcol - 1);
                            message[curcol - 1] = key;
                            strcpy(message + curcol, temp);
                            curcol++;
                            needupdate = TRUE;
                            delete temp;
                        } else if (key == 13 || key == 27) {
                            ungetch(key);
                        }
                        break;
                }
                break;
            } else {
                if (active_input_id != ourid && activescreenclass->lastrow == \
                    row && activescreenclass->lastcol >= col && \
                    activescreenclass->lastcol <= col + width - 1) \
                    activescreenclass->locate(1, 1, 0);
                break;
            }
        }
    }

    // Drain the keyboard if nothing has read it for awhile
    static passesby = 0, kbstart = 0;
    if (peek(0x40, 0x1A) != peek(0x40, 0x1C) && kbstart == peek(0x40, 0x1A)) {
        if (passesby++ > 5) while(kbhit()) getch();
    } else {passesby = 0; kbstart = peek(0x40, 0x1A);}
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
// The passed pointer points to desired text to set the input window to.  The
// cusor will be moved to the end of the string.  The input window will also
// be forced to be the active window.  If the pointer points to null text,
// the input window will be cleared.  If the pointer itself is null, then
// the text currently contained in the input window will not be affected, but
// the window will still be made active.
void textinputtype::setdefault(char *text)
{
    if (text) {
        strncpy(message, text, width);
        message[width] = 0;
        curcol = strlen(message) + 1;
        needupdate = TRUE;
    }
    if (active_input_id != ourid) {
        active_input_id = ourid;
        needupdate = TRUE;
    }
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
