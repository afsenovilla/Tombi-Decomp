// FUNC 80116efc 140 X016
// MATCHING 80116efc 140
#include "TOBJ.H"
extern void func_80116E60(TObj *);
extern void PoolFree_1F800210(TObj *);

void func_80116EFC(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        break;
    case 1:
        func_80116E60(o);
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        PoolFree_1F800210(o);
        break;
    }
}
