//          C++ Text User Interface Kit
//
//              by Jeff Lawson of JL Enterprises, 1994
//
#ifndef __MENUBOX_HPP__
#define __MENUBOX_HPP__

#ifndef __LARGE__
    #error must be compiled in large model
#endif

#include "screen.hpp"
#include "mouse.hpp"
#include "colors.hpp"
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
struct choiceinfotype {
    char notvalid;
    char checked;
};
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
class menuboxtype {
    int row, col, numchoices, menuwidth, menuheight;
    int lasttoprow, maxtoprow;
    color normalattr, hiliteattr, grayattr;
    char *choices, *curptr;
    struct choiceinfotype *choiceinfo;
public:
    int currentchoice, toprow, needupdate;

    menuboxtype(color mynormal, color myhilite, color mygray, int myrow, \
            int mycol, int myheight, char *mychoices);
    menuboxtype(color mynormal, color myhilite, color mygray, int myrow, \
            int mycol, char *mychoices);
    ~menuboxtype(void);
    void setchoiceinfo(int choice, int notvalid, int checked);
    int ischecked(int choice);
    void draw(void);
    void handle(void);
};
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
#endif
