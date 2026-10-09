// FUNC 8011a6c4 432 X003
// MATCHING 8011a6c4 432
#include "TOBJ.H"

extern unsigned short D_1F80016A, D_1F80016E;
extern unsigned char D_800A60DA, D_800A60DB, D_8009C93A;

int func_8011A6C4(TObj *o)
{
    switch (o->state) {
    case 0:
        if ((unsigned short)(o->h->p.whole - D_1F80016A + 0x20) > 0x40) return 0;
        if ((unsigned short)(o->y.p.whole - D_1F80016E + 0x30) > 0x60) break;
        if (D_800A60DA == 1) return 1;
        if (D_800A60DA == 2) {
            o->d30 = 0;
            o->state++;
            return 2;
        }
        if (D_8009C93A) return 0;
        o->state = 3;
        o->d30 = 0;
        o->h->p.whole -= 0x20;
        return 2;
    case 1:
        o->h->raw += -0x18000;
        o->d30 += -0x18000;
        if ((o->d30 >> 16) < -0x1f) o->state++;
        break;
    case 2:
        if (D_800A60DB == 2) o->state++;
        break;
    case 3:
        if (!D_800A60DB) o->state++;
        break;
    case 4:
        o->h->raw += 0x18000;
        o->d30 += 0x18000;
        if (o->d30 >= 0) o->state = 0;
        break;
    }
    return 0;
}
