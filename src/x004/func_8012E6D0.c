// FUNC 8012e6d0 128 X004
// MATCHING 8012e6d0 128
#include "TOBJ.H"
extern void func_8012E554(TObj *);
extern void PoolFree_1F800210(TObj *);

void func_8012E6D0(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04 = 3;
        break;
    case 1:
        func_8012E554(o);
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        PoolFree_1F800210(o);
        break;
    }
}
