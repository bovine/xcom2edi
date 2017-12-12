// Xcom2edi
// Copyright (c) 1997, Jeff Lawson <jeff@bovine.net>
// All rights reserved.
// See LICENSE for distribution information.

#include <stdlib.h>
#include <conio.h>
#include <stdio.h>
#include <dos.h>
#include <dir.h>
#include <string.h>
#include <iostream.h>
#include <ctype.h>
#include <process.h>
#include <io.h>
#include <time.h>
#include "mouse.hpp"
#include "screen.hpp"
#include "colors.hpp"
#include "button.hpp"
#include "menubox.hpp"
#include "textbox.hpp"
#include "txtinput.hpp"
#include "checkbox.hpp"
#include "dropdown.hpp"
#include "spawno.h"
#include "stealth.h"
#include "define.h"


// []-------------------------[]
// |  define any other macros  |
// []-------------------------[]
#define TRUE 1
#define FALSE 0


// []----------------------[]
// |  define external data  |
// []----------------------[]
extern far char Farewell[];
extern far char FacilDat[];
extern far char ProductDat[];
extern far char ResearchDat[];
extern far char UpDat[];


// []----------------------------------------[]
// |  prototype all error messages we'll use  |
// []----------------------------------------[]
extern void no_memory (void);
extern void no_games (void);
extern void file_error (void);
extern void bad_crc (void);
extern void weapon_error (void);
extern void memory_corrupt(void);


// EDITOR1.CPP
#define TOTAL_BUILDINGS 18
extern char building_names[][19];
extern struct baselisting_type {unsigned char name[17], basedatidx, location;} baselisting[8];
extern unsigned numbases;
extern struct object_type {unsigned char name[31]; unsigned id;} objects[];
#define TOTAL_ITEMS 90
extern char damagetype[][15];


// EDITOR2.CPP
extern int file_exists (char *filename);
extern int ask_yes_no(char *question);
extern void ok_box(char *message);
extern int stat_edit (unsigned &initial, unsigned minimum, unsigned maximum, \
    char *text, long minmax);
extern int stat_edit (unsigned char &initial, unsigned minimum, unsigned maximum, \
    char *text, long minmax);
extern void statcat (char *target, char *format, int value);
extern int generic_button_pick (char *text, char *text1, char *text2=NULL, char *text3=NULL, \
    char *text4=NULL, char *text5=NULL, char *text6=NULL, char *text7=NULL, char *text8=NULL, char *text9=NULL);


// EDITOR3.CPP
extern void edit_soldiers(int savedgame);


// EDITOR4.CPP
extern void import_soldiers(int savedgame);


// EDITOR5.CPP
extern void money_edit (int savedgame);
extern void edit_inventory(int savedgame, int basenum);
extern void edit_buildings(int savedgame, int basenum);
extern void do_edit_astore(int savedgame, int basenum);
extern void do_edit_objects(void);


// EDITOR6.CPP
extern void tactical_editor(int savedgame);


// EDITOR7.CPP
extern void edit_diplomacy(int savedgame);

// EDITOR8.CPP
extern void submarine_editor(int savedgame);

