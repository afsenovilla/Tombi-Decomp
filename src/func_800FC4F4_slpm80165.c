// VERSION SLPM_80165 x00 800FC4F4 120
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo-only player reset around playerSetAngleFromSlope (src/playerSetAngleFromSlope.c, inlined): clears the speeds,
// sets the angle from the slope and copies the +0x8C byte to the player extension.
#include "TOBJ.H"
extern TObj *D_8009B194;

static __inline__ void setAngle(TObj *o)
{
    short v = o->wb0;
    if (v < 0) o->wb6 = (short)((v << 2) + 0x100) & 0xff;
    else if (v > 0) o->wb6 = (short)(v << 2) & 0xff;
    else o->wb6 = 0;
}

void func_800FC4F4(TObj *o)
{
    o->b69 = 0;
    *(unsigned char *)&o->wac = 3;
    o->b9c = 0;
    o->velX = 0;
    o->velY = 0;
    o->velH = 0;
    o->velV = 0;
    o->wb2 = 0;
    setAngle(o);
    D_8009B194->d8c = *(unsigned char *)&o->d8c;
}
#endif
