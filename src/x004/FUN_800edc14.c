// FUNC 800edc14 488 X004
// MATCHING 800edc14 488
#include "TOBJ.H"
extern short FUN_8001fddc(int, int);
extern short FUN_8001fdac(int, int);
extern unsigned short DAT_800a6066[];
extern unsigned short DAT_800a60ee;
extern unsigned char DAT_800a60d6[];
extern short DAT_800a60ea;

void FUN_800edc14(TObj *o)
{
    unsigned int ang;
    short v;

    switch (o->state) {
    case 0:
        o->velX = o->h->p.whole;
        o->velY = o->y.p.whole;
        o->velH = 1;
        o->w74 = o->d84;
        o->w76 = o->d88;
        o->d84 = 0;
        o->d88 = 0;
        if (DAT_800a6066[0] & 1) o->state = 2;
        else o->state = 1;
        break;
    case 1:
        ang = DAT_800a60ee;
        ang >>= 4;
        ang &= 0xff;
        o->d84 = FUN_8001fddc(ang, o->velH) << 4;
        o->d88 = FUN_8001fdac(ang, o->velH) << 4;
        if (DAT_800a60d6[0] == 0) {
            o->active = 2;
            o->timer = 10;
            o->b69 = 0;
            o->state = 3;
            o->d84 = o->w74;
            o->d88 = o->w76;
        }
        break;
    case 2:
        ang = 0x100;
        ang -= (short)DAT_800a60ee >> 4;
        ang &= 0xff;
        o->d84 = FUN_8001fddc(ang, o->velH) << 4;
        o->d88 = FUN_8001fdac(ang, o->velH) << 4;
        if (DAT_800a60d6[0] == 0) {
            o->active = 2;
            o->timer = 10;
            o->b69 = 0;
            o->state = 3;
            o->d84 = o->w74;
            o->d88 = o->w76;
        }
        break;
    case 3:
        if (--o->timer <= 0) {
            o->active = 1;
            o->timer = 0;
            o->b69 = 0;
            o->step = 0;
            o->state = 0;
        }
        break;
    }
    switch (DAT_800a60ea) {
    case 0: v = 1; break;
    case 1: v = 2; break;
    case 2: v = 3; break;
    default: v = 4; break;
    }
    o->velH = v;
}
