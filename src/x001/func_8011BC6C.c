// FUNC 8011bc6c 280 X001
// MATCHING 8011bc6c 280
#include "TOBJ.H"
extern unsigned short D_8009C962;
extern short D_1F80016A, D_1F80016E;

void func_8011BC6C(TObj *o)
{
    switch (o->state) {
    case 0:
        switch (D_8009C962) {
        case 0:
        case 1:
            o->state++;
            break;
        case 2:
            o->d88 = 0x800;
            o->state = 4;
            break;
        }
        break;
    case 1:
        if (D_1F80016E >= -0x140 && (unsigned short)(D_1F80016A - 0xb23) < 0x95)
            o->state++;
        break;
    case 2:
        o->d88 = (o->d88 + 8) & 0xfff;
        if (o->d88 >= 0x600) o->state++;
        break;
    case 3:
        if (o->visible == 0 && ((TObj *)o->d94)->visible == 0) {
            o->d88 = 0;
            o->state = 1;
        }
        break;
    case 4:
        break;
    }
}
