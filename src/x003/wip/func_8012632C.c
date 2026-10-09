// FUNC 8012632c 2108 X003
/* score 216: all but two blocks match (case 4's two "w22 = K; substep++; wac = 0x1d; set" branches, ~10 insns each):
   game keeps substep in v1 / reloaded wac in v0 and hoists the lbu to the block top; ours swaps v0/v1, so the lbu
   cannot move. Tried all orders of substep++/w22/wac/anim/p (macro, inline set(), inline go3(o,w,n), temp t for
   substep, D_80139574 vs D_80139500[n]); the near() test is the 0x8011B870 "short lim" form. */
#include "TOBJ.H"
extern unsigned short D_1F80027E;
extern unsigned short D_1F80016A, D_1F80016E, D_1F800172;
extern unsigned short D_1F8001F8;
extern int D_1F800198;
extern short FUN_80040278(TObj *, int, int);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern int Rand(void);
extern short isObjectBelowGround(TObj *);
extern void FUN_8001fab4(TObj *);
extern int func_801256F4(TObj *, short);
extern void *D_80139500[];
extern void *D_8013950C, *D_80139530, *D_80139538, *D_80139574;
extern unsigned char D_80135CB0[];
extern unsigned char D_80135D54[], D_80135D4C[];
extern unsigned short D_80135D2C[];
extern char D_80077CF4[], D_80077CDC[];

static __inline__ void set(TObj *o)
{
    unsigned char *p;
    o->anim = D_80139500[o->wac];
    p = &D_80135CB0[o->wac * 4];
    o->box0 = *p++;
    o->box1 = *p++;
    o->box2 = *p;
    o->box3 = p[1];
    AnimLoadDuration(o);
}

#define SET() set(o)

static __inline__ void setn(TObj *o, short n)
{
    unsigned char *p;
    o->wac = n;
    o->anim = D_80139500[n];
    p = &D_80135CB0[o->wac * 4];
    o->box0 = *p++;
    o->box1 = *p++;
    o->box2 = *p;
    o->box3 = p[1];
    AnimLoadDuration(o);
}
#define BOX() \
    { unsigned char *p = &D_80135CB0[o->wac * 4]; \
    o->box0 = *p++; \
    o->box1 = *p++; \
    o->box2 = *p; \
    o->box3 = p[1]; } \
    AnimLoadDuration(o);

static __inline__ void go1(TObj *o, short w)
{
    o->substep++;
    o->w22 = w;
    o->wac = 0x1d;
    set(o);
}

static __inline__ void go2(TObj *o, short w)
{
    o->w22 = w;
    o->substep++;
    o->wac = 0x1d;
    set(o);
}

static __inline__ void go3(TObj *o, short w, short n)
{
    o->substep++;
    o->anim = D_80139500[n];
    o->wac = n;
    o->w22 = w;
    BOX();
}

static __inline__ void slope(TObj *o)
{
    unsigned int v;

    if (o->b69 == 1) {
        o->b69 = 0;
    } else if (FUN_80040278(o, o->h->p.whole, (short)(o->y.p.whole + 0xe))) {
        if (o->wac >= 0xc) {
            o->d8c = 0;
        } else {
            v = ((D_1F80027E << 2) + o->d8c) & 0xff;
            if (v != 0) {
                if (v < 0x80) o->d8c = o->d8c - 1;
                else o->d8c = o->d8c + 1;
                o->d8c = *(unsigned char *)&o->d8c;
            }
        }
    }
}

static __inline__ int near(TObj *o)
{
    unsigned short t;
    short lim = 0x40;
    t = D_1F800172 - o->d->p.whole + 0x2d;
    if (t >= 0x5b) return 0;
    t = D_1F80016A - o->h->p.whole + 0x20;
    if (t > lim) return 0;
    t = D_1F80016E - o->y.p.whole + 0xa0;
    return t <= lim + 0xa0;
}

void func_8012632C(TObj *o)
{
    short *w = &o->wb4;

    switch (o->substep) {
    case 0:
        o->substep++;
        o->timer = D_80135D54[Rand() & 7];
        if (*w != 0) o->wac = 7;
        else o->wac = 0x1e;
        SET();
        break;
    case 1:
        o->y.p.whole++;
        slope(o);
        if (--o->timer == -1)
            o->substep++;
        break;
    case 2:
        if (*w != 0) {
            w[1] = 1;
            o->substep = 7;
            o->wac = 0xd;
            o->d8c = 0;
        } else {
            o->wac = 0x1b;
            o->substep++;
        }
        SET();
        break;
    case 3:
        if (AnimAdvance(o)) {
            o->movetab = D_80077CF4;
            o->timer = D_80135D4C[Rand() & 7];
            if (isObjectBelowGround(o) != o->animFrame) {
                o->w22 = 0x1e;
                o->substep = 5;
                o->wac = 0x1d;
            } else {
                o->wac = 3;
                o->substep++;
            }
            SET();
        }
        break;
    case 4:
        if (--o->timer != -1) {
        FUN_8001fab4(o);
        AnimAdvance(o);
        slope(o);
        if (func_801256F4(o, o->animFrame)) {
            go3(o, 0x3c, 0x1d);
        } else if (((D_1F8001F8 + D_1F800198) & 0x3f) == 0 && isObjectBelowGround(o) != o->animFrame) {
            go3(o, 0x1e, 0x1d);
        }
        } else {
            o->state = 1;
            o->substep = 2;
        }
        break;
    case 5:
        if (AnimAdvance(o)) {
            o->substep++;
            o->animFrame = 1 - o->animFrame;
            o->wac = 3;
            SET();
        }
        break;
    case 6:
        AnimAdvance(o);
        if (--o->w22 == -1)
            o->substep = 4;
        break;
    case 7:
        if (AnimAdvance(o)) {
            o->timer = D_80135D2C[Rand() & 7];
            w[1] = 1;
            o->movetab = D_80077CDC;
            o->wac = 0xc;
            SET();
            if (isObjectBelowGround(o) != o->animFrame) {
                o->substep = 9;
                o->w22 = 0x14;
            } else {
                o->substep++;
            }
        }
        break;
    case 8:
        if (near(o)) {
            o->substep = 0;
            o->state++;
            break;
        }
        if (--o->timer == 0) {
            o->substep = 10;
            w[1] = 0;
            o->wac = 0xe;
            SET();
            break;
        }
        FUN_8001fab4(o);
        AnimAdvance(o);
        slope(o);
        if (func_801256F4(o, o->animFrame)) {
            o->w22 = 0x3c;
            o->substep++;
        } else if (((D_1F8001F8 + D_1F800198) & 0x3f) == 0 && isObjectBelowGround(o) != o->animFrame) {
            o->w22 = 0x1e;
            o->substep++;
        }
        break;
    case 9:
        AnimAdvance(o);
        if (--o->w22 == -1) {
            o->substep--;
            o->animFrame = 1 - o->animFrame;
        }
        break;
    case 10:
        if (AnimAdvance(o)) {
            o->state = 1;
            o->substep = 0;
        }
        break;
    }
}
