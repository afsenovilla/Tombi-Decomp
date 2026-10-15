// FUNC 8012d5d8 152 X010
// MATCHING 8012d5d8 152
#include "TOBJ.H"
extern void spawnNpcSet21(TObj *);
extern void PoolFree_1F800210(TObj *);

void updateNpcSet21(TObj *o)
{
    switch (o->b04) {
    case 0:
        spawnNpcSet21(o);
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
