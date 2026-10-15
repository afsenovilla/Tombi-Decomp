// JPDEMO x00 8012A424 124
// Demo version of PAL x010 8012D858 (func_8012D858.c): a disabled object: state 0 goes straight to the free state.
#include "TOBJ.H"
extern void func_8012D678(TObj *);
extern void PoolFree_1F800210(TObj *);

void func_8012D858(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04 = 3;
        break;
    case 1:
        func_8012D678(o);
        break;
    case 2:
        break;
    case 3:
        PoolFree_1F800210(o);
        break;
    }
}
