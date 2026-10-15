// FUNC 80118df8 324 X003
// MATCHING 80118df8 324
#include "spad.h"
#include "TOBJ.H"

extern int Rand(void);

void windStreakRespawn(TObj *o)
{
    short *p = &(*(short*)&SPAD.s186);

    o->a.raw = ((*(short*)((char*)&SPAD.i174 + 2)) - 0x10) << 16;
    if (*p >= -0x103) {
        o->subtype = Rand() & 3;
        o->y.raw = (-0x154 - (Rand() & 0xf) * 14) << 16;
        if (o->subtype == 3 || o->subtype == 0) {
            o->subtype = 4;
        }
    } else {
        o->subtype = Rand() & 7;
        o->y.raw = (*p - (Rand() & 0xf) * 14) << 16;
    }
    switch (o->subtype) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        o->b0b = 1;
        o->b0f = 4;
        o->b.raw = 0;
        break;
    case 5:
    case 6:
        o->b0b = 0;
        o->b0f = 0;
        o->b.raw = 0x2d0000;
        break;
    case 7:
        o->b0b = 0;
        o->b0f = 0;
        o->b.raw = 0x5a0000;
        break;
    }
}
