// FUNC 8011ac58 320 X000
// MATCHING 8011ac58 320
#include "TOBJ.H"
extern unsigned char DAT_800a603c[];
extern unsigned char DAT_800a603d, DAT_800a603e, DAT_8009c93f, DAT_800a60a1;
extern unsigned char DAT_8009cdae;
extern unsigned char DAT_800a603c_b;
extern short DAT_800a6066, DAT_800a604e;

void FUN_8011ac58(TObj *o)
{
    switch (o->step) {
    case 1:
        switch (o->state) {
        case 0:
            o->timer = 0xf0;
            o->state++;
        case 1:
            o->d34++;
            if (--o->timer == 0) {
                o->step = 2;
                o->state = 0;
                if (DAT_800a603c[0] != 1) {
                    DAT_800a6066 = 0;
                    DAT_800a603c[0] = 1;
                    DAT_800a603d = 0;
                    DAT_800a603e = 0;
                    DAT_8009c93f = 0;
                }
            }
        }
        break;
    case 5:
        if (DAT_8009cdae == 0xff && DAT_800a604e > -200 && DAT_800a60a1 != 0 && DAT_800a603c_b != 1) {
            o->step = 1;
            o->state = 0;
        }
        break;
    case 0: case 2: case 3: case 4:
        break;
    case 6:
        o->d34 = 0x54;
        break;
    }
}
