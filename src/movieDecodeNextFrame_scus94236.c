// VERSION SCUS_94236 scus94236 8001EFE8 368
// FLAGS -O2 -G0
// HEADERS tomba26
#if defined(VERSION_SCUS_94236)
// Portado de psx_tomba (main/video/movie.c, movieDecodeNextFrame); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "game.h"
#include "psyq/libcd.h"

// INCLUDE_ASM("nonmatchings/main/video/movie", movieDecodeNextFrame);
int movieDecodeNextFrame(DecEnv* dec)
{
    u_long* addr;
    StHEADER* header;
    register u_long* next asm("$16");

    do {
        if (StGetNext(&addr, (u_long**)&header) != 0) {
            next = NULL;
            break;
        }
        if (header->frameCount >= D_80077728[MOVIE_ID] - 3) {
            (CURRENT_TASK)->state0 = 3;
            CdMix(&D_80077758);
        }
        if (SCRATCHPAD.movieId == 0x15 && (SCRATCHPAD.currentTask)->loadGameSelected == 0 && header->frameCount >= 0xF) {
            startBgmTrack(0);
            (CURRENT_TASK)->loadGameSelected = 1;
        }
        next = addr;
        dec->rect[0].w = dec->rect[1].w = header->width;
        dec->rect[0].h = dec->rect[1].h = header->height;
        dec->slice.h = header->height;
    } while (0);

    if (next == NULL) {
        asm("" : : : "$16");
        return 0;
    }
    dec->vlcid = 1 - dec->vlcid;
    DecDCTvlc(next, dec->vlcbuf[dec->vlcid]);
    StFreeRing(next);
    (CURRENT_TASK)->step.value = 1;
    return 1;
}
#endif
