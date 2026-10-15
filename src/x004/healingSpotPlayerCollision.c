// FUNC 8011f5b8 32 X004
// MATCHING 8011f5b8 32
#include "TOBJ.H"

extern void func_80043260(TObj *, TObj *);

void healingSpotPlayerCollision(TObj *o, TObj *p)
{
    p->b69 = 0;
    func_80043260(o, p);
}
