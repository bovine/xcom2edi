//          C++ Text User Interface Kit
//
//              by Jeff Lawson of JL Enterprises, 1994
//
#ifndef __TEXTBOX_HPP__
#define __TEXTBOX_HPP__

#ifndef __LARGE__
    #error must be compiled in large model
#endif

#include "screen.hpp"
#include "mouse.hpp"
#include "colors.hpp"
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
class textboxtype {
    int row, col, width, height, linecount, lasttoprow, maxtoprow;
    color normalattr, hiliteattr;
    char *curptr;
public:
    int needupdate, toprow;
    char *message;

    textboxtype(color mynormal, color myhilite, int myrow, int mycol, int mywidth, \
            int myheight, char *mymessage);
    ~textboxtype (void);
    void draw(void);
    void handle(void);
};
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
#endif
