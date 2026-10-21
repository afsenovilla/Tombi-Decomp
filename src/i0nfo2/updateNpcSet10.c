// FUNC 800fde6c 184 I0NFO2
// MATCHING 800fde6c 184
#include "TOBJ.H"
extern void func_80018980(TObj *o);
extern void func_8001FEC0(TObj *o);
extern void spawnNpcSet10(TObj *o);

void updateNpcSet10(TObj *o)
{
    switch (o->b04) {
    case 0:
        spawnNpcSet10(o);
        o->b04++;
        break;
    case 1:
        if (o->step == 0) func_8001FEC0(o);
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        func_80018980(o);
        break;
    }
}
