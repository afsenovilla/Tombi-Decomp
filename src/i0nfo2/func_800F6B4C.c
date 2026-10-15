// FUNC 800f6b4c 168 I0NFO2
// MATCHING 800f6b4c 168
#include "TOBJ.H"
extern void func_80018790(TObj *o);
extern void func_800202B4(TObj *o);
extern void func_800F67B4(TObj *o);
extern void func_800F698C(TObj *o);

void func_800F6B4C(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_800F698C(o);
        break;
    case 1:
        func_800202B4(o);
        func_800F67B4(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        func_80018790(o);
        break;
    }
}
