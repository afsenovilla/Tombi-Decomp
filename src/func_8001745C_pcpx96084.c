// VERSION PCPX_96084 tomba 8001745C 384
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// HEADERS.BETA_980203 tomba26
// Beta version: screen rectangle (0, 0, 256, 240) set at once. // Portado de psx_tomba (main/main.c, func_8001745C_pcpx96084); licencia MIT del proyecto original.
#ifndef SKIP_ASM
#define SKIP_ASM
#endif
#include "common.h"
#include "game.h"
#include "psyq/libcd.h"
extern u_long _ramsize;
extern u_long _stacksize;
void func_8001745C_pcpx96084(u_char r0, u_char g0, u_char b0)
{
    DISPENV* dispenv1;
    DISPENV* dispenv2;
    DRAWENV* drawenv1;
    DRAWENV* drawenv2;

    drawenv1 = (DRAWENV*)&DRAW_ENV_1;
    dispenv1 = (DISPENV*)((byte*)(&DRAW_ENV_1) - sizeof(DISPENV));
    drawenv2 = (DRAWENV*)((byte*)(&DRAW_ENV_1) + 0xD10);
    dispenv2 = (DISPENV*)((byte*)(&DRAW_ENV_1) + 0xD10 - sizeof(DISPENV));
    SetDefDrawEnv(drawenv1, 384, 0, 640, 480);
    SetDefDispEnv(dispenv1, 384, 0, 640, 480);
    SetDefDrawEnv(drawenv2, 384, 0, 640, 480);
    SetDefDispEnv(dispenv2, 384, 0, 640, 480);
    setRECT(&dispenv1->screen, 0, 0, 256, 240);
    setRECT(&dispenv2->screen, 0, 0, 256, 240);
    drawenv1->isbg = drawenv2->isbg = true;
    drawenv1->dtd = drawenv2->dtd = true;
    drawenv1->dfe = drawenv2->dfe = true;
    setRGB0(drawenv1, r0, g0, b0);
    setRGB0(drawenv2, r0, g0, b0);
    PutDispEnv(dispenv1);
    PutDrawEnv(drawenv1);
    return;
}
#endif
