// FUNC 80037dc0 88 MAIN0
// MATCHING 80037dc0 88
// Ported from psx_tomba (script.c, projectOriginToScreen); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


s32 projectOriginToScreen(s32 arg0, s16* arg1)
{
    SVECTOR v;
    long sxy;
    long p;
    long flag;
    long v2;
    long r;

    v.vx = 0;
    v.vy = 0;
    v.vz = 0;
    r = RotTransPers(&v, &sxy, &p, &flag);
    v2 = sxy;
    arg1[0] = v2;
    arg1[1] = v2 >> 16;
    return r;
}
