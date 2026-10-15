// FUNC 80121934 48 X009
// MATCHING 80121934 48
#include "TOBJ.H"
extern int pushObjectOutOfObjectBoxB(TObj *, TObj *);

void flowerPlatformPushOut(TObj *o, TObj *p)
{
    if (o->type != 0x1c) pushObjectOutOfObjectBoxB(o, p);
}
