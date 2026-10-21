// FUNC 800faf68 56 I0NFO2
// MATCHING 800faf68 56
#include "TOBJ.H"
extern void npcSet0CMessageScene(TObj *o);

void stepNpcSet0C(TObj *o)
{
    switch (o->step) {
    case 0:
        break;
    case 1:
        npcSet0CMessageScene(o);
        break;
    }
}
