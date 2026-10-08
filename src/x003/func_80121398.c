// FUNC 80121398 1776 X003
// MATCHING 80121398 1776
#include "TOBJ.H"
typedef struct A { short w0; unsigned short w2; unsigned short w4; unsigned short w6; } A;
typedef struct B { unsigned char c[4]; } B;
typedef struct { unsigned char b0, b1; unsigned short w2; short w4; } X;
extern char D_80077CDC[];
extern char D_80077CF4[];
extern char D_80077D0C[];
extern B D_80135B30[];
extern A *D_801386F8[];
extern void playSFX(int);
extern void FUN_8001fa88(TObj *, unsigned short);
extern short func_8004065C(TObj *, short, short, short);
extern int AnimAdvanceWithBox(TObj *);
extern int func_801206F0(TObj *);

#define SETANIM(k) \
    o->anim = D_801386F8[k]; \
    { unsigned char *p; { B *t = D_80135B30; p = t[((A *)o->anim)->w2].c; } \
    o->box0 = *p++; \
    o->box1 = *p++; \
    o->box2 = *p; \
    o->box3 = p[1]; } \
    o->animTimer = ((A *)o->anim)->w6 & 0x3fff;

#define COLL() \
    { \
        X *x = (X *)((char *)o + 0xb4); \
        if (o->movetab != 0) { \
            FUN_8001fa88(o, 1 - o->animFrame); \
        } \
        if (o->b9d & 2) { \
            if (o->animFrame != (o->b9d & 1)) { \
                o->movetab = 0; \
            } \
        } \
        if (o->animFrame == 1) { \
            x->w4 = 0x10; \
        } else { \
            x->w4 = -0x10; \
        } \
        if (func_8004065C(o, o->h->p.whole + x->w4, o->y.p.whole, 1 - o->animFrame)) { \
            o->movetab = 0; \
        } \
    }

void func_80121398(TObj *o)
{
    X *x2 = (X *)((char *)o + 0xb4);
    unsigned char v;

    switch (o->substep) {
    case 0:
        playSFX(7);
        o->timer = 4;
        o->velV = -0x400;
        o->movetab = D_80077D0C;
        o->wac = 0x19;
        o->substep++;
        SETANIM(0x19);
        o->b9c = 1;
    case 1:
        AnimAdvanceWithBox(o);
        COLL();
        o->velV += 0x40;
        o->y.raw += o->velV << 8;
        if (o->velV > 0) {
            o->b9c = 2;
            o->substep++;
        }
        break;
    case 2:
        AnimAdvanceWithBox(o);
        COLL();
        o->velV += 0x40;
        o->y.raw += o->velV << 8;
        if (func_801206F0(o) == 0) break;
        if (o->movetab == 0) {
            o->d8c = x2->w2;
            o->substep = 4;
            o->timer = 0x1e;
            o->wac = 0x1b;
            SETANIM(0x1b);
            break;
        }
        o->b9c = 1;
        o->velV = -0x280;
        o->d8c = 0;
        o->movetab = D_80077CF4;
        o->substep++;
        x2->b1 = 1;
        o->wac = 0x1a;
        SETANIM(0x1a);
        break;
    case 3:
        o->d8c = ((o->animFrame != 0) ? o->d8c - 0x14 : o->d8c + 0x14) & 0xff;
        COLL();
        o->velV += 0x30;
        o->y.raw += o->velV << 8;
        if (o->velV <= 0) break;
        o->b9c = 2;
        if (func_801206F0(o) == 0) break;
        if (o->movetab == 0) {
            o->d8c = x2->w2;
            o->substep = 4;
            o->timer = 0x1e;
            o->wac = 0x1b;
            SETANIM(0x1b);
            break;
        }
        v = x2->b1--;
        if (v != 0) {
            o->velV = ~(o->velV - 0x80) + 1;
            if (o->velV < 0) {
                o->b9c = 1;
            }
            o->movetab = D_80077CDC;
            break;
        }
        o->d8c = x2->w2;
        o->timer = 0x1e;
        o->wac = 0x1b;
        o->substep++;
        SETANIM(0x1b);
        break;
    case 4:
        if (--o->timer == -1) {
            o->wac = 0x22;
            o->substep++;
            SETANIM(0x22);
        }
        o->y.p.whole += 2;
        func_801206F0(o);
        break;
    case 5:
        if (AnimAdvanceWithBox(o)) {
            o->timer = 0x3c;
            o->wac = 0x23;
            o->substep++;
            SETANIM(0x23);
        }
        o->y.p.whole += 2;
        func_801206F0(o);
        break;
    case 6:
        AnimAdvanceWithBox(o);
        if (--o->timer == -1) {
            o->timer = 0x3c;
            o->wac = 0x24;
            o->substep++;
            SETANIM(0x24);
        }
        o->y.p.whole += 2;
        func_801206F0(o);
        break;
    case 7:
        AnimAdvanceWithBox(o);
        if (--o->timer == -1) {
            o->b69 = 0;
            o->state = 0;
            o->substep = 6;
            o->b68 = 0;
            o->b9c = 0;
        }
        o->y.p.whole += 2;
        func_801206F0(o);
        break;
    }
}
