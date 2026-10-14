// FUNC 8013013c 116 X004
// MATCHING 8013013c 116
#include "gcd.h"
#include "TOBJ.H"
extern void hidingSonTalk(TObj *);

void func_8013013C(TObj *o)
{
    switch (o->step) {
    case 0:
        if (GCD.bcdab == 1 && GCD.bcfcb == 0) {
            o->step = 1;
            o->state = 0;
        }
        break;
    case 1:
        hidingSonTalk(o);
        break;
    }
}
