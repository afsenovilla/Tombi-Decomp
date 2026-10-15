// FUNC 800f5590 136 I0NFO2
// MATCHING 800f5590 136
#include "TOBJ.H"
#include "raw7.h"
extern void func_8001FE94(TObj *o, int n);
extern void **D_800FEFFC[][3];

void func_800F5590(TObj *o, int i, short n)
{
    short f = o->animFrame + i;
    if (f != U16(o, 0xD2)) {
        o->anim = D_800FEFFC[o->subtype][0][f];
        func_8001FE94(o, n);
        S16(o, 0xD2) = f;
    }
}
