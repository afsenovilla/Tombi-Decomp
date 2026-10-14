// FUNC 8012d1bc 68 X010
// MATCHING 8012d1bc 68
#include "TOBJ.H"
#include "gamestate.h"
extern void func_8012CFCC(TObj *);

void func_8012D1BC(TObj *o)
{
    if (o->subtype == 0 && GAME.selectedSection == 0) func_8012CFCC(o);
}
