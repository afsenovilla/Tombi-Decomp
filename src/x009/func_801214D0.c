// FUNC 801214d0 112 X009
// MATCHING 801214d0 112
#include "TOBJ.H"

extern char *D_1F800278;
extern char *func_8003F1BC(short, unsigned char);
extern short FUN_800406e8(TObj *, short, short);

short func_801214D0(TObj *o, short x, short y, unsigned char k)
{
    D_1F800278 = func_8003F1BC(x, k);
    return FUN_800406e8(o, x, y);
}
