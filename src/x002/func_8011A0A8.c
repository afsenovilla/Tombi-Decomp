// FUNC 8011a0a8 108 X002
// MATCHING 8011a0a8 108
#include "TOBJ.H"
extern unsigned char D_8009CE41;
extern unsigned char D_8009CFF7;
extern unsigned char D_8009D2C3;

void func_8011A0A8(TObj *o)
{
    unsigned char c = D_8009CE41;

    if (c == 0xff) {
        o->step = 3;
    } else if (D_8009CFF7 == 6) {
        o->step = 2;
    } else if (c == 0 && D_8009D2C3 == 0x7f) {
        o->step = 1;
    } else {
        o->step = 0;
    }
    o->state = 0;
}
