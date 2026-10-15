// FUNC 80133668 152 X003
// MATCHING 80133668 152
#include "TOBJ.H"
extern void spawnNpcSet20(TObj *);
extern void PoolFree_1F800210(TObj *);

void updateNpcSet20(TObj *o)
{
    switch (o->b04) {
    case 0:
        spawnNpcSet20(o);
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
