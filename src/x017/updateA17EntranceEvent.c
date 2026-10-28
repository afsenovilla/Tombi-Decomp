// FUNC 80118880 140 X017
// MATCHING 80118880 140
#include "TOBJ.H"
extern void func_801185E4(TObj *);
extern void PoolFree_1F800210(TObj *);

void func_80118880(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        break;
    case 1:
        func_801185E4(o);
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        PoolFree_1F800210(o);
        break;
    }
}
