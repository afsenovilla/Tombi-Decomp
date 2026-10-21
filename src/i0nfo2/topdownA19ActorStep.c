// FUNC 800f67b4 472 I0NFO2
// MATCHING 800f67b4 472
#include "TOBJ.H"
#include "raw7.h"
extern void func_8001FE94(TObj *o, int n);
extern void func_8001FEC0(TObj *o);
extern void topdownA19ActorPatrolVStep(TObj *o);
extern void topdownA19ActorPatrolHStep(TObj *o);
extern void topdownA19ActorPatrolDiagStepA(TObj *o);
extern void topdownA19ActorPatrolDiagStepB(TObj *o);
extern void topdownA19ActorTalkStep(TObj *o);
extern unsigned char D_800A60A3;
extern void **D_800FEFFC[][3];

void topdownA19ActorStep(TObj *o)
{
    short f;
    switch (o->step) {
    case 0:
        switch ((unsigned short)o->wb8) {
        case 0:
            switch (o->state) {
            case 0:
                f = o->animFrame;
                if (f != U16(o, 0xD2)) {
                    o->anim = D_800FEFFC[o->subtype][0][f];
                    func_8001FE94(o, 0);
                    S16(o, 0xD2) = f;
                }
                o->state++;
            case 1:
                func_8001FEC0(o);
                break;
            }
            break;
        case 1:
            topdownA19ActorPatrolVStep(o);
            break;
        case 2:
            topdownA19ActorPatrolHStep(o);
            break;
        case 3:
            topdownA19ActorPatrolDiagStepA(o);
            break;
        case 4:
            topdownA19ActorPatrolDiagStepB(o);
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
        topdownA19ActorTalkStep(o);
        break;
    case 2:
        func_8001FEC0(o);
        break;
    }
    if (D_800A60A3 != 0) {
        if (D_800A60A3 == *(unsigned char *)&o->wb6) o->b6b = 0x80;
        else o->b6b = 0;
    } else {
        if ((unsigned short)o->wb6 != 0) o->b6b = 0;
        else o->b6b = 0x80;
    }
}
