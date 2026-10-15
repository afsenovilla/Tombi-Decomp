// FUNC 801184fc 224 X017
// MATCHING 801184fc 224
#include "TOBJ.H"

extern void initRaftReturnNpc(TObj *);
extern void raftReturnTalk(TObj *);
extern int ObjCullRegister(TObj *);
extern void AnimAdvance(TObj *);
extern void freeObjectLayer2(TObj *);

void updateRaftReturnNpc(TObj *o)
{
    switch (o->b04) {
    case 0:
        initRaftReturnNpc(o);
        break;
    case 1:
        ObjCullRegister(o);
        switch (o->step) {
        case 0:
            AnimAdvance(o);
            break;
        case 1:
            raftReturnTalk(o);
            break;
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        freeObjectLayer2(o);
        break;
    }
}
