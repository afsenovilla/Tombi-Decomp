// JPDEMO slpm80165 8004B50C 508
// Demo version of PAL x001 801240A8 (func_801240A8.c):attached object at +0xd0 (PAL +0xe4), no damage on touch,
// carried hit only knocks the enemy (no AP).
#include "spad.h"
#include "TOBJ.H"

typedef struct { TObj o; char pad[0x24]; TObj *e4; } TObjX;
extern short func_800435E0(TObj *, TObj *);
extern void decreaseObjectTimer(TObj *, int);
extern void FUN_8001e4f0(int);
extern void awardAPByArea(int, int, int, int);

void func_801240A8(TObj *o, TObj *e)
{
    short r;

    r = func_800435E0(o, e);
    if (r == -1) return;
    switch (*(unsigned char *)&o->wac) {
    case 1:
        if (r < 3) {
            if (e->subtype == 8) {
                if ((*(int *)&e->b04 & 0xffffff) == 0x80301 && e->substep < 4) {
                    e->step = 3;
                    e->state = 0;
                    goto common;
                }
                if (e->b0c == 0) (*(short *)e->d94)--;
                e->subtype = 0;
                e->step = 1;
                e->state = 0;
                e->b69 = 0;
                goto common;
            }
            e->step = 1;
            e->state = 0;
            e->b69 = 0;
        common:
            e->active = 4;
            e->b04 = 2;
            e->animFrame = o->animFrame & 1;
            *(TObj **)((char *)o + 0xd0) = e;
            *(unsigned char *)&o->wac = 2;
            break;
        }
    case 0:
    case 3:
        if (e->active & 2) break;
        e->b69 = 8;
        if (SPAD.b1a4) break;
        if (o->active & 2) break;
        o->active = 2;
        r = e->h->p.whole > o->h->p.whole;
        o->b04 = 2;
        o->step = 0;
        o->state = 0;
        o->animFrame = r;
        break;
    case 2:
        if (e->active == 3) break;
        e->active = 3;
        r = o->h->p.whole > e->h->p.whole;
        e->b04 = 2;
        e->step = 0;
        e->state = 0;
        e->b69 = 0;
        e->w7a = r;
        break;
    }
}
