// VERSION BETA_980203 beta980203 8001774C 432
#if defined(VERSION_BETA_980203)
// Beta version: PAL initHud.c without the playerIdleState clear and the func_80020CB0 call.
#define SKIP_ASM
#include "common.h"
#include "game.h"
//INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/gameinit", allocObjectLayer3);

void initHud(void)
{
    char *tmp;

    memset((u_char *)&GVARS, 0, 0x2C);
    *(s16* )0x1F8001C6 = 0;
    GAME.fadeScreenControl = 1;
    *(s8* )0x1F8003D0 = 0;
    initAreaScripts();
    
    tmp = D_800B07AC;
    *tmp++ = (GAME.playerAP / 10000000) % 10;
    *tmp++ = (GAME.playerAP / 1000000 ) % 10;
    *tmp++ = (GAME.playerAP / 100000  ) % 10;
    *tmp++ = (GAME.playerAP / 10000   ) % 10;
    *tmp++ = (GAME.playerAP / 1000    ) % 10;
    *tmp++ = (GAME.playerAP / 100     ) % 10;
    *tmp++ = (GAME.playerAP / 10      ) % 10;
    *tmp++ = (GAME.playerAP / 1       ) % 10;
}
#endif
