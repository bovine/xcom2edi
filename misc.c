#include <stdlib.h>
#include <stdio.h>
#include <process.h>
#include <string.h>
#include <dos.h>
#include <dir.h>
#include <conio.h>
#include "modulatr.h"

/****************************************************************************/
/***    Beep the user's speaker in an error                               ***/
/****************************************************************************/
void beep_user(void)
{
    sound(550);
    delay(50);
    nosound();
}


/****************************************************************************/
/***    return true if the specified file exists                          ***/
/****************************************************************************/
int file_exists (char *filename)
{
    struct ffblk f;
    return (!findfirst(filename, &f, 0));
}


/****************************************************************************/
/***    return true if the specified drive exists                         ***/
/****************************************************************************/
int drive_exists (int drive)
{
    char filespec[3], fcb[25];

    sprintf(filespec, "%c:", 'A' + drive);
    
    asm mov ax,2900h
    asm push ds
    asm mov cx,ss
    asm mov ds,cx
    asm lea si,filespec
    asm mov es,cx
    asm lea di,fcb
    asm int 21h
    asm pop ds

    return(_AL == 0 || _AL == 1);
}


/****************************************************************************/
/***    convert a number to ASCII text and justify it                     ***/
/****************************************************************************/
char *process_num(unsigned int number, int base, char *buffer, int length, char fill)
{
    int i, j;

    itoa((unsigned) number, buffer, base);
    i = length - strlen(buffer);
    if (i > 0) {
        for (j = 0; j < length; j++) buffer[j] = fill;
        itoa((unsigned) number, buffer + i, base);
    }
    strupr(buffer);
    return(buffer);
}


/****************************************************************************/
/***    read in all the filenames and store them in the array             ***/
/****************************************************************************/
void get_files(char *file, char files[][13], int *totalfiles, int maxfiles)
{
    struct ffblk f;
    int done, i = 0;

    done = findfirst(file, &f, 0);
    while (!done && i < maxfiles) {
        strcpy(files[i++], f.ff_name);
        done = findnext(&f);
    }
    *totalfiles = i;
}


/****************************************************************************/
/***    read in all the directory names and store them in the array       ***/
/****************************************************************************/
void get_directories(char directory[][13], int *totaldirs, int maxdirs)
{
    struct ffblk f;
    int done, i = 0;
    char file[4] = {"*.*"};

    done = findfirst(file, &f, FA_DIREC);
    while (!done && i < maxdirs) {
        if (f.ff_attrib == FA_DIREC) strcpy(directory[i++], f.ff_name);
        done = findnext(&f);
    }
    *totaldirs = i;
}


/****************************************************************************/
/***    dissects a filename with path and returns only the filename       ***/
/****************************************************************************/
char *short_file(char *fullpath, char *filename)
{
    char *i;

    if ((i = strrchr(fullpath, '\\')) != NULL) {
        strcpy(filename, strupr(i + 1));
    } else if ((i = strrchr(fullpath, ':')) != NULL) {
        strcpy(filename, strupr(i + 1));
    } else {
        strcpy(filename, strupr(fullpath));
    }
    return(filename);
}


/****************************************************************************/
/***    shells to DOS and then returns to the program                     ***/
/****************************************************************************/
int shell_to_dos (char *message)
{
    char *comspec;

    comspec = getenv("COMSPEC");
    if (comspec != NULL) {
        clrscr();
        printf(message);
        return(spawnlp(P_WAIT, comspec, NULL));
    } else {
        return(-1); /* error */
    }
}


/****************************************************************************/
/***    returns a the full path that the program is executing in          ***/
/****************************************************************************/
char *get_our_path (char *path)
{
    strcpy(path, "\0");                 // set the pathname first to nothing
    asm mov     ax,6200h                // return the PSP of this process
    asm int     21h                     // call DOS
    asm mov     es,bx                   // set ES to the segment
    asm mov     es,word ptr es:[002Ch]  // read in the segment
    asm xor     di,di                   // ES:DI ==> place to look
label1:
    asm cmp     word ptr es:[di],0      // \
    asm jz      label2                  //  \ look for the start of it
    asm inc     di                      //  /
    asm jmp     label1                  // /
label2:
    asm add     di,4                    // ES:DI ==> source buffer
    asm push    ds
    asm mov     si,word ptr [path]      // DS:SI ==> target buffer
    asm mov     ax,word ptr [path+2]
    asm mov     ds,ax
label3:
    asm mov     ah,byte ptr es:[di]     // read in a byte
    asm mov     byte ptr ds:[si],ah     //    and write it out
    asm inc     di                      // increment source string pointer
    asm inc     si                      // increment target string pointer
    asm or      ah,ah                   // are we done?
    asm jnz     label3                  //    no -- keep looping
    asm pop     ds

    *strrchr(path, '\\') = '\0';        // chop off the filename
    return(path);                       // return the pointer to it
}


/****************************************************************************/
/***    Detects whether the user has a VGA monitor                        ***/
/****************************************************************************/
int vga_test(void)
{
    asm mov     ax,1A00h                // get display combination
    asm int     10h                     // call VIDEO
    asm xor     bh,bh                   // we only want the active monitor
    return (_BX >= 7 && _BX <= 8);
}


/****************************************************************************/
/***    Changes the video mode to 50-line color text                      ***/
/****************************************************************************/
void vgatextmode (void)
{
    asm MOV     AX,1202h
    asm MOV     BL,30h
    asm INT     10h
    asm MOV     AX,0003h
    asm INT     10h
    textmode(C4350); clrscr();
}


/****************************************************************************/
/***    Checks to see if Windows is running                               ***/
/****************************************************************************/
int windows_test(void)
{
    asm MOV AX,1600H
    asm INT 2FH
    asm AND AX,7FH
    asm JNZ WE_GOT_WINDOWS
    return(0);
WE_GOT_WINDOWS:
    return(1);
}


/****************************************************************************/
/***    Tests for the presence of an 80286 processor                      ***/
/****************************************************************************/
int is_286_present (void)
{
    asm pushf                   // \ put the flags in AX
    asm pop     ax              // /
    asm and     ax,0fffh        // now try to clear bits 12-15 of CPU flags
    asm push    ax              // \ set modified CPU flags
    asm popf                    // /
    asm pushf                   // \ get flags again
    asm pop     ax              // /
    return ((_AX & 0xF000) != 0xF000);
}
