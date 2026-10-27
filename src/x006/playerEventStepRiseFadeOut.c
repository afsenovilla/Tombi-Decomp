// FUNC 8010ac90 516 X006
// MATCHING 8010ac90 516
#include "TOBJ.H"
#include "gametask.h"
extern void playerReleaseBittenAndClearFlags(TObj *);
extern void FUN_8001e4f0(int);
extern void FUN_8001fec0(TObj *);
extern void playerAirSpinToFall(TObj *);
extern void FUN_800eeb5c(TObj *, int);
extern TObj *PLAYER_EXT;
extern unsigned char DAT_8009c93f;
extern unsigned char DAT_8009c942;
extern unsigned char GAME_PLAYER_MODE;
extern unsigned char GAME_FADE_SCREEN_CONTROL[];
extern unsigned char DAT_8009c93c[];
extern unsigned short TICK_COUNT;

void playerEventStepRiseFadeOut(TObj *o)
{
    char pad;

    switch (o->state) {
    case 0:
        o->ba4 = 0;
        o->ba5 = 0;
        o->b9c = 1;
        o->visible = 1;
        o->ba7 = 0;
        o->active = 4;
        o->wb2 = 0;
        o->velX = 0;
        o->velY = 0;
        PLAYER_EXT->timer = 0;
        o->d8c = 0;
        o->timer = 0x1e;
        o->w22 = 0;
        playerReleaseBittenAndClearFlags(o);
        DAT_8009c93f = 1;
        DAT_8009c942 = 1;
        GAME_PLAYER_MODE = 3;
        o->d8c = 0;
        o->state++;
        break;
    case 1:
        if (o->substep != 0 && (TICK_COUNT & 3) == 0) FUN_8001e4f0(0x16);
        FUN_8001fec0(o);
        playerAirSpinToFall(o);
        o->y.raw += o->velY << 8;
        o->velY -= 0x30;
        FUN_800eeb5c(o, 4);
        if (--o->timer <= 0) {
            o->timer = 0x3c;
            GAME_FADE_SCREEN_CONTROL[0] = 3;
            DAT_8009c93c[0] = 0;
            o->state++;
        }
        break;
    case 2:
        FUN_8001fec0(o);
        playerAirSpinToFall(o);
        o->y.raw += o->velY << 8;
        o->velY -= 0x30;
        FUN_800eeb5c(o, 4);
        if (o->timer != 0) {
            if (--o->timer <= 0) {
                TASK_C->mode = 7;
                TASK_C->step = 0;
            }
        }
        break;
    }
}
