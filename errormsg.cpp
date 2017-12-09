// Xcom2edi
// Copyright (c) 1997, Jeff Lawson <jeff@bovine.net>
// All rights reserved.
// See LICENSE for distribution information.

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
void weapon_error(void);

//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void no_memory (void)
{
    if (activemouseclass) delete activemouseclass;
    if (activescreenclass) {activescreenclass->registerfarewell(NULL); delete activescreenclass;}
    cout << "Insufficient free conventional memory to run program. Try\n"
            "removing any unnecessary TSR programs and then run this\n"
            "program again.\n";
    exit(1);
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void no_mouse (void)
{
    if (activemouseclass) delete activemouseclass;
    if (activescreenclass) {activescreenclass->registerfarewell(NULL); delete activescreenclass;}
    cout << "This program requires a mouse to be installed in the system.\n"
            "If a mouse is already installed, make sure that you also\n"
            "load an appropriate Microsoft compatible mouse driver.\n";
    exit(1);
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void no_display (void)
{
    if (activemouseclass) delete activemouseclass;
    if (activescreenclass) {activescreenclass->registerfarewell(NULL); delete activescreenclass;}
    cout << "Cannot identify monitor installed in the system.  At least\n";
            "a monochrome monitor is required, however a VGA monitor is\n";
            "recommended for best results.\n";
    exit(1);
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void no_games (void)
{
    if (activemouseclass) delete activemouseclass;
    if (activescreenclass) {activescreenclass->registerfarewell(NULL); delete activescreenclass;}
    cout << "No saved games found in current directory.  Please make sure\n"
            "that you have put this program into your main XCOM directory\n"
            "and are running it from that location.\n";
    exit(1);
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void file_error (void)
{
    if (activemouseclass) delete activemouseclass;
    if (activescreenclass) {activescreenclass->registerfarewell(NULL); delete activescreenclass;}
    cout << "Error accessing saved game or other data file.\n";
    exit(1);
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void bad_crc (void)
{
    if (activemouseclass) delete activemouseclass;
    if (activescreenclass) {activescreenclass->registerfarewell(NULL); delete activescreenclass;}
    cout << "Load Checksum Error\n";
    exit(1);
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void no_copies (void)
{
    if (activemouseclass) delete activemouseclass;
    if (activescreenclass) {activescreenclass->registerfarewell(NULL); delete activescreenclass;}
    cout << "Internal program error!  Attempt to create another non-reentrant class.\n";
    exit(1);
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void weapon_error (void)
{
    if (activemouseclass) delete activemouseclass;
    if (activescreenclass) {activescreenclass->registerfarewell(NULL); delete activescreenclass;}
    cout << "Unable to access the weapon/ammo data file.  This file normally\n"
            "resides under the filename OBDATA.DAT in your GEODATA directory.\n"
            "If you have the CD-ROM version of the game, then you will not be\n"
            "able to use either of the weapon or ammo editing functions as this\n"
            "file is required to be located on the CD-ROM (which is of course\n"
            "not modifyable).  Please see the documentation for more details\n"
            "about this problem.\n";
    exit(1);
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void memory_corrupt(void)
{
    if (activemouseclass) delete activemouseclass;
    if (activescreenclass) {activescreenclass->registerfarewell(NULL); delete activescreenclass;}
    cout << "Internal program error!  Memory overrun attempt.\n";
    exit(1);
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
