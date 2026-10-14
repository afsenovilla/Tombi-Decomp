// FUNC 801168a0 304 X018
// MATCHING 801168a0 304
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
extern short D_1F800172;
extern unsigned short D_1F80016A, D_1F80016E;

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
        if ((*(unsigned char*)((char*)&PLAYER.obj.da0 + 2)) != 1 && (*(unsigned char*)((char*)&PLAYER.obj.da0 + 2)) == 2) {
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
        if (GVARS.b3a != 0)
            o->state = 0;
        break;
    }
}
