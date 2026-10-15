// FUNC 8011f5b8 32 X004
// MATCHING 8011f5b8 32
#include "TOBJ.H"

extern void pushObjectOutOfObjectBoxA(TObj *, TObj *);

void healingSpotPlayerCollision(TObj *o, TObj *p)
{
    p->b69 = 0;
    pushObjectOutOfObjectBoxA(o, p);
}
