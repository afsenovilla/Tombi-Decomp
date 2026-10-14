// FUNC 80134c84 116 X003
// MATCHING 80134c84 116
#include "gcd.h"
#include "TOBJ.H"
extern void hidingSonTalk(TObj *);

void stepHidingSonTalk(TObj *o)
{
    switch (o->step) {
    case 0:
        if (GCD.bcdab == 1 && GCD.bcfcd == 0) {
            o->step = 1;
            o->state = 0;
        }
        break;
    case 1:
        hidingSonTalk(o);
        break;
    }
}
