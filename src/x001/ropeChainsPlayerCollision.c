// FUNC 8012538c 32 X001
// MATCHING 8012538c 32
#include "TOBJ.H"

extern void ropeChainsOverlapTest(TObj *, TObj *);

void ropeChainsPlayerCollision(TObj *o, TObj *p)
{
    p->b69 = 0;
    ropeChainsOverlapTest(o, p);
}
