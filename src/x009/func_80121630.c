// FUNC 80121630 112 X009
// MATCHING 80121630 112
#include "TOBJ.H"

extern char *D_1F800278;
extern char *getCollisionColumnPtr(short, unsigned char);
extern short func_80040F78(TObj *, short, short);

short func_80121630(TObj *o, short x, short y, unsigned char k)
{
    D_1F800278 = getCollisionColumnPtr(x, k);
    return func_80040F78(o, x, y);
}
