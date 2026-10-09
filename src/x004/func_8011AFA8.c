// FUNC 8011afa8 400 X004
// MATCHING 8011afa8 400
#include "TOBJ.H"

extern unsigned short D_1F80016A, D_1F80016E;
extern unsigned char D_800A60DA, D_8009C93A;
extern signed char D_8009D2B0;

int func_8011AFA8(TObj *o)
{
    switch (o->state) {
    case 0:
        if ((unsigned short)(o->h->p.whole - D_1F80016A + 0x30) >= 0x49) return 0;
        if ((unsigned short)(o->y.p.whole - D_1F80016E + 0x20) >= 0x41) return 0;
        if (D_800A60DA == 1) return 1;
        if (D_800A60DA == 2) {
            o->velY = 0;
            o->state++;
            return 2;
        }
        if (D_8009C93A) return 0;
        if (D_8009D2B0 >= 3) break;
        o->state = 3;
        o->velY = 0xa00;
        o->d88 += 0xa00;
        return 2;
    case 1:
        o->d88 = (o->d88 - 0x20) & 0xfff;
        o->velY = (o->velY - 0x20) & 0xfff;
        if (o->velY > 0xa00) return 0;
        o->state++;
        break;
    case 3:
        o->d88 = (o->d88 + 0x20) & 0xfff;
        o->velY = (o->velY + 0x20) & 0xfff;
        if (o->velY != 0) return 0;
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
