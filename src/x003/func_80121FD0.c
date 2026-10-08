// FUNC 80121fd0 1744 X003
// MATCHING 80121fd0 1744
#include "TOBJ.H"
typedef struct A { short w0; unsigned short w2; unsigned short w4; unsigned short w6; } A;
typedef struct B { unsigned char c[4]; } B;
typedef struct { unsigned char b0, b1; char p2[2]; short w04; char p6[4]; unsigned char b0a, b0b; char p0c[2]; short w0e; } X;
extern char D_80077CF4[];
extern B D_80135B30[];
extern unsigned char D_80135B00[];
extern A *D_801386F8[];
extern unsigned short D_1F80016A;
extern unsigned short D_1F80016E;
extern unsigned short D_1F800172;
extern void FUN_8001f8e4(TObj *);
extern void FUN_8001fab4(TObj *);
extern void FUN_8001fa20(TObj *, unsigned short);
extern int Rand(void);
extern short func_8004065C(TObj *, short, short, short);
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

static __inline__ unsigned char inrange(TObj *o)
{
    if ((unsigned short)(o->d->p.whole - D_1F800172 + 0x2d) < 0x5b
        && (unsigned short)(o->y.p.whole - D_1F80016E + 0x46) < 0x8d
        && (unsigned short)(o->h->p.whole - D_1F80016A + 0x80) < 0x101) return 1;
    return 0;
}

void func_80121FD0(TObj *o)
{
    X *x = (X *)((char *)o + 0xb4);

    switch (o->substep) {
    case 0:
        o->b9d = 0;
        o->b9c = 0;
        x->w0e = 0;
        x->b1 = 0;
        o->timer = 0x3c;
        o->substep++;
        FUN_8001f8e4(o);
        o->movetab = D_80077CF4;
        o->wac = 10;
        SETANIM(10);
    case 1:
        AnimAdvanceWithBox(o);
        if (--o->timer == -1) {
            if (inrange(o)) {
                o->timer = 8;
                o->wac = 0xb;
                o->substep++;
                SETANIM(0xb);
            } else {
                o->state = 0;
                o->substep = 0;
            }
        }
        fall(o);
        break;
    case 2:
        AnimAdvanceWithBox(o);
        FUN_8001fab4(o);
        if (func_801206F0(o) == 0 && x->w0e++ >= 4) {
            x->b0b = 6;
            x->b0a = 0;
            o->state = 7;
            o->substep = 0;
            break;
        }
        if (--o->timer == -1 || func_8012080C(o)) {
            o->timer = 0x3c;
            o->wac = 10;
            o->substep++;
            SETANIM(10);
        }
        break;
    case 3:
        AnimAdvanceWithBox(o);
        if (--o->timer == -1) {
            if (inrange(o)) {
                o->substep++;
                x->w0e = 0;
                o->timer = 8;
                o->wac = 0xb;
                SETANIM(0xb);
            } else {
                o->state = 0;
                o->substep = 0;
            }
        }
        fall(o);
        break;
    case 4: {
        unsigned char c;
        AnimAdvanceWithBox(o);
        FUN_8001fa20(o, 1 - o->animFrame);
        if (func_801206F0(o) == 0 && x->w0e++ >= 4) {
            x->b0b = 6;
            x->b0a = 0;
            o->state = 7;
            o->substep = 0;
            break;
        }
        c = o->b9d;
        if ((c & 2) && o->animFrame != (c & 1)) o->timer = 0;
        if (o->animFrame == 1) x->w04 = 0x10;
        else x->w04 = -0x10;
        if (func_8004065C(o, o->h->p.whole + x->w04, o->y.p.whole, (1 - o->animFrame) & 1)) o->timer = 0;
        if (--o->timer == -1) {
            o->substep++;
            FUN_8001f8e4(o);
            o->timer = 0x3c;
            o->wac = 10;
            SETANIM(10);
        }
        break;
    }
    case 5:
        AnimAdvanceWithBox(o);
        if (inrange(o)) {
            if (--o->timer == -1) o->substep++;
        } else {
            o->state = 0;
            o->substep = 0;
        }
        fall(o);
        break;
    case 6:
        fall(o);
        if (x->b1 == 2 || D_80135B00[Rand() & 0xf] == 0) {
            o->state = 3;
            o->substep = 0;
            break;
        }
        o->substep = 2;
        x->b1++;
        o->timer = 8;
        o->wac = 0xb;
        SETANIM(0xb);
        break;
    }
}
