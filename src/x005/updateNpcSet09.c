// FUNC 8011762c 152 X005
// MATCHING 8011762c 152
#include "TOBJ.H"
extern void spawnNpcSet09(TObj *);
extern void PoolFree_1F800210(TObj *);

void updateNpcSet09(TObj *o)
{
    switch (o->b04) {
    case 0:
        spawnNpcSet09(o);
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
