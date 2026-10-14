// FUNC 8012a8b8 344 X004
// MATCHING 8012a8b8 344
#include "spad.h"
#include "gcd.h"
#include "TOBJ.H"

extern int AnimAdvance(TObj *o);
extern int ObjCullRegister(TObj *o);
extern void func_8012A31C(TObj *o);
extern void func_8012A740(TObj *o);
extern void freeObjectLayer2(TObj *o);

void func_8012A8B8(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_8012A740(o);
        break;
    case 1:
        ObjCullRegister(o);
        AnimAdvance(o);
        switch (o->step) {
        case 0:
            if (GCD.bcf2d != 0) break;
            if (o->b68 == 0) break;
            if (SPAD.s246 < 2) {
                o->step++;
                o->state = 0;
            } else {
                o->b68 = 0;
            }
            break;
        case 1:
            func_8012A31C(o);
            break;
        case 3:
            AnimAdvance(o);
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
