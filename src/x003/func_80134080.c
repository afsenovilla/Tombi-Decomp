// FUNC 80134080 152 X003
// MATCHING 80134080 152
#include "TOBJ.H"
extern void func_80133E94(TObj *);
extern void PoolFree_1F800210(TObj *);

void func_80134080(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_80133E94(o);
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
