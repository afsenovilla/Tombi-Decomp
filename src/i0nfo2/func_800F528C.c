// FUNC 800f528c 168 I0NFO2
// MATCHING 800f528c 168
#include "TOBJ.H"
extern void func_80018790(TObj *o);
extern void func_800202B4(TObj *o);
extern void func_800F4F70(TObj *o);
extern void func_800F5134(TObj *o);

void func_800F528C(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_800F5134(o);
        break;
    case 1:
        func_800202B4(o);
        func_800F4F70(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        func_80018790(o);
        break;
    }
}
