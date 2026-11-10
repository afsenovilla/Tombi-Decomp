// VERSION SLPM_80165 x00 801181C0 128
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of func_8011E2FC_slps01144 (src/updateA00L4Type11.c).
#include "gcd.h"
/* The second function of the JP unit. */
#include "TOBJ.H"
#include "player.h"
extern void *D_8013B2BC[];
extern int D_1F8002D4[];
extern Fix16 *PLAYER_D[];
extern void AnimLoadDuration(TObj *o);
extern int ObjCullRegister(TObj *o);
extern void AnimAdvance(TObj *o);
extern void ObjFreeDup(TObj *o);

void func_801181C0_slpm80165(TObj *o)
{
    if (o->subtype == 0) {
        o->box0 = 0x58;
        o->box1 = 0x60;
        o->box2 = 4;
        o->box3 = 8;
        o->b0a = 0x11;
        o->b0f = ((unsigned char*)&PLAYER.obj.b0f)[0] + 1;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->d->p.whole = PLAYER_D[0]->p.whole;
        o->b04++;
    }
}
#endif
