// FUNC 80134c84 116 X003
// MATCHING 80134c84 116
#include "gcd.h"
#include "TOBJ.H"
extern void func_801346C8(TObj *);

void func_80134C84(TObj *o)
{
    switch (o->step) {
    case 0:
        if (GCD.bcdab == 1 && GCD.bcfcd == 0) {
            o->step = 1;
            o->state = 0;
        }
        break;
    case 1:
        func_801346C8(o);
        break;
    }
}
