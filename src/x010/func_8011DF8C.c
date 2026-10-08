// FUNC 8011df8c 116 X010
// MATCHING 8011df8c 116
#include "TOBJ.H"

extern short D_1F80019E;
extern unsigned short D_1F8003BC;
extern TObj *D_1F8003C0;
extern short FUN_8004b57c(TObj *, TObj *);

void func_8011DF8C(TObj *o, TObj *e)
{
    e->b6a = 0;
    if (FUN_8004b57c(o, e)) {
        o->b9e = 4;
        o->wba = 0;
        o->velY = 0;
        o->wb8 = D_1F8003BC;
        e->b6a = 1;
        D_1F80019E = 0;
        D_1F8003C0 = e;
    }
}
