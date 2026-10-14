// FUNC 8012a670 208 X004
// MATCHING 8012a670 208
#include "spad.h"
#include "gcd.h"
#include "TOBJ.H"

extern int AnimAdvance(TObj *o);
extern void func_8012A31C(TObj *o);

void func_8012A670(TObj *o)
{
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
}
