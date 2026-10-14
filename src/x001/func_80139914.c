// FUNC 80139914 364 X001
// MATCHING 80139914 364
#include "gcd.h"
/* Box stores as inline setbox(o, 8, 0x10, 8, 0x10) (8 loads in the dispatch delay slot); D_1F8002F0 and ((unsigned char*)&GCD.bcdbf)
   as [0] arrays keep their loads after the struct stores. */
#include "TOBJ.H"

extern void *D_8013FA90[];
extern int D_1F8002F0[];
extern void AnimLoadDuration(TObj *);
extern int FUN_800202b4(TObj *);
extern void freeObjectLayer2(TObj *);
extern void func_8013946C(TObj *);
extern void func_80139770(TObj *);

static __inline__ void setbox(TObj *o, short a, short b, short c, short d)
{
    o->box0 = a;
    o->box1 = b;
    o->box2 = c;
    o->box3 = d;
}

void func_80139914(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->active = 2;
        *(signed char *)&o->b0f = -6;
        setbox(o, 8, 0x10, 8, 0x10);
        o->w1e = 5;
        o->w98 = o->animFrame;
        o->b0d = 0;
        o->d3c = D_1F8002F0[0];
        o->anim = D_8013FA90[o->wac = o->b0c];
        o->b6b = 0;
        if (((unsigned char*)&GCD.bcdbf)[0] != 0xff) break;
        if (o->b0c) {
            o->b04 = 3;
            break;
        }
        o->wac = 10;
        o->anim = D_8013FA90[10];
        AnimLoadDuration(o);
        o->b6b = 1;
        break;
    case 1:
        FUN_800202b4(o);
        if (o->b6b == 0)
            func_8013946C(o);
        else
            func_80139770(o);
        break;
    case 2:
        break;
    case 3:
        freeObjectLayer2(o);
        break;
    }
}
