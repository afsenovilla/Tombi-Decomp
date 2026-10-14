// FUNC 8011a114 7316 X002
// MATCHING 8011a114 7316
#include "gcd.h"
#include "gv2.h"
#include "gvars.h"
/* Whole function: the csv splits it into 8011A114, 8011A448, 8011B170, 8011B330, 8011B47C and 8011B7D4. */
#include "TOBJ.H"
#include "gamestate.h"

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

extern TObj PLAYER;
extern TObj *SCRIPT_OBJECTS, *D_8009F2DC, *D_8009F2F8, *D_8009F328;
extern unsigned short D_1F8001F8;
extern short D_1F8001C6;
extern TObj *ObjAlloc(void);
extern int MulCos(short, short);
extern int MulNegSinScaled(short, short);
extern void setEventComplete(int, int);
extern void setEventStarted(int, int, int);
extern void FUN_80059d44(short);
extern TObj *showMessageBoxDirect(int, int, V6 *);
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

void pigGateDiscoveryScene(Ctl *o)
{
    TObj *a = SCRIPT_OBJECTS;
    TObj *b = D_8009F2DC;
    TObj *c = D_8009F2F8;
    TObj *d = D_8009F328;
    V6 v;

    switch (o->state) {
    case 0:
        { Fix16 *h = PLAYER.h, *dd = PLAYER.d; Spawn(h->p.whole, PLAYER.y.p.whole, dd->p.whole); }
        if (GVARS.b3a) {
            ((unsigned char*)&GVARS.b42)[0] = 1;
            ((unsigned char*)&GVARS.b3f)[0] = 1;
            GV2.bb0 = 2;
            o->state++;
        }
        break;
    case 1:
        { Fix16 *h = PLAYER.h, *dd = PLAYER.d; Spawn(h->p.whole, PLAYER.y.p.whole, dd->p.whole); }
        if (PLAYER.b69) o->state++;
        break;
    case 2:
        { Fix16 *h = c->h, *dd = c->d; Spawn(h->p.whole, c->y.p.whole, dd->p.whole); }
        c->y.p.whole++;
        if (PLAYER.y.p.whole - 0x20 < c->y.p.whole) {
            c->b04 = 3;
            setEventComplete(0x9f, 0);
            o->w08 = 0x15e;
            o->state++;
        }
        break;
    case 3:
        if (--o->w08 <= 0) {
            setEventComplete(0x90, 0);
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
        if (!GAME.fadeScreenControl) {
            GVARS.b3c = 1;
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
            a->d90 = (int)showMessageBoxDirect(7, 3, &v);
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
            setEventStarted(0xa0, 0, 0);
            o->w08 = 200;
            o->state++;
        }
        break;
    case 11:
        if (--o->w08 <= 0 && !GAME.fadeScreenControl) {
            GVARS.b3c = 1;
            o->w08 = 0x80;
            o->state++;
        }
        break;
    case 12:
        FUN_80059d44(o->w08);
        GCD.bcfea = 1;
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
            a->d90 = (int)showMessageBoxDirect(9, 0, &v);
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
            GCD.bcfea = 2;
            o->w08 = 0x12c;
            o->state++;
        }
        break;
    case 17:
        if (--o->w08 <= 0) {
            v = *(V6 *)&a->a;
            v.v[3] -= 0x50;
            func_80116928(d, 3, 0);
            a->d90 = (int)showMessageBoxDirect(7, 4, &v);
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
            ((unsigned char*)&GVARS.b3e)[0] = 1;
            setEventStarted(0x9d, 0, 0);
            o->w08 = 200;
            o->state++;
        }
        break;
    case 20:
        if (--o->w08 <= 0) {
            v = *(V6 *)&a->a;
            v.v[3] -= 0x50;
            a->d90 = (int)showMessageBoxDirect(7, 5, &v);
            func_80117CEC(a, 2, 0);
            o->state++;
        }
        break;
    case 21:
        AnimAdvance(o);
        if (((TObj *)a->d90)->b04 == 2) {
            ((TObj *)a->d90)->b04 = 3;
            func_80117CEC(a, 0, 0);
            if (GCD.bce15 == 0xff) {
                a->d90 = (int)showMessageBoxDirect(7, 6, &v);
                func_80117CEC(a, 2, 0);
                o->state = 0x19;
            } else if (GCD.bcdbb) {
                v = *(V6 *)&a->a;
                v.v[3] -= 0x50;
                a->d90 = (int)showMessageBoxDirect(7, 7, &v);
                func_80117CEC(a, 2, 0);
                o->state = 0x16;
            } else {
                v = *(V6 *)&a->a;
                v.v[3] -= 0x50;
                a->d90 = (int)showMessageBoxDirect(7, 8, &v);
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
            o->w08 = b->h->p.whole - PLAYER.h->p.whole;
            o->state++;
        }
        break;
    case 23:
        b->h->p.whole--;
        AnimAdvance(b);
        if (--o->w08 <= 0) {
            if (!GCD.bd0b2) addItemToInventory(0xe, 1, 1);
            b->b04 = 3;
            o->w08 = 0x3c;
            o->state++;
        }
        break;
    case 24:
        if (--o->w08 <= 0) {
            v = *(V6 *)&a->a;
            v.v[3] -= 0x50;
            a->d90 = (int)showMessageBoxDirect(7, 6, &v);
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
        ((unsigned char*)&GVARS.b42)[0] = 0;
        ((unsigned char*)&GVARS.b3f)[0] = 0;
        ((unsigned char*)&GVARS.b3e)[0] = 0;
        PLAYER.wb2 = 0;
        D_1F8001C6 = 0;
        o->b0f = o->w24;
        PLAYER.b04 = 1;
        PLAYER.step = 0;
        PLAYER.state = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
