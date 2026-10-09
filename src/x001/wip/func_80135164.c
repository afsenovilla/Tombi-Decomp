// FUNC 80135164 1372 X001
/* score 309 (o27: D_800A60B2 is player field D_800A6038.w7a, keeps the dy store first; o17: short ang, dx/dy short, constant 0x80 through a local): control flow matches. Differences: the angle uses lbu+sll/sra (game) instead of lb
   for (signed char)e->c, dx/ang get s3/s0 in the game (frame 0x28), the pl->h/y/d update order.
   Tried: int/short temps, casts, shifts.
   o27: the lb issue is gone (lbu+sll/sra now). Left: game `andi v0; move s0,v0` + dx in s3 (ang still live when
   dx is copied from v0; ours sched1 puts `a0 = ang` for the 2nd call before `dx = v0`, so dx reuses s0 and
   the frame is 8 B smaller), and the D_80011DD8 base register in case 1. Tried: block-local a = ang & 0xff
   (short/int/uchar), nested inline calc(o,e,ang&0xff), short/uchar callee prototypes, local type search. */
#include "TOBJ.H"
typedef struct { signed char a; unsigned char b; unsigned char c; signed char d; } E4;
extern TObj D_800A6038;
extern E4 D_80011DD8[];
extern signed char D_80011EB4[];
extern void *D_8013DE4C[];
extern void *D_8013DDF8[];
extern unsigned short D_8009C960, D_8009C962;
extern unsigned char D_8009C938, D_8009C93E, D_8009C93F, D_8009C942;
extern unsigned char D_800A603C, D_800A603D, D_800A603E;
extern int FUN_8001f9e0(void);
extern short FUN_8001fddc(int, int);
extern short FUN_8001fdac(int, int);
extern int FUN_8002dc50(int, int, int, int);

static __inline__ void follow(TObj *o, E4 *e)
{
    short ang;
    short dx;
    short dy;

    if (D_8009C960 == 1 && D_8009C962 < 2)
        o->anim = D_8013DE4C[e->a];
    else
        o->anim = D_8013DDF8[e->a];
    if (o->animFrame & 1)
        { int k = 0x80; ang = D_800A6038.d8c + k - (signed char)e->c; }
    else
        ang = (signed char)e->c + D_800A6038.d8c;
    ang &= 0xff;
    dx = FUN_8001fddc(ang, e->d);
    dy = FUN_8001fdac(ang, e->d);
    D_800A6038.w7a = dy;
    o->h->p.whole = D_800A6038.h->p.whole + dx;
    o->y.p.whole = D_800A6038.y.p.whole + dy;
    o->d->p.whole = D_800A6038.d->p.whole;
    o->d8c = D_800A6038.d8c;
    o->b0f = D_800A6038.b0f + e->b;
}

void func_80135164(TObj *o)
{
    E4 *e;
    TObj *p;

    o->visible = D_800A6038.visible;
    switch (o->state) {
    case 0:
        o->active = 2;
        o->animFrame = D_800A6038.animFrame & 1;
        o->category |= 0x80;
        e = &D_80011DD8[D_80011EB4[*(unsigned short *)D_800A6038.anim]];
        follow(o, e);
        if (D_8009C938) break;
        switch (D_8009C962) {
        case 1:
            if (D_800A6038.h->p.whole >= 0x1000) break;
            D_800A6038.h->p.whole = 0x1000;
            goto set;
        case 3:
            if ((unsigned short)(D_800A6038.y.p.whole + 0x2d6) < 0x48) {
                if ((unsigned short)(D_800A6038.h->p.whole - 0x7f0) >= 0x20) break;
                D_800A6038.h->p.whole = 0x7ee;
            } else {
                if (D_800A6038.h->p.whole < 0xad3) break;
                D_800A6038.h->p.whole = 0xad2;
            }
        set:
            D_800A603C = 5;
            D_800A603D = 0;
            D_800A603E = 0;
            o->state = 1;
            break;
        }
        break;
    case 1:
        e = &D_80011DD8[D_80011EB4[*(unsigned short *)D_800A6038.anim]];
        if (e->a < 2 && (FUN_8001f9e0() & 3) == 0)
            e->a = FUN_8001f9e0() & 1;
        follow(o, e);
        if (D_800A6038.b69) {
            D_8009C942 = 1;
            o->d90 = FUN_8002dc50(2, 0xc, 0x80, 0x6c);
            o->state = 2;
        }
        break;
    case 2:
        p = (TObj *)o->d90;
        if (p->b04 == 2) {
            p->b04 = 3;
            D_8009C942 = 0;
            D_8009C93E = 0;
            D_8009C93F = 0;
            D_800A603C = 1;
            D_800A603D = 0;
            D_800A603E = 0;
            o->state = 0;
        }
        break;
    }
}
