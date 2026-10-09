// FUNC 801212d4 284 X010
/* score 25. Not a csv start: the csv piece func_801213D0 is the tail of this function (0x801212D4..0x801213F0).
   Same family as X001 wip func_80126D2C. o29: h reused for the d + d38 sum (game t0) and d = -e->d34 + o->ea fix the
   first half. Left: combine merges the table lhu with (short)h into an extra lh (game: lhu t0 once, sll/sra for the
   d30 store); then box1 (lh v1) copied into d after the compare value. Tried: volatile table (la form, worse),
   pointer/index temps, (h<<16)>>16, short copy, ushort/short/uint h, all orders of b0/d30/box1/compare temp. */
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
    d = -e->d34 + o->ea;
    h = d + e->d38;
    if ((unsigned short)h > e->d38) return;
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
