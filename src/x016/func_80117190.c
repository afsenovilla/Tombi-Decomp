// FUNC 80117190 152 X016
// MATCHING 80117190 152
#include "TOBJ.H"
extern void func_80116F88(TObj *);
extern void PoolFree_1F800210(TObj *);

void func_80117190(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_80116F88(o);
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
