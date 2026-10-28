// FUNC 801163d4 692 X011
// MATCHING 801163d4 692
#include "gcd.h"
#include "TOBJ.H"

extern int ObjCullRegister(TObj *o);
extern void ObjFreeDup(TObj *o);
extern int foodForFuelModelStepA(TObj *o);
extern int foodForFuelModelStepB(TObj *o);

void updateFoodForFuelObject(TObj *o)
{
    unsigned char *c;

    switch (o->b04) {
    case 0:
        if (GCD.bce0c == 0xff) {
            o->b04 = 2;
            break;
        }
        o->b04++;
        GCD.bd082 = 0;
        o->b68 = 0;
        if (o->b0c == 0) {
            o->da0 = 0;
            o->active = 2;
        } else if (o->b0c == 1) {
            o->d88 = -0x400;
            o->d84 = 0;
            o->d8c = 0;
            o->ba4 = 0;
            o->box0 = 8;
            o->box1 = 0x10;
            o->box2 = 4;
            o->box3 = 8;
        } else {
            o->d84 = 0;
            o->d88 = 0;
            o->d8c = 0;
            o->active = 2;
        }
        break;
    case 1:
        if (ObjCullRegister(o) == 0) break;
        if (o->b0c != 1) {
            if (GCD.bd082 == 4) {
                o->velX = -0x180;
                o->a.raw += -0x18000;
                o->d8c += 0x80;
            }
            break;
        }
        switch (o->step) {
        case 0:
            if (o->b68 != 0) {
                o->b68 = 0;
                o->substep = 0;
                o->step++;
            }
            break;
        case 1:
            if (foodForFuelModelStepA(o) != 0) {
                o->timer = 0x1e;
                o->step++;
                GCD.bd082 = 1;
            }
            break;
        case 2:
            c = &GCD.bd082;
            if (*c == 2) {
                o->step++;
            }
            if (*c == 4) {
                o->velX = -0x180;
                o->timer = 0x3c;
                o->step = 5;
            }
            break;
        case 3:
            if (foodForFuelModelStepB(o) != 0) {
                GCD.bd082 = 3;
                o->step = 4;
            }
            break;
        case 4:
            break;
        case 5:
            o->a.raw += o->velX << 8;
            if (--o->timer == 0) {
                o->step = 4;
                GCD.bd082 = 5;
            }
            break;
        }
        break;
    case 2:
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}
