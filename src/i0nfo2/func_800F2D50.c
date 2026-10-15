// FUNC 800f2d50 116 I0NFO2
// MATCHING 800f2d50 116
#include "TOBJ.H"
#include "raw7.h"
extern void func_8001FE94(TObj *o, int n);
extern void **D_800FEEFC[][3];

void func_800F2D50(TObj *o, int i, short n)
{
    short f = o->animFrame + i;
    o->anim = D_800FEEFC[o->subtype][0][f];
    func_8001FE94(o, n);
    S16(o, 0xD2) = f;
}
