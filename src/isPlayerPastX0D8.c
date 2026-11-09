// FUNC 801363c4 32 X000
// MATCHING 801363c4 32
// VERSION PCPX_96084 tomba 800841D0 32
#if defined(VERSION_PCPX_96084)
// PrePre 8 version of PAL x000 801363C4 (isPlayerPastX0D8.c): SLPS_01144 C of src/isPlayerPastX0D8.c (func_80134AB8_slps01144, twin score 1.000), edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
#include "player.h"

int isPlayerPastX0D8(void)
{
    return (*(short **)&PLAYER.obj.h)[1] > 0xe8;
}
#else
#include "player.h"

int isPlayerPastX0D8(void)
{
    return (*(short **)&PLAYER.obj.h)[1] > 0xd8;
}
#endif
