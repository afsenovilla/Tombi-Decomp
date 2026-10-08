// FUNC 80110eec 240 X019
// MATCHING 80110eec 240
#include "TOBJ.H"

static __inline__ void inc(TObj *o, short w)
{
    if (w < -0x144) o->wb2 = w + 0x20;
    else if (w < -0x84) o->wb2 = w + 0x22;
    else if (w < 0) o->wb2 = w + 0x26;
    else if (w == 0) o->wb2 = 0x88;
    else if (w < 0x144) o->wb2 = w + 0x10;
    else if (w < 0x500) o->wb2 = w + 0x20;
    else o->wb2 = 0x500;
}

static __inline__ void dec(TObj *o, short w)
{
    if (w >= 0x145) o->wb2 = w - 0x20;
    else if (w >= 0x85) o->wb2 = w - 0x22;
    else if (w > 0) o->wb2 = w - 0x26;
    else if (w == 0) o->wb2 = -0x88;
    else if (w >= -0x143) o->wb2 = w - 0x10;
    else if (w >= -0x4ff) o->wb2 = w - 0x20;
    else o->wb2 = -0x500;
}

void FUN_80110eec(TObj *o)
{
    switch (o->bbe & 1) {
    case 0:
        inc(o, o->wb2);
        break;
    case 1:
        dec(o, o->wb2);
        break;
    }
}
