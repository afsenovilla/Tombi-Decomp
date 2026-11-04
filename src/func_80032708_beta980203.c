// VERSION BETA_980203 beta980203 80032708 364
#if defined(VERSION_BETA_980203)
// Beta version: JP C of src/updateWeaponTrailObject.c (func_80030F30_slps01144) (1.000 masked) [-O2 -G0] (tools/demo_sweep.py)
#include "TOBJ.H"
typedef struct { short k; short w1e; void **anims; int pad2; } T12;
extern T12 WEAPON_TRAIL_DEFS[];
extern int SPR_DATA[];
extern unsigned char PLAYER_ACTIVE;
extern void AnimLoadDuration(TObj *);
extern void func_80030D78_beta980203(TObj *);
extern void ObjFree(TObj *);

void func_80032708_beta980203(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->w1e = WEAPON_TRAIL_DEFS[o->subtype].w1e;
        o->d3c = SPR_DATA[WEAPON_TRAIL_DEFS[o->subtype].k];
        o->anim = WEAPON_TRAIL_DEFS[o->subtype].anims[o->b0c];
        AnimLoadDuration(o);
        o->b0a = 13;
        o->b0d = 0x80;
        *(signed char *)&o->b0f = -7;
        o->d8c = 0;
        o->b6b = 0;
        o->category |= 0x80;
        o->b04++;
        break;
    case 1:
        if (PLAYER_ACTIVE != 5)
            func_80030D78_beta980203(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        ObjFree(o);
        break;
    }
}
#endif
