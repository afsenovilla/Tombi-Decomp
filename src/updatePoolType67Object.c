// FUNC 8002c920 360 MAIN0
// MATCHING 8002c920 360
#include "player.h"
#include "TOBJ.H"
extern void *D_800121F4[];
extern unsigned short PLAYER_Z[];
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void ObjCullRegister(TObj *);
extern void ObjFree(TObj *);

void updatePoolType67Object(TObj *o)
{
    unsigned char t = o->b04;
    int x;

    switch (t) {
    case 0:
        if (o->step == 0) {
            o->anim = D_800121F4[o->subtype];
            AnimLoadDuration(o);
            x = PLAYER.obj.a.p.whole;
            if (PLAYER.obj.animFrame & 1) {
                o->a.p.whole = x + 8;
            } else {
                o->a.p.whole = x - 8;
            }
            o->y.p.whole = (*(unsigned short*)&PLAYER.obj.y.p.whole) + 0x10;
            o->b.p.whole = PLAYER_Z[0];
            o->b04 = 1;
            o->step = 0;
        }
        break;
    case 1:
        ObjCullRegister(o);
        if (o->visible != 0) {
            switch (o->step) {
            case 0:
                o->step++;
            case 1:
                if (AnimAdvance(o)) {
                    o->b04 = 2;
                }
                break;
            }
        }
        break;
    case 2:
        o->b04 = t + 1;
        break;
    case 3:
        ObjFree(o);
        break;
    }
}
