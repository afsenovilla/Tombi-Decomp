// VERSION SCUS_94236 scus94236 8001F1C0 788
// FLAGS -O2 -G0
// HEADERS tomba26
#if defined(VERSION_SCUS_94236)
// Portado de psx_tomba (main/video/movie.c, moviePlayerTask); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "game.h"
#include "psyq/libcd.h"

// INCLUDE_ASM("nonmatchings/main/video/movie", moviePlayerTask);
void moviePlayerTask(void)
{
    u_short state;
    Task* gameControl;
    Task* gameControlTemp;

    gameControl = CURRENT_TASK;
    MOVIE_PLAY_STATE = MOVIE_STATE_STARTING;
    gameControl->state0 = 0;
    gameControl->step.value = 0;
    gameControl->loadGameSelected = 0;
    do {
        if (MOVIE_SKIP_REQUEST == 1) {
            (CURRENT_TASK)->state0 = 3;
            CdMix(&D_80077758);
        }
        gameControlTemp = CURRENT_TASK;
        state = gameControlTemp->state0;
        switch (state) {
            case 0:
                movieInitDecodeEnv((DecEnv*)&MOVIE_DEC_ENV, 384, 256, 704, 256);
#if REGION == REGION_JP
                startMovieStream((int)&FILE_LINKS[*D_8007775C[MOVIE_ID]]);
#else
                startMovieStream((int)&FILE_LINKS[MOVIE_FILE_IDS[D_8007775C[MOVIE_ID]]]);
#endif
                gameControl = SCRATCHPAD.currentTask;
                gameControl->state0+=1;
                do {
                } while (movieDecodeNextFrame((DecEnv*)&MOVIE_DEC_ENV) == 0);
                break;
            case 1:
                MOVIE_PLAY_STATE = MOVIE_STATE_PLAYING;
                gameControlTemp->state0 = 2;
            case 2:
                while ((CURRENT_TASK)->step.value == 0) {
                    movieDecodeNextFrame((DecEnv*)&MOVIE_DEC_ENV);
                }
                DecDCTin(*(MOVIE_DEC_VLCID + &MOVIE_DEC_ENV), 2);
                *(int*)&MOVIE_DEC_DISPENV->disp.w = FRAME_BUFFER_INDEX;
                MOVIE_DEC_DISPENV->screen.x = ((short*)&MOVIE_DEC_RECT_X)[(FRAME_BUFFER_INDEX) * 4];
                MOVIE_DEC_DISPENV->screen.y = ((short*)&MOVIE_DEC_RECT_Y)[(FRAME_BUFFER_INDEX) * 4];
                DecDCTout(
                    *(u_long**)&MOVIE_DEC_IMGBUF[MOVIE_DEC_IMGID],
                    (MOVIE_DEC_DISPENV->screen.w * MOVIE_DEC_DISPENV->screen.h) / 2
                );
                (CURRENT_TASK)->step.value = 0;
                while (movieDecodeNextFrame((DecEnv*)&MOVIE_DEC_ENV) == 0) {
                    if (*(int*)&MOVIE_DEC_DISPENV->isinter == 1) {
                        break;
                    }
                }
                if (*(int*)&MOVIE_DEC_DISPENV->isinter == 0) {
                    do {
                    } while (*(int*)&MOVIE_DEC_DISPENV->isinter == 0);
                }
                SetDispMask(1);
                *(int*)&MOVIE_DEC_DISPENV->isinter = 0;
                MOVIE_PLAY_STATE = MOVIE_STATE_ENDING;
                (*(short *)&SPAD->vblankCount) = 0;
                break;
            case 3:
                DecDCToutCallback(NULL);
                StUnSetRing();
                StClearRing();
                CdControlB(CdlPause, 0, 0);
                MOVIE_PLAY_STATE = MOVIE_STATE_IDLE;
                SCRATCHPAD.movieSkipRequest = 0;
                exitTask();
                break;
        }
        sleepTask(1);
    } while(true);
}
#endif
