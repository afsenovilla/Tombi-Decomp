// FUNC 8012de2c 116 X010
// MATCHING 8012de2c 116
#include "gcd.h"
#include "TOBJ.H"
extern void func_8012D8E4(TObj *);

void func_8012DE2C(TObj *o)
{
    switch (o->step) {
    case 0:
        if (GCD.bcdab == 1 && GCD.bcfcc == 0) {
            o->step = 1;
            o->state = 0;
        }
        break;
    case 1:
        func_8012D8E4(o);
        break;
    }
}
