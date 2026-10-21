// FUNC 800fafac 168 I0NFO2
// MATCHING 800fafac 168
#include "TOBJ.H"
extern void func_80018980(TObj *o);
extern void spawnNpcSet0C(TObj *o);
extern void npcSet0CMessageScene(TObj *o);

void updateNpcSet0C(TObj *o)
{
    switch (o->b04) {
    case 0:
        spawnNpcSet0C(o);
        break;
    case 1:
        if (o->step != 0 && o->step == 1) npcSet0CMessageScene(o);
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        func_80018980(o);
        break;
    }
}
