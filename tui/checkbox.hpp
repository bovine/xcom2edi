//          C++ Text User Interface Kit
//
//              by Jeff Lawson of JL Enterprises, 1994
//
#ifndef __CHECKBOX_HPP__
#define __CHECKBOX_HPP__

#ifndef __SCREEN_HPP__
    #include "screen.hpp"
#endif
#ifndef __MOUSE_HPP__
    #include "mouse.hpp"
#endif
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
class checkboxtype {
    int row, col, messagelength, normalattr;
    char *message;
public:
    int checked, needupdate;
    checkboxtype(int mynormal, int myrow, int mycol, char *mymessage);
    ~checkboxtype(void);
    void draw(void);
    void handle(void);
};
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
#endif
