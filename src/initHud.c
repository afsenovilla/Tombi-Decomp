// FUNC 80017b44 448 MAIN0
// MATCHING 80017b44 448
// VERSION SLPM_80165 slpm80165 800167F0 412
// Portado de psx_tomba (gameinit.c, initHud); licencia MIT del proyecto original.
#if defined(VERSION_SLPM_80165)
// Demo version of PAL main0 80017B44 (initHud.c): only clears 0x1F8001C6 and runs initAreaScripts before the AP
// digits.
// Portado de psx_tomba (gameinit.c, initHud); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "game.h"
//INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/gameinit", allocObjectLayer3);
















void initHud(void)
{
    char *tmp;

    memset(D_8009BC98, 0, sizeof(D_8009BC98));
    *(s16* )0x1F8001C6 = 0;
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
#else
#define SKIP_ASM
#include "common.h"
#include "game.h"
//INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/gameinit", allocObjectLayer3);

void initHud(void)
{
    char *tmp;

    memset(D_8009BC98, 0, sizeof(D_8009BC98));
    *(s16* )0x1F8001C6 = 0;
    GAME.fadeScreenControl = 1;
    *(s8* )0x1F8003D0 = 0;
    GAME.playerIdleState = 0;
    initAreaScripts();
    func_80020CB0();
    
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
