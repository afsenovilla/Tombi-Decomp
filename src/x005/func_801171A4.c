// FUNC 801171a4 168 X005
// MATCHING 801171a4 168
#include "TOBJ.H"
extern void func_80117050(TObj *);
extern int FUN_800202b4(TObj *);
extern void func_80116E78(TObj *);
extern void FUN_80018790(TObj *);

void func_801171A4(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_80117050(o);
        break;
    case 1:
        FUN_800202b4(o);
        func_80116E78(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
