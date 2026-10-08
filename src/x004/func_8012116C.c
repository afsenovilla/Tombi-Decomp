// FUNC 8012116c 1352 X004
// MATCHING 8012116c 1352
#include "TOBJ.H"
typedef struct { char p0; unsigned char b01; char p2[2]; short w04; char p6[4]; unsigned char b0a, b0b; char p0c[2]; short w0e; } X;
extern char D_80077D3C[];
extern unsigned char D_80130FD4[];
extern unsigned short *D_80133B80[], *D_80133BA0[], *D_80133BA4[], *D_80133BA8[], *D_80133BF8[];
extern short D_1F80016A;
extern void ObjSetFacingToPlayer(TObj *);
extern void FUN_8001fb20(TObj *);
extern int func_8011FDD4(TObj *);
extern int func_8011FEF0(TObj *);
extern int AnimAdvanceWithBox(TObj *);

#define SETANIM(o, T) \
    { \
        unsigned short *a; \
        unsigned char *p; \
        a = T[0]; \
        o->anim = a; \
        p = &D_80130FD4[a[1] * 4]; \
        o->box0 = *p++; \
        o->box1 = *p++; \
        o->box2 = p[0]; \
        o->box3 = p[1]; \
        o->animTimer = ((unsigned short *)o->anim)[3] & 0x3fff; \
    }

static __inline__ short past(TObj *o)
{
    if (o->animFrame) {
        if (o->h->p.whole >= D_1F80016A - 0x30) {
            return 0;
        }
        return 1;
    }
    if (D_1F80016A + 0x30 < o->h->p.whole) {
        return 1;
    }
    return 0;
}

void func_8012116C(TObj *o)
{
    X *x = (X *)((char *)o + 0xb4);

    switch (o->substep) {
    case 0:
        o->b9d = 0;
        o->b9c = 0;
        x->w0e = 0;
        o->substep++;
        ObjSetFacingToPlayer(o);
        o->timer = 0x28;
        o->wac = 9;
        SETANIM(o, D_80133B80);
        func_8011FDD4(o);
        break;
    case 1:
        AnimAdvanceWithBox(o);
        if (--o->timer == -1) {
            o->b68 = 1;
            o->velH = 0x280;
            o->movetab = D_80077D3C;
            o->b69 = 0;
            o->wac = 0x11;
            o->substep++;
            SETANIM(o, D_80133BA0);
            break;
        }
        o->y.p.whole += 2;
        func_8011FDD4(o);
        break;
    case 2:
        AnimAdvanceWithBox(o);
        FUN_8001fb20(o);
        if (o->animFrame) {
            o->h->raw -= o->velH << 8;
        } else {
            o->h->raw += o->velH << 8;
        }
        if (func_8011FDD4(o) == 0 && x->w0e++ > 3) {
            x->b0b = 6;
            x->b0a = 0;
            o->state = 7;
            o->substep = 0;
            break;
        }
        if (func_8011FEF0(o)) {
            o->state = 4;
            o->substep = 0;
            break;
        }
        if (past(o)) {
            o->substep = 3;
            x->w0e = 0;
        }
        o->velH += 0x10;
        if (o->velH > 0x300) {
            o->velH = 0x300;
        }
        break;
    case 3:
        o->velH = 0x200;
        o->b68 = 0;
        o->wac = 0x12;
        o->substep++;
        SETANIM(o, D_80133BA4);
    case 4:
        AnimAdvanceWithBox(o);
        FUN_8001fb20(o);
        if (o->animFrame) {
            o->h->raw -= o->velH << 8;
        } else {
            o->h->raw += o->velH << 8;
        }
        func_8011FEF0(o);
        if (func_8011FDD4(o) == 0 && x->w0e++ > 3) {
            x->b0b = 6;
            x->b0a = 0;
            o->state = 7;
            o->substep = 0;
            break;
        }
        o->velH -= 0x10;
        if (o->velH < 0) {
            o->timer = 0x28;
            o->wac = 0x13;
            o->substep++;
            SETANIM(o, D_80133BA8);
        }
        break;
    case 5:
        AnimAdvanceWithBox(o);
        if (--o->timer == -1) {
            o->timer = 0x5a;
            o->wac = 0x27;
            o->substep++;
            SETANIM(o, D_80133BF8);
        }
        o->y.p.whole += 2;
        func_8011FDD4(o);
        break;
    case 6:
        AnimAdvanceWithBox(o);
        if (--o->timer == -1) {
            o->substep = 6;
            o->state = 0;
            o->animFrame = 1 - o->animFrame;
        }
        o->y.p.whole += 2;
        func_8011FDD4(o);
        break;
    }
}
