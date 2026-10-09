// FUNC 801244d0 888 X014
// MATCHING 801244d0 888
#include "TOBJ.H"

extern int D_1F8002DC[];
extern void *D_80129FF0;
extern unsigned char D_8009C942;
extern unsigned short D_8009C962;
extern short D_8007A3F0[], D_8007A5F0[];
extern short D_800A457C, D_800A457E, D_800A4580, D_800A4582;
extern void FUN_8001fe6c(TObj *);
extern int ObjCullRegister(TObj *);
extern void AnimAdvance(TObj *);
extern void func_8011709C(TObj *, int);
extern int FUN_8001f9e0(void);
extern short FUN_800411cc(TObj *, int, int);
extern void FUN_80018790(TObj *);

static __inline__ void setup(TObj *p)
{
    p->d3c = D_1F8002DC[0];
    p->wac = 0;
    p->anim = D_80129FF0;
    FUN_8001fe6c(p);
}

static __inline__ short out(TObj *o)
{
    if (o->h->p.whole < D_800A457C - 0xa0) return 1;
    if (D_800A457E + 0xa0 < o->h->p.whole) return 1;
    if (D_800A4582 + 0x80 < o->y.p.whole) return 1;
    return o->y.p.whole < D_800A4580 - 0x80;
}

void func_801244D0(TObj *o)
{
    unsigned char t = o->b04;
    int r;
    short k;
    int a, v, x, y;

    switch (t) {
    case 0:
        o->b04 = t + 1;
        o->box0 = 8;
        o->box1 = 0x10;
        o->box3 = 0x10;
        *(signed char *)&o->b0f = -9;
        o->b0a = 2;
        o->box2 = 8;
        o->d84 = 0;
        o->d88 = 0;
        o->w1e = 1;
        o->animFrame = 1;
        o->b0d = 0;
        setup(o);
        break;
    case 1:
        if (D_8009C942 == 1) {
            ObjCullRegister(o);
            break;
        }
        ObjCullRegister(o);
        switch (o->step) {
        case 0:
            o->velH = 0x80;
            o->velX = 8;
            o->step++;
            if (D_8009C962 != 7) func_8011709C(o, 1);
            o->timer = 6;
            break;
        case 1:
            if (--o->timer == -1) {
                o->timer = (FUN_8001f9e0() & 3) + 6;
                r = FUN_8001f9e0() & 3;
                k = r;
                if (r == 0) k = 2;
                if (D_8009C962 != 7) func_8011709C(o, k);
            }
            a = o->d8c;
            v = o->velH;
            x = v * D_8007A5F0[a];
            y = v * D_8007A3F0[a];
            o->h->raw += ((x << 4) >> 16) << 8;
            o->y.raw += ((y << 4) >> 16) << 8;
            o->velH += o->velX;
            if (o->velH > 0x300) o->velH = 0x300;
            if (o->velH > 0x140) o->velX = 0x10;
            if (FUN_800411cc(o, o->h->p.whole, o->y.p.whole)) {
                o->active = 2;
                o->b04 = 2;
                o->step = 0;
                o->state = 0;
            } else if (out(o)) {
                o->active = 2;
                o->b04 = 3;
            }
            break;
        }
        AnimAdvance(o);
        break;
    case 2:
        func_8011709C(o, 0);
        o->b04++;
        ObjCullRegister(o);
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
