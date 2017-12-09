#include <stdlib.h>
#include <conio.h>
#include <iostream.h>
#include "mouse.hpp"
#include "screen.hpp"

// define all procedures in this module
void no_memory (void);
void no_mouse (void);
void no_display (void);
void no_games (void);
void file_error (void);
void bad_crc (void);
void no_copies (void);
void error_pause(void);

//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void no_memory (void)
{
    if (activemouseclass) delete activemouseclass;
    clrscr();
    cout << "Insufficient free memory to run program. Try removing any\n";
    cout << "unnecessary TSR programs and running this program again.\n";
    error_pause();
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void no_mouse (void)
{
    if (activemouseclass) delete activemouseclass;
    clrscr();
    cout << "This program requires a mouse to be installed in the system.\n";
    cout << "If a mouse is already installed, make sure that you also\n";
    cout << "load an appropriate mouse driver.\n";
    error_pause();
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void no_display (void)
{
    if (activemouseclass) delete activemouseclass;
    clrscr();
    cout << "Cannot identify monitor installed in the system.  At least\n";
    cout << "a monochrome monitor is required, however a VGA monitor is\n";
    cout << "recommended for best results.\n";
    error_pause();
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void no_games (void)
{
    if (activemouseclass) delete activemouseclass;
    clrscr();
    cout << "No saved games found in current directory.\n";
    error_pause();
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void file_error (void)
{
    if (activemouseclass) delete activemouseclass;
    clrscr();
    cout << "Error accessing saved game or other data file.\n";
    error_pause();
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void bad_crc (void)
{
    if (activemouseclass) delete activemouseclass;
    clrscr();
    cout << "I've been hacked!\n";
    error_pause();
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void no_copies (void)
{
    if (activemouseclass) delete activemouseclass;
    clrscr();
    cout << "Attempt to create another non-reentrant class.\n";
    error_pause();
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void error_pause(void)
{
    cout << "\nPress any key to continue...\n";
    if (!getch()) getch();
    exit(1);
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
