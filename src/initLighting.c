// FUNC 80021fac 152 MAIN0
// MATCHING 80021fac 152
// VERSION PCPX_96084 tomba 8001AD2C 136
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 version of PAL main0 80021FAC (initLighting.c): SLPS_01144 C of src/initLighting.c (initLighting, twin score 0.667), edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
#include "TOBJ.H"
typedef struct B32 { int w[8]; } B32;
extern B32 BASE_MATRIX;
extern void applyLighting(void);

void initLighting(TObj *o)
{
    *(short *)&o->d = 0x638;
    *((short *)&o->d + 1) = 0x800;
    o->w48 = 0;
    *(B32 *)&o->timer = BASE_MATRIX;
    *(char *)&o->h = 0xc0;
    *((char *)&o->h + 1) = 0xc0;
    *((char *)&o->h + 2) = 0xc0;
    applyLighting();
}
#else
#include "TOBJ.H"
typedef struct B32 { int w[8]; } B32;
extern B32 BASE_MATRIX;
extern void applyLighting(void);

void initLighting(TObj *o)
{
    *(short *)&o->d = 0x638;
    *((short *)&o->d + 1) = 0x800;
    o->w48 = 0;
    *(B32 *)&o->timer = BASE_MATRIX;
    o->timer = 0xd00;
    *((short *)&o->anim + 1) = 0xd00;
    o->animTimer = 0xd00;
    *(char *)&o->h = 0xc0;
    *((char *)&o->h + 1) = 0xc0;
    *((char *)&o->h + 2) = 0xc0;
    applyLighting();
}
#endif
