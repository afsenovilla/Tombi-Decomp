// FUNC 801206c0 1428 X014
// MATCHING 801206c0 1428
#include "TOBJ.H"

extern TObj D_800A6038;
extern unsigned char D_8009C942;
extern int DAT_1f8002dc[];
extern void *D_80129F9C[];
extern short D_800A457C, D_800A457E, D_800A4580, D_800A4582;
extern void FUN_8001fe6c(TObj *);
extern int FUN_8001fec0(TObj *);
extern int FUN_800202b4(TObj *);
extern short FUN_800411cc(TObj *, short, short);
extern void FUN_8002052c(TObj *, int);
extern void FUN_80018790(TObj *);

static __inline__ int touch(TObj *o, TObj *p)
{
    if ((unsigned short)(p->h->p.whole - o->h->p.whole + (o->box0 + p->box0)) > o->box1 + p->box1)
        return 0;
    return !((unsigned short)(p->y.p.whole - o->y.p.whole + (o->box2 + p->box2)) > p->box3 + o->box3 + 0x40);
}

static __inline__ int out(TObj *o)
{
    if (o->h->p.whole < D_800A457C - 0xa0)
        return 1;
    if (D_800A457E + 0xa0 < o->h->p.whole)
        return 1;
    if (D_800A4582 + 0x80 < o->y.p.whole)
        return 1;
    return o->y.p.whole < D_800A4580 - 0x80;
}

void func_801206C0(TObj *o)
{
    TObj *p = &D_800A6038;
    short d, v;
    int t;

    switch (o->b04) {
    case 0:
        o->b04++;
        o->box0 = 0x18;
        o->box1 = 0x30;
        o->box2 = 0x40;
        o->box3 = 0x70;
        *(signed char *)&o->b0f = -0x10;
        o->velX = 0x80;
        o->b0a = 0xa;
        o->w1e = 1;
        o->b0d = 0x80;
        o->b6a = 0;
        o->ba5 = 0;
        o->ba6 = 0;
        o->ba7 = 0;
        o->animFrame = 0;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->d3c = DAT_1f8002dc[0];
        o->anim = D_80129F9C[o->subtype];
        FUN_8001fe6c(o);
        o->timer = 20;
        if (*(unsigned short *)&o->wb4)
            o->velH = -0x200;
        else
            o->velH = 0x200;
        o->velV = 0;
        o->w22 = 0;
        break;
    case 1:
        if (D_8009C942 == 1) {
            FUN_800202b4(o);
            break;
        }
        FUN_800202b4(o);
        FUN_8001fec0(o);
        if (o->step != 0)
            break;
        if (o->timer == 0) {
            o->ba5 += 2;
            if (o->ba5 >= 0xc0)
                *(signed char *)&o->ba5 = -0x40;
            v = o->velV + 8;
            o->velV = v;
            t = o->ba5;
            *((unsigned char *)o + 0xa5) = t;
            *((unsigned char *)o + 0xa6) = t;
            if (v > 0x100)
                o->velV = 0x100;
            if (o->velH < 0) {
                o->velH += 8;
                if (o->velH >= -0x100)
                    o->velH = -0x100;
            } else {
                o->velH -= 8;
                if (o->velH <= 0x100)
                    o->velH = 0x100;
            }
        } else {
            o->timer--;
        }
        o->h->raw += o->velH << 8;
        o->y.raw -= o->velV << 8;
        if (FUN_800411cc(o, o->h->p.whole, o->y.p.whole)) {
            o->active = 2;
            o->b04 = 2;
            o->step = 0;
            o->state = 0;
        } else {
            if (out(o)) {
                o->active = 2;
                o->b04 = 3;
            }
        }
        if (o->b6a == 0)
            break;
        p->h->raw += o->velH << 8;
        p->y.raw -= o->velV << 8;
        p->active = 6;
        if (o->subtype)
            p->d8c += 0x14;
        else
            p->d8c -= 0x14;
        p->d8c = (unsigned char)p->d8c;
        if (o->h->p.whole != p->h->p.whole) {
            if (p->h->p.whole < o->h->p.whole) {
                t = (short)(o->h->p.whole - p->h->p.whole);
                if (t >= 4)
                    d = 3;
                else
                    d = t;
            } else {
                t = (short)(o->h->p.whole - p->h->p.whole);
                d = (t < -3) ? -3 : t;
            }
        } else
            d = 0;
        p->h->p.whole += d;
        p->y.p.whole += 6;
        if (FUN_800411cc(p, p->h->p.whole, p->y.p.whole)) {
            p->y.p.whole -= 6;
            o->b04 = 2;
        }
        if (touch(o, p) && o->b04 == 1)
            break;
        p->active = 1;
        p->b04 = 1;
        p->step = 0x3e;
        p->state = 0;
        o->active = 2;
        o->b04 = 2;
        o->step = 0;
        o->state = 0;
        o->b6a = 0;
        break;
    case 2:
        if (D_8009C942 == 1) {
            FUN_800202b4(o);
            break;
        }
        FUN_800202b4(o);
        FUN_8001fec0(o);
        o->velX -= 4;
        if (o->velX < 0) {
            o->velX = 0;
            o->b04++;
        } else {
            FUN_8002052c(o, 0x50);
        }
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
