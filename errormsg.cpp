// []-----------------------------------------------------------------------[]
// |                                                                         |
// |                                                                         |
// |  This source and its associated executable are copyright by Jeff Lawson |
// |  of JL Enterprises, however some portions of this code may or may not   |
// |  have been originally inspired by other authors and credit is given if  |
// |  due.  You are encouraged to redistribute this package, but you must    |
// |  include all files originally distributed in this archive in their      |
// |  original state and format and without modifications of any kind.       |
// |  Under no circumstances may you make modifications to this product and  |
// |  redistribute the resulting code or executables.  If you believe you    |
// |  have made some useful or otherwise important enhancements and you      |
// |  would like to share these changes with others, please contact the      |
// |  author through one of the methods listed below.  Additionally, no      |
// |  fees may be charged for the usage of this product by anyone other than |
// |  the author of this file, except for fees (not to exceed US $10) by     |
// |  disk distributors to cover disk duplication and handling.  If there    |
// |  is any need to question this policy, please contact the author.        |
// |  This source code and its executable are distributed without any kind   |
// |  of warranty and the author may not be held accountable for damages of  |
// |  any kind.                                                              |
// |                                                                         |
// |                                                                         |
// |  I can reached via postal mail at:                                      |
// |                                                                         |
// |          Jeff Lawson                                                    |
// |          1893 Kaweah Drive                                              |
// |          Pasadena, CA 91105-2174                                        |
// |          USA                                                            |
// |                                                                         |
// |  or through E-mail at:                                                  |
// |                                                                         |
// |          jlawson@hmc.edu    or   JeffLawson@Aol.Com                     |
// |                                                                         |
// |  also via phone at:                                                     |
// |                                                                         |
// |          (213) 258-5604     or   (213) 258-4264                         |
// |                                                                         |
// []-----------------------------------------------------------------------[]

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
