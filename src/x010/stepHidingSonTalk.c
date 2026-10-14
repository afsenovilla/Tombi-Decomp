// FUNC 8012de2c 116 X010
// MATCHING 8012de2c 116
#include "gcd.h"
#include "TOBJ.H"
extern void hidingSonTalk(TObj *);

void stepHidingSonTalk(TObj *o)
{
    switch (o->step) {
    case 0:
        if (GCD.bcdab == 1 && GCD.bcfcc == 0) {
            o->step = 1;
            o->state = 0;
        }
        break;
    case 1:
        hidingSonTalk(o);
        break;
    }
}
