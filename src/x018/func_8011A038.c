// FUNC 8011a038 380 X018
// MATCHING 8011a038 380
#include "TOBJ.H"

extern unsigned char D_8009CE28, D_8009CE18, D_8009CFDA, D_8009CE1C, D_8009CE16, D_8009CFD9;
extern unsigned char D_8009C940, D_8009C941, D_8009CE0B;

void func_8011A038(TObj *o)
{
    unsigned char *p;
    int k;

    if (o->b68) {
        if (D_8009CE28 == 0xff) {
            o->step = 9;
            o->state = 0;
        } else if (D_8009CE18 == 0xff) {
            if (D_8009CFDA) {
                o->step = 7;
                o->state = 0;
            } else {
                o->step = 6;
                o->state = 0;
            }
        } else if (D_8009CE1C == 0) {
            o->step = 1;
            o->state = 0;
        } else if (D_8009CE16 == 0) {
            o->step = 2;
            o->state = 0;
        } else if (D_8009CE16 != 0xff) {
            o->step = 3;
            o->state = 0;
        } else if (D_8009CFD9) {
            o->step = 5;
            o->state = 0;
        } else {
            o->step = 4;
            o->state = 0;
        }
    } else {
        p = &D_8009C940;
        if (*p == 0) return;
        k = D_8009C941;
        *p = 0;
        switch (k) {
        case 0x99:
            if (D_8009CE18 != 0xff) return;
            if (D_8009CFDA == 0) return;
            if (D_8009CE28 == 0xff) return;
            o->step = 8;
            o->state = 0;
            break;
        case 8:
        case 0x66 ... 0x6a:
            if (D_8009CE0B == 0) return;
            if (D_8009CE1C == 0xff) return;
            o->step = 10;
            o->state = 0;
            break;
        }
    }
}
