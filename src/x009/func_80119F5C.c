// FUNC 80119f5c 1356 X009
// MATCHING 80119f5c 1356
#include "spad.h"
#include "gvars.h"
#include "TOBJ.H"
#include "gamestate.h"
#include "gametask.h"

typedef struct {
    short w0, w2;
    unsigned short w4, w6;
    unsigned int d8, dc;
} X80119F5C;

typedef struct {
    short x, y, z, pad;
} P80119F5C;

typedef struct {
    char p0[0x20];
    int d20, d24;
} C80119F5C;

extern C80119F5C D_800A4550;
extern P80119F5C D_8012AC88[];
extern unsigned char PLAYER[];
extern void func_80119D2C(TObj *);
extern void func_80119E4C(TObj *);

static __inline__ int turn(unsigned short tgt, int cur)
{
    int t = (tgt - cur) & 0xfff;
    short d = t;
    unsigned short u = t;
    int r;
    if (d == 0) return cur & 0xfff;
    if (u < 0x800) {
        if (d >= 0x20) r = cur + 0x10;
        else if (d >= 0x10) r = cur + 4;
        else r = cur + 1;
    } else {
        if (d < 0xfe1) r = cur - 0x10;
        else if (d < 0xff1) r = cur - 4;
        else r = cur - 1;
    }
    return r & 0xfff;
}

#define TURN2(var, tgt)                                  \
    {                                                    \
        int t = ((tgt) - var) & 0xfff;                   \
        short v = t;                                     \
        unsigned short u = t;                            \
        if (v != 0) {                                    \
            if (u < 0x800) {                             \
                if (v >= 0x20) var += 8;                 \
                else if (v >= 8) var += 2;               \
                else var += 1;                           \
            } else {                                     \
                if (v < 0xfe1) var -= 8;                 \
                else if (v < 0xff9) var -= 2;            \
                else var -= 1;                           \
            }                                            \
            var &= 0xfff;                                \
        }                                                \
        var &= 0xfff;                                    \
    }

#define ADVANCE(o, x)                                    \
    x->d8 += x->w2;                                      \
    if (x->dc < x->d8) {                                 \
        x->d8 -= x->dc;                                  \
        x->w0++;                                         \
        func_80119D2C(o);                                \
    }                                                    \
    func_80119E4C(o);

void func_80119F5C(TObj *o)
{
    C80119F5C *c = &D_800A4550;
    X80119F5C *x = (X80119F5C *)&o->wb4;
    int n;
    TObj **l;
    TObj *e;

    switch (o->step) {
    case 0:
        o->step++;
        x->w2 = 0x400;
        x->w0 = 2;
        x->d8 = 0;
        o->a.p.whole = D_8012AC88[2].x;

        o->y.p.whole = D_8012AC88[x->w0].y - 8;
        o->b.p.whole = D_8012AC88[x->w0].z;
        func_80119D2C(o);
        break;
    found:
        e->active = 6;
        e->b04 = 2;
        e->step = 0;
        e->state = 0;
        goto next;
    case 1:
        x->w2 += 0x20;
        if (x->w2 > 0xc00) x->w2 = 0xc00;
        ADVANCE(o, x);
        if (x->w0 >= 0x66) {
            n = SPAD.s256;
            l = (*(TObj ***)&SPAD.p26c);
            if (n != 0) {
                do {
                    e = *l++;
                    n--;
                    if (e->active & 3) goto found;
                } while (n);

            }
        next:
            o->step++;
        }
        break;
    case 2:
        x->w2 += 0x20;
        if (x->w2 > 0xc00) x->w2 = 0xc00;
        ADVANCE(o, x);
        if (x->w0 >= 0x6c) {
            o->step++;
            GAME.fadeScreenControl = 3;
            (*(short*)&GAME.nextArea) = 9;
            GVARS.b3c = 0;
            (*(short*)&GAME.nextSection) = 0;
            (*(short*)&GAME.nextSpawnPoint) = 2;
        }
        break;
    case 3:
        if (x->w0 < 0x77) {
            ADVANCE(o, x);
        }
        if (GAME.fadeScreenControl == 1) {
            TASK_C->mode = 7;
            TASK_C->step = 0;
        }
        break;
    }
    PLAYER[0] = 5;
    ((int*)&SPAD.iec)[0] = o->a.raw;
    ((int*)&SPAD.if0)[0] = o->y.raw;
    ((int*)&SPAD.if4)[0] = o->b.raw;
    o->d8c = turn(x->w6, o->d8c);
    o->d88 = turn(x->w4, o->d88);
    TURN2(c->d24, o->d88);
    TURN2(c->d20, o->d8c);
}
