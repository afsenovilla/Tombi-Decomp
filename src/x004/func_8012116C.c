// FUNC 8012116c 1352 X004
// MATCHING 8012116c 1352
#include "TOBJ.H"
typedef struct A { short w0; unsigned short w2; unsigned short w4; unsigned short w6; } A;
typedef struct B { unsigned char c[4]; } B;
typedef struct { char p[0xa]; unsigned char ba, bb; char pc[2]; short we; } X;
extern char D_80077D3C[];
extern B D_80130FD4[];
extern A *D_80133B80[];
extern short D_1F80016A;
extern void FUN_8001f8e4(TObj *);
extern void FUN_8001fb20(TObj *);
extern void func_8011FB1C(TObj *);
extern int func_8011FDD4(TObj *);
extern int func_8011FEF0(TObj *);

#define SETANIM(k) \
    o->anim = D_80133B80[k]; \
    { unsigned char *p; { B *t = D_80130FD4; p = t[((A *)o->anim)->w2].c; } \
    o->box0 = *p++; \
    o->box1 = *p++; \
    o->box2 = *p; \
    o->box3 = p[1]; } \
    o->animTimer = ((A *)o->anim)->w6 & 0x3fff;

#define MOVEH() \
    if (o->animFrame) \
        o->h->raw -= o->velH << 8; \
    else \
        o->h->raw += o->velH << 8;

static __inline__ short Far(TObj *o)
{
    if (o->animFrame) {
        if (o->h->p.whole >= D_1F80016A - 0x30)
            return 0;
        return 1;
    }
    if (D_1F80016A + 0x30 < o->h->p.whole)
        return 1;
    return 0;
}

void func_8012116C(TObj *o)
{
    X *x = (X *)((char *)o + 0xb4);

    switch (o->substep) {
    case 0:
        o->b9d = 0;
        o->b9c = 0;
        x->we = 0;
        o->substep++;
        FUN_8001f8e4(o);
        o->timer = 0x28;
        o->wac = 9;
        SETANIM(0);
        func_8011FDD4(o);
        break;
    case 1:
        func_8011FB1C(o);
        if (--o->timer == -1) {
            o->b68 = 1;
            o->velH = 0x280;
            o->movetab = D_80077D3C;
            o->b69 = 0;
            o->wac = 0x11;
            o->substep++;
            SETANIM(8);
            break;
        }
        goto tail;
    case 2:
        func_8011FB1C(o);
        FUN_8001fb20(o);
        MOVEH();
        if (func_8011FDD4(o) == 0) {
            if (x->we++ >= 4)
                goto kill;
        }
        if (func_8011FEF0(o)) {
            o->state = 4;
            o->substep = 0;
            break;
        }
        if (Far(o)) {
            o->substep = 3;
            x->we = 0;
        }
        o->velH += 0x10;
        if (o->velH > 0x300)
            o->velH = 0x300;
        break;
    case 3:
        o->velH = 0x200;
        o->b68 = 0;
        o->wac = 0x12;
        o->substep++;
        SETANIM(9);
    case 4:
        func_8011FB1C(o);
        FUN_8001fb20(o);
        MOVEH();
        func_8011FEF0(o);
        if (func_8011FDD4(o) == 0) {
            if (x->we++ >= 4) {
            kill:
                x->bb = 6;
                x->ba = 0;
                o->state = 7;
                o->substep = 0;
                break;
            }
        }
        o->velH -= 0x10;
        if (o->velH < 0) {
            o->timer = 0x28;
            o->wac = 0x13;
            o->substep++;
            SETANIM(10);
        }
        break;
    case 5:
        func_8011FB1C(o);
        if (--o->timer == -1) {
            o->timer = 0x5a;
            o->wac = 0x27;
            o->substep++;
            SETANIM(30);
        }
        goto tail;
    case 6:
        func_8011FB1C(o);
        if (--o->timer == -1) {
            o->substep = 6;
            o->state = 0;
            o->animFrame = 1 - o->animFrame;
        }
    tail:
        o->y.p.whole += 2;
        func_8011FDD4(o);
        break;
    }
}
