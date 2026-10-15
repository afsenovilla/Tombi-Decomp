// FUNC 80117190 152 X016
// MATCHING 80117190 152
#include "TOBJ.H"
extern void spawnNpcSet22(TObj *);
extern void PoolFree_1F800210(TObj *);

void updateNpcSet22(TObj *o)
{
    switch (o->b04) {
    case 0:
        spawnNpcSet22(o);
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
