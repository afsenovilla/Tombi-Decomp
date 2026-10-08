// FUNC 80126fe4 144 X001
// MATCHING 80126fe4 144
#include "TOBJ.H"
extern short D_1F80019E;
extern unsigned short D_1F8003BC;
extern TObj *D_1F8003C0;
extern short FUN_8004b57c(TObj *, TObj *);

void func_80126FE4(TObj *o, TObj *e)
{
    if (e->subtype == 0 && FUN_8004b57c(o, e)) {
        o->b9e = 4;
        o->wba = 0;
        o->velY = 0;
        D_1F80019E = 0;
        o->wb8 = D_1F8003BC;
        e->b6a = 1;
        D_1F8003C0 = e;
        e->animFrame = (o->animFrame & 1) << 1;
    }
}
