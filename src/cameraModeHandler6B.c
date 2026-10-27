// FUNC 80027810 332 MAIN0
// MATCHING 80027810 332
// VERSION SLPS_01144 slps01144 80028620 332
#if defined(VERSION_SLPS_01144)
#include "include_asm.h"
INCLUDE_ASM_V("slps01144/nonmatchings/slps01144", func_80028620_slps01144);
#else
#include "gvars.h"
#include "TOBJ.H"
typedef struct {
    char p0[0x2c];
    short s2c, s2e, s30, s32;
    Fix16 *f34;
} S;
typedef struct { char p[0x14]; int d14; } G;
extern Fix16 *PLAYER_H[];
extern G PLAYER;
extern signed char GAME_PLAYER_MODE[];
extern Fix16 D_1F8000F0;

void cameraModeHandler6B(S *o)
{
    Fix16 *f;
    G *g;
    Fix16 *c;
    int v;

    if (GVARS.b39 != 0) return;
    o->f34->raw = PLAYER_H[0]->raw;
    f = o->f34;
    g = &PLAYER;
    if (o->s2e < f->p.whole) {
        f->raw = o->s2e << 16;
        if (GAME_PLAYER_MODE[0] != 3) {
            if (o->s2e + 150 < PLAYER_H[0]->p.whole) PLAYER_H[0]->p.whole = o->s2e + 150;
        }
    } else if (f->p.whole < o->s2c) {
        f->raw = o->s2c << 16;
        if (GAME_PLAYER_MODE[0] != 3) {
            if (PLAYER_H[0]->p.whole < o->s2c - 150) PLAYER_H[0]->p.whole = o->s2c - 150;
        }
    }
    if (GAME_PLAYER_MODE[0] < 3) {
        c = &D_1F8000F0;
        c->raw = g->d14;
        if (D_1F8000F0.p.whole > o->s32) {
            c->raw = o->s32 << 16;
        } else if (D_1F8000F0.p.whole < o->s30) {
            c->raw = o->s30 << 16;
        }
    }
}
#endif
