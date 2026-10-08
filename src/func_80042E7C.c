// FUNC 80042e7c 320 MAIN0
// MATCHING 80042e7c 320
#include "TOBJ.H"
extern int func_800425C4(TObj *a, TObj *b);
extern void playSFX(int);
extern void FUN_8001f96c(int, int, int, int);

int func_80042E7C(TObj *a, TObj *b)
{
    int r = 1;
    b->b68 = 1;
    b->b9e = 0;
    switch (func_800425C4(a, b)) {
    case 9:
    case 12:
        playSFX(7);
    case 0:
        r = 2;
        b->w98 = 0;
        FUN_8001f96c(0, a->a.p.whole, a->y.p.whole, a->b.p.whole);
        break;
    case 1:
    case 4:
    case 5:
        playSFX(6);
        FUN_8001f96c(1, a->a.p.whole, a->y.p.whole, a->b.p.whole);
        break;
    case 2:
    case 8:
        r = 0;
        b->b9e = 1;
        b->b9f = 0;
        b->b68 = 0;
        FUN_8001f96c(1, a->a.p.whole, a->y.p.whole, a->b.p.whole);
        playSFX(6);
        break;
    case 3:
    case 6:
    case 7:
    case 10:
    case 13:
        playSFX(7);
        if (b->w98 != 0) b->w98--;
        FUN_8001f96c(0, a->a.p.whole, a->y.p.whole, a->b.p.whole);
        break;
    case 11:
        playSFX(7);
        b->w98 -= 2;
        if (b->w98 < 0) b->w98 = 0;
        FUN_8001f96c(0, a->a.p.whole, a->y.p.whole, a->b.p.whole);
        break;
    }
    return r;
}
