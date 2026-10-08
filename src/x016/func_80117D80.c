// FUNC 80117d80 132 X016
// MATCHING 80117d80 132
#include "TOBJ.H"
extern Fix16 *D_800A6078;
extern void func_80117B40(TObj *);

void func_80117D80(TObj *o)
{
    switch (o->step) {
    case 0:
        if (o->h->p.whole < D_800A6078->p.whole) {
            D_800A6078->p.whole = o->h->p.whole;
        }
        if (o->b68) {
            o->step = 1;
            o->state = 0;
        }
        break;
    case 1:
        func_80117B40(o);
        break;
    }
}
