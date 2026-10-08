// FUNC 801173ec 152 X018
// MATCHING 801173ec 152
#include "TOBJ.H"
extern void func_801171AC(TObj *);
extern void PoolFree_1F800210(TObj *);

void func_801173EC(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_801171AC(o);
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
