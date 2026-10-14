// FUNC 8003a26c 108 MAIN0
// MATCHING 8003a26c 108
#include "TOBJ.H"
#include "scriptctx.h"
extern TObj *SCRIPT_OBJECTS[];

void scriptOpSetObjectPosition(void)
{
    ScriptCtx *g = SCRIPT_CTX;
    TObj *o = SCRIPT_OBJECTS[g->arg[0]];
    if (o != 0) {
        o->h->p.whole = g->arg[1];
        o->y.p.whole = g->arg[2];
        o->d->p.whole = g->arg[3];
    }
    g->pc++;
}
