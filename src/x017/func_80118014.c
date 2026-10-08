// FUNC 80118014 168 X017
// MATCHING 80118014 168
#include "TOBJ.H"
extern void func_80117E9C(TObj *);
extern int FUN_800202b4(TObj *);
extern void func_80117C84(TObj *);
extern void FUN_80018790(TObj *);

void func_80118014(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_80117E9C(o);
        break;
    case 1:
        FUN_800202b4(o);
        func_80117C84(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
