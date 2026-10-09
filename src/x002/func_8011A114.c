// FUNC 8011a114 7316 X002
// MATCHING 8011a114 7316
/* Whole function: the csv splits it into 8011A114, 8011A448, 8011B170, 8011B330, 8011B47C and 8011B7D4. */
#include "TOBJ.H"

typedef struct { short v[6]; } V6;
typedef struct {
    unsigned char active, visible, type, subtype, b04, step, state, substep;
    short w08;
    short w0a;
    unsigned char pad0c[3];
    signed char b0f;
    int ang;
    unsigned char pad14[0x10];
    unsigned short w24;
} Ctl;

extern TObj D_800A6038;
extern TObj *D_8009F2D8, *D_8009F2DC, *D_8009F2F8, *D_8009F328;
extern unsigned char D_8009C93A, D_8009C942[], D_8009C93F[], D_8009D2B0, D_8009C975, D_8009C93C;
extern unsigned char D_8009CFEA, D_8009C93E[], D_8009CE15, D_8009CDBB, D_8009D0B2;
extern unsigned short D_1F8001F8;
extern short D_1F8001C6;
extern TObj *ObjAlloc(void);
extern int MulCos(short, short);
extern int MulNegSinScaled(short, short);
extern void FUN_8005a9a4(int, int);
extern void FUN_8005a8a8(int, int, int);
extern void FUN_80059d44(short);
extern TObj *FUN_8002dcc8(int, int, V6 *);
extern void addItemToInventory(int, int, int);
extern int AnimAdvance(void *);
extern void func_80116928(TObj *, int, int);
extern void func_80117CEC(TObj *, int, int);

#define Spawn(X, Y, Z) { \
    unsigned short x = X, y = Y, z = Z; \
    short t[16][2] = { \
        { 0, -6 }, { -6, 0 }, { 0, 6 }, { 6, 0 }, \
        { -4, -4 }, { -4, 4 }, { 4, 4 }, { 4, -4 }, \
        { 0, -3 }, { -3, 0 }, { 0, 3 }, { 3, 0 }, \
        { 0, -3 }, { -3, 0 }, { 0, 3 }, { 3, 0 }, \
    }; \
    TObj *p = ObjAlloc(); \
    if (p) { \
        p->active = 1; \
        p->type = 0x31; \
        p->subtype = 1; \
        p->a.p.whole = x; \
        p->y.p.whole = y; \
        p->b.p.whole = z; \
        p->h->p.whole += t[D_1F8001F8 & 7][0]; \
        p->y.p.whole += t[D_1F8001F8 & 7][1]; \
    } \
}

#define RING(o) \
    o->ang = (o->ang + 4) & 0xff; \
    Spawn(MulCos(0xff - o->ang, 0x40) + 0xa0, -(MulNegSinScaled(0xff - o->ang, 0x40) + 0x60), 0x12c); \
    Spawn(MulCos(o->ang, 0x20) + 0xa0, -(MulNegSinScaled(o->ang, 0x40) + 0x60), 0x12c); \
    Spawn(MulCos(o->ang, 0x40) + 0xa0, -(MulNegSinScaled(o->ang, 0x20) + 0x60), 0x12c);

void func_8011A114(Ctl *o)
{
    TObj *a = D_8009F2D8;
    TObj *b = D_8009F2DC;
    TObj *c = D_8009F2F8;
    TObj *d = D_8009F328;
    V6 v;

    switch (o->state) {
    case 0:
        { Fix16 *h = D_800A6038.h, *dd = D_800A6038.d; Spawn(h->p.whole, D_800A6038.y.p.whole, dd->p.whole); }
        if (D_8009C93A) {
            D_8009C942[0] = 1;
            D_8009C93F[0] = 1;
            D_8009D2B0 = 2;
            o->state++;
        }
        break;
    case 1:
        { Fix16 *h = D_800A6038.h, *dd = D_800A6038.d; Spawn(h->p.whole, D_800A6038.y.p.whole, dd->p.whole); }
        if (D_800A6038.b69) o->state++;
        break;
    case 2:
        { Fix16 *h = c->h, *dd = c->d; Spawn(h->p.whole, c->y.p.whole, dd->p.whole); }
        c->y.p.whole++;
        if (D_800A6038.y.p.whole - 0x20 < c->y.p.whole) {
            c->b04 = 3;
            FUN_8005a9a4(0x9f, 0);
            o->w08 = 0x15e;
            o->state++;
        }
        break;
    case 3:
        if (--o->w08 <= 0) {
            FUN_8005a9a4(0x90, 0);
            o->w08 = 0x12c;
            o->state++;
        }
        break;
    case 4:
        RING(o);
        if (--o->w08 <= 0) o->state++;
        break;
    case 5:
        RING(o);
        if (!D_8009C975) {
            D_8009C93C = 1;
            func_80116928(d, 0, 0);
            o->w0a = 200;
            o->state++;
        }
        break;
    case 6:
        RING(o);
        o->w08 = (o->w08 + 8) & 0xff;
        FUN_80059d44(MulCos(o->w08, 0x40) + 0x40);
        if (--o->w0a <= 0) {
            o->w08 = 0x80;
            o->state++;
        }
        break;
    case 7:
        RING(o);
        o->w08 -= 0x10;
        FUN_80059d44(o->w08);
        if (o->w08 <= 0) {
            o->w08 = 0x78;
            func_80116928(d, 1, 0);
            o->state = 8;
        }
        break;
    case 8:
        if (--o->w08 <= 0) {
            o->w08 = 0x3c;
            v = *(V6 *)&a->a;
            v.v[3] -= 0x50;
            a->d90 = (int)FUN_8002dcc8(7, 3, &v);
            func_80117CEC(a, 2, 0);
            o->state++;
        }
        break;
    case 9:
        AnimAdvance(a);
        if (((TObj *)a->d90)->b04 == 2) {
            ((TObj *)a->d90)->b04 = 3;
            func_80117CEC(a, 1, 0);
            o->w08 = 0x40;
            o->state++;
        }
        break;
    case 10:
        AnimAdvance(a);
        a->h->p.whole++;
        if (--o->w08 <= 0) {
            FUN_8005a8a8(0xa0, 0, 0);
            o->w08 = 200;
            o->state++;
        }
        break;
    case 11:
        if (--o->w08 <= 0 && !D_8009C975) {
            D_8009C93C = 1;
            o->w08 = 0x80;
            o->state++;
        }
        break;
    case 12:
        FUN_80059d44(o->w08);
        D_8009CFEA = 1;
        o->state++;
        break;
    case 13:
        o->w08 -= 0x10;
        FUN_80059d44(o->w08);
        if (o->w08 <= 0) {
            o->w08 = 0x78;
            o->state++;
        }
        break;
    case 14:
        if (--o->w08 <= 0) {
            v.v[1] = 0xa0;
            v.v[3] = -0x50;
            a->d90 = (int)FUN_8002dcc8(9, 0, &v);
            o->state++;
        }
        break;
    case 15:
        if (((TObj *)a->d90)->b04 == 2) {
            ((TObj *)a->d90)->b04 = 3;
            o->w08 = 0x50;
            o->state++;
        }
        break;
    case 16:
        if (--o->w08 <= 0) {
            D_8009CFEA = 2;
            o->w08 = 0x12c;
            o->state++;
        }
        break;
    case 17:
        if (--o->w08 <= 0) {
            v = *(V6 *)&a->a;
            v.v[3] -= 0x50;
            func_80116928(d, 3, 0);
            a->d90 = (int)FUN_8002dcc8(7, 4, &v);
            func_80117CEC(a, 2, 0);
            o->state++;
        }
        break;
    case 18:
        if (((TObj *)a->d90)->b04 == 2) {
            ((TObj *)a->d90)->b04 = 3;
            func_80117CEC(a, 1, 0);
            o->w08 = 8;
            o->state++;
        }
        break;
    case 19:
        AnimAdvance(a);
        a->h->p.whole++;
        if (--o->w08 <= 0) {
            func_80117CEC(a, 0, 0);
            D_8009C93E[0] = 1;
            FUN_8005a8a8(0x9d, 0, 0);
            o->w08 = 200;
            o->state++;
        }
        break;
    case 20:
        if (--o->w08 <= 0) {
            v = *(V6 *)&a->a;
            v.v[3] -= 0x50;
            a->d90 = (int)FUN_8002dcc8(7, 5, &v);
            func_80117CEC(a, 2, 0);
            o->state++;
        }
        break;
    case 21:
        AnimAdvance(o);
        if (((TObj *)a->d90)->b04 == 2) {
            ((TObj *)a->d90)->b04 = 3;
            func_80117CEC(a, 0, 0);
            if (D_8009CE15 == 0xff) {
                a->d90 = (int)FUN_8002dcc8(7, 6, &v);
                func_80117CEC(a, 2, 0);
                o->state = 0x19;
            } else if (D_8009CDBB) {
                v = *(V6 *)&a->a;
                v.v[3] -= 0x50;
                a->d90 = (int)FUN_8002dcc8(7, 7, &v);
                func_80117CEC(a, 2, 0);
                o->state = 0x16;
            } else {
                v = *(V6 *)&a->a;
                v.v[3] -= 0x50;
                a->d90 = (int)FUN_8002dcc8(7, 8, &v);
                func_80117CEC(a, 2, 0);
                o->state = 0x16;
            }
        }
        break;
    case 22:
        if (((TObj *)a->d90)->b04 == 2) {
            ((TObj *)a->d90)->b04 = 3;
            func_80117CEC(a, 0, 0);
            *(signed char *)&b->b0f = -8;
            b->animFrame = 1;
            func_80117CEC(b, 3, 0);
            o->w08 = b->h->p.whole - D_800A6038.h->p.whole;
            o->state++;
        }
        break;
    case 23:
        b->h->p.whole--;
        AnimAdvance(b);
        if (--o->w08 <= 0) {
            if (!D_8009D0B2) addItemToInventory(0xe, 1, 1);
            b->b04 = 3;
            o->w08 = 0x3c;
            o->state++;
        }
        break;
    case 24:
        if (--o->w08 <= 0) {
            v = *(V6 *)&a->a;
            v.v[3] -= 0x50;
            a->d90 = (int)FUN_8002dcc8(7, 6, &v);
            func_80117CEC(a, 2, 0);
            o->state = 0x19;
        }
        break;
    case 25:
        if (((TObj *)a->d90)->b04 == 2) {
            ((TObj *)a->d90)->b04 = 3;
            o->w08 = 0x30;
            o->state = 99;
        }
        break;
    case 99:
        D_8009C942[0] = 0;
        D_8009C93F[0] = 0;
        D_8009C93E[0] = 0;
        D_800A6038.wb2 = 0;
        D_1F8001C6 = 0;
        o->b0f = o->w24;
        D_800A6038.b04 = 1;
        D_800A6038.step = 0;
        D_800A6038.state = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
