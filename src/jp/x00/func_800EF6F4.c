// JP x00 800EF6F4 164
// Japanese version of PAL x000 800EF588 (setWeaponAimFromPadTurn.c): 128 -> 32768, 32 -> 8192, 16 -> 4096.
#include "gv6.h"
#include "TOBJ.H"

void setWeaponAimFromPadTurn(TObj *o)
{
    volatile unsigned short *k = ((unsigned short*)&GV6.s70);
    unsigned short f = o->animFrame & 1;
    o->animFrame = f;
    if (*k & 0x1000) {
        if (*k & 0x8000) o->animFrame = 5;
        else if (*k & 0x2000) o->animFrame = 4;
        else if (f) o->animFrame = 7;
        else o->animFrame = 6;
    } else {
        if (*k & 0x8000) o->animFrame = 3;
        else if (*k & 0x2000) o->animFrame = 2;
        else if (f) o->animFrame = 3;
        else o->animFrame = 2;
    }
}
