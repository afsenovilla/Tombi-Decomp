// FUNC 80108f60 128 X000
// MATCHING 80108f60 128
// VERSION BETA_980203 x00 8010CE84 128
// probably: player step entry: sfx (0x1c,0x7f), ba3 = 1, anim PLAYER_LANE_JUMP_ANIM sub 2, state 3, slope tilt; unreferenced
#if defined(VERSION_BETA_980203)
// Beta version of PAL x000 80108F60 (playerStepEntryAnim80010E34.c): PAL C of src/playerStepEntryAnim80010E34.c, edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
#include "raw7.h"
extern unsigned char SLOPE_TILT_TABLE[];
extern char PLAYER_LANE_JUMP_ANIM[];
extern void FUN_8001e5f4(int, int);
extern void FUN_8001fe94(char *, int);
void playerStepEntryAnim80010E34(char *o)
{
    int t;
    FUN_8001e5f4(0x1c, 0x7f);
    U8(o, 0xa3) = 1;
    PTR(o, 0x24) = PLAYER_LANE_JUMP_ANIM;
    FUN_8001fe94(o, 2);
    S16(*(char **)(o + 0x44), 2) = U16(o, 0xea);
    t = SLOPE_TILT_TABLE[S16(o, 0xb0)];
    U8(o, 0xac) = 0;
    U8(o, 6) = 3;
    S32(o, 0x8c) = t;
}
#else
#include "raw7.h"
extern unsigned char SLOPE_TILT_TABLE[];
extern char PLAYER_LANE_JUMP_ANIM[];
extern void FUN_8001e5f4(int, int);
extern void FUN_8001fe94(char *, int);
void playerStepEntryAnim80010E34(char *o)
{
    int t;
    FUN_8001e5f4(0x1c, 0x7f);
    U8(o, 0xa3) = 1;
    PTR(o, 0x24) = PLAYER_LANE_JUMP_ANIM;
    FUN_8001fe94(o, 2);
    S16(*(char **)(o + 0x44), 2) = U16(o, 0xf6);
    t = SLOPE_TILT_TABLE[S16(o, 0xb0)];
    U8(o, 0xac) = 0;
    U8(o, 6) = 3;
    S32(o, 0x8c) = t;
}
#endif
