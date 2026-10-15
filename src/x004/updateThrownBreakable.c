// FUNC 8011dff8 348 X004
// MATCHING 8011dff8 348
#include "spad.h"
#include "gv2.h"
#include "TOBJ.H"
extern void *D_8013B104[];
extern void FUN_8001fe6c(TObj *);
extern int FUN_800202b4(TObj *);
extern void thrownBreakableFly(TObj *);
extern void FUN_8001e4f0(int);
extern void spawnPoolType05Ring(TObj *, short, short, short);
extern void freeObjectLayer1(TObj *);

void updateThrownBreakable(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->box0 = 0xa;
        o->box1 = 0x14;
        o->box2 = 0x10;
        o->box3 = 0x20;
        o->d3c = ((int*)&SPAD.i2d4)[0];
        o->b0a = 2;
        o->active = 2;
        o->w1e = 0xa;
        o->b0d = 0;
        o->d8c = 0;
        o->ba5 = 1;
        o->anim = D_8013B104[0];
        FUN_8001fe6c(o);
        break;
    case 1:
        if (FUN_800202b4(o)) thrownBreakableFly(o);
        break;
    case 2:
        if (FUN_800202b4(o)) {
            FUN_8001e4f0(0x34);
            spawnPoolType05Ring(o, o->a.p.whole, o->y.p.whole, o->b.p.whole);
            { unsigned char *c = &GV2.bae; *c |= 1 << o->subtype; }
            o->b04 = 3;
        }
        break;
    case 3:
        freeObjectLayer1(o);
        break;
    }
}
