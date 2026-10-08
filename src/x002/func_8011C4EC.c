// FUNC 8011c4ec 304 X002
// MATCHING 8011c4ec 304
#include "TOBJ.H"

extern unsigned char D_8009CE41, D_8009CFF7, D_8009D2C3;
extern Fix16 *D_800A6078;
extern unsigned char D_800A60D8, D_800A60E0;
extern void func_8011A114(TObj *);
extern void func_8011BDA8(TObj *);

void func_8011C4EC(TObj *o)
{
    switch (o->step) {
    case 0:
        if (D_8009CE41 == 0xff) {
            o->step = 3;
            o->state = 0;
        } else if (D_8009CFF7 == 6) {
            o->step = 2;
            o->state = 0;
        } else if (D_8009CE41 == 0 && D_8009D2C3 == 0x7f) {
            o->step = 1;
            o->state = 0;
        } else {
            o->step = 0;
            o->state = 0;
        }
        break;
    case 1:
        func_8011A114(o);
        break;
    case 2:
        func_8011BDA8(o);
        break;
    case 3:
        if ((unsigned short)(D_800A6078->p.whole - 0x92) < 0x20) {
            D_800A60D8 = 1;
            D_800A60E0 = 8;
        } else {
            D_800A60D8 = 0;
            D_800A60E0 = 0;
        }
        break;
    }
}
