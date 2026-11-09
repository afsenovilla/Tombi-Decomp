// FUNC 8001d63c 104 MAIN0
// MATCHING 8001d63c 104
// VERSION SLPS_01144 slps01144 8001D584 92
#if defined(VERSION_SLPS_01144)
#if defined(VERSION_SLPS_01144)
#include "globals.h"
extern short *D_80077F3C[];
extern char FILE_LINKS[];
extern int CdControl(int, void *, void *);
extern int CdControlF(int, void *);

void func_8001D584(short a)
{
    if (CdControl(2, FILE_LINKS + *D_80077F3C[a] * 8, 0) != 0)
        CdControlF(0x15, 0);
}
#endif
#else
#include "globals.h"
extern unsigned char MOVIE_FILE_ID_INDEX[];
extern char FILE_LINKS[];
extern int CdControl(int, void *, void *);
extern int CdControlF(int, void *);

void cdSeekStream(short a)
{
    if (CdControl(2, FILE_LINKS + MOVIE_FILE_IDS[MOVIE_FILE_ID_INDEX[a]] * 8, 0) != 0)
        CdControlF(0x15, 0);
}
#endif
