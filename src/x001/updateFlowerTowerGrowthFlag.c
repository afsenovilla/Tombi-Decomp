// FUNC 8011ffbc 304 X001
// MATCHING 8011ffbc 304
#include "gcd.h"
#include "gv2.h"
#include "TOBJ.H"
extern void cullObjectToDrawListMargin(TObj *, int);
extern void ObjFreeDup(TObj *);

void updateFlowerTowerGrowthFlag(TObj *o)
{
    switch (o->b04) {
    case 0:
        if (!(GV2.bc3 & 1)) {
            o->b04 = 3;
            break;
        }
        o->b04++;
        o->active = 2;
        if (GCD.bce48) {
            o->step = 1;
        } else {
            o->step = 0;
        }
        break;
    case 1:
        switch (o->step) {
        case 0:
            switch (o->state) {
            case 0: {
                unsigned char *c = &GCD.bd081;
                if (*c >= 4) {
                    *c = 5;
                    o->state++;
                }
                break;
            }
            case 1:
                cullObjectToDrawListMargin(o, 0x90);
                break;
            }
            break;
        case 1:
            cullObjectToDrawListMargin(o, 0x90);
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
