// FUNC 80045344 572 MAIN0
/* score 22 (b39, no register asm any more): side() written as a macro so its 7 field reads count as refs of o
   (o 22 refs/84 insns now outranks p for s0; with the inline, o had 15/90 < p 20/115 and o/p were swapped).
   Left: (1) jump.c turns `res = 1; if (dx < 0) res = 0;` into a store-flag (nor; srl) where the game keeps
   bgez + move s3,zero; an inline sgn() helper gives the branch but moves the zero block out of line (77);
   (2) box0/box1 test: lhu order of b->box0/a->box0 and v0/v1 in the box1 sum.
   b46: `(b->box0 + a->box0)` gives the game's lhu order (24, regs still swapped: -dl shows qty {p-box0,sum,total,andi}
   (16 refs over 9 insns) loses to qty {p-box1,sum,slt} (12 refs/4 insns); game must have the box1 loads earlier in
   sched1). Store-flag not avoided by: res=1 before the sh, else-forms, res--, goto form, int dx with (short)/<<16 tests,
   sgn() inline with r var (all converted); sgn() with direct returns gives the branch but out-of-line zero block (79). */
#include "TOBJ.H"
extern int func_800425C4(TObj *a, TObj *b);
extern void FUN_8001f96c(int a, int b, int c, int d);
extern void playSFX(int a);
extern short D_1F80019E;

#define SIDE(a, b, res) do { short dx; res = -1; \
    if ((unsigned short)(a->d->p.whole - b->d->p.whole + 0x2d) >= 0x5b) break; \
    dx = a->h->p.whole - b->h->p.whole; \
    if ((unsigned short)(dx + (a->box0 + b->box0)) > b->box1 + a->box1) break; \
    if ((unsigned short)(a->y.p.whole - b->y.p.whole + (a->box2 + b->box2)) > a->box3 + b->box3) break; \
    D_1F80019E = 0; res = 1; if (dx < 0) res = 0; } while (0)
static __inline__ int touch(TObj *o, TObj *p)
{
    int r;
    r = 1;
    p->b68 = 1;
    p->b9e = 0;
    switch (func_800425C4(o, p)) {
    case 1: case 7:
        r = 1;
        break;
    case 4: case 5: case 10:
        r = 1;
        goto L;
    case 0: case 3: case 9:
        r = 2;
        break;
    case 13:
        p->b68 = 0;
        r = -1;
        break;
    case 6: case 11: case 12:
        r = 2;
    L:
        o->b6a = 1;
        o->active = 2;
        o->wa8 = 0x4ff;
        break;
    case 2: case 8:
        r = 0;
        p->b9e = 1;
        p->b9f = 0;
        break;
    }
    if (r < 0) {
        r = 0;
    } else {
        int s;
        FUN_8001f96c(1, o->a.p.whole, o->y.p.whole, o->b.p.whole);
        s = 7;
        if ((p->category & 0x7f) == 4) s = 6;
        playSFX(s);
    }
    return r;
}

void func_80045344(TObj *o, TObj *p)
{
    short s;
    TObj *q, *q2;
    unsigned char v;
    SIDE(o, p, s);
    if (s < 0) return;
    if (touch(o, p) == 0) return;
    switch (p->subtype) {
    case 0:
        q = (TObj *)p->d94;
        q2 = (TObj *)q->d94;
        break;
    case 1:
        q = (TObj *)p->d90;
        q2 = (TObj *)p->d94;
        break;
    }
    v = s | 2;
    p->b68 = v;
    q->b68 = v;
    q2->b68 = v;
}
