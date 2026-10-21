// FUNC 800eec18 168 I0NFO3
// MATCHING 800eec18 168
#include "TOBJ.H"
extern void func_80018980(TObj *o);
extern void tunnelSpawnActors(TObj *o);
extern void tunnelControllerStep(TObj *o);

void tunnelControllerHandler(TObj *o)
{
    switch (o->b04) {
    case 0:
        tunnelSpawnActors(o);
        o->b04++;
        break;
    case 1:
        tunnelControllerStep(o);
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        func_80018980(o);
        break;
    }
}
