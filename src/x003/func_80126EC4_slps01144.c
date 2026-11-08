// VERSION SLPS_01144 x03 80126EC4 384
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "tobj.h"
extern unsigned char D_800716F4[];
extern unsigned char D_80134EAC[];
extern void *D_80136340;
extern void func_80020DE4();
extern void func_80020A00();

void func_80126EC4_slps01144(TObj *o)
{
    unsigned char *t;
    switch (o->state) {
    case 0:
        o->b0b = 1;
        o->b0f = 4;
        o->velV = -1024;
        o->movetab = D_800716F4;
        o->wac = 10;
        *(short *)&o->animFrame &= 1;
        { register int k asm("$3") = 2; o->active = k; o->b0a = k; }
        o->d8c = 0;
        o->d88 = 0;
        o->d84 = 0;
        o->state++;
        o->anim = D_80136340;
        t = D_80134EAC + o->wac * 4;
        o->box0 = *t++;
        o->box1 = *t++;
        o->box2 = *t;
        o->box3 = t[1];
        func_80020DE4(o);
        break;
    case 1:
        func_80020A00(o, (unsigned short)(1 - o->animFrame));
        o->velV += 64;
        if (o->velV > 1024) o->velV = 1024;
        o->y.raw += o->velV << 8;
        break;
    }
    if (o->animFrame & 1)
        o->d8c = (o->d8c + 20) & 0xff;
    else
        o->d8c = (o->d8c - 20) & 0xff;
}
#endif
