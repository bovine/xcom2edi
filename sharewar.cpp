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
// |          JeffLawson@Aol.Com                                             |
// |                                                                         |
// |  also via phone at:                                                     |
// |                                                                         |
// |          (213) 258-5604                                                 |
// []-----------------------------------------------------------------------[]

#include "editor.h"


// our global flag
char registered;

extern "C" void far rkv(char far *username, char far *usercode, char far \
    *appcode, char far *yourname, long yourcode, char far *registered);

//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
// return TRUE if serial number is correct, FALSE if otherwise
static int serial_check(char *name, char *phone, char *serial)
{
    char *buffer;

    if ((buffer = new char[strlen(name) + strlen(phone) + 2]) == NULL) no_memory();
    sprintf(buffer, "%s;%s", name, phone);
    rkv(buffer, serial, REGCODE, "Jeff Lawson", 955605336, &registered);
    if (registered == FALSE) {
        char *s, *t = buffer;
        for (s = name; *s; s++) if (isalpha(*s)) *t++ = toupper(*s);
        *t++ = ';';
        for (s = phone; *s; s++) if (isdigit(*s)) *t++ = toupper(*s);
        *t++ = 0;
        rkv(buffer, serial, REGCODE, "Jeff Lawson", 955605336, &registered);
    }

    return(registered != FALSE);
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void getourpath(char far *filename)
{
    // figure out what directory we're running from
    asm mov ax,6200h                // return the PSP of this process
    asm int 21h                     // call DOS
    asm mov es,bx                   // set ES to the segment
    asm mov es,word ptr es:[002Ch]  // read in the segment
    asm xor di,di                   // ES:DI ==> place to look
label1:
    asm cmp word ptr es:[di],0      // \
    asm jz label2                   //  \ look for the start of it
    asm inc di                      //  /
    asm jmp label1                  // /
label2:
    asm add di,4                    // ES:DI ==> source buffer
    asm push ds
    asm lds si,dword ptr [filename] // DS:SI ==> target buffer
label3:
    asm mov ah,byte ptr es:[di]     // read in a byte
    asm mov byte ptr ds:[si],ah     //    and write it out
    asm inc di                      // increment source string pointer
    asm inc si                      // increment target string pointer
    asm or ah,ah                    // are we done?
    asm jnz label3                  //    no -- keep looping
    asm pop ds
    *(strrchr(filename, '\\') + 1) = 0;
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
// return TRUE if still shareware (not registered), otherwise FALSE
static int shareware_check(void)
{
    FILE *fp;
    char name[31], phone[31], serial[21], *buffer, filename[MAXPATH];

    // check the registration file
    getourpath(filename);
    strcat(filename, REGFILE);
    if ((fp = fopen(filename, "rb")) == NULL) return(TRUE);
    fread(name, 31, sizeof(char), fp);
    fread(phone, 21, sizeof(char), fp);
    fread(serial, 21, sizeof(char), fp);
    fclose(fp);

    // check the info
    if (serial_check(name, phone, serial) == FALSE) {
    	char filename[MAXPATH];
        getourpath(filename);
	    strcat(filename, REGFILE);
        unlink(filename);
        return(TRUE);
    }

    // display the info, if registered
    #define ALREADY_ROW 6
    #define ALREADY_COL 15
    activescreenclass->drawrepeat('�', 80*25, 1, 1, COLOR(LIGHT_WHITE, LIGHT_BLUE));
    activescreenclass->refresh();
    buffer = new char[50*5];
    sprintf(buffer, "\nThis copy of " APPNAME " has been registered to:\n\n"
                    "\2%s\n"
                    "%s\1\n\n"
                    "Usage of this program by anyone except for the\n"
                    "above person is illegal and is prohibited by law.\n", name, phone);
    textboxtype message(COLOR(LIGHT_WHITE, BLUE), COLOR(LIGHT_YELLOW, BLUE), \
        ALREADY_ROW, ALREADY_COL, 53, 11, buffer);
    delete buffer;
    message.handle();
    activemouseclass->setcursor(MOUSE_WAIT);
    delay(3000);
    activemouseclass->setcursor(MOUSE_ARROW);

    return(FALSE);
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
static void shareware_register(void)
{
    // clear the screen
    activescreenclass->drawrepeat('�', 80*25, 1, 1, COLOR(LIGHT_WHITE, LIGHT_BLUE));
    activescreenclass->refresh();

    // create all controls we're going to use
    #define REGISTER_ROW 6
    #define REGISTER_COL 18
    textboxtype message(COLOR(LIGHT_WHITE, BLUE), COLOR(LIGHT_YELLOW, BLUE), \
        REGISTER_ROW, REGISTER_COL, 46, 15, "\nPlease enter your registration information\n\n"
        "Full Name:\n\nPhone No:\n\nSerial No:\n");
    textinputtype name_entry(COLOR(BLACK, LIGHT_WHITE), \
        REGISTER_ROW + 4, REGISTER_COL + 13, 30, NULL);
    textinputtype phone_entry(COLOR(BLACK, LIGHT_WHITE), \
        REGISTER_ROW + 6, REGISTER_COL + 13, 20, NULL);
    textinputtype serial_entry(COLOR(BLACK, LIGHT_WHITE), \
        REGISTER_ROW + 8, REGISTER_COL + 13, 20, NULL);
    buttontype ok_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY),\
        REGISTER_ROW + 10, REGISTER_COL + 5, "OK");
    buttontype cancel_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY),\
        REGISTER_ROW + 10, REGISTER_COL + 13, "Cancel");
    buttontype order_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY),\
        REGISTER_ROW + 10, REGISTER_COL + 25, "Ordering Info");

    // main loop
    while (TRUE) {
        message.handle();
        name_entry.handle();
        phone_entry.handle();
        serial_entry.handle();
        ok_button.handle();
        if (ok_button.pressed && strlen(name_entry.message) && strlen(phone_entry.message) && strlen(serial_entry.message)) {
	    	if (serial_check(name_entry.message, phone_entry.message, serial_entry.message) == FALSE) {
    		    activescreenclass->locate(1, 1, 0);
                ok_box("Invalid number!  Check your entry.\n"
                       "Note:  Spacing, capitalization, and\n"
                       "punctuation is ignored.\n");
		    } else {
		        activescreenclass->drawrepeat('�', 80*25, 1, 1, COLOR(LIGHT_WHITE, LIGHT_BLUE));
		        activescreenclass->locate(1, 1, 0);
		        #define THANKS_ROW 10
		        #define THANKS_COL 25
		        textboxtype message(COLOR(LIGHT_WHITE, BLUE), COLOR(LIGHT_YELLOW, BLUE), \
		            THANKS_ROW, THANKS_COL, 30, 5, "\nThank you for registering!\n");
		        message.handle();
		        activemouseclass->setcursor(MOUSE_WAIT);
		        delay(3000);
		        activemouseclass->setcursor(MOUSE_ARROW);
                break;
		    }
        }
        cancel_button.handle();
        if (cancel_button.pressed) return;
        order_button.handle();
        if (order_button.pressed) {
            activescreenclass->save();
            activescreenclass->locate(1, 1, 0);
            #define INFO_ROW 2
            #define INFO_COL 23
            textboxtype message(COLOR(LIGHT_WHITE, BLUE), COLOR(LIGHT_YELLOW, BLUE), \
                INFO_ROW, INFO_COL, 36, 23, "\n"
                "To register " APPNAME " and receive\n"
                "your serial number, simply send a\n"
                "check for US $5 with your \2name\1,\n"
                "your \2phone number\1, and a self-\n"
                "addressed stamped envelope to:\n\n"
                "    \2Jeff Lawson\n"
                "    1893 Kaweah Drive\n"
                "    Pasadena, CA 91105-2174\n"
                "    USA\1\n\n"
                "Including an e-mail address\n"
                "instead of an envelope will\n"
                "allow a faster reply.  Checks\n"
                "should be made to \2Jeff Lawson\1.\n"
                );
            buttontype ok_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY),\
                INFO_ROW + 18, INFO_COL + 15, "OK");
            while (!ok_button.pressed) {
                message.handle();
                ok_button.handle();
            }
            activescreenclass->restore();
        }
    }

    // create the registration file
    FILE *fp;
    char filename[MAXPATH];
    getourpath(filename);
    strcat(filename, REGFILE);
    if ((fp = fopen(filename, "wb")) == NULL) file_error();
    fwrite(name_entry.message, 31, sizeof(char), fp);
    fwrite(phone_entry.message, 21, sizeof(char), fp);
    fwrite(serial_entry.message, 21, sizeof(char), fp);
    fclose(fp);
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void shareware_annoyance(void)
{
    char format[] = "\nSince you have not registered this\n" \
                    "program, you wouldn't mind entering\n"
                    "this little number for me, would you?\n" \
                    "            \2%s\1\n";
    char buffer[35*4], the_number[14];
    randomize();

    while (shareware_check()) {
        // clear the screen
        activescreenclass->drawrepeat('�', 80*25, 1, 1, COLOR(LIGHT_WHITE, LIGHT_BLUE));
        activescreenclass->refresh();

        // create all controls we're going to use
        #define ANNOY_ROW 6
        #define ANNOY_COL 20
        sprintf(the_number, "%lu", ((unsigned long) rand() << 16) | rand());
        sprintf(buffer, format, the_number);
        textboxtype message(COLOR(LIGHT_WHITE, BLUE), COLOR(LIGHT_YELLOW, \
            BLUE), ANNOY_ROW, ANNOY_COL, 41, 14, buffer);
        textinputtype text_entry(COLOR(BLACK, LIGHT_WHITE), \
            ANNOY_ROW + 7, ANNOY_COL + 12, 15, NULL);
        buttontype ok_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, \
            GRAY), ANNOY_ROW + 9, ANNOY_COL + 4, "OK");
        buttontype cancel_button(COLOR(BLACK, LIGHT_WHITE), COLOR(\
            LIGHT_WHITE, GRAY),ANNOY_ROW + 9, ANNOY_COL + 12, "Cancel");
        buttontype register_button(COLOR(BLACK, LIGHT_WHITE), COLOR(\
            LIGHT_WHITE, GRAY), ANNOY_ROW + 9, ANNOY_COL + 24, "Register");

        // main loop
        while (TRUE) {
            // update the controls
            message.handle();
            text_entry.handle();
            ok_button.handle();
            if (ok_button.pressed) break;
            cancel_button.handle();
            if (cancel_button.pressed) exit(0);
            register_button.handle();
            if (register_button.pressed) {
                text_entry.destroy();   // only have what you need active
                shareware_register();
                break;
            }
        }
        if (strcmp(text_entry.message, the_number) == 0) break;
    }
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void unregistered_warning(void)
{
    ok_box("This function has been crippled\n"
           "because you haven't registered.\n"
           "Changes made here won't be saved.\n");
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void registered_only(void)
{
    ok_box("The requested changes weren't saved\n"
           "because you have not registered.\n"
           "Please register today!\n");
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같

