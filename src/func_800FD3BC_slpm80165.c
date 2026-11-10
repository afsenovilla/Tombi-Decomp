// VERSION SLPM_80165 x00 800FD3BC 136
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of playerBiteAttachSlope (src/x009/playerBiteAttachSlope.c) with playerSetAngleFromSlope inline.
#include "TOBJ.H"
#include "raw7.h"

extern TObj *BITTEN_OBJECT;

static __inline__ void demoSetAngleFromSlope(TObj *o)
{
    short v = o->wb0;
    if (v < 0) o->wb6 = (short)((v << 2) + 0x100) & 0xff;
    else if (v > 0) o->wb6 = (short)(v << 2) & 0xff;
    else o->wb6 = 0;
}

void playerBiteAttachSlope(TObj *o)
{
    o->b69 = 0;
    (*(unsigned char*)&o->wac) = 3;
    o->b9c = 0;
    o->velX = 0;
    o->velY = 0;
    o->velH = 0;
    o->velV = 0;
    o->wb2 = 0;
    demoSetAngleFromSlope(o);
    BITTEN_OBJECT->d8c = (0x100 - o->d8c) & 0xff;
    o->state = 3;
}
#endif
