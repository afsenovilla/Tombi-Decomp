// FUNC 8011aa04 320 X009
// MATCHING 8011aa04 320
#include "TOBJ.H"
extern unsigned short D_1F80016A, D_1F80016E;
extern unsigned char D_800A60DA, D_8009C93A;

void func_8011AA04(TObj *o)
{
    switch (o->state) {
    case 0:
        if ((unsigned short)(o->h->p.whole - D_1F80016A + 0x20) < 0x41 &&
            (unsigned short)(o->y.p.whole - D_1F80016E + 0x30) < 0x61) {
            if (D_800A60DA == 1) break;
            if (D_800A60DA == 2) {
                o->state++;
                o->velY = 0;
            } else if (D_8009C93A == 0) {
                o->state = 3;
                o->d88 = 0xa00;
            }
        }
        break;
    case 1:
        o->d88 = (o->d88 - 0x20) & 0xfff;
        if (o->d88 < 0xa01) o->state++;
        break;
    case 2:
        break;
    case 3:
        o->d88 = (o->d88 + 0x20) & 0xfff;
        if (o->d88 == 0) o->state = 0;
        break;
    }
}
