// FUNC 8012a740 376 X004
// MATCHING 8012a740 376
#include "gcd.h"
#include "player.h"
#include "TOBJ.H"
typedef struct { unsigned char b0, b1, b2, b3; unsigned short w; short idx; void **ptrs; } E;
extern E TOWN_HALL_WORKER_DEFS[];
extern int SPR_DATA[];
extern void FUN_8001fe6c(TObj *);

void initTownHallWorker(TObj *o)
{
    if (GCD.bcf2d == 0) {
        o->box0 = TOWN_HALL_WORKER_DEFS[o->subtype].b0;
        o->box1 = TOWN_HALL_WORKER_DEFS[o->subtype].b1;
        o->box2 = TOWN_HALL_WORKER_DEFS[o->subtype].b2;
        o->box3 = TOWN_HALL_WORKER_DEFS[o->subtype].b3;
        o->w1e = TOWN_HALL_WORKER_DEFS[o->subtype].w;
        o->d3c = SPR_DATA[TOWN_HALL_WORKER_DEFS[o->subtype].idx];
        o->anim = TOWN_HALL_WORKER_DEFS[o->subtype].ptrs[o->b6b];
        FUN_8001fe6c(o);
        o->b0a = 2;
        o->d8c = 0;
        o->b0d = 0;
        o->b68 = 0;
        o->category |= 0x80;
        o->b0f = PLAYER.obj.b0f + 10;
        o->b04++;
    }
}
