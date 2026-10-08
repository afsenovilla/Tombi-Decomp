// FUNC 8011dc4c 252 X003
// MATCHING 8011dc4c 252
#include "TOBJ.H"

extern int D_8009F0EC;
extern unsigned short D_8009C962, D_8009C982;
extern unsigned char D_8009CDC3, D_8009C93A, D_8009D2B0, D_8009D2C2;

void func_8011DC4C(TObj *o)
{
    unsigned char *p;

    D_8009F0EC = 0;
    if ((D_8009C962 == 1 || D_8009C962 == 5) && D_8009C982 == 1) {
        o->b04 = 5;
        o->step = 9;
    }
    if ((D_8009C962 == 0 || D_8009C962 == 4) && D_8009CDC3 != 0xff && D_8009C982 == 4) {
        D_8009C93A = 1;
        D_8009D2B0 = 0;
        o->active = 1;
        o->b04 = 1;
        o->step = 2;
        o->state = 3;
        o->substep = 0;
    }
    p = &D_8009D2C2;
    if (*p == 1) {
        o->active = 5;
        o->b04 = 5;
        o->step = 0x40;
        o->visible = 0;
        D_8009C93A = 1;
        *p = 2;
    }
}
