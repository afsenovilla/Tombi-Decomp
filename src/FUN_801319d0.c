// FUNC 801319d0 1752 X000
// MATCHING 801319d0 1752
#include "TOBJ.H"
#include "raw7.h"
typedef struct { short x, y; } P;
extern int FUN_8002078c(P a, P b);
extern void FUN_8001fe6c(TObj *);
extern void FUN_8001fec0(TObj *);
extern void FUN_8001e4f0(int);
extern unsigned FUN_8001f9e0(void);
extern void FUN_8001fd48(TObj *);
extern void FUN_8004258c(void *, int);
extern int FUN_8001fddc(int, int);
extern int FUN_8001fdac(int, int);
extern short FUN_800411cc(TObj *, short, short);
extern Fix16 *DAT_800a6078;
extern unsigned short DAT_800a604e;
extern unsigned char DAT_800a6038[];
extern unsigned char DAT_800a603c[], DAT_800a603d[], DAT_800a603e[], DAT_800a603f;
extern short DAT_800a6066, DAT_800a60b6;
extern void *DAT_8013ad70[];
extern void *DAT_8013ad38[];
extern void *DAT_8013ad78[];
extern void *DAT_8013ad7c[];
extern short DAT_80139280[];
extern short *DAT_80139234[];

static __inline__ short hit(TObj *o)
{
    if (FUN_800411cc(o, o->h->p.whole + 0x20, o->y.p.whole + 0x18)) return 1;
    if (FUN_800411cc(o, o->h->p.whole - 0x20, o->y.p.whole + 0x18)) return 1;
    return 0;
}

void FUN_801319d0(TObj *o)
{
    P a, b;
    short *t;
    short d;
    unsigned short v;
    Fix16 *p;
    unsigned char *q;

    switch (o->substep) {
    case 0:
        if (o->h->p.whole >= DAT_800a6078->p.whole)
            o->animFrame = 1;
        else
            o->animFrame = 0;
        o->w22 = 0x38;
        o->wac = 0x10;
        U16(o, 0xc8) = o->animFrame;
        o->anim = DAT_8013ad70[0];
        FUN_8001fe6c(o);
        if (U16(o, 0xca))
            FUN_8001e4f0(0x13);
        o->substep++;
        break;
    case 1:
        if (--o->w22 == 0) {
            o->velV = -0x400;
            o->w22 = 0x38;
            o->velH = 0;
            o->wac = 2;
            o->anim = DAT_8013ad38[0];
            FUN_8001fe6c(o);
            o->substep++;
        }
        break;
    case 2:
        if (o->velV >= 0) {
            o->w22 = 0x1e;
            o->substep++;
        }
        o->velV += 0x20;
        o->y.raw += o->velV << 8;
        break;
    case 3:
        if (--o->w22 == 0) {
            a.x = o->h->p.whole;
            a.y = o->y.p.whole;
            b.x = DAT_800a6078->p.whole;
            b.y = DAT_800a604e;
            U16(o, 0xcc) = FUN_8002078c(a, b) + 0x100;
            o->w22 = 0x78;
            o->wb6 = 0;
            o->b6a = 1;
            o->substep++;
        }
        break;
    case 4:
        a.x = o->h->p.whole;
        a.y = o->y.p.whole;
        b.x = DAT_800a6078->p.whole;
        b.y = DAT_800a604e;
        d = FUN_8002078c(a, b) + 0x100;
        U16(o, 0xcc) = d;
        o->h->raw += (short)FUN_8001fddc(d & 0xf8, o->wb6) << 8;
        o->y.raw += (short)FUN_8001fdac(U16(o, 0xcc) & 0xf8, o->wb6) << 8;
        if ((unsigned short)o->wb6 < 0x400)
            o->wb6 += 0x20;
        if (o->b6a == 2) {
            o->wac = 0x12;
            o->anim = DAT_8013ad78[0];
            FUN_8001fe6c(o);
            o->y.p.whole -= 8;
            o->w22 = DAT_80139280[FUN_8001f9e0() & 0xf];
            FUN_8001e4f0(0x14);
            o->substep++;
        } else if (--o->w22 == 0) {
            o->b6a = 0;
            o->wac = 2;
            o->anim = DAT_8013ad38[0];
            FUN_8001fe6c(o);
            o->velV = -0x400;
            o->timer = 0x1e;
            o->substep = 0xb;
        } else if (hit(o)) {
            o->b6a = 0;
            o->wac = 2;
            o->anim = DAT_8013ad38[0];
            FUN_8001fe6c(o);
            o->velV = -0x400;
            o->timer = 0x1e;
            o->substep = 0xb;
        } else {
            t = DAT_80139234[o->subtype];
            if (o->h->p.whole < t[0] || t[1] < o->h->p.whole) {
                o->b6a = 0;
                o->wac = 2;
                o->anim = DAT_8013ad38[0];
                FUN_8001fe6c(o);
                o->velV = -0x400;
                o->timer = 0x1e;
                o->substep = 9;
            }
        }
        break;
    case 5:
        if (--o->w22 == 0) {
            o->velY = -0x100;
            o->w22 = 0x60;
            o->velX = 0;
            o->substep = 6;
        } else {
            p = DAT_800a6078;
            d = o->h->p.whole - p->p.whole;
            v = p->p.whole;
            if (d != 0) {
                if (d < 0)
                    p->p.whole = v - 1;
                else
                    p->p.whole = v + 1;
            }
        }
        break;
    case 6:
        FUN_8001fd48(o);
        q = DAT_800a6038;
        DAT_800a6078->p.whole = o->h->p.whole;
        DAT_800a604e = o->y.p.whole + 0x28;
        if (DAT_800a603f == 1) {
            o->b6a = 0;
            *q = 1;
            DAT_800a603c[0] = 1;
            DAT_800a603d[0] = 0;
            DAT_800a603e[0] = 0;
            o->wac = 0x13;
            o->anim = DAT_8013ad7c[0];
            FUN_8001fe6c(o);
            o->substep = 8;
        } else if (--o->w22 == 0) {
            o->b6a = 0;
            *q = 2;
            DAT_800a6066 = 2;
            DAT_800a60b6 = 0;
            DAT_800a603c[0] = 2;
            DAT_800a603d[0] = 0;
            DAT_800a603e[0] = 1;
            FUN_8004258c(q, 1);
            o->wac = 0x13;
            o->anim = DAT_8013ad7c[0];
            FUN_8001fe6c(o);
            o->substep = 8;
        }
        break;
    case 8:
        o->velV = -0x400;
        o->timer = 0x1e;
        o->substep++;
    case 9:
        o->y.raw += o->velV << 8;
        if (--o->timer == 0) {
            o->active = 1;
            o->timer = 0x1e;
            o->substep++;
        }
        break;
    case 10:
        if (--o->timer == 0) {
            o->state = 3;
            o->step = 0;
            o->substep = 0;
            o->wb4 = 1;
        }
        break;
    case 11:
        o->y.raw += o->velV << 8;
        if (--o->timer == 0) {
            o->timer = 0x1e;
            o->substep = 10;
        }
        break;
    }
    FUN_8001fec0(o);
}
