// FUNC 800f3e2c 116 I0NFO2
// MATCHING 800f3e2c 116
#include "TOBJ.H"
#include "raw7.h"
extern void func_8001FE94(TObj *o, int n);
extern void **D_800FEF6C[][3];

void func_800F3E2C(TObj *o, int i, short n)
{
    short f = o->animFrame + i;
    o->anim = D_800FEF6C[o->subtype][0][f];
    func_8001FE94(o, n);
    S16(o, 0xD2) = f;
}
