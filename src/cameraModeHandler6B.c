// FUNC 80027810 332 MAIN0
// MATCHING 80027810 332
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
extern Fix16 VIEW_CAMERA_Y_FIX;

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
        c = &VIEW_CAMERA_Y_FIX;
        c->raw = g->d14;
        if (VIEW_CAMERA_Y_FIX.p.whole > o->s32) {
            c->raw = o->s32 << 16;
        } else if (VIEW_CAMERA_Y_FIX.p.whole < o->s30) {
            c->raw = o->s30 << 16;
        }
    }
}
