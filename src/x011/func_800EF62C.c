// FUNC 800ef62c 172 X011
// MATCHING 800ef62c 172
#include "TOBJ.H"
extern unsigned short D_8009D670;

void func_800EF62C(TObj *o)
{
    volatile unsigned short *k = &D_8009D670;
    unsigned short f = o->animFrame & 1;

    o->animFrame = f;
    if (*k & 0x10) {
        if (*k & 0x80) {
            o->animFrame = 5;
        } else if (*k & 0x20) {
            o->animFrame = 4;
        } else if (f) {
            o->animFrame = 7;
        } else {
            o->animFrame = 6;
        }
    } else {
        if (*k & 0x80) {
            o->animFrame = 1;
        } else if (*k & 0x20) {
            o->animFrame = 0;
        } else if (f) {
            o->animFrame = 3;
        } else {
            o->animFrame = 2;
        }
    }
}
