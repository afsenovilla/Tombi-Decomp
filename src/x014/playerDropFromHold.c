// FUNC 800f6d80 168 X014
// MATCHING 800f6d80 168
// VERSION SLPM_80165 x00 800F33FC 112
#if defined(VERSION_SLPM_80165)
// Demo version of PAL x014 800F6D80 (playerDropFromHold.c): PAL C of src/x014/playerDropFromHold.c (playerDropFromHold, twin score 0.671), edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
#include "TOBJ.H"
extern unsigned char *PLAYER_EXT;
extern unsigned char *GRABBED_OBJECT;
extern unsigned char SLOPE_TILT_TABLE[];
void playerDropFromHold(TObj *o)
{
    int v;
    int y;
    int c;
    PLAYER_EXT[9] = 0;
    *(unsigned char *)&o->wac = 1;
    o->b9e = 0;
    o->wb2 = 0;
    o->velH = 0;
    o->velV = 0;
    o->d30 = 0;
    o->d34 = 0;
    o->velX = 0;
    o->velY = 0;
    if (o->animFrame & 1) {
    }
    c = o->animFrame & 1;
    /* the parameter register is reused for the hitbox pointer */
    o = (TObj *)((volatile TObj *)o)->h;
    y = ((Fix16 *)o)->p.whole;
    if (c) {
        y += 14;
    } else {
        y -= 14;
    }
    ((Fix16 *)o)->p.whole = y;
    GRABBED_OBJECT[0x69] = 0;
}
#else
#include "TOBJ.H"
extern unsigned char *PLAYER_EXT;
extern unsigned char *GRABBED_OBJECT;
extern unsigned char SLOPE_TILT_TABLE[];
void playerDropFromHold(TObj *o)
{
    int v;
    int y;
    int c;
    PLAYER_EXT[9] = 0;
    v = 0x10;
    *(unsigned char *)&o->wac = 1;
    o->b9e = 0;
    o->wb2 = 0;
    o->velH = 0;
    o->velV = 0;
    o->d30 = 0;
    o->d34 = 0;
    o->velX = 0;
    o->velY = 0;
    o->timer = 10;
    o->d84 = 0;
    if (o->animFrame & 1) {
        v = 0xf0;
    }
    o->d88 = v;
    *(volatile int *)&o->d8c = SLOPE_TILT_TABLE[o->wb0];
    c = o->animFrame & 1;
    /* the parameter register is reused for the hitbox pointer */
    o = (TObj *)((volatile TObj *)o)->h;
    y = ((Fix16 *)o)->p.whole;
    if (c) {
        y += 14;
    } else {
        y -= 14;
    }
    ((Fix16 *)o)->p.whole = y;
    GRABBED_OBJECT[0x69] = 0;
}
#endif
