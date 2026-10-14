// FUNC 80121540 128 X009
// MATCHING 80121540 128
#include "spad.h"
#include "TOBJ.H"

extern char *getCollisionColumnPtr(short, unsigned char);
extern short FUN_800416b8(TObj *, short, short, int);

short func_80121540(TObj *o, short x, short y, int k)
{
    (*(char **)&SPAD.p278) = getCollisionColumnPtr(x, k);
    return FUN_800416b8(o, x, y, k & 0xff);
}
