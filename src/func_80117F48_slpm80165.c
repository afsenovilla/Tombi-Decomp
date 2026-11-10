// VERSION SLPM_80165 x00 80117F48 632
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of func_8011E2FC_slps01144 (src/updateA00L4Type11.c).
#include "gcd.h"
/* Only the first of the three functions of the JP unit: in the demo they have their own symbols. */
#include "TOBJ.H"
#include "player.h"
extern void *D_8013B2BC[];
extern int D_1F8002D4[];
extern Fix16 *PLAYER_D[];
extern void AnimLoadDuration(TObj *o);
extern int ObjCullRegister(TObj *o);
extern void AnimAdvance(TObj *o);
extern void ObjFreeDup(TObj *o);

void func_80117F48_slpm80165(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b0a = 0;
        o->b0f = ((unsigned char*)&PLAYER.obj.b0f)[0] + 1;
        o->anim = D_8013B2BC[0];
        o->d3c = D_1F8002D4[0];
        o->b0d = 0;
        o->w22 = 0;
        o->b.raw = 0;
        o->w1e = 9;
        o->timer = 10;
        o->a.raw = 0xf00000;
        o->y.raw = 0xfe7c0000;
        o->animFrame = 1;
        o->b69 = 0;
        o->box0 = 8;
        o->box1 = 0x10;
        o->box2 = 0x58;
        o->box3 = 0x88;
        AnimLoadDuration(o);
        o->step = 0;
        o->state = 0;
        o->b04++;
        break;
    case 1:
        switch (o->step) {
        case 1:
            switch (o->state) {
            case 0:
                o->timer = 0xf0;
                o->state++;
            case 1:
                o->y.p.whole++;
                if (--o->timer == 0) {
                    o->step = 2;
                    o->state = 0;
                    if (((unsigned char*)&PLAYER.obj.b04)[0] != 1) {
                        PLAYER.obj.animFrame = 0;
                        ((unsigned char*)&PLAYER.obj.b04)[0] = 1;
                        PLAYER.obj.step = 0;
                        PLAYER.obj.state = 0;
                    }
                }
                break;
            }
            break;
        case 5:
            if (GCD.bcdae == 0xff && PLAYER.obj.y.p.whole >= -0xc7 && PLAYER.obj.b69 != 0 && ((unsigned char*)&PLAYER.obj.b04)[0] != 1) {
                o->step = 1;
                o->state = 0;
            }
            break;
        case 6:
            o->y.raw = 0xff6c0000;
            break;
        case 0: case 2: case 3: case 4:
            break;
        }
        if (ObjCullRegister(o)) AnimAdvance(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}
#endif
