// FUNC 800f1e48 92 I0NFO2
// MATCHING 800f1e48 92
#include "TOBJ.H"
extern void func_8001FE94(TObj *o, int n);
extern void **D_800FEEC0[][3];

void func_800F1E48(TObj *o, int i, short n)
{
    short f = o->animFrame + i;
    o->anim = D_800FEEC0[o->subtype][0][f];
    func_8001FE94(o, n);
}
