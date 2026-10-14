// JPDEMO x00 800ED424 164
// Demo version of PAL x000 800EF588 (setWeaponAimFromPadTurn.c): d-pad bits 0x1000-0x8000 (PAL 0x10-0x80).
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
