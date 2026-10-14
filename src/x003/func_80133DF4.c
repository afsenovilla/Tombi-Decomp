// FUNC 80133df4 160 X003
// MATCHING 80133df4 160
#include "gvars.h"
#include "TOBJ.H"

extern void func_80133CB4(TObj *);
extern void PoolFree_1F800210(TObj *);

void func_80133DF4(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        break;
    case 1:
        if (GVARS.b3a) func_80133CB4(o);
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        PoolFree_1F800210(o);
        break;
    }
}
