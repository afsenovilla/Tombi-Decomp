// FUNC 80119ba8 152 X011
// MATCHING 80119ba8 152
#include "TOBJ.H"
extern void spawnNpcSet1E(TObj *);
extern void PoolFree_1F800210(TObj *);

void updateNpcSet1E(TObj *o)
{
    switch (o->b04) {
    case 0:
        spawnNpcSet1E(o);
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
