// FUNC 801294b4 168 X009
// MATCHING 801294b4 168
#include "TOBJ.H"
extern void func_80129344(TObj *);
extern int FUN_800202b4(TObj *);
extern void func_801290F4(TObj *);
extern void FUN_80018790(TObj *);

void func_801294B4(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_80129344(o);
        break;
    case 1:
        FUN_800202b4(o);
        func_801290F4(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
