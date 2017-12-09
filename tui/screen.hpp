//          C++ Text User Interface Kit
//
//              by Jeff Lawson of JL Enterprises, 1994
//
#ifndef __SCREEN_HPP__
#define __SCREEN_HPP__

#ifndef __LARGE__
    #error must be compiled in large model
#endif

#include "mouse.hpp"
#include "colors.hpp"

#define VidNone 0
#define VidMDA 1
#define VidCGA 2
#define VidEGAmono 3
#define VidEGAcolor 4
#define VidVGAmono 5
#define VidVGAcolor 6
#define VidMCGAmono 7
#define VidMCGAcolor 8

//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
class screentype {
    char *virtualbuffer, *savebuffer, *farewell;
    int savecol, saverow, savecursor;
public:
    int lastcol, lastrow, lastcursor, displaytype;
    char *refreshbuffer;
    int buffersize;
    screentype(void);       // constructor
    ~screentype(void);      // destructor
    void registerfarewell(char *image);
    void save(void);
    void restore(void);
    void save(char *buffer);
    void restore(char *buffer);
    void refresh(void);
    void locate(int row = 1, int col = 1, int cursor = 1);
    void drawchar(int letter, int row, int col, color attr);
    void drawstring(char *text, int row, int col, color attr);
    void drawrepeat(int letter, unsigned length, int row, int col, color attr);
    void padstring(char *text, int letter, unsigned length, int row, \
        int col, color attr);
};
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
extern screentype *activescreenclass;
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
#endif
