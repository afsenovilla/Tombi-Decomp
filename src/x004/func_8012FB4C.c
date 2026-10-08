// FUNC 8012fb4c 168 X004
// MATCHING 8012fb4c 168
#include "TOBJ.H"
extern void func_8012F9F4(TObj *);
extern int FUN_800202b4(TObj *);
extern void func_8012F75C(TObj *);
extern void FUN_80018790(TObj *);

void func_8012FB4C(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_8012F9F4(o);
        break;
    case 1:
        FUN_800202b4(o);
        func_8012F75C(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
