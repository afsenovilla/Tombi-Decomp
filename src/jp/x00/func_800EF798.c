// JP x00 800EF798 172
// Japanese version of PAL x000 800EF62C (setWeaponAimFromPad.c): 128 -> 32768, 32 -> 8192, 16 -> 4096.
#include "gv6.h"
#include "TOBJ.H"

void setWeaponAimFromPad(TObj *o)
{
    volatile unsigned short *k = &GV6.s70;
    unsigned short f = o->animFrame & 1;

    o->animFrame = f;
    if (*k & 0x1000) {
        if (*k & 0x8000) {
            o->animFrame = 5;
        } else if (*k & 0x2000) {
            o->animFrame = 4;
        } else if (f) {
            o->animFrame = 7;
        } else {
            o->animFrame = 6;
        }
    } else {
        if (*k & 0x8000) {
            o->animFrame = 1;
        } else if (*k & 0x2000) {
            o->animFrame = 0;
        } else if (f) {
            o->animFrame = 3;
        } else {
            o->animFrame = 2;
        }
    }
}
