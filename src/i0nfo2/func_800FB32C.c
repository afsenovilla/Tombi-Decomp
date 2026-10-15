// FUNC 800fb32c 152 I0NFO2
// MATCHING 800fb32c 152
#include "TOBJ.H"
extern void func_80018980(TObj *o);
extern void func_800FB054(TObj *o);

void func_800FB32C(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_800FB054(o);
        o->b04++;
        break;
    case 1:
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        func_80018980(o);
        break;
    }
}
