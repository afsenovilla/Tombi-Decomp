// FUNC 8011b218 544 X004
// MATCHING 8011b218 544
#include "TOBJ.H"

extern unsigned short D_1F80016A, D_1F80016E;
extern unsigned char D_800A60DA, D_8009C93A;
extern signed char D_8009D2B0;

int func_8011B218(TObj *o)
{
    switch (o->state) {
    case 0:
        if (o->subtype == 0xc) {
            if ((unsigned short)(o->h->p.whole - D_1F80016A + 0x40) >= 0x61) return 0;
            if ((unsigned short)(o->y.p.whole - D_1F80016E + 0x30) >= 0x61) return 0;
        } else {
            if ((unsigned short)(o->h->p.whole - D_1F80016A + 0x30) >= 0x49) return 0;
            if ((unsigned short)(o->y.p.whole - D_1F80016E + 0x20) >= 0x41) return 0;
        }
        if (D_800A60DA == 1) return 1;
        if (D_800A60DA == 2) {
            o->d30 = 0;
            o->state++;
            return 2;
        }
        if (D_8009C93A) return 0;
        if (D_8009D2B0 >= 3) break;
        o->state = 3;
        o->d30 = 0;
        o->h->p.whole -= 0x20;
        return 2;
    case 1:
        o->h->raw += -0x18000;
        o->d30 += -0x18000;
        if ((o->d30 >> 16) >= -0x1f) return 0;
        o->state++;
        break;
    case 3:
        o->h->raw += 0x18000;
        o->d30 += 0x18000;
        if ((o->d30 >> 16) < 0x20) return 0;
        o->state++;
        break;
    case 4:
        if (D_8009C93A == 0) return 0;
        o->state = 0;
        break;
    case 2:
        break;
    }
    return 0;
}
