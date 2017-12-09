//          C++ Text User Interface Kit
//
//              by Jeff Lawson of JL Enterprises, 1994
//
#include <iostream.h>
#include <stdlib.h>
#include <dos.h>
#include <conio.h>
#include <string.h>
#include <stdio.h>
#include "screen.hpp"
#include "mouse.hpp"

// some defines
#define TRUE 1
#define FALSE 0

// define any external error message routines we'll use
extern void no_mouse (void);
extern void no_copies (void);

// define our globally accessible pointer
mousetype *activemouseclass = NULL;

//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
mousetype::mousetype(void)     // constructor
{
    // don't allow two copies of ourself
    if (activemouseclass) no_copies();

    // check for the presence of a mouse driver & reset it
    if (getvect(0x33) == NULL) no_mouse();
    asm mov ax,0021h        // For complete reset: asm xor ax,ax
    asm int 33h
    if (_AX != 0xFFFF) no_mouse();

    // enable the graphical by default
    graphicalmouse = (activescreenclass->displaytype >= VidEGAmono);

    // if Windows is running, then default to non-graphical mouse
    asm mov ax,1600h
    asm int 2Fh
    asm and ax,7Fh
    if (_AX) graphicalmouse = FALSE;

    // set the limits of the mouse
    setlimits(1, 1, 25, 80);

    // reset all of our variables
    activemouseclass = this;
    oldpos = -1;
    visible = button = 0;
    locatemouse(12, 40);
    setcursor(MOUSE_ARROW);

    // install our new handler
    _ES = FP_SEG(handler);
    _DX = FP_OFF(handler);
    asm mov ax,000Ch
    asm mov cx,01111111b
    asm int 33h

    // turn on the mouse cursor
    show();
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
mousetype::~mousetype(void)     // destructor
{
    if (!activemouseclass) return;

    // turn off the mouse cursor
    hide();

    // reset the mouse driver and remove our handler
    asm mov ax,0021h        // For complete reset: asm xor ax,ax
    asm int 33h

    activemouseclass = NULL;
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void mousetype::setlimits (int row1, int col1, int row2, int col2)
{
    // make sure that the limits are within range
    if (row1 < 1) row1 = 1;
    if (row2 > 25) row2 = 25;
    if (col1 < 1) col1 = 1;
    if (col2 > 80) col2 = 80;

    if (activescreenclass->displaytype >= VidEGAmono && graphicalmouse) {
        // set the horizontal limits
        asm mov ax,7
        asm mov cx,[col1]
        asm dec cx
        asm shl cx,3+3
        asm mov dx,[col2]
        asm shl dx,3+3
        asm dec dx
        asm int 33h

        // set the vertical limits
        asm mov ax,8
        asm mov cx,[row1]
        asm dec cx
        asm shl cx,3+3
        asm mov dx,[row2]
        asm shl dx,3+3
        asm dec dx
        asm int 33h

        // set the mouse speed
        asm mov ax,000Fh
           asm mov cx,32h/32
        asm mov dx,32h/16
           asm int 33h
    } else {
        // set the horizontal limits
        asm mov ax,7
        asm mov cx,[col1]
        asm dec cx
        asm shl cx,3
        asm mov dx,[col2]
        asm shl dx,3
        asm dec dx
        asm int 33h

        // set the vertical limits
        asm mov ax,8
        asm mov cx,[row1]
        asm dec cx
        asm shl cx,3
        asm mov dx,[row2]
        asm shl dx,3
        asm dec dx
        asm int 33h

        // set the mouse speed
        asm mov ax,000Fh
           asm mov cx,32h/4
        asm mov dx,32h/2
           asm int 33h
    }
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void mousetype::setcursor (enum mousecursor cursornum)
{
    char defaultcursors[][MOUSE_CHARHEIGHT] = {
        {0x80,0xC0,0xE0,0xF0,0xF8,0xFC,0xFE,0xFF,0xF8,0xD8,0x8C,0x0C,0x06,0x06},  // arrow
        {0xFF,0x81,0x81,0xBD,0x5A,0x24,0x18,0x24,0x42,0x81,0xBD,0xBD,0x81,0xFF},  // hourglass
        {0xC0,0xC0,0xC0,0xD5,0xD5,0xD5,0xFF,0xFF,0xFF,0xFE,0x7C,0x3C,0x3C,0x3C},  // hand
        {0x3C,0x3C,0x3C,0x7E,0x81,0x91,0x9D,0x81,0x81,0x7E,0x3C,0x3C,0x3C,0x3C}}; // watch

    memset(cursordata, 0, MOUSE_CHARHEIGHT * 3);
    memmove(&cursordata[MOUSE_CHARHEIGHT], defaultcursors[cursornum], MOUSE_CHARHEIGHT);
    show();
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void mousetype::setcursor (char *mousedata)
{
    memset(cursordata, 0, MOUSE_CHARHEIGHT * 3);
    memmove(&cursordata[MOUSE_CHARHEIGHT], mousedata, MOUSE_CHARHEIGHT);
    show();
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void mousetype::locatemouse (int newrow, int newcol)
{
    row = newrow;
    col = newcol;
    colfrac = 0; rowfrac = 0;
    if (activescreenclass->displaytype >= VidEGAmono && graphicalmouse) {
        asm mov ax,0004h
        asm mov cx,[newcol]
        asm dec cx
        asm shl cx,3+3
        asm mov dx,[newrow]
        asm dec dx
        asm shl dx,3+3
        asm int 33h
    } else {
        asm mov ax,0004h
        asm mov cx,[newcol]
        asm dec cx
        asm shl cx,3
        asm mov dx,[newrow]
        asm dec dx
        asm shl dx,3
        asm int 33h
    }
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void mousetype::hide (void)
{
    if (activescreenclass->displaytype < VidEGAmono || !graphicalmouse) {
        if (visible) {
            asm mov ax,0002h
            asm int 33h
        }
    } else if (oldpos != -1) restore_background();
    visible = 0;
    oldpos = -1;
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void mousetype::show (void)
{
    if (activescreenclass->displaytype < VidEGAmono || !graphicalmouse) {
        if (!visible) {
            asm mov ax,0001h
            asm int 33h
        }
	    visible = 1;
    } else {
    	if (oldpos != -1) restore_background();
	    visible = 1;
	    save_background();
	    draw_cursor();
	}
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
static void _loadds handler(void)
{
    unsigned newbutton = _BX, newcol = _CX >> 3, newrow = _DX >> 3;

    if (activescreenclass->displaytype < VidEGAmono || !activemouseclass->graphicalmouse) {
        // save what's given to us
        activemouseclass->button = (activemouseclass->visible ? newbutton : 0);
        activemouseclass->col = newcol + 1;
        activemouseclass->row = newrow + 1;
        activemouseclass->colfrac = activemouseclass->rowfrac = 0;
    } else {
        // save what's given to us
        activemouseclass->button = newbutton;
        activemouseclass->col = (newcol >> 3) + 1;
        activemouseclass->row = (newrow >> 3) + 1;
        activemouseclass->colfrac = newcol & 7;
        activemouseclass->rowfrac = newrow & 7;

        // restore what was behind us, if necessary
        if (activemouseclass->oldpos != -1) activemouseclass->restore_background();

        // if the mouse is visible the draw it, otherwise don't
        if (activemouseclass->visible) {
            // save what's on the background where we now are
            activemouseclass->save_background();

            // draw the mouse block on the screen
            activemouseclass->draw_cursor();
        } else {
            activemouseclass->oldpos = -1;
            activemouseclass->button = 0;
        }
    }
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void mousetype::restore_background(void)
{
    char far *screenbuffer = activescreenclass->refreshbuffer;
    screenbuffer[oldpos] = oldblock[0];
    if (!(clipped & 1)) screenbuffer[oldpos+2] = oldblock[1];
    if (!(clipped & 2)) screenbuffer[oldpos+160] = oldblock[2];
    if (!(clipped & 3)) screenbuffer[oldpos+162] = oldblock[3];
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void mousetype::save_background(void)
{
    char far *screenbuffer = activescreenclass->refreshbuffer;
    oldpos = ((row-1) * 80 + (col-1)) * 2;

    clipped = 0;
    if (col == 80) clipped |= 1;
    if (row == 25) clipped |= 2;
    oldblock[0] = screenbuffer[oldpos];
    if (!(clipped & 1)) oldblock[1] = screenbuffer[oldpos+2];
    if (!(clipped & 2)) oldblock[2] = screenbuffer[oldpos+160];
    if (!(clipped & 3)) oldblock[3] = screenbuffer[oldpos+162];
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void mousetype::draw_cursor(void)
{
    int segment;
    asm cli

    // switch the video memory to now hold the font values
    if (graphicalmouse == 1) {
        // new method--base at A000
        asm mov dx,3C4h
        asm mov ax,0402h        // enable write to plane 2
        asm out dx,ax
        asm mov ax,0704h        // text, >64k, even-odd, video-mem
        asm out dx,ax
        asm mov dx,3CEh
        asm mov ax,0204h        // read mode 0 reads from plane 2
        asm out dx,ax
        asm mov ax,0005h        // Mode 0: Read-Modify-Write
        asm out dx,ax
        asm mov ax,0006h        // text-mode, A000h-BFFFh
        asm out dx,ax
        segment = 0xA000;
    } else {
        // old method--base at B800
        asm mov dx,3C4h
        asm mov ax,0402h        // enable write to plane 2
        asm out dx,ax
        asm mov ax,0604h        // graphics, >64k, even-odd, video-mem
        asm out dx,ax
        asm mov dx,3CEh
        asm mov ax,0204h        // read mode 0 reads from plane 2
        asm out dx,ax
        asm mov ax,0005h        // Mode 0: Read-Modify-Write
        asm out dx,ax
        asm mov ax,0C06h        // text-mode, B800h-BFFFh
        asm out dx,ax
        segment = 0xB800;
    }


    // remap the characters to simulate movement
    _CX = colfrac;
    asm xor si,si
Loop1:  _DH = cursordata[(7-rowfrac) * 2 + _SI];
        _DI = oldblock[0] * 32 + _SI;
        _BX = oldblock[1] * 32 + _SI;
        asm mov es,[segment]
        asm xor dl,dl
        asm shr dx,cl
        asm mov ah,byte ptr es:[di]
        asm mov al,byte ptr es:[bx]
        asm or ah,dh
        asm or al,dl
        asm mov byte ptr es:[32*(MOUSE_BASECHAR+0) + si],ah
        asm mov byte ptr es:[32*(MOUSE_BASECHAR+1) + si],al
    asm inc si
    asm cmp si,MOUSE_CHARHEIGHT
    asm jb Loop1
    asm xor si,si
Loop2:  _DH = cursordata[MOUSE_CHARHEIGHT + (7-rowfrac) * 2 + _SI];
        _DI = oldblock[2] * 32 + _SI;
        _BX = oldblock[3] * 32 + _SI;
        asm mov es,[segment]
        asm xor dl,dl
        asm shr dx,cl
        asm mov ah,byte ptr es:[di]
        asm mov al,byte ptr es:[bx]
        asm or ah,dh
        asm or al,dl
        asm mov byte ptr es:[32*(MOUSE_BASECHAR+2) + si],ah
        asm mov byte ptr es:[32*(MOUSE_BASECHAR+3) + si],al
    asm inc si
    asm cmp si,MOUSE_CHARHEIGHT
    asm jb Loop2


    // switch back the video memory to hold the screen data
    if (graphicalmouse == 1) {
        // new method
        asm mov dx,3C4h
        asm mov ax,302h         // Enable writes to planes 0 and 1
        asm out dx,ax
        asm mov ax,304h         // text, >64k
        asm out dx,ax

        asm mov dx,3CEh
        asm mov ax,0004h        // Read mode 0 will read from plane 0
        asm out dx,ax
        asm mov ax,1005h        // Enable even/odd
        asm out dx,ax
        asm mov ax,0E06h        // B800h-BFFFh, even/odd
        asm out dx,ax
    } else {
        // old method
        asm mov dx,3C4h
        asm mov ax,0302h        // Enable writes to planes 0 and 1
        asm out dx,ax
        asm mov ax,0204h        // graphics, >64k
        asm out dx,ax

        asm mov dx,3CEh
        asm mov ax,0004h        // Read mode 0 will read from plane 0
        asm out dx,ax
        asm mov ax,1005h        // Enable even/odd
        asm out dx,ax
        asm mov ax,0E06h        // B800h-BFFFh, even/odd
        asm out dx,ax
    }

    // draw the mouse block on the screen
    char far *screenbuffer = activescreenclass->refreshbuffer;
    screenbuffer[oldpos] = MOUSE_BASECHAR;
    if (!(clipped & 1)) screenbuffer[oldpos+2] = MOUSE_BASECHAR+1;
    if (!(clipped & 2)) screenbuffer[oldpos+160] = MOUSE_BASECHAR+2;
    if (!(clipped & 3)) screenbuffer[oldpos+162] = MOUSE_BASECHAR+3;

    asm sti
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
