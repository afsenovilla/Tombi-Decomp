// FUNC 801168a0 304 X018
// MATCHING 801168a0 304
#include "TOBJ.H"
extern short D_1F800172;
extern unsigned short D_1F80016A, D_1F80016E;
extern unsigned char D_800A60DA, D_8009C93A;

void func_801168A0(TObj *o)
{
    switch (o->state) {
    case 0:
        if (D_1F800172 <= 0)
            break;
        if ((unsigned short)(o->h->p.whole - D_1F80016A + 0x20) > 0x40)
            break;
        if ((unsigned short)(o->y.p.whole - D_1F80016E + 0x30) > 0x60)
            break;
        if (D_800A60DA != 1 && D_800A60DA == 2) {
            o->d88 = 0;
            o->state++;
        }
        break;
    case 1:
        o->d88 = (o->d88 - 0x20) & 0xfff;
        if (o->d88 <= 0xa00)
            o->state++;
        break;
    case 2:
        break;
    case 3:
        o->d88 = (o->d88 + 0x20) & 0xfff;
        if (o->d88 == 0)
            o->state++;
        break;
    case 4:
        if (D_8009C93A != 0)
            o->state = 0;
        break;
    }
}
