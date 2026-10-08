// FUNC 8011b460 296 X003
// MATCHING 8011b460 296
#include "TOBJ.H"

extern unsigned char D_1F8001A4;
extern unsigned char D_800A6038;
extern unsigned char D_800A60E4;
extern unsigned char D_8009C940;
extern short D_1F80016A, D_1F80016E, D_1F800172;
extern int SquareRoot0(int);
extern int FUN_800205d8(int, int);

int func_8011B460(TObj *o)
{
    int dx, dy;
    int a;

    if (D_1F8001A4 != 0) goto ret0;
    if (D_800A6038 != 1) return 0;
    if (D_800A60E4 == 2) return 0;
    if (D_8009C940 != 0) return 0;
    dx = D_1F800172 - o->d->p.whole + 0x2d;
    if ((unsigned short)dx >= 0x5b) return 0;
    dx = D_1F80016A - o->h->p.whole;
    dy = D_1F80016E - o->y.p.whole;
    a = FUN_800205d8(dx, dy);
    o->wb0 = (unsigned char)a;
    if ((unsigned char)(a - o->d38 + 0x30) >= 0x61) {
    ret0:
        return 0;
    }
    return SquareRoot0(dx * dx + dy * dy) < 0x29;
}
