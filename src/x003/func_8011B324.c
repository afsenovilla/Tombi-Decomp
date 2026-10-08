// FUNC 8011b324 316 X003
// MATCHING 8011b324 316
#include "TOBJ.H"

extern short D_1F80016A, D_1F80016E;
extern short D_8007A1F0[], D_8007A5F0[];
extern int *D_800A6078;
extern int D_800A604C[];
extern int SquareRoot0(int);
extern int FUN_800205d8(int, int);

int func_8011B324(TObj *o)
{
    int dx, dy;
    int a;

    dx = D_1F80016A - o->h->p.whole;
    dy = D_1F80016E - o->y.p.whole;
    if (SquareRoot0(dx * dx + dy * dy) > 0x10) {
    a = (unsigned char)FUN_800205d8(dx, dy);
    o->wb0 = a;
    a += 0x80;
    a &= 0xff;
    dy = (o->velH * D_8007A1F0[a]) >> 12;
    dx = (o->velH * D_8007A5F0[a]) >> 12;
    *D_800A6078 += dx << 8;
    D_800A604C[0] += dy << 8;
    o->velH += 0x80;
    if (o->velH > 0x800) o->velH = 0x800;
    return 0;
    }
    return 1;
}
