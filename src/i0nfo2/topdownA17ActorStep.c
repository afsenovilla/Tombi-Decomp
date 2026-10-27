// FUNC 800f4f70 452 I0NFO2
// MATCHING 800f4f70 452
#include "TOBJ.H"
#include "raw7.h"
extern void func_8001FE94(TObj *o, int n);
extern void func_8001FEC0(TObj *o);
extern void topdownA17ActorPatrolVStep(TObj *o);
extern void topdownA17ActorPatrolHStep(TObj *o);
extern void topdownA17ActorPatrolDiagStepA(TObj *o);
extern void topdownA17ActorPatrolDiagStepB(TObj *o);
extern void topdownA17ActorTalkStep(TObj *o);
extern unsigned char D_800A60A3;
extern void **TOPDOWN_A17_ACTOR_ANIMS[][3];

static inline void setAnim(TObj *o, int i, short n)
{
    short f = o->animFrame + i;
    o->anim = TOPDOWN_A17_ACTOR_ANIMS[o->subtype][0][f];
    func_8001FE94(o, n);
    S16(o, 0xD2) = f;
}

void topdownA17ActorStep(TObj *o)
{
    switch (o->step) {
    case 0:
        switch (U16(o, 0xB8)) {
        case 0:
            switch (o->state) {
            case 0:
                setAnim(o, 0, 0);
                o->state++;
            case 1:
                func_8001FEC0(o);
                break;
            }
            break;
        case 1:
            topdownA17ActorPatrolVStep(o);
            break;
        case 2:
            topdownA17ActorPatrolHStep(o);
            break;
        case 3:
            topdownA17ActorPatrolDiagStepA(o);
            break;
        case 4:
            topdownA17ActorPatrolDiagStepB(o);
            break;
        case 5:
        case 6:
            break;
        }
        if (o->b68 != 0) {
            o->step = 1;
            o->state = 0;
        }
        break;
    case 1:
        topdownA17ActorTalkStep(o);
        break;
    case 2:
        func_8001FEC0(o);
        break;
    }
    if (D_800A60A3 != 0) {
        if (D_800A60A3 == U8(o, 0xB6)) {
            o->b6b = 0x80;
        } else {
            o->b6b = 0;
        }
    } else if (U16(o, 0xB6) != 0) {
        o->b6b = 0;
    } else {
        o->b6b = 0x80;
    }
}
