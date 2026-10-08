/* score 10: two leftovers. (1) case 0 of the func_80121540 switch: game loads D_8009C330 into v0 and the
   constant 1 into v1 (ours swapped: local-alloc gives the const pseudo v0); tried p/q copies, char*, int one, s=switch value.
   (2) tail block: game schedules lw o->h after sb b9c/lhu animFrame, ours before sb b9c (sched2 order); tried
   statement perms, U8 raw stores, f=animFrame first, scalar/[0] D_8009D2B0. */
// FUNC 8011e688 1600 X009
#include "TOBJ.H"
#include "raw7.h"
extern TObj *DAT_8009c330;
extern TObj *DAT_8009f0ec;
extern unsigned char DAT_801152e8[];
extern volatile unsigned short DAT_8009d670[];
extern unsigned short D_1F8001F8, D_1F8001FC, D_1F8003C6, D_1F8003C4;
extern unsigned char D_8009D2B0[];
extern int D_8009C984;
extern void FUN_800eeb5c(TObj *, int);
extern void FUN_8001e560(int, int);
extern void FUN_8001e5f4(int, int);
extern short FUN_8003fd78(TObj *, int, int);
extern void FUN_800ee428(TObj *);
extern int func_801214D0(TObj *, short, short, int);
extern short func_801215C0(TObj *, short, short, int);
extern short func_80121540(TObj *, short, short, int);

void func_8011E688(TObj *o)
{
    TObj *p;
    short x, s;
    int v;
    short yy;
    unsigned short dx, dy;
    volatile unsigned short *k;

    o->h->p.whole += DAT_8009f0ec->velX;
    o->y.p.whole += DAT_8009f0ec->velY;
    switch (o->state) {
    case 0:
        FUN_800eeb5c(o, 10);
        o->d8c = 0;
        o->wb6 = 0;
        U16(DAT_8009c330, 2) = 0;
        o->wb2 = 0;
        o->velH = 0;
        o->velV = 0;
        U8(o, 0xaa) = 1;
        o->state++;
    case 1:
        k = DAT_8009d670;
        s = 0;
        if (!(*k & 0x50)) {
            FUN_800eeb5c(o, 10);
        } else if (*k & 0x10) {
            FUN_800eeb5c(o, 0xb);
            v = o->h->p.whole;
            if (o->animFrame & 1)
                x = v - 8;
            else
                x = v + 8;
            yy = o->y.p.whole;
            p = DAT_8009f0ec;
            dx = x - (p->h->p.whole - p->box0);
            dy = p->y.p.whole + (p->box3 - p->box2);
            if ((unsigned short)dx < p->box1) {
                int c = -0x15;
                s += func_801214D0(o, dx, yy + c - dy, p->subtype);
            }
            p = DAT_8009f0ec;
            dx = x - (p->h->p.whole - p->box0);
            dy = p->y.p.whole + (p->box3 - p->box2);
            if ((unsigned short)dx < p->box1) {
                int c = -0x15;
                s += func_801214D0(o, dx, o->y.p.whole + c - dy, p->subtype);
            }
            if (s == 0)
                o->y.raw += -0x18000;
            if ((D_1F8001F8 & 0xf) == 0)
                FUN_8001e560(0x1d, 0);
        } else if (*k & 0x40) {
            FUN_800eeb5c(o, 0xc);
            if ((D_1F8001F8 & 0xf) == 0)
                FUN_8001e560(0x1d, 0);
            o->y.raw += 0x28000;
            if (o->b69) {
                o->d8c = DAT_801152e8[o->wb0];
                U8(o, 0xaa) = 0;
                o->b9e = 0;
                o->step = 0;
                o->state = 0;
                break;
            }
            v = o->h->p.whole;
            if (o->animFrame & 1)
                x = v + 8;
            else
                x = v - 8;
            yy = o->y.p.whole + 0x10;
            p = DAT_8009f0ec;
            dx = x - (p->h->p.whole - p->box0);
            dy = yy - (p->y.p.whole + (p->box3 - p->box2));
            if ((unsigned short)dx < p->box1) {
                if (o->b69 || func_801215C0(o, dx, dy, p->subtype)) {
                    o->d8c = DAT_801152e8[o->wb0];
                    U8(o, 0xaa) = 0;
                    o->b9e = 0;
                    o->step = 0;
                    o->state = 0;
                }
            }
        }
        {
            Fix16 *h = o->h;
            v = h->p.whole;
            if (o->animFrame & 1)
                h->p.whole = v - 2;
            else
                h->p.whole = v + 2;
        }
        v = o->h->p.whole;
        if (o->animFrame & 1)
            x = v - 8;
        else
            x = v + 8;
        yy = o->y.p.whole - 0x10;
        p = DAT_8009f0ec;
        dx = x - (p->h->p.whole - p->box0);
        dy = yy - (p->y.p.whole + (p->box3 - p->box2));
        if ((unsigned short)dx < p->box1) {
            s = func_80121540(o, dx, dy, p->subtype);
            switch (s) {
            case 0:
                U8(DAT_8009c330, 8) = 1;
                {
                    Fix16 *h = o->h;
                    short n;
                    v = h->p.whole;
                    if (o->animFrame & 1)
                        n = v + 4;
                    else
                        n = v - 4;
                    h->p.whole = n;
                }
                o->velX = 0;
                o->velY = 0;
                o->wb2 = 0;
                o->velH = 0;
                o->velV = 0;
                U8(o, 0xac) = 1;
                FUN_800ee428(o);
                o->step = 2;
                o->state = 3;
                U8(o, 0xaa) = 0;
                break;
            case 2:
                o->b9e = 5;
                o->step = 0x26;
                o->state = 0;
                break;
            }
        }
        break;
    }
    if (D_1F8001FC & D_1F8003C6) {
        D_8009D2B0[0] = 0;
        o->b9c = 1;
        o->b9e = 0;
        U8(o, 0xaa) = 0;
        {
            Fix16 *h = o->h;
            v = h->p.whole;
            if (o->animFrame & 1)
                h->p.whole = v + 0x10;
            else
                h->p.whole = v - 0x10;
        }
        if (D_8009C984 & 0x40) {
            if (DAT_8009d670[0] & D_1F8003C4)
                o->ba7 = 1;
        }
        o->step = 2;
        o->state = 0;
    }
    if (o->b69) {
        FUN_8001e5f4(0x1c, 0x7f);
        U8(DAT_8009c330, 8) = 0;
        o->ba7 = 0;
        U8(o, 0xac) = 0;
        o->b9c = 0;
        o->b9e = 0;
        U8(o, 0xaa) = 0;
        o->d8c = DAT_801152e8[o->wb0];
        o->step = 3;
        o->state = 1;
    } else if (FUN_8003fd78(o, 0, 0)) {
        FUN_8001e5f4(0x1c, 0x7f);
        U8(DAT_8009c330, 8) = 0;
        o->ba7 = 0;
        U8(o, 0xac) = 0;
        o->b9c = 0;
        o->b9e = 0;
        U8(o, 0xaa) = 0;
        o->d8c = DAT_801152e8[o->wb0];
        o->step = 0;
        o->state = 0;
    }
}
