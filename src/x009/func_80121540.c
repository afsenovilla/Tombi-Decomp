// FUNC 80121540 128 X009
// MATCHING 80121540 128
#include "TOBJ.H"

extern char *D_1F800278;
extern char *func_8003F1BC(short, unsigned char);
extern short FUN_800416b8(TObj *, short, short, int);

short func_80121540(TObj *o, short x, short y, int k)
{
    D_1F800278 = func_8003F1BC(x, k);
    return FUN_800416b8(o, x, y, k & 0xff);
}
