// FUNC 80117d34 76 X016
// MATCHING 80117d34 76
#include "TOBJ.H"
extern Fix16 *D_800A6078;

void func_80117D34(TObj *o)
{
    if (o->h->p.whole < D_800A6078->p.whole) D_800A6078->p.whole = o->h->p.whole;
    if (o->b68) {
        o->step = 1;
        o->state = 0;
    }
}
