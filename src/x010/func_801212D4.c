// FUNC 801212d4 284 X010
// MATCHING 801212d4 284
#include "spad.h"
/* Player vs. slope-table object collision (X000 family: func_8004B454 / func_8004B7C4).
   Shape notes (all needed): `h` is used (via d) between the table lhu and the d30 store so combine
   cannot fold the lhu into an lh; `s` is a scratch variable reused for (short)h, box1, the compare
   flag and e->y (multi-set: lh not hoisted, flag lands in the same register); `d2 = s` before the
   compare with the flag stored into s keeps local-alloc from rewriting the compare to d2; `d` holds
   the e->h->whole + h temporary (untied from the lhu so the sll temp gets v1); `oy` reused for the
   o->h store value, the constant 7 and o->y keeps the li after the store and the lhu after the sb. */
#include "TOBJ.H"
typedef struct {
    TObj t;
    char pad[0xe8 - 0xc0];
    unsigned short e8;
    unsigned short ea;
} P;
extern unsigned short D_8012F2F8[];

void func_801212D4(P *o, TObj *e)
{
    int d;
    int h;
    int b0;
    int s;
    int d2;
    int u;
    char pad[8];
    unsigned short oy;
    int c;

    if ((unsigned short)(o->t.d->p.whole - e->d->p.whole + 0x2d) >= 0x5b) return;
    d = -e->d34 + o->ea;
    h = d + e->d38;
    if ((unsigned short)h > e->d38) return;
    h = D_8012F2F8[(short)-d >> 3];
    b0 = (unsigned short)e->box0;
    d = (unsigned short)e->h->p.whole + h;
    c = b0 + (o->e8 - d);
    s = (short)h;
    e->d30 = s;
    s = e->box1;
    u = (unsigned short)c;
    d2 = s;
    s = s < u;
    if (s) return;
    if (o->t.animFrame & 1) d = d2 - b0;
    else d = -b0;
    oy = d + (e->h->p.whole + h);
    o->t.h->p.whole = oy;
    oy = 7;
    o->t.b9e = oy;
    oy = (unsigned short)o->t.y.p.whole;
    o->t.wb8 = d;
    s = (unsigned short)e->y.p.whole;
    o->t.velY = 0;
    SPAD.s19e = 0;
    (*(TObj **)&SPAD.p3c0) = e;
    o->t.wba = oy - s;
}
