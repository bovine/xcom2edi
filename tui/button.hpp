//          C++ Text User Interface Kit
//
//              by Jeff Lawson of JL Enterprises, 1994
//
#ifndef __BUTTON_HPP__
#define __BUTTON_HPP__

#ifndef __LARGE__
    #error must be compiled in large model
#endif

#include "screen.hpp"
#include "mouse.hpp"
#include "colors.hpp"

//����������������������������������������������������������������������������
class buttontype {
    int row, col, buttonwidth, buttonheight;
    color normalattr, hiliteattr;
    char *message;
public:
    int pressed, needupdate;
    buttontype(color mynormal, color myhilite, int myrow, int mycol, \
            char *mymessage);
    ~buttontype(void);
    void draw(void);
    void handle(void);
};
//����������������������������������������������������������������������������
#endif
