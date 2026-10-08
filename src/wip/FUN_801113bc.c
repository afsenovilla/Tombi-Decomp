// FUNC 801113bc 160 X000
#include "raw7.h"
extern unsigned short g610;
extern unsigned char g619;
extern unsigned short g670;
void FUN_801113bc(char *o)
{
    short v = 0x20;
    if (g610 == 7) v = 0x20 >> (3 - g619);
    if (g670 & 0x40) v = S16(o, 0x7e) + v;
    else v = S16(o, 0x7e) - v;
    S16(o, 0x7e) = v;
    if (S16(o, 0x7e) > 0x300) S16(o, 0x7e) = 0x300;
    if (S16(o, 0x7e) < 0xc0) S16(o, 0x7e) = 0xc0;
}
