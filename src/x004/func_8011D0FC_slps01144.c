// VERSION SLPS_01144 x04 8011D0FC 484
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "spad.h"
#include "TOBJ.H"
extern unsigned char D_8009E4E8, D_8009E4E9;
extern unsigned char D_8009E941, D_8009E945, D_800B2F45, D_800B2F3D, D_800B2F41;
extern void *D_80130DA0;
extern void AnimLoadDuration(TObj *);
extern int ObjCullRegister(TObj *);
extern void AnimAdvance(TObj *);
extern int rcos(int);
extern void setEventComplete(int, int);
extern void printInfoMessage(int, int);
extern void freeObjectLayer5(TObj *);
static __inline__ void setbox(TObj *q, short a, short b, short c, short d)
{
    q->box0 = a;
    q->box1 = b;
    q->box2 = c;
    q->box3 = d;
}

void func_8011D0FC_slps01144(TObj *o)
{
    unsigned char *p;

    switch (o->b04) {
    case 0:
        p = &D_8009E4E8;
        if (*p != 0) {
            if (D_8009E4E9 != 0x7e) break;
            *p = 0;
            setbox(o, 8, 0x10, 8, 0x10);
            o->active = 1;
            o->b69 = 0;
            o->b0a = 0;
            o->w1e = 9;
            o->b0d = 1;
            o->w08 = 0x7a0e;
            o->anim = D_80130DA0;
            o->d3c = ((int*)&SPAD.i2d4)[0];
            o->d30 = o->a.p.whole;
            o->w22 = 0;
            AnimLoadDuration(o);
            o->b04++;
        } else {
            o->active = 2;
        }
        break;
    case 1:
        if (ObjCullRegister(o)) AnimAdvance(o);
        o->a.p.whole = o->d30 + (rcos(o->w22 & 0xfff) >> 7);
        if ((o->w22 & 0xfff) > 0x800) o->animFrame = 0;
        else o->animFrame = 1;
        o->w22 += 0x10;
        break;
    case 2:
        setEventComplete(0x77, 0);
        printInfoMessage(0x32, 2);
        D_8009E941 = 9;
        D_8009E945 = 0x3c;
        D_800B2F45 = 1;
        D_800B2F3D = 0;
        D_800B2F41 = 0x1e;
        o->b04++;
        break;
    case 3:
        freeObjectLayer5(o);
        break;
    }
}
#endif
