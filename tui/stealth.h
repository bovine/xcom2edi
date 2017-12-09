/*
Stealth Bomber Version 2.2

Kevin Dean
Fairview Mall P.O. Box 55074
1800 Sheppard Avenue East
Willowdale, Ontario
CANADA    M2J 5B9
CompuServe ID: 76336,3114

February 10, 1992

    This is the interface to the anti-virus system and CRC checks in the
Stealth Bomber package.

    This code is public domain.
*/


#include <stdio.h>


typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned long crc32_t;

typedef union {
    char searchstr[8];    /* Used by Stealth Bomber package only. */

    struct {
        crc32_t polynomial; /* Polynomial for the file. */
        crc32_t crc;    /* Calculated CRC for the file. */
    } x;
} filecrc;


/* Anti-virus validation return codes. */
enum VirTestResult {
    STEALTH_OK            = 0x0000,   /* No virus found. */
    STEALTH_FILE_ERR      = 0x0001,   /* File not found or unable to open. */
    STEALTH_FILE_DATE_ERR = 0x0002,   /* File date/time stamp invalid. */
    STEALTH_FILE_SIZE_ERR = 0x0004,   /* File size inconsistent between directory and file open checks. */
    STEALTH_CRC_BAD_POLY  = 0x0008,   /* CRC polynomial is invalid. */
    STEALTH_NO_MEM        = 0x0010,   /* No memory to perform CRC check. */
    STEALTH_CRC_INVALID   = 0x0020    /* CRC is invalid. */
};


#if defined(__cplusplus)
extern "C" {
#endif

unsigned stealth_file_check(const char *filename, filecrc crc);

#if defined(__cplusplus)
}
#endif
