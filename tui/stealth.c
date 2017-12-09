/*
Stealth Bomber Version 2.2

Kevin Dean
Fairview Mall P.O. Box 55074
1800 Sheppard Avenue East
Willowdale, Ontario
CANADA    M2J 5B9
CompuServe ID: 76336,3114

February 10, 1992

    This module checks a file for consistency.  It checks the file
date/time stamp, compares the file size from a directory search with the size
returned by opening the file, and then checks the CRC of the file against the
CRC passed to it.  The code was designed as an anti-virus algorithm.

    This code will not detect all viruses that attach themselves to files.
There are some that are capable of circumventing all these checks.  For this
reason, the stealth_sys_check() function should be called first to check the
system itself for viruses before calling this function.

    This code is public domain.
*/


#include <dir.h>
#include <dos.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stealth.h"

/* Macros to extract low and high bytes of a word. */
#define lowb(x)  (*(unsigned char *)&(x))
#define hib(x)   (*((unsigned char *)&(x) + 1))

/* Macros to extract low and high words of a dword. */
#define loww(x)  (*(unsigned short *)&(x))
#define hiw(x)   (*((unsigned short *)&(x) + 1))

static void *bufalloc(size_t *size, size_t minsize);
static crc32_t calccrc(FILE *f, byte *buffer, size_t bufsize, crc32_t polynomial);
unsigned stealth_file_check(const char *filename, filecrc fcrc);

/*************************************************************************/
/* Check file header consistency and calculate CRC of file. */
unsigned stealth_file_check(const char *filename, filecrc fcrc)
{
    unsigned result = STEALTH_OK;   /* Assume file passes all tests. */
    char fn[MAXPATH];               /* Complete file name with path. */
    struct ffblk dirinfo;           /* File directory information. */

    /* If name contains drive or directory, use unmodified, else build full name. */
    if (filename[1] == ':' || strchr(filename, '\\'))
        strcpy(fn, filename);
    else {
        fn[0] = 0;  /* Assume file not found. */

        /* DOS versions 3 and above save program name in _argv[0]. */
        if (_osmajor >= 3) {
            char *sptr;
            strcpy(fn, _argv[0]);

            /* Find last subdirectory delimiter. */
            if ((sptr = strrchr(fn, '\\')) != NULL)
                *(sptr + 1) = 0;    /* Clear file name. */
            else if (fn[1] == ':')  /* Check for drive designator. */
                fn[2] = 0;          /* Clear file name. */
            else
                fn[0] = 0;          /* _argv[0] is file name only. */

            /* Merge drive and directory with file name. */
            strcat(fn, filename);

            /* Attempt to access file; if failed, pass control onto path search. */
            if (access(fn, 4)) fn[0] = 0;
        }

        if (!fn[0]) {
            /* Search path for file. */
            const char *fnptr = searchpath(filename);

            /* Copy file name if found. */
            if (fnptr) strcpy(fn, fnptr);
        }
    }

    if (fn[0] && !findfirst(fn, &dirinfo, FA_RDONLY | FA_HIDDEN | FA_SYSTEM | FA_ARCH)) {
        struct ftime *timestamp;  /* Pointer to timestamp within dirinfo. */
        FILE *f;
        byte *buffer;         /* Buffer for program image. */
        size_t bufsize;       /* Buffer size. */

        /* ftime structure maps over time and date in ffblk structure. */
        timestamp = (struct ftime *)&dirinfo.ff_ftime;

        /* Check file time, day, and year. */
        if (timestamp->ft_tsec * 2 > 59 || timestamp->ft_min > 59 || timestamp->ft_hour > 23 ||
            timestamp->ft_day == 0 || timestamp->ft_year >= 100)
            result |= STEALTH_FILE_DATE_ERR;

        /* Check month and day based on month. */
        switch (timestamp->ft_month) {
            case 4:
            case 6:
            case 9:
            case 11:
                /* Thirty days hath September, April, June, and November. */
                if (timestamp->ft_day > 30)
                    result |= STEALTH_FILE_DATE_ERR;
                break;
            case 1:
            case 3:
            case 5:
            case 7:
            case 8:
            case 10:
            case 12:
                /* All the rest have thirty-one, excepting February alone. */
                if (timestamp->ft_day > 31)
                    result |= STEALTH_FILE_DATE_ERR;
                break;
            case 2:
                /* February hath twenty-eight days clear, and twenty-nine in each leap year. */
                if (timestamp->ft_day > (timestamp->ft_year % 4 ? 28 : 29))
                    result |= STEALTH_FILE_DATE_ERR;
                break;
            default:
                result |= STEALTH_FILE_DATE_ERR;
                break;
        }
        f = fopen(fn, "rb");

        if (f) {
            /* Seek to end of file and compare length to length returned by directory search. */
            fseek(f, 0L, SEEK_END);
            if (ftell(f) != dirinfo.ff_fsize)
                result |= STEALTH_FILE_SIZE_ERR;

            /* Rewind file. */
            rewind(f);

            /* Make sure that polynomial has its last bit and at least one other set. */
            if (!(fcrc.x.polynomial & 0x00000001) || fcrc.x.polynomial == 0x00000001)
                result |= STEALTH_CRC_BAD_POLY;

            /* Allocate 16k buffer if possible, but get at least 512 bytes. */
            bufsize = 16384;
            buffer = bufalloc(&bufsize, 512);

            if (buffer) {
                /* CRC is valid if calculated CRC matches what is stored in fcrc. */
                if (fcrc.x.crc != calccrc(f, buffer, bufsize, fcrc.x.polynomial))
                    result |= STEALTH_CRC_INVALID;

                free(buffer);
            } else
                result |= STEALTH_NO_MEM;

            fclose(f);
        } else
            result |= STEALTH_FILE_ERR;
    } else
        result |= STEALTH_FILE_ERR;

    return (result);
}
/*************************************************************************/
/* Allocate a buffer of flexible size. */
void *bufalloc(size_t *size, size_t minsize)
{
    void *buffer;   /* Buffer allocated. */
    size_t bufsize; /* Size of buffer allocated. */

    /* Allocate as big a buffer as possible (at least minsize). */
    for (bufsize = *size; bufsize >= minsize && !(buffer = malloc(bufsize)); bufsize /= 2);

    /* Save buffer size. */
    *size = bufsize;

    return (buffer);
}
/*************************************************************************/
/* Calculate the CRC of a file. The file is assumed to be open and the buffer is assumed to be valid. */
crc32_t calccrc(FILE *f, byte *buffer, size_t bufsize, crc32_t polynomial)
{
    crc32_t table[256];     /* CRC table. */
    register size_t i;      /* Byte counter. */
    register crc32_t *halfi;    /* Pointer to CRC of i / 2. */
    crc32_t crc;            /* Current CRC. */
    byte *bufptr;           /* Pointer to walk through buffer. */

    /* Generate a CRC lookup table for faster calculation. */
    for (i = 0, halfi = table, table[0] = 0; i < 256; i += 2, halfi++)
        if (hib(hiw(*halfi)) & 0x80)
            table[i] = (table[i + 1] = *halfi << 1) ^ polynomial;
        else
            table[i + 1] = (table[i] = *halfi << 1) ^ polynomial;

    /* Calculate CRC. */
    crc = 0;
    while ((i = fread(buffer, 1, bufsize, f)) != 0)
        for (bufptr = buffer; i--; bufptr++)
            crc = (crc << 8) ^ table[hib(hiw(crc)) ^ *bufptr];

    return (crc);
}
/*************************************************************************/
