// FUNC 80042d5c 288 MAIN0
// score 40: only s-reg assignment differs (game a=s0, b=s1, r=s2; ours a=s1, b=s0)
#include "TOBJ.H"
int func_800425C4(TObj *a, TObj *b);
void playSFX(int);
void FUN_8001f96c(int, int, int, int);
int func_80042D5C(TObj *a, TObj *b)
{
    int r = 1;
    b->b68 = 1;
    b->b9e = 0;
    switch (func_800425C4(a, b)) {
    case 1:
    case 4:
        playSFX(7);
        if (b->w98 != 0) b->w98--;
        break;
    case 2:
    case 8:
        r = 0;
        b->b9e = 1;
        b->b9f = 0;
        b->b68 = 0;
        FUN_8001f96c(1, a->a.p.whole, a->y.p.whole, a->b.p.whole);
        playSFX(6);
        goto out;
    case 3:
    case 6:
    case 7:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
        playSFX(7);
        b->w98 = 0;
        break;
    default:
        goto out;
    case 5:
        playSFX(7);
        b->w98 -= 2;
        if (b->w98 >= 0) break;
    case 0:
        b->w98 = 0;
        break;
    }
    FUN_8001f96c(0, a->a.p.whole, a->y.p.whole, a->b.p.whole);
out:
    return r;
}
