// VERSION SCUS_94236 scus94236 8001F634 160
// FLAGS -O2 -G0
// HEADERS tomba26
#if defined(VERSION_SCUS_94236)
// Portado de psx_tomba (main/video/movie.c, startMovieStream); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "game.h"
#include "psyq/libcd.h"

// INCLUDE_ASM("nonmatchings/main/video/movie", startMovieStream);
void startMovieStream(s32 arg0)
{
    int mode;
    
    DecDCTReset(0);
    DecDCToutCallback(&mdecSliceCallback);
    CdMix(&D_80077754);
    StSetRing(&D_800D7188, 0x20);
    StSetStream(0, 1, -1, 0, 0);
    do {

    } while (CdControl(CdlSetloc, arg0, 0) == 0);
    mode = CdlModeStream | CdlModeSpeed | CdlModeRT;
    do {
        
    } while (CdRead2(mode) == 0);
    return;
}
#endif
