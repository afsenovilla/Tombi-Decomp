// FUNC 8012e6d0 128 X004
// MATCHING 8012e6d0 128
#include "TOBJ.H"
extern void stepKeyHoles(TObj *);
extern void PoolFree_1F800210(TObj *);

void updateKeyHoles(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04 = 3;
        break;
    case 1:
        stepKeyHoles(o);
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        PoolFree_1F800210(o);
        break;
    }
}
