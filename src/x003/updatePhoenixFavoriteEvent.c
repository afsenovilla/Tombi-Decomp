// FUNC 80133df4 160 X003
// MATCHING 80133df4 160
#include "gvars.h"
#include "TOBJ.H"

extern void stepPhoenixFavoriteEvent(TObj *);
extern void PoolFree_1F800210(TObj *);

void updatePhoenixFavoriteEvent(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        break;
    case 1:
        if (GVARS.b3a) stepPhoenixFavoriteEvent(o);
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        PoolFree_1F800210(o);
        break;
    }
}
