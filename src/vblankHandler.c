// FUNC 800173d8 60 MAIN0
// MATCHING 800173d8 60
// VERSION SLPM_80165 slpm80165 80016030 192
// Ported from psx_tomba (task.c, vblankHandler); MIT licence of the original project.
#if defined(VERSION_SLPM_80165)
// Demo version of PAL main0 800173D8 (vblankHandler.c): JP C of src/vblankHandler_slps01144.c (vblankHandler) (1.000 masked) [-O2 -G0] (tools/demo_sweep.py)
extern volatile int D_800A1380[];
extern volatile unsigned short D_800A1382;
extern volatile unsigned short D_800A1384;
extern volatile unsigned short D_800A1386 asm("D_800A1384+2");
extern unsigned short D_8009E4B6;
extern volatile unsigned short D_1F8001E8;
extern unsigned short FRAME_COUNT;
extern unsigned short D_1F8001FA;
extern int func_80064B74(int);

void vblankHandler(void)
{
    D_800A1384 = *(volatile unsigned short *)D_800A1380;
    D_800A1386 = D_800A1382;
    D_800A1380[0] = func_80064B74(1);
    {
        unsigned short o0 = D_800A1384;
        unsigned short c0 = *(volatile unsigned short *)D_800A1380;
        unsigned short o1 = D_800A1386;
        unsigned short c1 = D_800A1382;
        unsigned short f = FRAME_COUNT + 1;

        D_1F8001E8++;
        D_1F8001FA = c0 & ~o0;
        D_8009E4B6 = c1 & ~o1;
        FRAME_COUNT = f;
    }
}
#else
#define SKIP_ASM
#include "common.h"
#include "game.h"


void vblankHandler(void)
{
    scratchpad* scratch = PSX_SCRATCH;

    scratch->vblankCount++;
    scratch->frameCount++;
}
#endif
