// VERSION BETA_980203 beta980203 800166CC 128
// FLAGS -O2 -G0
// HEADERS tomba26
#if defined(VERSION_BETA_980203)
// Beta version of USA initGlobalState with the JP-region body (no western statements).
// Portado de psx_tomba (main/main.c, initGlobalState); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "game.h"
#include "psyq/libcd.h"

// INCLUDE_ASM("nonmatchings/main/main", initGlobalState);
void initGlobalState(void)
{
    scratchpad* scratch = PSX_SCRATCH;
    
    scratch->pauseFlags = 0;
    scratch->unk1F2 = 0;
    scratch->pauseToggle = 0;
    scratch->frameCount = 0;
    scratch->activeFrameCount = 0;
    scratch->moviePlayState = 0;
    scratch->inGame = 0;
    scratch->inTitle = 0;
    scratch->loadComplete = 0;
    scratch->unk1CF = 0;
    scratch->movieSkipRequest = 0;
    scratch->drawFrozen = 0;
    scratch->unk1C4 = 0;
    scratch->unk1C5= 0;
    scratch->unk3CC = 0;
}
#endif
