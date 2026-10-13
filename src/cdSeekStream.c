// FUNC 8001d63c 104 MAIN0
// MATCHING 8001d63c 104
#include "globals.h"
extern unsigned char DAT_80077f3c[];
extern char FILE_LINKS[];
extern int CdControl(int, void *, void *);
extern int CdControlF(int, void *);

void cdSeekStream(short a)
{
    if (CdControl(2, FILE_LINKS + MOVIE_FILE_IDS[DAT_80077f3c[a]] * 8, 0) != 0)
        CdControlF(0x15, 0);
}
