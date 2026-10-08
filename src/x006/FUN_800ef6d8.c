// FUNC 800ef6d8 136 X006
// MATCHING 800ef6d8 136
#include "TOBJ.H"
extern unsigned short DAT_8009d670;

void FUN_800ef6d8(TObj *o)
{
    if (o->animFrame & 1) {
        volatile unsigned short *pad = &DAT_8009d670;
        if ((*pad & 0x10) == 0) o->animFrame = 3;
        else if ((*pad & 0x80) != 0) o->animFrame = 5;
        else o->animFrame = 7;
    } else {
        volatile unsigned short *pad = &DAT_8009d670;
        if ((*pad & 0x10) == 0) o->animFrame = 2;
        else if ((*pad & 0x20) != 0) o->animFrame = 4;
        else o->animFrame = 6;
    }
}
