// VERSION SLPM_80165 x00 80103580 212
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of playerStepForcedHold (src/x014/playerStepForcedHold.c) with the demo PlayerSetAnimIfChanged
// (func_800EC020) written inline.
#include "TOBJ.H"
extern TObj *PLAYER_EXT;
extern void func_800EDA04(TObj *);
extern void func_80020554(TObj *);
extern void func_800205A8(TObj *);

void func_80103580(TObj *o)
{
    switch (o->state) {
    case 0:
        o->d8c = 0;
        o->wb0 = 0;
        ((char *)PLAYER_EXT)[8] = 0;
        o->b69 = 0;
        *(char *)&o->wac = 0;
        o->velY = 0;
        o->velV = 0;
        o->animFrame = o->animFrame & 1;
        {
            TObj *p = PLAYER_EXT;
            p->animTimer = 0x1e;
            if (p->animFrame != 0x1e) {
                func_800EDA04(o);
                func_80020554(o);
                PLAYER_EXT->animFrame = PLAYER_EXT->animTimer;
            }
        }
        o->state++;
    case 1:
        func_800205A8(o);
    }
}
#endif
