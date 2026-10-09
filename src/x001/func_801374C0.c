// FUNC 801374c0 220 X001
// MATCHING 801374c0 220
#include "TOBJ.H"
extern TObj D_800A6038;
extern void *D_8013E568;
extern int D_1F8002D4[];
extern TObj *FUN_800183b8(void);
extern void AnimLoadDuration(TObj *);
void func_801374C0(unsigned char sub, short x, short y, short z)
{
    TObj *o = FUN_800183b8();
    unsigned short t;
    if (o != 0) {
        o->active = 1;
        o->type = 0x17;
        t = D_800A6038.animFrame;
        o->a.raw = x << 16;
        o->y.raw = y << 16;
        o->b.raw = z << 16;
        o->subtype = sub;
        o->w1e = 10;
        o->b0d = 0;
        o->animFrame = t & 1;
        o->d3c = D_1F8002D4[0];
        o->anim = D_8013E568;
        o->b0a = 2;
        AnimLoadDuration(o);
        o->b04 = 2;
        o->step = 3;
        o->state = 0;
    }
}
