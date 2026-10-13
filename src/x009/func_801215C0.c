// FUNC 801215c0 112 X009
// MATCHING 801215c0 112
#include "TOBJ.H"

extern char *D_1F800278;
extern char *getCollisionColumnPtr(short, unsigned char);
extern short func_8003FF48(TObj *, short, short);

short func_801215C0(TObj *o, short x, short y, unsigned char k)
{
    D_1F800278 = getCollisionColumnPtr(x, k);
    return func_8003FF48(o, x, y);
}
