// FUNC 80121a88 1352 X003
// MATCHING 80121a88 1352
#include "spad.h"
#include "TOBJ.H"
typedef struct A { short w0; unsigned short w2; unsigned short w4; unsigned short w6; } A;
typedef struct B { unsigned char c[4]; } B;
typedef struct { unsigned char b0, b1; char p2[0xa - 2]; unsigned char b0a, b0b; char p0c[2]; short w0e; } X;
extern char D_80077D3C[];
extern B D_80135B30[];
extern A *D_801386F8[];
extern void FUN_8001f8e4(TObj *);
extern void applyFrameVelocityY(TObj *);
extern int AnimAdvanceWithBox(TObj *);
extern int komaPigCheckLanded(TObj *);
extern int komaPigCheckWall(TObj *);

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
    komaPigCheckLanded(q);
}

static __inline__ short near(TObj *o)
{
    if (o->animFrame) {
        if (o->h->p.whole >= (*(short*)((char*)&SPAD.i168 + 2)) - 0x30) return 0;
        return 1;
    }
    if ((*(short*)((char*)&SPAD.i168 + 2)) + 0x30 < o->h->p.whole) return 1;
    return 0;
}

void komaPigRest(TObj *o)
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
        komaPigCheckLanded(o);
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
        applyFrameVelocityY(o);
        if (o->animFrame) o->h->raw -= o->velH << 8;
        else o->h->raw += o->velH << 8;
        if (komaPigCheckLanded(o) == 0 && x->w0e++ >= 4) goto fail;
        if (komaPigCheckWall(o)) {
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
        applyFrameVelocityY(o);
        if (o->animFrame) o->h->raw -= o->velH << 8;
        else o->h->raw += o->velH << 8;
        komaPigCheckWall(o);
        if (komaPigCheckLanded(o) == 0 && x->w0e++ >= 4) {
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
