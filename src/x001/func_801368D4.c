// FUNC 801368d4 292 X001
// MATCHING 801368d4 292
#include "TOBJ.H"

extern unsigned short D_8009C960[], D_8009C962;
extern int D_1F8002D0[], D_1F8002E4[];
extern void *D_8013DE3C[], *D_8013DDE4[];
extern TObj *FUN_800183b8(void);
extern void AnimLoadDuration(TObj *);

void func_801368D4(short x, short y, short z)
{
    TObj *o = FUN_800183b8();

    if (o != 0) {
        o->active = 1;
        o->type = 0x16;
        o->a.raw = x << 16;
        o->y.raw = y << 16;
        o->b.raw = z << 16;
        o->subtype = 0;
        o->w1e = 1;
        o->d8c = 0;
        if (D_8009C960[0] == 1
 && D_8009C962 < 2) {
            o->d3c = D_1F8002D0[0];
            o->anim = D_8013DE3C[0];
        } else {
            o->d3c = D_1F8002E4[0];
            o->anim = D_8013DDE4[0];
        }
        o->b0d = 0;
        o->b0a = 2;
        AnimLoadDuration(o);
        o->b04 = 2;
        o->step = 3;
        o->state = 0;
        o->category |= 0x80;
    }
}
