// FUNC 80116234 156 X013
// MATCHING 80116234 156
// VERSION SLPS_01144 x13 80115530 156
#if defined(VERSION_SLPS_01144)
#include "include_asm.h"
INCLUDE_ASM_V("x13/nonmatchings/x13", func_80115530_slps01144);
#else
#include "TOBJ.H"
typedef struct {
    char pad[0x2c];
    short xmin, xmax, ymin, ymax;
    Fix16 *pos;
} Cam;
extern TObj PLAYER;
extern Fix16 D_1F8000F0;

void cameraFollowPlayerClamped(Cam *c)
{
    TObj *pl = &PLAYER;
    Fix16 *q;

    c->pos->raw = pl->h->raw;
    if (c->xmax < c->pos->p.whole) {
        c->pos->raw = c->xmax << 16;
    } else if (c->pos->p.whole < c->xmin) {
        c->pos->raw = c->xmin << 16;
    }
    D_1F8000F0.raw = pl->y.raw;
    if (D_1F8000F0.p.whole > c->ymax) {
        D_1F8000F0.raw = c->ymax << 16;
    } else if (D_1F8000F0.p.whole < c->ymin) {
        D_1F8000F0.raw = c->ymin << 16;
    }
}
#endif
