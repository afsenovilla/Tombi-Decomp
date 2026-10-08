// FUNC 80117408 152 X016
// MATCHING 80117408 152
#include "TOBJ.H"
extern void func_80117228(TObj *);
extern void PoolFree_1F800210(TObj *);

void func_80117408(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_80117228(o);
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
