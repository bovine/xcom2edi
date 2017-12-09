//          C++ Text User Interface Kit
//
//              by Jeff Lawson of JL Enterprises, 1994
//
#ifndef __TXTINPUT_HPP__
#define __TXTINPUT_HPP__

#include "screen.hpp"
#include "mouse.hpp"
#include "colors.hpp"

//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
class textinputtype {
    int row, col, curcol, width, maskchar, ourid;
    color normalattr;
public:
    int needupdate;
    char *message;
    textinputtype (color mynormal, int myrow, int mycol, int mywidth, int mymaskchar);
    ~textinputtype(void);
    void destroy(void);
    void draw(void);
    void handle(void);
    void setdefault(char *text);
};
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
#endif
