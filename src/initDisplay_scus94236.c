// VERSION SCUS_94236 scus94236 80016C4C 400
// FLAGS -O2 -G0
// HEADERS tomba26
#if defined(VERSION_SCUS_94236)
// Portado de psx_tomba (main/main.c, initDisplay); licencia MIT del proyecto original.
#ifndef SKIP_ASM
#define SKIP_ASM
#endif
#include "common.h"
#include "game.h"
#include "psyq/libcd.h"
extern u_long _ramsize;
extern u_long _stacksize;
void initDisplay(u_char r0, u_char g0, u_char b0)
{
    DISPENV* dispenv1;
    DISPENV* dispenv2;
    DRAWENV* drawenv1;
    DRAWENV* drawenv2;

    drawenv1 = (DRAWENV*)&DRAW_ENV_1;
    dispenv1 = (DISPENV*)((byte*)(&DRAW_ENV_1) - sizeof(DISPENV));
    drawenv2 = (DRAWENV*)((byte*)(&DRAW_ENV_1) + 0xD10);
    dispenv2 = (DISPENV*)((byte*)(&DRAW_ENV_1) + 0xD10 - sizeof(DISPENV));
    SetDefDrawEnv(drawenv1, 384, 256, 320, 224);
    SetDefDispEnv(dispenv1, 704, 256, 320, 224);
    SetDefDrawEnv(drawenv2, 704, 256, 320, 224);
    SetDefDispEnv(dispenv2, 384, 256, 320, 224);
    setWH(&dispenv1->screen, 256, 224);
    setWH(&dispenv2->screen, 256, 224);
    drawenv1->isbg = drawenv2->isbg = true;
    drawenv1->dtd = drawenv2->dtd = true;
    drawenv1->dfe = drawenv2->dfe = false;
    setRGB0(drawenv1, r0, g0, b0);
    setRGB0(drawenv2, r0, g0, b0);
    setXY(&dispenv1->screen, D_8009C864, D_8009C866);
    setXY(&dispenv2->screen, D_8009C864, D_8009C866);
    PutDispEnv(dispenv1);
    PutDrawEnv(drawenv1);
    return;
}
#endif
