// FUNC 8011765c 240 X018
// MATCHING 8011765c 240
#include "gcd.h"
#include "TOBJ.H"

extern void spawnNpcSet38(TObj *);
extern void PoolFree_1F800210(TObj *);

void updateNpcSet38(TObj *o)
{
    switch (o->b04) {
    case 0:
        spawnNpcSet38(o);
        if (GCD.bce16 == 0xff) GCD.bcfd9 = 1;
        else GCD.bcfd9 = 0;
        if (GCD.bce18 == 0xff) GCD.bcfda = 1;
        else GCD.bcfda = 0;
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
