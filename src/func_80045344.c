// FUNC 80045344 572 MAIN0
// MATCHING 80045344 572
/* side() as a real static inline (direct returns give bgez; li 1; move zero instead of jump.c's store-flag) and the
   case 4/5/10 tail written out in full (cross-jumping merges it; the extra o refs make o outrank p in global-alloc). */
#include "TOBJ.H"
extern int func_800425C4(TObj *a, TObj *b);
extern void FUN_8001f96c(int a, int b, int c, int d);
extern void playSFX(int a);
extern short D_1F80019E;

static __inline__ short side(TObj *a, TObj *b)
{
    short dx;
    if ((unsigned short)(a->d->p.whole - b->d->p.whole + 0x2d) >= 0x5b) return -1;
    dx = a->h->p.whole - b->h->p.whole;
    if ((unsigned short)(dx + (b->box0 + a->box0)) > b->box1 + a->box1) return -1;
    if ((unsigned short)(a->y.p.whole - b->y.p.whole + (a->box2 + b->box2)) > a->box3 + b->box3) return -1;
    D_1F80019E = 0;
    if (dx < 0) return 0;
    return 1;
}
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
        o->b6a = 1;
        o->active = 2;
        o->wa8 = 0x4ff;
        break;
    case 0: case 3: case 9:
        r = 2;
        break;
    case 13:
        p->b68 = 0;
        r = -1;
        break;
    case 6: case 11: case 12:
        r = 2;
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
    s = side(o, p);
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
