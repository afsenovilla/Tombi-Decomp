// FUNC 80021fac 152 MAIN0
// MATCHING 80021fac 152
// VERSION SLPS_01144 slps01144 80022940 152
#if defined(VERSION_SLPS_01144)
#include "include_asm.h"
INCLUDE_ASM_V("slps01144/nonmatchings/slps01144", initLighting);
#else
#include "TOBJ.H"
typedef struct B32 { int w[8]; } B32;
extern B32 DAT_1f8000f8;
extern void applyLighting(void);

void initLighting(TObj *o)
{
    *(short *)&o->d = 0x638;
    *((short *)&o->d + 1) = 0x800;
    o->w48 = 0;
    *(B32 *)&o->timer = DAT_1f8000f8;
    o->timer = 0xd00;
    *((short *)&o->anim + 1) = 0xd00;
    o->animTimer = 0xd00;
    *(char *)&o->h = 0xc0;
    *((char *)&o->h + 1) = 0xc0;
    *((char *)&o->h + 2) = 0xc0;
    applyLighting();
}
#endif
