// FUNC 8013675c 376 X001
// MATCHING 8013675c 376
#include "TOBJ.H"

extern unsigned short D_8009C960[], D_8009C962;
extern unsigned short D_8009D2C0;
extern int D_1F8002D0, D_1F8002E4;
extern void *D_8013DE08, *D_8013DD50;
extern TObj *FUN_800183b8(void);
extern void AnimLoadDuration(TObj *);

TObj *func_8013675C(short k, short d, short n, short x, short y, short z)
{
    TObj *o;

    if (n != 0) {
    if ((D_8009D2C0 >> (n - 1)) & 1) return 0;
    o = FUN_800183b8();
    if (o != 0) {
    o->active = 1;
    o->type = 0x16;
    o->a.raw = x << 16;
    o->y.raw = y << 16;
    o->b.raw = z << 16;
    o->animFrame = k & 1;
    o->subtype = (k >> 1) + 1;
    o->d38 = d;
    o->w1e = 1;
    o->b0c = n;
    o->d8c = 0;
    o->b0d = 0;
    o->b0a = 2;
    if (D_8009C960[0] == 1 && D_8009C962 < 2) {
        o->d3c = D_1F8002D0;
        o->anim = D_8013DE08;
    } else {
        o->d3c = D_1F8002E4;
        o->anim = D_8013DD50;
    }
    AnimLoadDuration(o);
    return o;
    }
    }
    return 0;
}
