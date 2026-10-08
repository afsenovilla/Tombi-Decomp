// FUNC 80126f64 128 X001
// MATCHING 80126f64 128
#include "TOBJ.H"

extern short D_1F80019E;
extern short D_1F8003BC;
extern TObj *D_1F8003C0;
extern short FUN_8004b57c(TObj *, TObj *);

void func_80126F64(TObj *o, TObj *e)
{
    if (e->subtype && FUN_8004b57c(o, e)) {
        o->b9e = 4;
        o->wba = 0;
        o->velY = 0;
        D_1F80019E = 0;
        o->wb8 = D_1F8003BC;
        e->b68 = 1;
        D_1F8003C0 = e;
    }
}
