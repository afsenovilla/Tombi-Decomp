// FUNC 80126d2c 352 X001
// MATCHING 80126d2c 352
// FLAGS -O2 -G0 -fno-expensive-optimizations
#include "spad.h"
/* Debt: -fno-expensive-optimizations stops local-alloc's optimize_reg_copy_1 from folding the
   `t2 = s` copy (game: lh v0; move t0,v0; slt v0,v0,v1). The dead `s = 0` (after the compare) and
   `s = 1` (at the end) keep cse1 from making t2 canonical on its not-taken rescan (make_regs_eqv:
   t2's last use beyond the block and later than s's). `c2 = c; c = b0 + c2;` gives addu b0,c
   (expand swaps operands when target == op1). c multi-set keeps the chain unboosted (placed
   before the sll/sra/sw/lh, which are single-set temps and boosted in sched1). */
#include "TOBJ.H"
typedef struct {
    TObj t;
    char pad[0xe8 - 0xc0];
    unsigned short e8;
    unsigned short ea;
} P;
extern short D_1F80019E;
extern unsigned short *D_8013C6A4[];

void func_80126D2C(P *o, TObj *e)
{
    int t;
    short dy;
    unsigned short h;
    int b0; int c; int s; int t2; int cc; int c2; int x;
    char pad[8];

    t = e->subtype;
    if (t == 7) return;
    if ((unsigned short)(o->t.d->p.whole - e->d->p.whole + 0x2d) >= 0x5b) return;
    dy = e->d34;
    dy = o->ea - dy;
    h = dy + e->d38;
    if ((unsigned short)h > e->d38) return;
    h = D_8013C6A4[t][(short)-dy >> 3];
    b0 = (unsigned short)e->box0;
    c = o->e8 - ((unsigned short)e->h->p.whole + h); c2 = c; c = b0 + c2;
    e->d30 = (short)h;
    s = e->box1; cc = (unsigned short)c; t2 = s;
    if (s < cc) return;
    s = 0;
    if (!(o->t.animFrame & 1)) x = -b0;
    else x = t2 - b0;
    o->t.h->p.whole = x + ((unsigned short)e->h->p.whole + h);
    o->t.b9e = 7;
    o->t.wb8 = x;
    D_1F80019E = 0;
    (*(TObj **)&SPAD.p3c0) = e;
    o->t.wba = o->t.y.p.whole - e->y.p.whole;
    o->t.velY = 0;
    if (((unsigned char *)o)[0xa9] == 0) {
        e->b68 = 2;
        e->animFrame = o->t.animFrame & 1;
    }
    s = 1;
}
