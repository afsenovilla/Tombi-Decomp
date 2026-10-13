// FUNC 8011f978 104 X004
// MATCHING 8011f978 104
#include "TOBJ.H"
extern short boxesOverlapSide(TObj *, TObj *);
extern int resolveHitResultA(TObj *, TObj *);
extern short D_1F80019E;

void func_8011F978(TObj *o, TObj *p)
{
    if (p->b0c == 0 && boxesOverlapSide(o, p) != -1) {
        resolveHitResultA(o, p);
        D_1F80019E = 0;
    }
}
