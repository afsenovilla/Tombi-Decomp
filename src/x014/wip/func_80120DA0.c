// FUNC 80120da0 976 X014
/* score 178: logic complete. Main difference: the game keeps the `(m & 3) == 1` and `m & 4` tests in the first
   spawn loop, but gcc folds them here because m is only ever set to the constants 1/2 (combine nonzero_bits); the
   game also reaches the Y table through a per-iteration `i*4 + &D_801265B2` instead of a strength-reduced giv.
   Tried: short/int/char m, m as a dead parameter, testing r instead of m. Next idea: make m live at function
   start (possibly uninitialised path) or a shared static inline debris(o, f) for all three loops. */
#include "TOBJ.H"

typedef struct { unsigned short x, y; } V2;
extern V2 D_801265B0[];
extern short D_1F800284;
extern TObj *FUN_800183b8(void);
extern short func_80041240(TObj *, short, short);
extern short func_80041EBC(TObj *, short, short);
extern void FUN_8001f96c(int, short, short, short);
extern void FUN_8002ee50(int, short, short, short);

int func_80120DA0(TObj *o)
{
    short dx, ex, af, r;
    int m;
    int i;
    TObj *n;

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
            for (i = 0; i < 3; i++) {
                n = FUN_800183b8();
                if (n) {
                    n->active = 2;
                    n->type = 0x46;
                    n->subtype = i + 1;
                    n->animFrame = af;
                    n->velH = D_801265B0[i].x;
                    if ((m & 3) == 1) n->velH = -n->velH;
                    n->velV = D_801265B0[i].y;
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
            FUN_8002ee50(0, o->a.p.whole + dx, o->y.p.whole, o->b.p.whole);
            o->velV -= o->velV >> 2;
        }
    }
    if (func_80041EBC(o, o->h->p.whole, o->y.p.whole + 0x24) == 0) return 0;
    if (D_1F800284) return 0;
    for (i = 0; i < 3; i++) {
        n = FUN_800183b8();
        if (n) {
            n->active = 2;
            n->type = 0x46;
            n->subtype = i + 1;
            n->animFrame = 0;
            n->velH = -D_801265B0[i].x;
            n->b0c = 1;
            n->velV = D_801265B0[i].y - 0x200;
            n->a.p.whole = o->a.p.whole;
            n->y.p.whole = o->y.p.whole;
            n->b.p.whole = o->b.p.whole;
        }
    }
    for (i = 0; i < 3; i++) {
        n = FUN_800183b8();
        if (n) {
            n->active = 2;
            n->type = 0x46;
            n->subtype = i + 1;
            n->animFrame = 1;
            n->velH = D_801265B0[i].x;
            n->b0c = 1;
            n->velV = D_801265B0[i].y - 0x200;
            n->a.p.whole = o->a.p.whole;
            n->y.p.whole = o->y.p.whole;
            n->b.p.whole = o->b.p.whole;
        }
    }
    FUN_8002ee50(2, o->a.p.whole, o->y.p.whole + 0x24, o->b.p.whole);
    return 1;
}
