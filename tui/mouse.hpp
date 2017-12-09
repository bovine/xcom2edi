//          C++ Text User Interface Kit
//
//              by Jeff Lawson of JL Enterprises, 1994
//
#ifndef __MOUSE_HPP__
#define __MOUSE_HPP__

#ifndef __LARGE__
    #error must be compiled in large model
#endif

//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
// values used by the mouse routines
#define MOUSE_CHARHEIGHT 14
#define MOUSE_BASECHAR 0

// define some mouse cursor setting enumerations
enum mousecursor {MOUSE_ARROW, MOUSE_WAIT, MOUSE_HAND, MOUSE_WATCH};
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
class mousetype {
    friend void _loadds handler(void);
    void restore_background(void);
    void save_background(void);
    void draw_cursor(void);
    unsigned char cursordata[MOUSE_CHARHEIGHT*3], oldblock[4];
    int oldpos, clipped;
public:
    int visible, row, col, button, colfrac, rowfrac, graphicalmouse;
    mousetype(void);            // constructor
    ~mousetype(void);           // destructor
    void setlimits (int row1, int col1, int row2, int col2);
    void setcursor (char *cursordata);
    void setcursor (enum mousecursor cursornum);
    void locatemouse (int newrow, int newcol);
    void hide (void);
    void show (void);
};
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
extern mousetype *activemouseclass;
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
#endif
