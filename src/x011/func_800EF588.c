// FUNC 800ef588 164 X011
// MATCHING 800ef588 164
#include "TOBJ.H"
extern unsigned short D_8009D670[];

void func_800EF588(TObj *o)
{
    volatile unsigned short *k = D_8009D670;
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
