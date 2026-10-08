// FUNC 800429d0 220 MAIN0
// MATCHING 800429d0 220
#include "TOBJ.H"
extern int func_800425C4();
extern void FUN_8001f96c(int a, int x, int y, int z);
extern void playSFX(int n);

int func_800429D0(TObj *a, TObj *o)
{
    int r;
    o->b68 = 1;
    o->b9e = 0;
    switch (func_800425C4(a, o)) {
    case 1: case 4: r = 1; break;
    case 5: r = 2; break;
    case 7: case 10: r = 3; break;
    case 11: r = 4; break;
    case 3: case 6: case 9: case 12: r = 5; break;
    case 0: r = 6; break;
    case 13: r = 7; break;
    case 2: case 8:
        r = 0;
        o->b68 = 0;
        o->b9e = 1;
        o->b9f = 0;
        FUN_8001f96c(1, a->a.p.whole, a->y.p.whole, a->b.p.whole);
        playSFX(6);
        break;
    }
    return r;
}
