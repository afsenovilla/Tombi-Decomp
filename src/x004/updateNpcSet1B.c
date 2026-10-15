// FUNC 8012dc5c 152 X004
// MATCHING 8012dc5c 152
#include "TOBJ.H"
extern void spawnNpcSet1B(TObj *);
extern void PoolFree_1F800210(TObj *);

void updateNpcSet1B(TObj *o)
{
    switch (o->b04) {
    case 0:
        spawnNpcSet1B(o);
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
