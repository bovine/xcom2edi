//          C++ Text User Interface Kit
//
//              by Jeff Lawson of JL Enterprises, 1994
//
#include <dos.h>
#include <mem.h>
#include <iostream.h>
#include <fstream.h>
#include <stdlib.h>
#include <conio.h>
#include <alloc.h>
#include <stdio.h>
#include "mouse.hpp"
#include "screen.hpp"

#undef  DEBUG_MEMORY        // define to help track memory leaks in your code
                            // will constantly display memory free at top

// define any external error message routines we'll use
extern void no_memory (void);
extern void no_display (void);
extern void no_copies (void);

// define our control-break handler
static int break_handler(void);

// define our globally accessible pointer
screentype *activescreenclass = NULL;

//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
screentype::screentype(void)    // constructor
{
    // don't allow two copies of ourself
    if (activescreenclass) no_copies();

    // do the monitor test
    asm mov ax,1A00h
    asm int 10h
    if (_AL == 0x1A) {
        switch (_BL) {
            case 0x01: displaytype = VidMDA; break;
            case 0x02: case 0x03: case 0x06: case 0x09:
                       displaytype = VidCGA; break;
            case 0x05: displaytype = VidEGAmono; break;
            case 0x04: displaytype = VidEGAcolor; break;
            case 0x07: displaytype = VidVGAmono; break;
            case 0x08: displaytype = VidVGAcolor; break;
            case 0x0B: displaytype = VidMCGAmono; break;
            case 0x0A: case 0x0C:
                       displaytype = VidMCGAcolor; break;
            case 0x00: displaytype = VidNone; break;
            default:   displaytype = VidCGA;
        }
    } else {
        asm mov ah,12h
        asm mov bl,10h
        asm int 10h
        if (_BL == 0x10) {
            asm int 11h
            asm and al,30h
            asm shr al,4
            if (_AL == 0) displaytype = VidNone;
            else if (_AL == 3) displaytype = VidMDA;
            else displaytype = VidCGA;
        } else {
            if (_BH == 0) displaytype = VidEGAcolor;
            else displaytype = VidEGAmono;
        }
    }
    if (displaytype == VidNone) no_display();

    // allocate memory for the virtual buffer
    buffersize = 80*25*2;
    virtualbuffer = new char[buffersize];
    if (virtualbuffer == NULL) no_memory();

    // allocate memory for the save buffer
    savebuffer = new char[buffersize];
    if (savebuffer == NULL) no_memory();

    // set up a pointer to the video memory
    switch (displaytype) {
        case VidCGA:
        case VidEGAcolor:
        case VidVGAcolor:
        case VidMCGAcolor:
            refreshbuffer = (char *) MK_FP(0xB800, 0); break;
        case VidMDA:
        case VidEGAmono:
        case VidVGAmono:
        case VidMCGAmono:
        default:
            refreshbuffer = (char *) MK_FP(0xB000, 0); break;
    }

    // initialize the appropriate text mode
    if (displaytype >= VidEGAmono) {
        // switch to the EGA font set
        asm mov ax,1201h
        asm mov bl,30h
        asm int 10h

        if (refreshbuffer == (char *) MK_FP(0xB000, 0)) {
            // change into 80x25 gray text mode
            asm mov ax,0002h
            asm int 10h
        } else {
            // change into 80x25 color text mode
            asm mov ax,0003h
            asm int 10h
        }

        // turn off the blink flag
        asm mov ax,1003h
        asm xor bl,bl
        asm int 10h
    } else {
        if (refreshbuffer == (char *) MK_FP(0xB000, 0)) {
            // change into 80x25 mono text mode
            asm mov ax,0007h
            asm int 10h
        } else {
            // change into 80x25 color text mode
            asm mov ax,0003h
            asm int 10h
        }

        if (displaytype == VidCGA) {
            // turn off the blink flag
            asm mov al,9
            asm mov dx,3D8h
            asm out dx,al
        }
    }

    // move cursor to top and turn off cursor
    locate(1, 1, 0);

    // set up a pointer to ourself
    activescreenclass = this;

    // set the control-break handler
    ctrlbrk(break_handler);
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
screentype::~screentype(void)   // destructor
{
    if (!activescreenclass) return;

    // free the virtual buffer
    delete(virtualbuffer);

    // free the virtual buffer
    delete(savebuffer);

    // initialize the appropriate text mode
    if (displaytype >= VidEGAmono) {
        // turn on the blink flag
        asm mov ax,1003h
        asm mov bl,1
        asm int 10h

        // switch to the VGA font set
        asm mov ax,1202h
        asm mov bl,30h
        asm int 10h

        if (refreshbuffer == (char *) MK_FP(0xB000, 0)) {
            // change into 80x25 gray text mode
            asm mov ax,0002h
            asm int 10h
        } else {
            // change into 80x25 color text mode
            asm mov ax,0003h
            asm int 10h
        }
    } else {
        if (refreshbuffer == (char *) MK_FP(0xB000, 0)) {
            // change into 80x25 mono text mode
            asm mov ax,0007h
            asm int 10h
        } else {
            // change into 80x25 color text mode
            asm mov ax,0003h
            asm int 10h
        }
    }

    // restore the cursor
    locate(1, 1);

    // display the farewell screen, if present
    if (farewell) {
        memmove(refreshbuffer, farewell, 80*25*2);
        locate(21, 1);
    }

    // empty the keyboard buffer
    while(kbhit()) getch();
    activescreenclass = NULL;
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void screentype::registerfarewell (char *image)
{
    farewell = image;
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void screentype::save (void)
{
    // hide the mouse, display the hidden work buffer, and restore mouse
    int visible = activemouseclass->visible;
    if (visible) activemouseclass->hide();
    memmove(savebuffer, virtualbuffer, buffersize);
    if (visible) activemouseclass->show();
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void screentype::restore (void)
{
    // hide the mouse, display the hidden work buffer, and restore mouse
    int visible = activemouseclass->visible;
    if (visible) activemouseclass->hide();
    memmove(virtualbuffer, savebuffer, buffersize);
    memmove(refreshbuffer, savebuffer, buffersize);
    if (visible) activemouseclass->show();
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void screentype::save (char *buffer)
{
    // hide the mouse, display the hidden work buffer, and restore mouse
    if (buffer == NULL) return;
    int visible = activemouseclass->visible;
    if (visible) activemouseclass->hide();
    memmove(buffer, virtualbuffer, buffersize);
    if (visible) activemouseclass->show();
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void screentype::restore (char *buffer)
{
    // hide the mouse, display the hidden work buffer, and restore mouse
    if (buffer == NULL) return;
    int visible = activemouseclass->visible;
    if (visible) activemouseclass->hide();
    memmove(virtualbuffer, buffer, buffersize);
    memmove(refreshbuffer, buffer, buffersize);
    if (visible) activemouseclass->show();
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
// updates the real screen with the current contents of the virtual screen
void screentype::refresh (void)
{
    // hide the mouse, display the hidden work buffer, and restore mouse
    int visible = activemouseclass->visible;
    if (visible) activemouseclass->hide();
    #ifdef DEBUG_MEMORY
        char buffer[14];
        sprintf(buffer, "%lu", coreleft());
        drawstring(buffer, 1, 1, COLOR(LIGHT_WHITE, BLACK));
    #endif
    memmove(refreshbuffer, virtualbuffer, buffersize);
    gotoxy(lastcol, lastrow);
    _setcursortype(lastcursor ? _NORMALCURSOR : _NOCURSOR);
    if (visible) activemouseclass->show();
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void screentype::locate (int row, int col, int cursor)
{
    gotoxy(lastcol = col, lastrow = row);
    _setcursortype(lastcursor = cursor ? _NORMALCURSOR : _NOCURSOR);
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void screentype::drawchar (int letter, int row, int col, color attr)
{
    // ES:DI ==> location to plot character
    _DI = FP_OFF(virtualbuffer);
    _ES = FP_SEG(virtualbuffer);
    asm mov ax,[row]
    asm dec ax
    asm mov bl,80
    asm mul bl
    asm add ax,[col]
    asm dec ax
    asm shl ax,1
    asm add di,ax

    // actually write it
    asm mov ax,[attr]
    asm mov ah,al
    asm mov bx,[letter]
    asm mov al,bl
    asm cld
    asm stosw
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void screentype::drawstring(char *text, int row, int col, color attr)
{
    // ES:DI ==> location to plot string
    _DI = FP_OFF(virtualbuffer);
    _ES = FP_SEG(virtualbuffer);
    asm mov ax,[row]
    asm dec ax
    asm mov bl,80
    asm mul bl
    asm add ax,[col]
    asm dec ax
    asm shl ax,1
    asm add di,ax

    // actually write it
    asm push ds
    asm lds si,dword ptr [text]
    asm mov ax,[attr]
    asm mov ah,al
    asm cld
printloop:
    asm lodsb
    asm or al,al
    asm jz quit
    asm stosw
    asm jmp printloop
quit:
    asm pop ds
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void screentype::drawrepeat(int letter, unsigned length, int row, int col, \
        color attr)
{
    // ES:DI ==> location to plot string
    _DI = FP_OFF(virtualbuffer);
    _ES = FP_SEG(virtualbuffer);
    asm mov ax,[row]
    asm dec ax
    asm mov bl,80
    asm mul bl
    asm add ax,[col]
    asm dec ax
    asm shl ax,1
    asm add di,ax

    // actually write it
    asm mov ax,[attr]
    asm mov ah,al
    asm mov bx,[letter]
    asm mov al,bl
    asm mov cx,[length]
    asm jcxz quit
    asm cld
    asm rep stosw
quit:
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void screentype::padstring(char *text, int letter, unsigned length, int row, \
        int col, color attr)
{
    // ES:DI ==> location to plot string
    _DI = FP_OFF(virtualbuffer);
    _ES = FP_SEG(virtualbuffer);
    asm mov ax,[row]
    asm dec ax
    asm mov bl,80
    asm mul bl
    asm add ax,[col]
    asm dec ax
    asm shl ax,1
    asm add di,ax
    
    // actually write it
    asm push ds
    asm mov cx,[length]
    asm lds si,dword ptr [text]
    asm mov ax,[attr]
    asm mov ah,al
    asm cld
printloop:
    asm lodsb
    asm or al,al
    asm jz printpad
    asm stosw
    asm dec cx
    asm jnz printloop
    asm jmp quit
printpad:

    // actually write it
    asm mov ax,[attr]
    asm mov ah,al
    asm mov bx,[letter]
    asm mov al,bl
    asm cld
    asm rep stosw
quit:
    asm pop ds
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
static int break_handler(void)
{
    activescreenclass->refresh();

    // say we should continue execution and not abort
    return(1);
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
