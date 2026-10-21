// FUNC 800fb32c 152 I0NFO2
// MATCHING 800fb32c 152
#include "TOBJ.H"
extern void func_80018980(TObj *o);
extern void spawnNpcSet0D(TObj *o);

void updateNpcSet0D(TObj *o)
{
    switch (o->b04) {
    case 0:
        spawnNpcSet0D(o);
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
