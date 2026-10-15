// FUNC 801173ec 152 X018
// MATCHING 801173ec 152
#include "TOBJ.H"
extern void spawnNpcSet39(TObj *);
extern void PoolFree_1F800210(TObj *);

void updateNpcSet39(TObj *o)
{
    switch (o->b04) {
    case 0:
        spawnNpcSet39(o);
        o->b04++;
        break;
    case 1:
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        PoolFree_1F800210(o);
        break;
    }
}
