// FUNC 80111880 156 X004
// MATCHING 80111880 156
#include "TOBJ.H"
extern void FUN_80111590(TObj *);
extern void FUN_80111714(TObj *);

void FUN_80111880(TObj *o)
{
    switch (o->subtype) {
    case 0:
        FUN_80111590(o);
        break;
    case 1:
        FUN_80111714(o);
        break;
    }
    if (o->w22 != 0) {
        o->w22--;
        if (o->w22 <= 0)
            o->active = 1;
    }
    if (o->b6a != 0) {
        o->b6a = 0;
        o->w22 = 3;
    }
}
