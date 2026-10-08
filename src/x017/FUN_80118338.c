// FUNC 80118338 80 X017
// MATCHING 80118338 80
#include "TOBJ.H"
extern void FUN_8001fec0(TObj *);
extern void FUN_80118110(TObj *);

void FUN_80118338(TObj *o)
{
    switch (o->step) {
    case 0:
        FUN_8001fec0(o);
        break;
    case 1:
        FUN_80118110(o);
        break;
    }
}
