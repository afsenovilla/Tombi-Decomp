// near-twin of PAL spawnLostSeedKidDwarf (JP: the event-flag branch alone, 12 bytes; the spawn body follows as func_80135A2C_slps01144)
// VERSION SLPS_01144 x00 80135A20 12
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
void func_80135A20(TObj *o)
{
    o->b04 = 3;
}
#endif
