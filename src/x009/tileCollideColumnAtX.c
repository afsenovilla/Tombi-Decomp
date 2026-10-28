// FUNC 801215c0 112 X009
// MATCHING 801215c0 112
#include "spad.h"
#include "TOBJ.H"

extern char *getCollisionColumnPtr(short, unsigned char);
extern short tileCollideColumnAt(TObj *, short, short);

short tileCollideColumnAtX(TObj *o, short x, short y, unsigned char k)
{
    (*(char **)&SPAD.p278) = getCollisionColumnPtr(x, k);
    return tileCollideColumnAt(o, x, y);
}
