// FUNC 80125e00 436 X010
// MATCHING 80125e00 436
#include "TOBJ.H"

extern unsigned char D_8009D0E7[];
extern unsigned char D_8009CE22, D_8009CE40, D_8009CE18, D_8009CE23;
extern unsigned char D_8009D0FD, D_8009D101, D_8009D102, D_8009D103, D_8009D117;

void func_80125E00(TObj *o)
{
    short i;
    short all;
    short n;

    if (o->b68) {
        o->animFrame = o->b68 & 1;
        all = 1;
        for (i = 0; i < 10; i++) {
            if (!D_8009D0E7[i]) all = 0;
        }
        if (D_8009CE22 && all) {
            o->step = 2;
            o->state = 0;
        } else if (D_8009CE22 == 0xff) {
            if (D_8009CE40 && D_8009CE18 == 0xff) {
                n = D_8009D0FD != 0;
                if (D_8009D101) n++;
                if (D_8009D102) n++;
                if (D_8009D103) n++;
                if (D_8009D117) n++;
                if (n == 5) {
                    o->step = 3;
                    o->state = 5;
                } else {
                    switch (D_8009CE23) {
                    case 0:
                        o->step = 3;
                        o->state = 0;
                        break;
                    case 0xff:
                        o->step = 3;
                        o->state = 0xb;
                        break;
                    default:
                        o->step = 3;
                        o->state = 4;
                        break;
                    }
                }
            } else {
                o->step = 2;
                o->state = 8;
            }
        } else {
            o->step = 1;
            o->state = 0;
        }
        o->b68 = 0;
    }
}
