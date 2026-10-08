// FUNC 8011f38c 108 X003
// MATCHING 8011f38c 108
#include "TOBJ.H"
extern short FUN_8004b57c(TObj *, TObj *);
extern short D_1F80019E;
extern TObj *D_1F8003C0;
extern short D_1F8003BC;

void func_8011F38C(TObj *o, TObj *p)
{
    p->b6a = 0;
    if (FUN_8004b57c(o, p)) {
        o->b9e = 4;
        o->wba = 0;
        o->velY = 0;
        D_1F80019E = 0;
        D_1F8003C0 = p;
        o->wb8 = D_1F8003BC;
    }
}
