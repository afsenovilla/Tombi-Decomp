// near-twin of PAL weaponSetPlayerAnimB (tools/jp_autofix.py)
// VERSION SLPS_01144 slps01144 800324B4 268
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "raw7.h"
extern void *PLAYER_ANIM;
extern char D_80010F30[], D_80010F50[], D_80010F70[], D_80011020[], D_80011040[], D_80011060[], D_800114C8[];
void func_800324B4(char *o)
{
    switch (U8(o, 5)) {
    case 0:
        switch (U16(o, 0x2e)) {
        case 0: case 1: case 2: case 3: PLAYER_ANIM = D_80010F30; break;
        case 4: case 5: PLAYER_ANIM = D_80010F50; break;
        case 6: case 7: PLAYER_ANIM = D_80010F70;
        }
        break;
    case 1:
        switch (U16(o, 0x2e)) {
        case 0: case 1: case 2: case 3: PLAYER_ANIM = D_80011020; break;
        case 4: case 5: PLAYER_ANIM = D_80011040; break;
        case 6: case 7: PLAYER_ANIM = D_80011060;
        }
        break;
    case 2:
        PLAYER_ANIM = D_800114C8;
        break;
    }
}
#endif
