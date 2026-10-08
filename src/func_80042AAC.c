// FUNC 80042aac 236 MAIN0
// MATCHING 80042aac 236
#include "TOBJ.H"
extern int func_800425C4();
extern void FUN_8001f96c(int, int, int, int);
extern void playSFX(int);

int func_80042AAC(TObj *a, TObj *b)
{
    int r = 1;
    b->b68 = 1;
    b->b9e = 0;
    switch (func_800425C4()) {
    case 1: case 4: case 5: case 7: case 10:
        r = 1;
        break;
    case 13:
        if ((b->category & 0x7f) == 4) {
            b->b68 = 0;
            return r;
        }
    case 0: case 3: case 6: case 9: case 11: case 12:
        r = 2;
        break;
    case 2: case 8:
        r = 0;
        b->b68 = 0;
        b->b9e = 1;
        b->b9f = 0;
        break;
    }
    FUN_8001f96c(1, a->a.p.whole, a->y.p.whole, a->b.p.whole);
    if ((b->category & 0x7f) == 4)
        playSFX(6);
    else
        playSFX(7);
    return r;
}
