// FUNC 80120da0 976 X014
// MATCHING 80120da0 976
// FLAGS -O2 -G0 -fno-strength-reduce
#include "spad.h"
/* Loop 1 is func_80120C54 (matched sibling) inlined with k = m; loops 2/3 are the same body with a stepped y
   pointer and af passed in. The asm("" : "=r"(t) : "0"(t)) keeps the la/move form of the table pointer; the $21
   clobber keeps the constant 2 from being hoisted into s5 in loop 3. */
#include "TOBJ.H"

extern TObj *allocObjectLayer2(void);
extern short func_80041240(TObj *, short, short);
extern short func_80041EBC(TObj *, short, short);
extern void spawnImpactEffect(int, short, short, short);
extern void FUN_8002ee50(int, short, short, short);

typedef struct { short a, b; } AB;
extern AB D_801265B0[];
extern short D_801265B2[];
static __inline__ void debris(TObj *o, int k)
{
    TObj *n;
    AB *p;
    char *b;
    int q;
    int m, one;
    int i;
    short dx;
    short f;

    if ((k & 3) == 2) {
        dx = -0x24;
        f = 1;
    } else {
        dx = 0x24;
        f = 0;
    }
    i = 0;
    m = k & 3;
    one = 1;
    { AB *t = D_801265B0; asm("" : "=r"(t) : "0"(t)); b = (char *)D_801265B2; p = t; }
    for (; i < 3; i++, p++) {
        n = allocObjectLayer2();
        if (n != 0) {
            n->active = 2;
            n->type = 0x46;
            n->subtype = i + 1;
            n->animFrame = f;
            q = i * 4;
            n->velH = p->a;
            if (m == one) {
                n->velH = -n->velH;
            }
            n->velV = *(short *)(q + (int)b);
            if ((k & 4) == 0) {
                n->a.p.whole = o->a.p.whole + dx;
                n->b0c = 0;
            } else {
                n->velV -= 0x200;
                n->b0c = one;
                n->a.p.whole = o->a.p.whole;
            }
            n->y.p.whole = o->y.p.whole;
            n->b.p.whole = o->b.p.whole;
        }
    }
}

static __inline__ void debris2(TObj *o, int k, short f)
{
    TObj *n;
    AB *p;
    char *b;
    int i;
    short dx;

    if ((k & 3) == 2) {
        dx = -0x24;
    } else {
        dx = 0x24;
    }
    i = 0;
    { AB *t = D_801265B0; asm("" : "=r"(t) : "0"(t)); b = (char *)&t->b; p = t; }
    for (; i < 3; b += 4, i++, p++) {
        asm("" ::: "$21");
        n = allocObjectLayer2();
        if (n != 0) {
            n->active = 2;
            n->type = 0x46;
            n->subtype = i + 1;
            n->animFrame = f;
            n->velH = p->a;
            if ((k & 3) == 1) {
                n->velH = -n->velH;
            }
            n->velV = *(short *)b;
            if ((k & 4) == 0) {
                n->a.p.whole = o->a.p.whole + dx;
                n->b0c = 0;
            } else {
                n->velV -= 0x200;
                n->b0c = 1;
                n->a.p.whole = o->a.p.whole;
            }
            n->y.p.whole = o->y.p.whole;
            n->b.p.whole = o->b.p.whole;
        }
    }
}

int func_80120DA0(TObj *o)
{
    short dx, af, r;
    int m;

    if (o->d38 < 0xc0) {
        m = 2;
        dx = -0x24;
    } else {
        m = 1;
        dx = 0x24;
    }
    if (o->d38 != 0xc0) {
        r = func_80041240(o, o->h->p.whole + dx, o->y.p.whole);
        if (r != 0 && m == r && SPAD.s284 == 0) {
            o->d38 = (0x80 - o->d38) & 0xff;
            spawnImpactEffect(1, o->a.p.whole + dx, o->y.p.whole, o->b.p.whole);
            debris(o, m);
            FUN_8002ee50(0, o->a.p.whole + dx, o->y.p.whole, o->b.p.whole);
            o->velV -= o->velV >> 2;
        }
    }
    if (func_80041EBC(o, o->h->p.whole, o->y.p.whole + 0x24) == 0) return 0;
    if (SPAD.s284) return 0;
    af = 0;
    debris2(o, 5, af);
    af = 1;
    debris2(o, 6, af);
    FUN_8002ee50(2, o->a.p.whole, o->y.p.whole + 0x24, o->b.p.whole);
    return 1;
}
