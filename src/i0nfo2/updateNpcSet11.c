// FUNC 800feb28 168 I0NFO2
// MATCHING 800feb28 168
#include "TOBJ.H"
extern void func_80018980(TObj *o);
extern void spawnNpcSet11(TObj *o);
extern void stepNpcSet11(TObj *o);

void updateNpcSet11(TObj *o)
{
    switch (o->b04) {
    case 0:
        spawnNpcSet11(o);
        o->b04++;
        break;
    case 1:
        stepNpcSet11(o);
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        func_80018980(o);
        break;
    }
}
