// FUNC 800eecc4 180 I0NFO2
// MATCHING 800eecc4 180
#include "TOBJ.H"
#include "raw7.h"
extern void func_8001FE94(TObj *o, int n);
extern void func_8001FEC0(TObj *o);
extern void **TOPDOWN_ACTOR_ANIMS[][3];

void topdownActorIdleStep(TObj *o)
{
    short f;
    switch (o->state) {
    case 0:
        f = o->animFrame;
        if (f != U16(o, 0xD2)) {
            o->anim = TOPDOWN_ACTOR_ANIMS[o->subtype][0][f];
            func_8001FE94(o, 0);
            S16(o, 0xD2) = f;
        }
        o->state++;
    case 1:
        func_8001FEC0(o);
        break;
    }
}
