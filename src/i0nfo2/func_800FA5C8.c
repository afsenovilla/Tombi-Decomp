// FUNC 800fa5c8 168 I0NFO2
// MATCHING 800fa5c8 168
#include "TOBJ.H"
extern void func_80018980(TObj *o);
extern void func_800F6F20(TObj *o);
extern void func_800FA424(TObj *o);

void func_800FA5C8(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_800F6F20(o);
        o->b04++;
        break;
    case 1:
        func_800FA424(o);
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        func_80018980(o);
        break;
    }
}
