// FUNC 8012a70c 168 X009
// MATCHING 8012a70c 168
#include "TOBJ.H"
extern void func_80129C98(TObj *);
extern void func_80129EF8(TObj *);
extern void PoolFree_1F800210(TObj *);

void func_8012A70C(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_80129C98(o);
        o->b04++;
        break;
    case 1:
        func_80129EF8(o);
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        PoolFree_1F800210(o);
        break;
    }
}
