//          C++ Text User Interface Kit
//
//              by Jeff Lawson of JL Enterprises, 1994
//
#ifndef __DROPDOWN_HPP__
#define __DROPDOWN_HPP__

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
class dropdowntype {
    int row, col, width, droppeddown, numchoices, normalattr, hiliteattr;
    char *choices;
public:
    int currentchoice, needupdate;
    char *currentchoicetext;

    dropdowntype(int mynormal, int myhilite, int myrow, int mycol, int mywidth, \
            char *mychoices);
    ~dropdowntype(void);
    void draw(void);
    void handle(void);
};
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
#endif
