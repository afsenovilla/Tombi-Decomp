// FUNC 8011a038 380 X018
// MATCHING 8011a038 380
#include "gcd.h"
#include "gvars.h"
#include "TOBJ.H"


void selectWitchMizunoTalkStep(TObj *o)
{
    unsigned char *p;
    int k;

    if (o->b68) {
        if (GCD.bce28 == 0xff) {
            o->step = 9;
            o->state = 0;
        } else if (GCD.bce18 == 0xff) {
            if (GCD.bcfda) {
                o->step = 7;
                o->state = 0;
            } else {
                o->step = 6;
                o->state = 0;
            }
        } else if (GCD.bce1c == 0) {
            o->step = 1;
            o->state = 0;
        } else if (GCD.bce16 == 0) {
            o->step = 2;
            o->state = 0;
        } else if (GCD.bce16 != 0xff) {
            o->step = 3;
            o->state = 0;
        } else if (GCD.bcfd9) {
            o->step = 5;
            o->state = 0;
        } else {
            o->step = 4;
            o->state = 0;
        }
    } else {
        p = &GVARS.b40;
        if (*p == 0) return;
        k = GVARS.b41;
        *p = 0;
        switch (k) {
        case 0x99:
            if (GCD.bce18 != 0xff) return;
            if (GCD.bcfda == 0) return;
            if (GCD.bce28 == 0xff) return;
            o->step = 8;
            o->state = 0;
            break;
        case 8:
        case 0x66 ... 0x6a:
            if (GCD.bce0b == 0) return;
            if (GCD.bce1c == 0xff) return;
            o->step = 10;
            o->state = 0;
            break;
        }
    }
}
