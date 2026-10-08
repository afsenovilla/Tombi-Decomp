// FUNC 800f8d2c 2076 X002
// MATCHING 800f8d2c 2076
/* Debt: volatile animFrame read + volatile d88 store in case 2 pin the lhu above "sw a0,0x88" (scheduler). */
/* Tried (debt2): raw S32(o,0x84)=0 after a temp read of animFrame breaks the CSE (reload ok, no volatile read needed), but sched2 still puts "sw a0,0x88" above the lhu (score 2) for every statement order/var split/pointer copy; only the volatile pair pins it. */
#include "TOBJ.H"
#include "raw7.h"
typedef struct {
    unsigned char b00, b01;
    unsigned short w02;
    char p04[4];
    unsigned char b08, b09;
    char p0a[2];
    unsigned short w0c;
    short w0e;
    char p10[0x28 - 0x10];
    unsigned short w28, w2a, w2c, w2e;
} P;
extern P *DAT_8009c330;
extern TObj *DAT_8009f0ec;
extern TObj *DAT_800a611c;
extern TObj *DAT_8009d2e8;
extern int DAT_8009c934;
extern unsigned short DAT_8009d670;
extern unsigned short DAT_1f8003c6;
extern unsigned char DAT_80115228[];
extern char D_80011108[];
extern void FUN_8001e5f4(int, int);
extern void FUN_8001fe94(TObj *, int);
extern int AnimAdvance(TObj *);
extern void FUN_800efc04(TObj *);
extern int FUN_8001fe0c(unsigned char, int);
extern void FUN_800f8b7c(TObj *, int, int);

#define BUTTONS(o) \
    if (*(volatile unsigned short *)&DAT_8009d670 & DAT_1f8003c6) { \
        DAT_8009c330->b09 = 0; \
        (o)->step = 0xb; \
        (o)->state = 0; \
        U8(o, 0xaa) = 0; \
        DAT_8009f0ec->b69 = 0; \
    } \
    if (*(volatile unsigned short *)&DAT_8009d670 & 0x10) { \
        DAT_8009c330->b09 = 0; \
        (o)->step = 0xb; \
        (o)->state = 0; \
        U8(o, 0xaa) = 0; \
        DAT_8009f0ec->b69 = 0; \
    }

static __inline__ void nextspd(TObj *o, P *pl)
{
    unsigned char *t = &DAT_80115228[--o->wb2 * 2];
    pl->w0e = t[0];
    pl->w02 = t[1];
}

void FUN_800f8d2c(TObj *o)
{
    unsigned char *t;
    int a;
    int v;
    short q;
    short d;

    switch (o->state) {
    case 0:
        if (U8(o, 0xac) >= 2) {
            DAT_8009d2e8 = DAT_800a611c;
            DAT_8009d2e8->b04 = 2;
            DAT_8009d2e8->step = 2;
            DAT_8009d2e8->state = 0;
        }
        U8(o, 0xac) = 0;
        DAT_8009c934 = 0;
        U8(o, 0xc7) = 1;
        o->b9d = 0;
        U8(o, 0xc6) = 0;
        U8(o, 0xe3) = 0;
        DAT_8009c330->b00 = 0;
        DAT_8009f0ec->b69 = 1;
        o->wb6 = -0x420;
        DAT_8009c330->w02 = 0x10;
        DAT_8009c330->w0e = 0;
        o->wb2 = 2;
        DAT_8009c330->b08 = 0;
        DAT_8009c330->b09 = 0;
        DAT_8009c330->w0c = 0;
        DAT_8009c330->w28 = 0xffff;
        DAT_8009c330->w2a = 0xffff;
        o->velH = 0;
        o->velV = 0;
        o->wb0 = 0;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->d30 = DAT_8009f0ec->h->p.whole + o->wb8;
        o->d34 = DAT_8009f0ec->y.p.whole + o->wba;
        FUN_8001e5f4(4, 0x7f);
        U8(o, 0xaa) = 1;
        o->state++;
    case 1:
        o->wb6 += DAT_8009c330->w0e;
        if ((unsigned short)o->wb6 < 0x800)
            DAT_8009c330->b08 = 1;
        if ((unsigned short)(o->wb6 - 0x800) < 0x800)
            DAT_8009c330->b08 = 0;
        if ((unsigned short)(o->wb6 + 0x7ff) < 0x800)
            DAT_8009c330->b08 = 0;
        if ((unsigned short)(o->wb6 + 0xfff) < 0x800)
            DAT_8009c330->b08 = 1;
        if (o->wb6 == 0) {
            P *pl = DAT_8009c330;
            if (pl->w0e > 0) {
                nextspd(o, pl);
            }
        }
        a = o->wb6 >> 4;
        DAT_8009c330->w2e = 0xffff;
        o->anim = D_80011108;
        FUN_8001fe94(o, 0);
        DAT_8009c330->b01 = 0x10;
        FUN_800f8b7c(o, q, a);
        if (o->wb2 < 2)
            o->state++;
        break;
    case 2:
        o->wb6 += DAT_8009c330->w0e;
        if ((unsigned short)o->wb6 < 0x800)
            DAT_8009c330->b08 = 1;
        if ((unsigned short)(o->wb6 - 0x800) < 0x800)
            DAT_8009c330->b08 = 0;
        if ((unsigned short)(o->wb6 + 0x7ff) < 0x800)
            DAT_8009c330->b08 = 0;
        if ((unsigned short)(o->wb6 + 0xfff) < 0x800)
            DAT_8009c330->b08 = 1;
        if (o->wb6 == 0) {
            P *pl = DAT_8009c330;
            if (pl->w0e > 0) {
                nextspd(o, pl);
            } else {
                pl->b09 = 1;
            }
        }
        DAT_8009c330->w2c = 7;
        if (DAT_8009c330->w2e != 7) {
            DAT_8009c330->w2c = 7;
            FUN_800efc04(o);
            FUN_8001fe94(o, 0);
            DAT_8009c330->w2e = DAT_8009c330->w2c;
        }
        AnimAdvance(o);
        {
            int b = o->wb6 >> 4;
            q = (unsigned)b >> 2 & 0x3f;
            FUN_800f8b7c(o, q, b);
        }
        if (o->wb2 == 0) {
            o->d8c = 0;
            o->state++;
        }
        if (DAT_8009c330->b09 == 0)
            break;
        BUTTONS(o);
        if (*(volatile unsigned short *)&DAT_8009d670 & 0x40) {
            v = 0x10;
            DAT_8009c330->b09 = 0;
            U8(o, 0xac) = 1;
            o->b9e = 0;
            o->wb2 = 0;
            o->velH = 0;
            o->velV = 0;
            o->d30 = 0;
            o->d34 = 0;
            o->velX = 0;
            o->velY = 0;
            o->timer = 10;
            o->d84 = 0;
            if (o->animFrame & 1)
                v = 0xf0;
            {
                int w; int f;
                f = *(volatile unsigned short *)&o->animFrame;
                *(volatile int *)&o->d88 = v;
                o->d8c = 0;
                v = (int)o->h;
                w = ((Fix16 *)v)->p.whole;
                if (f & 1)
                    ((Fix16 *)v)->p.whole = w + 10;
                else
                    ((Fix16 *)v)->p.whole = w - 10;
            }
            o->step = 2;
            o->state = 3;
            o->y.p.whole += 4;
            U8(o, 0xaa) = 0;
            DAT_8009f0ec->b69 = 0;
        }
        break;
    case 3:
        o->d30 = DAT_8009f0ec->h->p.whole + o->wb8;
        o->d34 = DAT_8009f0ec->y.p.whole + o->wba;
        o->h->p.whole = o->d30;
        if (o->animFrame & 1)
            o->y.p.whole = FUN_8001fe0c((DAT_8009f0ec->d8c >> 4) + 0x70, DAT_8009f0ec->box0) + o->d34 + 0x10;
        else
            o->y.p.whole = FUN_8001fe0c((DAT_8009f0ec->d8c >> 4) + 0x10, DAT_8009f0ec->box0) + o->d34 + 0x10;
        AnimAdvance(o);
        BUTTONS(o);
        if (*(volatile unsigned short *)&DAT_8009d670 & 0x40) {
            v = 0x10;
            DAT_8009c330->b09 = 0;
            U8(o, 0xac) = 1;
            o->timer = 10;
            o->b9e = 0;
            o->wb2 = 0;
            o->velH = 0;
            o->velV = 0;
            o->d30 = 0;
            o->d34 = 0;
            o->velX = 0;
            o->velY = 0;
            o->d84 = 0;
            o->y.p.whole += 8;
            if (o->animFrame & 1)
                v = 0xf0;
            o->step = 2;
            o->state = 3;
            U8(o, 0xaa) = 0;
            o->d88 = v;
            o->d8c = 0;
            DAT_8009f0ec->b69 = 0;
        }
        break;
    }
}
