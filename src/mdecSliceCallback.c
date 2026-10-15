// FUNC 8001d9d8 252 MAIN0
// MATCHING 8001d9d8 252
// Ported from psx_tomba (movie.c, mdecSliceCallback); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"

typedef struct {
    u_long* vlcbuf[2];
    int vlcid;
    u_short* imgbuf[2];
    int imgid;
    RECT rect[2];
    int rectid;
    RECT slice;
    int isdone;
} DecEnv;

void mdecSliceCallback(void)
{
    u_long* sliceRect = &MOVIE_DEC_DISPENV->screen;
    u_long* mdecImage = sliceRect - 0x8;
    int sliceSize;
    short screenX;
    int temp_v1;
    
    LoadImage(sliceRect, *(u_long**)&mdecImage[MOVIE_DEC_IMGID]);
    MOVIE_DEC_IMGID = 1 - MOVIE_DEC_IMGID;
    screenX = MOVIE_DEC_DISPENV->screen.x;
    MOVIE_DEC_DISPENV->screen.x += 0x10;

    asm("");
    temp_v1 = *(int*)&MOVIE_DEC_DISPENV->disp.w * 4;
    asm("");

    if (
            MOVIE_DEC_DISPENV->screen.x <
            (
                (((short*)&MOVIE_DEC_RECT_X)[temp_v1]) +
                (((short*)&MOVIE_DEC_RECT_W)[temp_v1])
            )
    ) {
        sliceSize = (MOVIE_DEC_DISPENV->screen.w * MOVIE_DEC_DISPENV->screen.h) / 2;
        DecDCTout(
            *(u_long**)&mdecImage[MOVIE_DEC_IMGID],
            sliceSize
        );
        return;
    }
    *(int*)&MOVIE_DEC_DISPENV->isinter = 1;
    MOVIE_DEC_DISPENV->screen.x = screenX;
    return;
}
