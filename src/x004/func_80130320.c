// FUNC 80130320 260 X004
// MATCHING 80130320 260
#include "gcd.h"
#include "TOBJ.H"

extern void func_801301B0(TObj *);
extern void func_8012FBF4(TObj *);
extern int ObjCullRegister(TObj *);
extern void freeObjectLayer2(TObj *);

void func_80130320(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_801301B0(o);
        break;
    case 1:
        ObjCullRegister(o);
        switch (o->step) {
        case 0:
            if (GCD.bcdab == 1 && GCD.bcfcb == 0) {
                o->step = 1;
                o->state = 0;
            }
            break;
        case 1:
            func_8012FBF4(o);
            break;
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        freeObjectLayer2(o);
        break;
    }
}
