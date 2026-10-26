// FUNC 800ef6d8 136 X000
// MATCHING 800ef6d8 136
// VERSION SLPS_01144 x00 800EF844 136
// VERSION SLPM_80165 x00 800ED574 136
#if defined(VERSION_SLPS_01144)
// Japanese version of PAL x000 800EF6D8 (setWeaponAimFromPadKeepFacing.c): 128 -> 32768, 32 -> 8192, 16 -> 4096.
#include "TOBJ.H"
extern unsigned short PAD_PRESSED;

void setWeaponAimFromPadKeepFacing(TObj *o)
{
    if (o->animFrame & 1) {
        volatile unsigned short *pad = &PAD_PRESSED;
        if ((*pad & 0x1000) == 0) o->animFrame = 3;
        else if ((*pad & 0x8000) != 0) o->animFrame = 5;
        else o->animFrame = 7;
    } else {
        volatile unsigned short *pad = &PAD_PRESSED;
        if ((*pad & 0x1000) == 0) o->animFrame = 2;
        else if ((*pad & 0x2000) != 0) o->animFrame = 4;
        else o->animFrame = 6;
    }
}
#elif defined(VERSION_SLPM_80165)
// Demo version of PAL x000 800EF6D8 (setWeaponAimFromPadKeepFacing.c): d-pad bits 0x1000-0x8000 (PAL 0x10-0x80).
#include "TOBJ.H"
extern unsigned short PAD_PRESSED;

void setWeaponAimFromPadKeepFacing(TObj *o)
{
    if (o->animFrame & 1) {
        volatile unsigned short *pad = &PAD_PRESSED;
        if ((*pad & 0x1000) == 0) o->animFrame = 3;
        else if ((*pad & 0x8000) != 0) o->animFrame = 5;
        else o->animFrame = 7;
    } else {
        volatile unsigned short *pad = &PAD_PRESSED;
        if ((*pad & 0x1000) == 0) o->animFrame = 2;
        else if ((*pad & 0x2000) != 0) o->animFrame = 4;
        else o->animFrame = 6;
    }
}
#else
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
#endif
