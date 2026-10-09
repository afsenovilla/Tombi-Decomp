// FUNC 80120da0 976 X014
/* score 155: the three spawn loops are one static inline debris(o, m, af, ex) (loops 2/3 = m 5/6; game keeps af in
   s4 for them, so af is passed as a variable). Left: loop 1 still folds `m & 4` (combine nonzero_bits: m is only set
   to 1/2; passing r instead keeps the andi but r gets its own reg), and loops 2/3 use one giv with offsets where the
   game has two pointers (x in s2, y = base+2 in s3).
   Older notes (score 178 without the inline): tried short/int/char m, m as a dead parameter, testing r instead of m;
   o26: else-if for the m=1 branch (CSE proves it, m still not live at start), m param, local table pointer
   variants (t[i], t++ pointer, per-iteration pointer). */
#include "TOBJ.H"

typedef struct { unsigned short x, y; } V2;
extern V2 D_801265B0[];
extern short D_1F800284;
extern TObj *FUN_800183b8(void);
extern short func_80041240(TObj *, short, short);
extern short func_80041EBC(TObj *, short, short);
extern void FUN_8001f96c(int, short, short, short);
extern void FUN_8002ee50(int, short, short, short);

static __inline__ void debris(TObj *o, int m, short af, short ex)
{
    int i;
    TObj *n;
    V2 *t = D_801265B0;

    for (i = 0; i < 3; i++) {
        n = FUN_800183b8();
        if (n) {
            n->active = 2;
            n->type = 0x46;
            n->subtype = i + 1;
            n->animFrame = af;
            n->velH = t[i].x;
            if ((m & 3) == 1) n->velH = -n->velH;
            n->velV = t[i].y;
            if (m & 4) {
                n->velV -= 0x200;
                n->b0c = 1;
                n->a.p.whole = o->a.p.whole;
            } else {
                n->b0c = 0;
                n->a.p.whole = o->a.p.whole + ex;
            }
            n->y.p.whole = o->y.p.whole;
            n->b.p.whole = o->b.p.whole;
        }
    }
}

int func_80120DA0(TObj *o)
{
    short dx, ex, af, r;
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
        if (r != 0 && m == r && D_1F800284 == 0) {
            o->d38 = (0x80 - o->d38) & 0xff;
            FUN_8001f96c(1, o->a.p.whole + dx, o->y.p.whole, o->b.p.whole);
            if (m == 2) {
                ex = -0x24;
                af = 1;
            } else {
                ex = 0x24;
                af = 0;
            }
            debris(o, m, af, ex);
            FUN_8002ee50(0, o->a.p.whole + dx, o->y.p.whole, o->b.p.whole);
            o->velV -= o->velV >> 2;
        }
    }
    if (func_80041EBC(o, o->h->p.whole, o->y.p.whole + 0x24) == 0) return 0;
    if (D_1F800284) return 0;
    af = 0;
    debris(o, 5, af, ex);
    af = 1;
    debris(o, 6, af, ex);
    FUN_8002ee50(2, o->a.p.whole, o->y.p.whole + 0x24, o->b.p.whole);
    return 1;
}
