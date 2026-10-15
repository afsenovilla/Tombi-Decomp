// FUNC 8012f094 120 X004
// MATCHING 8012f094 120
#include "gcd.h"
#include "TOBJ.H"

void checkBigKeyPaintingPanels(TObj *o)
{
    short i;

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
}
