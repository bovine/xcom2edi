//          C++ Text User Interface Kit
//
//              by Jeff Lawson of JL Enterprises, 1994
//
#ifndef __PULLDOWN_HPP__
#define __PULLDOWN_HPP__

#ifndef __SCREEN_HPP__
    #include "screen.hpp"
#endif
#ifndef __MOUSE_HPP__
    #include "mouse.hpp"
#endif
#ifndef __MENUBOX_HPP__
    #include "menubox.hpp"
#endif
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
class pulldowntype {
    int row, col, menuwidth, nummenus, normalattr, hiliteattr;
    menuboxtype **menus;
    char *menunames;
public:
    int currentmenu, currentchoice, needupdate;

    pulldowntype(int mynormal, int myhilite, int myrow, int mycol, int mywidth, \
            char *mymenus, ...);
    ~pulldowntype(void);
    void draw(void);
    void handle(void);
};
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
#endif
