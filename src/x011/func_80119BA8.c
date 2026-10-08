// FUNC 80119ba8 152 X011
// MATCHING 80119ba8 152
#include "TOBJ.H"
extern void func_801199F0(TObj *);
extern void PoolFree_1F800210(TObj *);

void func_80119BA8(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_801199F0(o);
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
