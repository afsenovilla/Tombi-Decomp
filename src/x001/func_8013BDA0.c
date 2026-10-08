// FUNC 8013bda0 168 X001
// MATCHING 8013bda0 168
#include "TOBJ.H"
extern unsigned char D_8009CE56;
extern void func_8013B344(TObj *);
extern void func_8013B51C(TObj *);
extern void PoolFree_1F800210(TObj *);

void func_8013BDA0(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_8013B344(o);
        break;
    case 1:
        { int x = D_8009CE56; x ^= 0xff; if (x) func_8013B51C(o); }
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        PoolFree_1F800210(o);
        break;
    }
}
