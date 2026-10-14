// JP slps01144 800214A4 172
// Japanese version of PAL main0 8002052C (despawnObjectOffscreenMargin.c): 160 -> 152.
#include "spad.h"
#include "TOBJ.H"
extern short D_800A4582;

void despawnObjectOffscreenMargin(TObj *o, int a)
{
    short y;
    if (o->active == 0) return;
    y = o->y.p.whole;
    if (D_800A4582 + 0x98 > y) {
        if ((unsigned short)(o->h->p.whole - (*(unsigned short*)((char*)&SPAD.i174 + 2)) + a) < (short)a * 2 + 0x140 &&
            (unsigned short)(SPAD.s186 - y + a) < (short)a * 2 + 0xf0) return;
    }
    o->b04 = 3;
    o->active = 2;
    o->visible = 0;
}
