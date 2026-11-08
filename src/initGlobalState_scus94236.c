// VERSION SCUS_94236 scus94236 80016A18 220
// FLAGS -O2 -G0
// HEADERS tomba26
#if defined(VERSION_SCUS_94236)
// Portado de psx_tomba (main/main.c, initGlobalState); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "game.h"
#include "psyq/libcd.h"

// INCLUDE_ASM("nonmatchings/main/main", initGlobalState);
void initGlobalState(void)
{
    scratchpad* scratch = PSX_SCRATCH;
    
#if REGION != REGION_JP
    D_8009EB5A = 0xFFFF;
    D_8009EB7C = 0xFFFF;
    scratch->loadedSoundSet = 0xFF;
    scratch->loadedListFile = 0xFF;
#endif
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
#if REGION != REGION_JP
    (*(short *)&D_8009C9DA) = 0;
    (*(short *)&D_8009C9DE) = 0;
    D_8009C940 = 0;
    D_8009EB52 = 0;
    D_8009C864 = 0;
    D_8009C866 = 8;
#endif
}
#endif
