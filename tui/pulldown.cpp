//          C++ Text User Interface Kit
//
//              by Jeff Lawson of JL Enterprises, 1994
//
#include <string.h>
#include <stdlib.h>
#include <iostream.h>
#include <stdarg.h>
#include "mouse.hpp"
#include "screen.hpp"
#include "menubox.hpp"
#include "pulldown.hpp"

#ifndef __LARGE__
    #error must be compiled in large model
#endif

// define any external error message routines we'll use
extern void no_memory (void);

//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
pulldowntype::pulldowntype(int mynormal, int myhilite, int myrow, int mycol, \
        int mywidth, char *mymenus, ...)
{
    // general initialization
    row = myrow;
    col = mycol;
    menuwidth = mywidth;
    currentmenu = currentchoice = 0;
    normalattr = mynormal;
    hiliteattr = myhilite;
    needupdate = 1;

    // count and copy the names of each menu
    char *p = mymenus;
    for (nummenus = 0; *p++; nummenus++) while(*p != NULL && *p != '\n') p++;
    menunames = new char[p - mymenus];
    if (menunames == NULL) {atexit(no_memory); exit(1); }
    memcpy(menunames, mymenus, p - mymenus);

    // copy the addresses to each of the procedures handling each menu
    menus = new menuboxtype*[nummenus];
    if (menus == NULL) {atexit(no_memory); exit(1); }
    int i; va_list ap; menuboxtype *menup;
    va_start(ap, mymenus);
    for (i = 0; i < nummenus; i++) {
        menup = va_arg(ap, menuboxtype *);
        menus[i] = menup;
    }
    va_end(ap);
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
pulldowntype::~pulldowntype(void)
{
    delete menunames;
    delete menus;
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void pulldowntype::draw(void)
{
    // first, clear out the length of the menu
    activescreenclass->drawrepeat(' ', menuwidth, row, col, normalattr);

    // draw all of the menus and the current drop down menu box, if necessary
    int i, curcol = col, attr; char *p = menunames;
    for (i = 0; i < nummenus; i++, p++) {
        if (currentmenu == i + 1) {
            while (*p != '\n' && *p != NULL) {
                activescreenclass->drawchar(*p++, row, curcol++, hiliteattr);
            }
            menus[i]->currentchoice = currentchoice;
            menus[i]->draw();
        } else {
            while (*p != '\n' && *p != NULL) {
                activescreenclass->drawchar(*p++, row, curcol++, normalattr);
            }
        }
    }

    // say the menu has just been redrawn
    needupdate = 0;
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void pulldowntype::handle(void)
{
    if (currentmenu) {currentmenu = 0; currentchoice = 0; needupdate = 1;}
    if (needupdate) {draw(); activescreenclass->refresh();}

    // make sure that the mouse button is down and is positioned on the menu
    if (activemouseclass->row != row || activemouseclass->col < col || activemouseclass->col > col + menuwidth - 1 || !activemouseclass->button) return;

    // actually handle the pull down menu
    activescreenclass->save();
    while (1) {
        if (needupdate) {draw(); activescreenclass->refresh();}

        if (activemouseclass->row == row && activemouseclass->col >= col && activemouseclass->col <= col + menuwidth - 1) {
            // determine which menu the mouse is on
            int i, curcol = col, menulength; char *p = menunames;
            for (i = 0; i < nummenus; i++, p++) {
                for (menulength = 0; *p != NULL && *p != '\n'; menulength++) p++;
                if (activemouseclass->col >= curcol && activemouseclass->col < curcol + menulength) {
                    if (currentmenu != i + 1) {
                        activescreenclass->restore();
                        currentmenu = i + 1; currentchoice = 0;
                        needupdate = 1; break;
                    }
                }
                curcol += menulength;
            }
        } else {
            // handle the current sub-menu
            if (currentmenu != 0) {
                menus[currentmenu-1]->currentchoice = currentchoice;
                menus[currentmenu-1]->handle();
                currentchoice = menus[currentmenu-1]->currentchoice;
                if (currentchoice) break;
            }
        }
        if (!activemouseclass->button) break;
    }
    if (currentchoice == 0) currentmenu = 0;
    activescreenclass->restore();
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
