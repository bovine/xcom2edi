// Xcom2edi
// Copyright (c) 1997, Jeff Lawson <jeff@bovine.net>
// All rights reserved.
// See LICENSE for distribution information.


#include "editor.h"

//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
// return true if the specified file name exists
int file_exists (char *filename)
{
    struct ffblk f;
    return (!findfirst(filename, &f, 0));
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
#define ASKSAVE_ROW 10
#define ASKSAVE_COL 20
int ask_yes_no(char *question)
{
    activescreenclass->save();
    textboxtype savemessage(COLOR(LIGHT_WHITE, BLUE), COLOR(LIGHT_YELLOW, BLUE), \
        ASKSAVE_ROW, ASKSAVE_COL, 39, 8, question);
    buttontype yes_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY),\
        ASKSAVE_ROW + 4, ASKSAVE_COL + 10, "Yes");
    buttontype no_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY),\
        ASKSAVE_ROW + 4, ASKSAVE_COL + 20, "No");
    while (!yes_button.pressed && !no_button.pressed) {
        savemessage.handle();
        yes_button.handle();
        no_button.handle();
    }
    activescreenclass->restore();
    return(yes_button.pressed);
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
#define OKBOX_ROW 10
#define OKBOX_COL 20
void ok_box(char *message)
{
    activescreenclass->save();
    textboxtype savemessage(COLOR(LIGHT_WHITE, BLUE), COLOR(LIGHT_YELLOW, BLUE), \
        OKBOX_ROW, OKBOX_COL, 39, 8, message);
    buttontype ok_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY),\
        OKBOX_ROW + 4, OKBOX_COL + 15, "OK");
    while (!ok_button.pressed) {
        savemessage.handle();
        ok_button.handle();
    }
    activescreenclass->restore();
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
// if minmax == minimum, then there will be a minimum button
// if minmax == maximum, then there will be a maximum button
// if minmax == -1,      then there will be an unchanged button (returns TRUE)
//                       else there will be a lots button
//
// (if minmax != -1 then a return value of TRUE indicates cancel)
//
int stat_edit (unsigned &initial, unsigned minimum, unsigned maximum, \
    char *text, long minmax)
{
    // []-----------------[]
    // |  save the screen  |
    // []-----------------[]
    activescreenclass->save();


    // []------------------------------------[]
    // |  create all controls that we'll use  |
    // []------------------------------------[]
    #define STAT_ROW 9
    #define STAT_COL 20
    textboxtype message(COLOR(LIGHT_WHITE, BLUE), COLOR(LIGHT_YELLOW, BLUE), \
        STAT_ROW, STAT_COL, 35, 11, text);
    textinputtype stat_text(COLOR(BLACK, LIGHT_WHITE), \
        STAT_ROW + 4, STAT_COL + 7, 15, NULL);
    if (minmax == -1) {
        strcpy(stat_text.message, "Unchanged");
    } else {
        sprintf(stat_text.message, "%u", initial);
    }
    buttontype other_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY), \
        STAT_ROW + 6, STAT_COL + 2, (minmax == maximum ? "Maximum" : \
         (minmax == minimum ? "Minimum" : (minmax == -1 ? "Unchanged" : "  Lots ") ) ) );
    buttontype ok_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY), \
        STAT_ROW + 6, STAT_COL + 15, "OK");
    buttontype cancel_button(COLOR(BLACK, LIGHT_WHITE), COLOR(LIGHT_WHITE, GRAY), \
        STAT_ROW + 6, STAT_COL + 23, "Cancel");


    // []-----------------[]
    // |  main input loop  |
    // []-----------------[]
    unsigned newvalue = initial, retvalue = FALSE;
    while (TRUE) {
        message.handle();
        stat_text.handle();

        // ok button
        ok_button.handle();
        if (ok_button.pressed) {
            // return the new amount
            sscanf(stat_text.message, "%u", &newvalue);
            if (newvalue < minimum) newvalue = minimum;
            if (newvalue > maximum) newvalue = maximum;
            break;
        }

        // cancel button
        cancel_button.handle();
        if (cancel_button.pressed) {
            newvalue = initial;
            if (minmax != -1) retvalue = TRUE;
            break;
        }

        // other button
        other_button.handle();
        if (other_button.pressed) {
            if (minmax == -1) {newvalue = initial; retvalue = TRUE;}
            sprintf(stat_text.message, "%u", minmax);
            stat_text.needupdate = TRUE;
        }
    }


    // []--------------------[]
    // |  restore the screen  |
    // []--------------------[]
    activescreenclass->restore();
    initial = newvalue;
    return(retvalue);
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
int stat_edit (unsigned char &initial, unsigned minimum, unsigned maximum, \
    char *text, long minmax)
{
    unsigned INITIAL = initial;
    int retvalue = stat_edit (INITIAL, minimum, maximum, text, minmax);
    initial = INITIAL;
    return(retvalue);
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void statcat (char *target, char *format, int value)
{
    char buffer[20];
    if (value == -1) {
        strcpy(buffer, "unchanged");
    } else {
        sprintf(buffer, "%i", value);
    }
    sprintf(strchr(target, 0), format, buffer);
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
int generic_button_pick (char *text, char *text1, char *text2, char *text3, \
    char *text4, char *text5, char *text6, char *text7, char *text8, char *text9)
{
    // []-----------------[]
    // |  save the screen  |
    // []-----------------[]
    activescreenclass->save();

    // []------------------------------------[]
    // |  create all controls that we'll use  |
    // []------------------------------------[]
    #define BUTTONPICK_ROW 9
    #define BUTTONPICK_COL 3
    textboxtype message(COLOR(LIGHT_WHITE, BLUE), COLOR(LIGHT_YELLOW, BLUE), \
        BUTTONPICK_ROW, BUTTONPICK_COL, 75, 8, text);
    buttontype *button1 = NULL, *button2 = NULL, *button3 = NULL, *button4 = NULL, \
        *button5 = NULL, *button6 = NULL, *button7 = NULL, *button8 = NULL, *button9 = NULL;
    int col = 2;
    if (text1) {
        button1 = new buttontype(COLOR(BLACK, LIGHT_WHITE), \
            COLOR(LIGHT_WHITE, GRAY), BUTTONPICK_ROW + 3, BUTTONPICK_COL + col, text1);
        col += 4 + strlen(text1);
    }
    if (text2) {
        button2 = new buttontype(COLOR(BLACK, LIGHT_WHITE), \
            COLOR(LIGHT_WHITE, GRAY), BUTTONPICK_ROW + 3, BUTTONPICK_COL + col, text2);
        col += 4 + strlen(text2);
    }
    if (text3) {
        button3 = new buttontype(COLOR(BLACK, LIGHT_WHITE), \
            COLOR(LIGHT_WHITE, GRAY), BUTTONPICK_ROW + 3, BUTTONPICK_COL + col, text3);
        col += 4 + strlen(text3);
    }
    if (text4) {
        button4 = new buttontype(COLOR(BLACK, LIGHT_WHITE), \
            COLOR(LIGHT_WHITE, GRAY), BUTTONPICK_ROW + 3, BUTTONPICK_COL + col, text4);
        col += 4 + strlen(text4);
    }
    if (text5) {
        button5 = new buttontype(COLOR(BLACK, LIGHT_WHITE), \
            COLOR(LIGHT_WHITE, GRAY), BUTTONPICK_ROW + 3, BUTTONPICK_COL + col, text5);
        col += 4 + strlen(text5);
    }
    if (text6) {
        button6 = new buttontype(COLOR(BLACK, LIGHT_WHITE), \
            COLOR(LIGHT_WHITE, GRAY), BUTTONPICK_ROW + 3, BUTTONPICK_COL + col, text6);
        col += 4 + strlen(text6);
    }
    if (text7) {
        button7 = new buttontype(COLOR(BLACK, LIGHT_WHITE), \
            COLOR(LIGHT_WHITE, GRAY), BUTTONPICK_ROW + 3, BUTTONPICK_COL + col, text7);
        col += 4 + strlen(text7);
    }
    if (text8) {
        button8 = new buttontype(COLOR(BLACK, LIGHT_WHITE), \
            COLOR(LIGHT_WHITE, GRAY), BUTTONPICK_ROW + 3, BUTTONPICK_COL + col, text8);
        col += 4 + strlen(text8);
    }
    if (text9) {
        button9 = new buttontype(COLOR(BLACK, LIGHT_WHITE), \
            COLOR(LIGHT_WHITE, GRAY), BUTTONPICK_ROW + 3, BUTTONPICK_COL + col, text9);
        col += 4 + strlen(text9);
    }


    // []-----------------[]
    // |  main input loop  |
    // []-----------------[]
    int initial;
    while (TRUE) {
        message.handle();
        if (button1) {
            button1->handle();
            if (button1->pressed) {initial = -1; break;}
        }
        if (button2) {
            button2->handle();
            if (button2->pressed) {initial = 0; break;}
        }
        if (button3) {
            button3->handle();
            if (button3->pressed) {initial = 1; break;}
        }
        if (button4) {
            button4->handle();
            if (button4->pressed) {initial = 2; break;}
        }
        if (button5) {
            button5->handle();
            if (button5->pressed) {initial = 3; break;}
        }
        if (button6) {
            button6->handle();
            if (button6->pressed) {initial = 4; break;}
        }
        if (button7) {
            button7->handle();
            if (button7->pressed) {initial = 5; break;}
        }
        if (button8) {
            button8->handle();
            if (button8->pressed) {initial = 6; break;}
        }
        if (button9) {
            button9->handle();
            if (button9->pressed) {initial = 7; break;}
        }
    }


    // []--------------------[]
    // |  restore the screen  |
    // []--------------------[]
    if (button1) delete button1;
    if (button2) delete button2;
    if (button3) delete button3;
    if (button4) delete button4;
    if (button5) delete button5;
    if (button6) delete button6;
    if (button7) delete button7;
    if (button8) delete button8;
    if (button9) delete button9;
    activescreenclass->restore();
    return(initial);
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
