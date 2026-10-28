// FUNC 80116efc 140 X016
// MATCHING 80116efc 140
#include "TOBJ.H"
extern void playerDepthNudgeStep(TObj *);
extern void PoolFree_1F800210(TObj *);

void updatePlayerDepthNudge(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        break;
    case 1:
        playerDepthNudgeStep(o);
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        PoolFree_1F800210(o);
        break;
    }
}
