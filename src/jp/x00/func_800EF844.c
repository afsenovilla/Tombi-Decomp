// JP x00 800EF844 136
// Japanese version of PAL x000 800EF6D8 (setWeaponAimFromPadKeepFacing.c): 128 -> 32768, 32 -> 8192, 16 -> 4096.
#include "TOBJ.H"
extern unsigned short DAT_8009d670;

void setWeaponAimFromPadKeepFacing(TObj *o)
{
    if (o->animFrame & 1) {
        volatile unsigned short *pad = &DAT_8009d670;
        if ((*pad & 0x1000) == 0) o->animFrame = 3;
        else if ((*pad & 0x8000) != 0) o->animFrame = 5;
        else o->animFrame = 7;
    } else {
        volatile unsigned short *pad = &DAT_8009d670;
        if ((*pad & 0x1000) == 0) o->animFrame = 2;
        else if ((*pad & 0x2000) != 0) o->animFrame = 4;
        else o->animFrame = 6;
    }
}
