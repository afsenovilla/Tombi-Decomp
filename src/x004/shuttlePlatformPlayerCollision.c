// FUNC 8011f4dc 140 X004
// MATCHING 8011f4dc 140
#include "TOBJ.H"
extern short LIST_COUNTER;
extern short pushObjectOutOfObjectBoxA(TObj *, TObj *);

void shuttlePlatformPlayerCollision(TObj *o, TObj *e)
{
    if (o->b9e == 0) {
        e->b69 = 0;
        if (pushObjectOutOfObjectBoxA(o, e) == 1) {
            LIST_COUNTER = 0;
            o->h->raw += e->velH << 8;
            o->y.raw += e->velV << 8;
        }
    }
}
