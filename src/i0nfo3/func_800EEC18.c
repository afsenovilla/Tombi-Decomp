// FUNC 800eec18 168 I0NFO3
// MATCHING 800eec18 168
#include "TOBJ.H"
extern void func_80018980(TObj *o);
extern void func_800EE260(TObj *o);
extern void func_800EEA70(TObj *o);

void func_800EEC18(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_800EE260(o);
        o->b04++;
        break;
    case 1:
        func_800EEA70(o);
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        func_80018980(o);
        break;
    }
}
