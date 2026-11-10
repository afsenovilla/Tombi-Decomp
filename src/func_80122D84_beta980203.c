// VERSION BETA_980203 x00 80122D84 128
#if defined(VERSION_BETA_980203)
// Beta version: the second function of PAL updateA00L4Type11.c (func_8011F500), split out in the beta.
#include "gcd.h"
/* Holds 3 functions merged by splat: updateA00L4Type11, func_8011F500, func_8011F580. */
#include "TOBJ.H"
#include "player.h"
extern void *D_8013B2BC[];
extern int D_1F8002D4[];
extern Fix16 *PLAYER_D[];
extern void AnimLoadDuration(TObj *o);
extern int ObjCullRegister(TObj *o);
extern void AnimAdvance(TObj *o);
extern void ObjFreeDup(TObj *o);

void func_80122D84_beta980203(TObj *o)
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
