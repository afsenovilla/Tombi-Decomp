// FUNC 8012bf58 172 X004
// MATCHING 8012bf58 172
#include "TOBJ.H"
extern int D_1F8002D0[];
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void ObjCullRegister(TObj *);
extern void FUN_80018790(TObj *);

void func_8012BF58(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->w1e = 1;
        o->b0d = 0;
        o->d3c = D_1F8002D0[0];
        AnimLoadDuration(o);
        break;
    case 1:
        ObjCullRegister(o);
        AnimAdvance(o);
        break;
    case 2:
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
