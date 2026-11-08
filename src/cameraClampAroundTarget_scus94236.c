// VERSION SCUS_94236 scus94236 8002A240 220
#if defined(VERSION_SCUS_94236)
// USA version of PAL cameraClampAroundTarget.c (src/cameraClampAroundTarget.c): o->w30 - 0x5c)=>o->w30 - 0x54); o->w30 - 0x5c;=>o->w30 - 0x54;; o->w30 - 0x90)=>o->w30 - 0x88); o->w30 - 0x90;=>o->w30 - 0x88;.
#include "gamestate.h"
typedef struct { unsigned char b0; char p[0x15]; short w16; } S;
typedef struct { short w0; short w2; } P;
typedef struct { char p0[0x30]; short w30; short w32; P *d34; } O;
extern S PLAYER;
extern P *PLAYER_H;

void cameraClampAroundTarget(O *o)
{
    S *s = &PLAYER;
    char pad;
    if (s->b0 < 4 || s->b0 == 7) {
        unsigned short x = o->d34->w2;
        short t;
        t = x - 0x90;
        if (PLAYER_H->w2 < t) {
            PLAYER_H->w2 = t;
        } else {
            t = x + 0x90;
            if (t < PLAYER_H->w2)
                PLAYER_H->w2 = t;
        }
        if ((*(int*)&GAME.selectedArea) == 0x2000E) {
            if (s->w16 < o->w30 - 0x54)
                s->w16 = o->w30 - 0x54;
        } else {
            if (s->w16 < o->w30 - 0x88)
                s->w16 = o->w30 - 0x88;
        }
    }
}
#endif
