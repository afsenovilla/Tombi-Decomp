// FUNC 801212d4 284 X010
/* score 41. Not a csv start: the csv piece func_801213D0 is the tail of this function (0x801212D4..0x801213F0).
   Same family as X001 wip func_80126D2C. Left: game reads the height table once (lhu t0) and sign-extends with
   sll/sra for the d30 store, gcc re-reads it with lh; game loads e->d34 before o->ea and copies box1 (lh v1) into
   the d register (move a0,v1) after the compare value. Tried: h/b0/d types, d30 cast forms, volatile table read,
   statement orders, x if/else forms. */
#include "TOBJ.H"
typedef struct {
    TObj t;
    char pad[0xe8 - 0xc0];
    unsigned short e8;
    unsigned short ea;
} P;
extern short D_1F80019E;
extern TObj *D_1F8003C0;
extern unsigned short D_8012F2F8[];

void func_801212D4(P *o, TObj *e)
{
    int d;
    int h;
    unsigned short b0;

    if ((unsigned short)(o->t.d->p.whole - e->d->p.whole + 0x2d) >= 0x5b) return;
    d = o->ea - e->d34;
    if ((unsigned short)(d + e->d38) > e->d38) return;
    h = D_8012F2F8[(short)-d >> 3];
    b0 = e->box0;
    e->d30 = (short)h;
    d = e->box1;
    if ((unsigned short)(b0 + (o->e8 - (e->h->p.whole + h))) > d) return;
    if (o->t.animFrame & 1) d = d - b0;
    else d = -b0;
    o->t.h->p.whole = d + (e->h->p.whole + h);
    o->t.b9e = 7;
    o->t.wb8 = d;
    o->t.velY = 0;
    D_1F80019E = 0;
    D_1F8003C0 = e;
    o->t.wba = o->t.y.p.whole - e->y.p.whole;
}
