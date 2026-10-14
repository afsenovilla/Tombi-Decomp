// FUNC 80118418 320 X010
// MATCHING 80118418 320
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
extern unsigned short D_1F80016A, D_1F80016E;

void func_80118418(TObj *o)
{
    switch (o->state) {
    case 0:
        if ((unsigned short)(o->h->p.whole - D_1F80016A + 0x20) < 0x41 &&
            (unsigned short)(o->y.p.whole - D_1F80016E + 0x30) < 0x61) {
            if ((*(unsigned char*)((char*)&PLAYER.obj.da0 + 2)) == 1) break;
            if ((*(unsigned char*)((char*)&PLAYER.obj.da0 + 2)) == 2) {
                o->state++;
                o->velY = 0;
            } else if (GVARS.b3a == 0) {
                o->state = 3;
                o->d88 = 0xa00;
            }
        }
        break;
    case 1:
        o->d88 = (o->d88 - 0x20) & 0xfff;
        if (o->d88 < 0xa01) o->state++;
        break;
    case 2:
        break;
    case 3:
        o->d88 = (o->d88 + 0x20) & 0xfff;
        if (o->d88 == 0) o->state = 0;
        break;
    }
}
