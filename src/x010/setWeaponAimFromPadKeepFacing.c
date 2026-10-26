// FUNC 800ef6d8 136 X010
// MATCHING 800ef6d8 136
#include "TOBJ.H"
extern unsigned short PAD_PRESSED;

void setWeaponAimFromPadKeepFacing(TObj *o)
{
    if (o->animFrame & 1) {
        volatile unsigned short *pad = &PAD_PRESSED;
        if ((*pad & 0x10) == 0) o->animFrame = 3;
        else if ((*pad & 0x80) != 0) o->animFrame = 5;
        else o->animFrame = 7;
    } else {
        volatile unsigned short *pad = &PAD_PRESSED;
        if ((*pad & 0x10) == 0) o->animFrame = 2;
        else if ((*pad & 0x20) != 0) o->animFrame = 4;
        else o->animFrame = 6;
    }
}
