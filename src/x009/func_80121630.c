// FUNC 80121630 112 X009
// MATCHING 80121630 112
#include "TOBJ.H"

extern char *D_1F800278;
extern char *func_8003F1BC(short, unsigned char);
extern short func_80040F78(TObj *, short, short);

short func_80121630(TObj *o, short x, short y, unsigned char k)
{
    D_1F800278 = func_8003F1BC(x, k);
    return func_80040F78(o, x, y);
}
