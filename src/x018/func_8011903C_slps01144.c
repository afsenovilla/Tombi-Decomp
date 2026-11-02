// VERSION SLPS_01144 x18 8011903C 388
// Matched: each branch stores o->step itself (gcc cross-jumps the stores); E9C0 == 0xff picks 7/6 by EB82.
// JP-only object handler: picks the next step from flag bytes (b68 set) or reacts to the D_8009E4E8 request.
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"

extern unsigned char D_8009E9D0, D_8009E9C0, D_8009EB82, D_8009E9C4, D_8009E9BE, D_8009EB81;
extern unsigned char D_8009E4E8, D_8009E4E9, D_8009E9B3;

void func_8011903C_slps01144(TObj *o)
{
    if (o->b68 != 0) {
        if (D_8009E9D0 == 0xff) o->step = 9;
        else if (D_8009E9C0 == 0xff) {
            if (D_8009EB82 != 0) o->step = 7;
            else o->step = 6;
        }
        else if (D_8009E9C4 == 0) o->step = 1;
        else if (D_8009E9BE == 0) o->step = 2;
        else if (D_8009E9BE != 0xff) o->step = 3;
        else if (D_8009EB81 != 0) o->step = 5;
        else o->step = 4;
        o->state = 0;
        return;
    }
    if (D_8009E4E8 != 0) {
        int x = D_8009E4E9;
        if (x < 0x6b) {
            if (x >= 0x66 || x == 8) goto l180;
        } else if (x == 0x99 && D_8009E9C0 == 0xff && D_8009EB82 != 0 && D_8009E9D0 != 0xff) {
            o->step = 8;
            goto l1ac;
        }
        goto l1b0;
    l180:
        if (D_8009E9B3 != 0 && D_8009E9C4 != 0xff) {
            o->step = 10;
    l1ac:
            o->state = 0;
        }
    l1b0:
        D_8009E4E8 = 0;
    }
}
#endif
