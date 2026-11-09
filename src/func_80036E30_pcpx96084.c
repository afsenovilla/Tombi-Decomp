// VERSION PCPX_96084 tomba 80036E30 224
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 version of PAL x000 8011AA30 (spawnLeafShakeLeaves.c): PAL C of src/spawnLeafShakeLeaves.c (0.964 masked), edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
#include "raw7.h"
extern char *alloc();
extern int rnd();
void spawnLeafShakeLeaves(short x, short y, short z)
{
    int i;
    char *o;
    for (i = 0; i < 3; i++) {
        o = alloc();
        if (o != 0) {
            U8(o, 0) = 1;
            U8(o, 2) = 0x1f;
            **(int **)(o + 0x40) = (x + i * 0x18 - 0x18) << 16;
            S32(o, 0x14) = (y + (i - 1) * 0x10) << 16;
            **(int **)(o + 0x44) = z << 16;
            U8(o, 3) = rnd() & 3;
            U8(o, 0xc) = rnd() & 1;
            U16(o, 0x20) = rnd() & 0x1f;
        }
    }
}
#endif
