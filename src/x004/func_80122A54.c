// FUNC 80122a54 1916 X004
// MATCHING 80122a54 1916
#include "TOBJ.H"
typedef struct A { short w0; unsigned short w2; unsigned short w4; unsigned short w6; } A;
typedef struct B { unsigned char c[4]; } B;
typedef struct { unsigned char b0, b1; unsigned short w2; short w4; } X;
extern char D_80077CDC[];
extern char D_80077CF4[];
extern char D_80077D0C[];
extern B D_80130FD4[];
extern A *D_80133B5C[];
extern void playSFX(int);
extern void FUN_8001fa88(TObj *, unsigned short);
extern short func_8004065C(TObj *, short, short, short);
extern int AnimAdvanceWithBox(TObj *);
extern int func_8011FDD4(TObj *);
extern void FUN_8002b920(TObj *);

#define SETANIM(k) \
    o->anim = D_80133B5C[k]; \
    { unsigned char *p; { B *t = D_80130FD4; p = t[((A *)o->anim)->w2].c; } \
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

void func_80122A54(TObj *o)
{
    X *x2 = (X *)((char *)o + 0xb4);
    unsigned char v;

    switch (o->state) {
    case 0:
        playSFX(0xf);
        o->b69 = 0;
        o->animFrame = 1 - o->w7a;
        o->state++;
        o->d8c = 0;
        o->velV = -0x400;
        o->movetab = D_80077D0C;
        o->b9c = 1;
        o->wac = 0x29;
        SETANIM(0x29);
    case 1:
        COLL();
        o->velV += 0x40;
        o->y.raw += o->velV << 8;
        if (o->velV > 0) {
            o->b9c = 2;
            o->b69 = 0;
            o->state++;
        }
        break;
    case 2:
        COLL();
        o->velV += 0x40;
        o->y.raw += o->velV << 8;
        if (func_8011FDD4(o) == 0) break;
        if (o->movetab == 0) {
            o->d8c = x2->w2;
            o->state = 4;
            o->timer = 0x168;
            o->wac = 0x22;
            SETANIM(0x22);
            FUN_8002b920(o);
            break;
        }
        o->b9c = 1;
        o->d8c = 0;
        o->state++;
        o->active = 3;
        o->velV = -0x280;
        o->movetab = D_80077CF4;
        x2->b1 = 1;
        o->b69 = 0;
        o->wac = 0x21;
        SETANIM(0x21);
        break;
    case 3:
        o->d8c = ((o->animFrame != 0) ? o->d8c - 0x14 : o->d8c + 0x14) & 0xff;
        COLL();
        o->velV += 0x30;
        o->y.raw += o->velV << 8;
        if (o->velV <= 0) break;
        o->b9c = 2;
        if (func_8011FDD4(o) == 0) break;
        if (o->movetab == 0) {
            o->d8c = x2->w2;
            o->state = 4;
            o->timer = 0x168;
            o->wac = 0x22;
            SETANIM(0x22);
            FUN_8002b920(o);
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
        o->timer = 0x168;
        o->wac = 0x22;
        o->state++;
        SETANIM(0x22);
        FUN_8002b920(o);
        break;
    case 4:
        if (--o->timer == -1) {
            o->wac = 0x2a;
            o->state++;
            SETANIM(0x2a);
        }
        o->y.p.whole += 2;
        func_8011FDD4(o);
        break;
    case 5:
        if (AnimAdvanceWithBox(o)) {
            o->timer = 0x78;
            o->wac = 0x2b;
            o->state++;
            SETANIM(0x2b);
        }
        o->y.p.whole += 2;
        func_8011FDD4(o);
        break;
    case 6:
        AnimAdvanceWithBox(o);
        if (--o->timer == -1) {
            o->timer = 0x3c;
            o->wac = 0x2c;
            o->state++;
            SETANIM(0x2c);
        }
        o->y.p.whole += 2;
        func_8011FDD4(o);
        break;
    case 7:
        AnimAdvanceWithBox(o);
        if (--o->timer == -1) {
            *(signed char *)&o->b0f = -9;
            o->active = 1;
            o->b04 = 1;
            o->step = 1;
            o->state = 0;
            o->substep = 6;
            o->b69 = 0;
            o->b68 = 0;
            o->b9c = 0;
        }
        o->y.p.whole += 2;
        func_8011FDD4(o);
        break;
    }
}
