// FUNC 80107590 72 X001
// MATCHING 80107590 72
// VERSION BETA_980203 x00 8010B4FC 64
#if defined(VERSION_BETA_980203)
// Beta version of PAL x001 80107590 (playerEnterStep10Fall.c): PAL C of src/x001/playerEnterStep10Fall.c (playerEnterStep10Fall, twin score 0.910), edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
#include "TOBJ.H"
void playerEnterStep10Fall(TObj *o)
{
    o->b9c = 1;
    o->d88 = -0x100;
    o->ba4 = 0;
    if (o->ba6 & 6) {
        o->wb2 = 0;
        o->velH = 0;
        o->velV = 0;
        o->velX = 0;
        o->velY = 0;
    }
    o->state = 2;
}
#else
#include "TOBJ.H"
void playerEnterStep10Fall(TObj *o)
{
    o->b9c = 1;
    o->d88 = -0x100;
    o->ba4 = 0;
    if (o->ba6 & 6) {
        o->wb2 = 0;
        o->velH = 0;
        o->velV = 0;
        o->velX = 0;
        o->velY = 0;
    }
    o->step = 0x10;
    o->state = 2;
}
#endif
