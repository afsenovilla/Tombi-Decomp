// FUNC 800f8660 136 X000
// MATCHING 800f8660 136
// VERSION SLPM_80165 x00 800F4F50 108
// probably: player step entry: anim PLAYER_VERTICAL_JUMP_ANIM sub 4, b0f -8, state++ (sets da0+2 unless step 0x49); unreferenced
#if defined(VERSION_SLPM_80165)
// Demo version of PAL x000 800F8660 (playerStepEntryAnim80010CA0.c): SLPS_01144 C of src/playerStepEntryAnim80010CA0.c (func_800F82F0_slps01144, twin score 0.754), edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
#include "TOBJ.H"
extern unsigned char SLOPE_TILT_TABLE[];
extern void func_8001e5f4(int a, int b);
extern void func_8001fe94(TObj *o, int n);
extern char PLAYER_VERTICAL_JUMP_ANIM[];

void playerStepEntryAnim80010CA0(TObj *o)
{
    unsigned char b;
    if (o->step != 0x49) {
    }
        *((char *)&o->da0 + 2) = 1;
    *(char *)&o->wac = 0;
    func_8001e5f4(0x1c, 0x7f);
    *(signed char *)&o->b0f = -8;
    o->anim = PLAYER_VERTICAL_JUMP_ANIM;
    func_8001fe94(o, 4);
    b = SLOPE_TILT_TABLE[o->wb0];
    o->state = o->state + 1;
}
#else
#include "TOBJ.H"
extern unsigned char SLOPE_TILT_TABLE[];
extern void func_8001e5f4(int a, int b);
extern void func_8001fe94(TObj *o, int n);
extern char PLAYER_VERTICAL_JUMP_ANIM[];

void playerStepEntryAnim80010CA0(TObj *o)
{
    unsigned char b;
    if (o->step != 0x49) {
        *((char *)&o->da0 + 2) = 1;
    }
    *(char *)&o->wac = 0;
    func_8001e5f4(0x1c, 0x7f);
    *(signed char *)&o->b0f = -8;
    o->anim = PLAYER_VERTICAL_JUMP_ANIM;
    func_8001fe94(o, 4);
    b = SLOPE_TILT_TABLE[o->wb0];
    o->state = o->state + 1;
    o->d8c = b;
}
#endif
