// near-twin of PAL weaponSetPlayerAnimB (tools/jp_autofix.py)
// VERSION SLPS_01144 slps01144 800323A8 268
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "raw7.h"
extern void *PLAYER_ANIM;
extern char D_80010FA8[], D_80010FB0[], D_80010FB8[], D_80011098[], D_800110A0[], D_800110A8[], D_800114F4[];
void func_800323A8(char *o)
{
    switch (U8(o, 5)) {
    case 0:
        switch (U16(o, 0x2e)) {
        case 0: case 1: case 2: case 3: PLAYER_ANIM = D_80010FA8; break;
        case 4: case 5: PLAYER_ANIM = D_80010FB0; break;
        case 6: case 7: PLAYER_ANIM = D_80010FB8;
        }
        break;
    case 1:
        switch (U16(o, 0x2e)) {
        case 0: case 1: case 2: case 3: PLAYER_ANIM = D_80011098; break;
        case 4: case 5: PLAYER_ANIM = D_800110A0; break;
        case 6: case 7: PLAYER_ANIM = D_800110A8;
        }
        break;
    case 2:
        PLAYER_ANIM = D_800114F4;
        break;
    }
}
#endif
