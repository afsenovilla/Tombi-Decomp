// FUNC 8012f35c 404 X004
// MATCHING 8012f35c 404
#include "gcd.h"
#include "gv2.h"
#include "TOBJ.H"

extern void func_8012ECA0(TObj *);
extern void func_8012EE14(TObj *);
extern void paintingBigKeyReward(TObj *);
extern void FUN_80018980(TObj *);

void func_8012F35C(TObj *o)
{
    short i;
    switch (o->b04) {
    case 0:
        if (GV2.bc3 & 8) {
            o->b04 = 3;
        } else if (GCD.bcddc == 0xff) {
            o->b04 = 3;
        } else {
            func_8012ECA0(o);
            o->step = 0;
            o->state = 0;
            o->b04++;
        }
        break;
    case 1:
        switch (o->step) {
        case 0:
            func_8012EE14(o);
            break;
        case 1:
            if (o->state == 0) {
                o->step = 0;
                o->state = 0;
                if (GCD.bcddc != 0xff) {
                    for (i = 0; i < 5; i++) {
                        if (((unsigned char*)&GCD.bcf24)[i] == 0) return;
                    }
                    o->step = 2;
                }
            }
            break;
        case 2:
            paintingBigKeyReward(o);
            break;
        }
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        FUN_80018980(o);
        break;
    }
}
