// FUNC 800fafac 168 I0NFO2
// MATCHING 800fafac 168
#include "TOBJ.H"
extern void func_80018980(TObj *o);
extern void func_800FA670(TObj *o);
extern void func_800FA84C(TObj *o);

void func_800FAFAC(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_800FA670(o);
        break;
    case 1:
        if (o->step != 0 && o->step == 1) func_800FA84C(o);
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        func_80018980(o);
        break;
    }
}
