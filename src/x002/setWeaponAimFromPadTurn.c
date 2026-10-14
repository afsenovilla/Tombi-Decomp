// FUNC 800ef588 164 X002
// MATCHING 800ef588 164
#include "gv6.h"
#include "TOBJ.H"

void setWeaponAimFromPadTurn(TObj *o)
{
    volatile unsigned short *k = ((unsigned short*)&GV6.s70);
    unsigned short f = o->animFrame & 1;
    o->animFrame = f;
    if (*k & 0x10) {
        if (*k & 0x80) o->animFrame = 5;
        else if (*k & 0x20) o->animFrame = 4;
        else if (f) o->animFrame = 7;
        else o->animFrame = 6;
    } else {
        if (*k & 0x80) o->animFrame = 3;
        else if (*k & 0x20) o->animFrame = 2;
        else if (f) o->animFrame = 3;
        else o->animFrame = 2;
    }
}
