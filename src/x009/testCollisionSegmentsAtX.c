// FUNC 80121630 112 X009
// MATCHING 80121630 112
#include "spad.h"
#include "TOBJ.H"

extern char *getCollisionColumnPtr(short, unsigned char);
extern short testCollisionSegmentList(TObj *, short, short);

short func_80121630(TObj *o, short x, short y, unsigned char k)
{
    (*(char **)&SPAD.p278) = getCollisionColumnPtr(x, k);
    return testCollisionSegmentList(o, x, y);
}
