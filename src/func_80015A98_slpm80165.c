// VERSION SLPM_80165 slpm80165 80015A98 384
// FLAGS -O2 -G0
// HEADERS tomba26
#if defined(VERSION_SLPM_80165)
// Demo version of initDisplayHiRes (src/initDisplayHiRes_scus94236.c): second environment at +0x2090.
// Portado de psx_tomba (main/main.c, initDisplayHiRes); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "game.h"
#include "psyq/libcd.h"

void initDisplayHiRes(u_char r0, u_char g0, u_char b0)
{
    DISPENV* dispenv1;
    DISPENV* dispenv2;
    DRAWENV* drawenv1;
    DRAWENV* drawenv2;

    drawenv1 = (DRAWENV*)&DRAW_ENV_1;
    dispenv1 = (DISPENV*)((byte*)(&DRAW_ENV_1) - sizeof(DISPENV));
    drawenv2 = (DRAWENV*)((byte*)(&DRAW_ENV_1) + 0x2090);
    dispenv2 = (DISPENV*)((byte*)(&DRAW_ENV_1) + 0x2090 - sizeof(DISPENV));
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
