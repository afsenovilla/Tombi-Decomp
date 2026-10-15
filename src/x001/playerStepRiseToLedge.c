// FUNC 80121d6c 728 X001
// MATCHING 80121d6c 728
// VERSION SLPM_80165 x00 800F93A0 848
#if defined(VERSION_SLPM_80165)
// Demo version of PAL x001 80121D6C (func_80121D6C.c): the player animation change is inline (PAL calls
// PlayerSetAnimIfChanged); +0xa0 is not cleared.
#include "spad.h"
#include "gvars.h"
#include "TOBJ.H"
#include "raw7.h"

extern TObj *D_8009C330;
extern void func_800EDA04(TObj *);
extern void func_80020554(TObj *);
#define FUN_800eeb5c(o, n) {                                          \
    TObj *p_ = D_8009C330;                                            \
    p_->animTimer = (n);                                              \
    if (p_->animFrame != (n)) {                                       \
        func_800EDA04(o);                                             \
        func_80020554(o);                                             \
        D_8009C330->animFrame = D_8009C330->animTimer;                \
    }                                                                 \
}
extern int FUN_8003facc(TObj *);
extern void FUN_8001e560(int, int);
extern int probeFlaggedSegmentA(short, short, short);
extern int AnimAdvance(TObj *);

void func_80121D6C(TObj *o)
{
    short y, z;
    short x;
    short w;

    switch (o->state) {
    case 0:
        FUN_800eeb5c(o, 0x1b);
        o->wb6 = 0;
        (*(short*)&D_8009C330->type) = 0;
        (*(unsigned char*)((char*)&o->da0 + 2)) = 3;
        o->wb0 = 0;
        o->wb2 = 0;
        o->velH = 0;
        o->velV = 0;
        o->d8c = 0;
        (*(unsigned char*)&o->waa) = 1;
        o->timer = 0xf;
        ((unsigned char*)&GVARS.b42)[0] = 1;
        ((unsigned char*)&GVARS.b3f)[0] = 1;
        o->state++;
    case 1:
        AnimAdvance(o);
        if (--o->timer == 0) {
            o->timer = 8;
            FUN_800eeb5c(o, 0x17);
            o->state++;
        }
        break;
    case 2:
        if (!FUN_8003facc(o)) o->y.p.whole--;
        AnimAdvance(o);
        if (!(SPAD.s1f8 & 0xf)) FUN_8001e560(0x1d, 0);
        y = o->y.p.whole - 8;
        w = o->h->p.whole + 8;
        z = o->d->p.whole;
        if ((probeFlaggedSegmentA(o->h->p.whole - 8, y, z) & 2) || (probeFlaggedSegmentA(w, y, z) & 2)) {
            o->b04 = 1;
            o->step = 0x16;
            o->state = 0;
        }
        break;
    case 3:
        AnimAdvance(o);
        o->y.raw += o->velY << 8;
        o->velY += 0x10;
        y = o->y.p.whole - 8;
        x = o->h->p.whole;
        w = x + 8;
        z = (short)(o->d->p.whole / 90) * 90;
        if ((probeFlaggedSegmentA(x - 8, y, z) & 2) || (probeFlaggedSegmentA(w, y, z) & 2)) {
            ((unsigned char*)&GVARS.b42)[0] = 0;
            o->b04 = 1;
            o->step = 0x16;
            o->state = 0;
        }
        break;
    }
}
#else
#include "spad.h"
#include "gvars.h"
#include "TOBJ.H"
#include "raw7.h"

extern TObj *D_8009C330;
extern void FUN_800eeb5c(TObj *, int);
extern int FUN_8003facc(TObj *);
extern void FUN_8001e560(int, int);
extern int probeFlaggedSegmentA(short, short, short);
extern int AnimAdvance(TObj *);

void playerStepRiseToLedge(TObj *o)
{
    short y, z;
    short x;
    short w;

    switch (o->state) {
    case 0:
        (*(unsigned char*)&o->da0) = 0;
        FUN_800eeb5c(o, 0x1b);
        o->wb6 = 0;
        (*(short*)&D_8009C330->type) = 0;
        (*(unsigned char*)((char*)&o->da0 + 2)) = 3;
        o->wb0 = 0;
        o->wb2 = 0;
        o->velH = 0;
        o->velV = 0;
        o->d8c = 0;
        (*(unsigned char*)&o->waa) = 1;
        o->timer = 0xf;
        ((unsigned char*)&GVARS.b42)[0] = 1;
        ((unsigned char*)&GVARS.b3f)[0] = 1;
        o->state++;
    case 1:
        AnimAdvance(o);
        if (--o->timer == 0) {
            o->timer = 8;
            FUN_800eeb5c(o, 0x17);
            o->state++;
        }
        break;
    case 2:
        if (!FUN_8003facc(o)) o->y.p.whole--;
        AnimAdvance(o);
        if (!(SPAD.s1f8 & 0xf)) FUN_8001e560(0x1d, 0);
        y = o->y.p.whole - 8;
        w = o->h->p.whole + 8;
        z = o->d->p.whole;
        if ((probeFlaggedSegmentA(o->h->p.whole - 8, y, z) & 2) || (probeFlaggedSegmentA(w, y, z) & 2)) {
            o->b04 = 1;
            o->step = 0x16;
            o->state = 0;
        }
        break;
    case 3:
        AnimAdvance(o);
        o->y.raw += o->velY << 8;
        o->velY += 0x10;
        y = o->y.p.whole - 8;
        x = o->h->p.whole;
        w = x + 8;
        z = (short)(o->d->p.whole / 90) * 90;
        if ((probeFlaggedSegmentA(x - 8, y, z) & 2) || (probeFlaggedSegmentA(w, y, z) & 2)) {
            ((unsigned char*)&GVARS.b42)[0] = 0;
            o->b04 = 1;
            o->step = 0x16;
            o->state = 0;
        }
        break;
    }
}
#endif
