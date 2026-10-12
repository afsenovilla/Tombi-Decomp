// FUNC 80120f34 588 X009
/* score 24: logic/layout match; (short)r lands in a0 and the switch value (o+0xac) in v1, game has them swapped,
   and in case 2 the game compares p->active with the dispatch's v0=2 and p->b6a with the switch register.
   Tried: int/ushort r with casts, separate switch variable (types, decl order), nested if, inline-call forms.
   o15: cc1 -dl/-dg: switch value pseudo has 8 refs/14 insns (prio 1.71), the (short)r copy 3 refs/8 insns (0.375),
   so global-alloc gives the switch value v1 first; do{}while(0) pins, t reused as switch var, register asm all fail.
   o29: int r + (short) casts + int callee gives the same 24 (pseudo 78 = (short)r 3 refs/8 insns, switch 155 8/14);
   separate switch var of any type (early/late decl, b6a == sw) no change.
   n55 (score 4, debt): `int r` + `register int rs asm("$3")` for the (short)r copy (t = r << 16; rs = t >> 16 keeps
   the sll temp in v0), other compares re-extend (short)r. Left: case 2's `p->active == 2` compares with a0 (switch
   reg) instead of the dispatch's li v0,2 (game: first compare v0, second a0); tried ||/nested if, switch copy k,
   (unsigned char)2, volatile b6a read, t = 2: no change. */
#include "TOBJ.H"
#include "raw7.h"

extern unsigned char D_1F8001A4;
extern short D_1F80019E;
extern int func_80043464(TObj *, TObj *);
extern void FUN_8001f96c(int, int, int, int);
extern void FUN_8004258c(TObj *, int);

void func_80120F34(TObj *o, TObj *p)
{
    int r;
    register int rs asm("$3");
    int t;

    if (o->b9e) return;
    r = func_80043464(o, p);
    t = r << 16;
    rs = t >> 16;
    if (rs == -1) return;
    switch (U8(o, 0xac)) {
    case 1:
        if (rs == 3) {
            p->active = 2;
            p->b04 = 2;
            p->step = 1;
            p->state = 0;
            p->b6a = 0;
            PTR(o, 0xe4) = p;
            U8(o, 0xac) = 2;
            FUN_8001f96c(2, o->a.p.whole, o->y.p.whole, o->b.p.whole);
            break;
        }
    case 0:
    case 3:
        if (p->active & 2) break;
        if (D_1F8001A4) break;
        if (o->active & 2) break;
        if ((short)r == 2 && p->b6a == 1) {
            if (U8(o, 0xac) == 1) U8(o, 0xac) = 0;
            p->active = 4;
            p->b6a = 2;
            o->active = 2;
            o->b04 = 2;
            o->step = 1;
            o->state = 0;
            o->substep = 0;
            FUN_8004258c(o, 1);
            break;
        }
        if (o->b9c && (short)r == (o->animFrame & 1)) break;
        o->active = 2;
        t = p->h->p.whole > o->h->p.whole;
        o->b04 = 2;
        o->step = 0;
        o->state = 0;
        o->animFrame = t;
        FUN_8004258c(o, 1);
        break;
    case 2:
        if (p->active == 2) break;
        if (p->b6a == 2) break;
        p->active = 3;
        p->b6a = 0;
        p->b04 = 2;
        t = o->h->p.whole > p->h->p.whole;
        p->step = 0;
        p->state = 0;
        p->animFrame = t;
        break;
    }
    D_1F80019E = 0;
}
