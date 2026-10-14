// FUNC 8003ab9c 36 MAIN0
// MATCHING 8003ab9c 36
#include "scriptctx.h"
#include "player.h"
#include "TOBJ.H"
void scriptOpGetPlayerHitFlag(void)
{
    ScriptCtx *p = SCRIPT_CTX;
    p->arg[0] = PLAYER.obj.b9e;
    (*(short*)&p->pc) += 1;
}
