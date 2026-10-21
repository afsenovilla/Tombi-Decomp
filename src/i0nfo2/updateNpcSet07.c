// FUNC 800fa5c8 168 I0NFO2
// MATCHING 800fa5c8 168
#include "TOBJ.H"
extern void func_80018980(TObj *o);
extern void spawnNpcSet07(TObj *o);
extern void stepNpcSet07(TObj *o);

void updateNpcSet07(TObj *o)
{
    switch (o->b04) {
    case 0:
        spawnNpcSet07(o);
        o->b04++;
        break;
    case 1:
        stepNpcSet07(o);
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        func_80018980(o);
        break;
    }
}
