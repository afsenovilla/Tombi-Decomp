// FUNC 80117864 392 X017
// MATCHING 80117864 392
#include "gcd.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
typedef struct { void **p; int x, y; } AT;
extern signed char D_8009D2B0;
extern AT D_80119990[];
extern void FUN_8001fe94(TObj *, int);
extern void FUN_8001fec0(TObj *);
extern void FUN_80026e0c(int, int);

void func_80117864(TObj *o)
{
    switch (o->state) {
    case 0:
        GVARS.b42 = 1;
        GVARS.b3f = 1;
        D_8009D2B0 = 2;
        PLAYER.obj.b04 = 5;
        PLAYER.obj.step = 0;
        PLAYER.obj.state = 0;
        o->anim = D_80119990[o->subtype].p[1];
        FUN_8001fe94(o, 0);
        o->animFrame = 1;
        o->timer = 60;
        o->state++;
        break;
    case 1:
        o->h->p.whole -= 2;
        FUN_8001fec0(o);
        if (--o->timer > 0) break;
        FUN_80026e0c(0x3c, 1);
        GCD.bcfd6 = 1;
        PLAYER.obj.b04 = 1;
        PLAYER.obj.step = 0x31;
        PLAYER.obj.state = 0;
        FUN_80026e0c(0x3c, 1);
        o->state++;
        break;
    case 2:
        o->h->p.whole -= 2;
        FUN_8001fec0(o);
        break;
    }
}
