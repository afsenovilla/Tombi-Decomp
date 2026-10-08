// FUNC 80121a88 1352 X003
// MATCHING 80121a88 1352
#include "TOBJ.H"
typedef struct A { short w0; unsigned short w2; unsigned short w4; unsigned short w6; } A;
typedef struct B { unsigned char c[4]; } B;
typedef struct { unsigned char b0, b1; char p2[0xa - 2]; unsigned char b0a, b0b; char p0c[2]; short w0e; } X;
extern char D_80077D3C[];
extern B D_80135B30[];
extern A *D_801386F8[];
extern short D_1F80016A;
extern void FUN_8001f8e4(TObj *);
extern void FUN_8001fb20(TObj *);
extern int AnimAdvanceWithBox(TObj *);
extern int func_801206F0(TObj *);
extern int func_8012080C(TObj *);

#define SETANIM(k) \
    o->anim = D_801386F8[k]; \
    { unsigned char *p; { B *t = D_80135B30; p = t[((A *)o->anim)->w2].c; } \
    o->box0 = *p++; \
    o->box1 = *p++; \
    o->box2 = *p; \
    o->box3 = p[1]; } \
    o->animTimer = ((A *)o->anim)->w6 & 0x3fff;

static __inline__ void fall(TObj *q)
{
    q->y.p.whole += 2;
    func_801206F0(q);
}

static __inline__ short near(TObj *o)
{
    if (o->animFrame) {
        if (o->h->p.whole >= D_1F80016A - 0x30) return 0;
        return 1;
    }
    if (D_1F80016A + 0x30 < o->h->p.whole) return 1;
    return 0;
}

void func_80121A88(TObj *o)
{
    X *x = (X *)((char *)o + 0xb4);

    switch (o->substep) {
    case 0:
        o->b9d = 0;
        o->b9c = 0;
        x->w0e = 0;
        o->substep++;
        FUN_8001f8e4(o);
        o->timer = 0x28;
        o->wac = 8;
        SETANIM(8);
        func_801206F0(o);
        break;
    case 1:
        AnimAdvanceWithBox(o);
        if (--o->timer == -1) {
            o->b68 = 1;
            o->velH = 0x280;
            o->movetab = D_80077D3C;
            o->b69 = 0;
            o->wac = 0xc;
            o->substep++;
            SETANIM(0xc);
            break;
        }
        fall(o);
        break;
    case 2:
        AnimAdvanceWithBox(o);
        FUN_8001fb20(o);
        if (o->animFrame) o->h->raw -= o->velH << 8;
        else o->h->raw += o->velH << 8;
        if (func_801206F0(o) == 0 && x->w0e++ >= 4) goto fail;
        if (func_8012080C(o)) {
            o->state = 4;
            o->substep = 0;
            break;
        }
        if (near(o)) {
            o->substep = 3;
            x->w0e = 0;
        }
        o->velH += 0x10;
        if (o->velH > 0x300) o->velH = 0x300;
        break;
    case 3:
        o->velH = 0x200;
        o->b68 = 0;
        o->wac = 0xd;
        o->substep++;
        SETANIM(0xd);
    case 4:
        AnimAdvanceWithBox(o);
        FUN_8001fb20(o);
        if (o->animFrame) o->h->raw -= o->velH << 8;
        else o->h->raw += o->velH << 8;
        func_8012080C(o);
        if (func_801206F0(o) == 0 && x->w0e++ >= 4) {
        fail:
            x->b0b = 6;
            x->b0a = 0;
            o->state = 7;
            o->substep = 0;
            break;
        }
        o->velH -= 0x10;
        if (o->velH < 0) {
            o->timer = 0x28;
            o->wac = 0xe;
            o->substep++;
            SETANIM(0xe);
        }
        break;
    case 5:
        AnimAdvanceWithBox(o);
        if (--o->timer == -1) {
            o->timer = 0x5a;
            o->wac = 0x20;
            o->substep++;
            SETANIM(0x20);
        }
        fall(o);
        break;
    case 6:
        AnimAdvanceWithBox(o);
        if (--o->timer == -1) {
            o->substep = 6;
            o->state = 0;
            o->animFrame = 1 - o->animFrame;
        }
        fall(o);
        break;
    }
}
