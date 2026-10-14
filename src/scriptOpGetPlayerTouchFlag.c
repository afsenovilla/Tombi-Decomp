// FUNC 8003ab78 36 MAIN0
// MATCHING 8003ab78 36
#include "scriptctx.h"
#include "player.h"
void scriptOpGetPlayerTouchFlag(void)
{
    ScriptCtx *p = SCRIPT_CTX;
    p->arg[0] = PLAYER.obj.b69;
    ((*(short*)&p->pc))++;
}
