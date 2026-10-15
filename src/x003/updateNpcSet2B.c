// FUNC 80134080 152 X003
// MATCHING 80134080 152
#include "TOBJ.H"
extern void spawnNpcSet2B(TObj *);
extern void PoolFree_1F800210(TObj *);

void updateNpcSet2B(TObj *o)
{
    switch (o->b04) {
    case 0:
        spawnNpcSet2B(o);
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
