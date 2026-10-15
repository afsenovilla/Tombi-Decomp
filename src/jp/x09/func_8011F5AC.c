// JP x09 8011F5AC 176
// Japanese version of PAL x009 801203CC (x009/func_801203CC.c): 228 -> 212.
#include "gcd.h"
#include "TOBJ.H"
typedef struct {
    TObj o;
    unsigned char pad[0xd4 - 0xc0];
    TObj *de4;
} TObjX;
extern short func_80043260(TObjX *, TObj *);
extern void spawnImpactEffect(int, int, int, int);

void func_801203CC(TObjX *o, TObj *e)
{
    short r = func_80043260(o, e);

    if (r != 0 && GCD.bcdca == 1 && r < 3 && *(unsigned char *)&o->o.wac == 1) {
        e->active = 2;
        e->b04 = 2;
        e->step = 0;
        e->state = 0;
        e->b69 = 0;
        e->b0f = o->o.b0f + 1;
        o->de4 = e;
        *(unsigned char *)&o->o.wac = 2;
        spawnImpactEffect(2, o->o.a.p.whole, o->o.y.p.whole, o->o.b.p.whole);
    }
}
