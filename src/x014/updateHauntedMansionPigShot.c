// FUNC 80122de8 304 X014
// MATCHING 80122de8 304
// VERSION SLPS_01144 x14 801200C0 304
#if defined(VERSION_SLPS_01144)
#include "include_asm.h"
INCLUDE_ASM_V("x14/nonmatchings/x14", func_801200C0_slps01144);
#else
#include "spad.h"
#include "TOBJ.H"
#include "gamestate.h"

extern void hauntedMansionPigShotPickPoint(TObj *);
extern void hauntedMansionPigShotAttack(TObj *);
extern void freeObjectLayer2(TObj *);

void updateHauntedMansionPigShot(TObj *o)
{
    switch (o->b04) {
    case 0: {
        int *pp = &SPAD.i334;
        int *q = (int *)*pp;
        q += 1;
        if (GAME.selectedSection == 7) o->da0 = *pp + q[5];
        else o->da0 = *pp + q[0];
        o->ba4 = 1;
        o->box0 = 0x10;
        o->box1 = 0x20;
        o->box2 = 0x140;
        o->box3 = 0x140;
        o->b0a = 0x11;
        o->b0f = 0;
        o->b0c = 0;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->b04++;
        break;
    }
    case 1:
        switch (o->step) {
        case 0:
            hauntedMansionPigShotPickPoint(o);
            break;
        case 1:
            hauntedMansionPigShotAttack(o);
            break;
        }
        break;
    case 2:
    case 3:
        freeObjectLayer2(o);
        break;
    }
}
#endif
