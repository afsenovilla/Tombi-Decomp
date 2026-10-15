// FUNC 801210b4 48 X010
// MATCHING 801210b4 48
#include "spad.h"
#include "TOBJ.H"
extern int pushObjectOutOfObjectBoxB(TObj *, TObj *);

void solidSceneryEnemyCollision(TObj *o, TObj *p)
{
    if (pushObjectOutOfObjectBoxB(o, p)) SPAD.s19e = 0;
}
