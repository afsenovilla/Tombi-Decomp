// FUNC 8012dc5c 152 X004
// MATCHING 8012dc5c 152
#include "TOBJ.H"
extern void func_8012DA60(TObj *);
extern void PoolFree_1F800210(TObj *);

void func_8012DC5C(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_8012DA60(o);
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
