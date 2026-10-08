// FUNC 801184fc 224 X017
// MATCHING 801184fc 224
#include "TOBJ.H"

extern void func_80118388(TObj *);
extern void func_80118110(TObj *);
extern int ObjCullRegister(TObj *);
extern void AnimAdvance(TObj *);
extern void FUN_80018790(TObj *);

void func_801184FC(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_80118388(o);
        break;
    case 1:
        ObjCullRegister(o);
        switch (o->step) {
        case 0:
            AnimAdvance(o);
            break;
        case 1:
            func_80118110(o);
            break;
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
