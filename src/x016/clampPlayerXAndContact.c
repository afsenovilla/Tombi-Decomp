// FUNC 80117d34 76 X016
// MATCHING 80117d34 76
#include "player.h"
#include "TOBJ.H"

void clampPlayerXAndContact(TObj *o)
{
    if (o->h->p.whole < (*(Fix16 **)&PLAYER.obj.h)->p.whole) (*(Fix16 **)&PLAYER.obj.h)->p.whole = o->h->p.whole;
    if (o->b68) {
        o->step = 1;
        o->state = 0;
    }
}
