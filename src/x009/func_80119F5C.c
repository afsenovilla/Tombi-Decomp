// FUNC 80119f5c 1356 X009
// MATCHING 80119f5c 1356
#include "TOBJ.H"

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
extern unsigned char D_800A6038[];
extern int D_1F8000EC[], D_1F8000F0[], D_1F8000F4[];
extern short D_1F800256;
extern TObj **D_1F80026C;
extern TObj *D_1F8001D4;
extern unsigned char D_8009C975, D_8009C93C;
extern short D_8009CD94, D_8009CD96, D_8009CDA0;
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
            n = D_1F800256;
            l = D_1F80026C;
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
            D_8009C975 = 3;
            D_8009CD94 = 9;
            D_8009C93C = 0;
            D_8009CD96 = 0;
            D_8009CDA0 = 2;
        }
        break;
    case 3:
        if (x->w0 < 0x77) {
            ADVANCE(o, x);
        }
        if (D_8009C975 == 1) {
            D_1F8001D4->w4c = 7;
            D_1F8001D4->w4e = 0;
        }
        break;
    }
    D_800A6038[0] = 5;
    D_1F8000EC[0] = o->a.raw;
    D_1F8000F0[0] = o->y.raw;
    D_1F8000F4[0] = o->b.raw;
    o->d8c = turn(x->w6, o->d8c);
    o->d88 = turn(x->w4, o->d88);
    TURN2(c->d24, o->d88);
    TURN2(c->d20, o->d8c);
}
