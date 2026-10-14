// FUNC 8003aeb8 36 MAIN0
// MATCHING 8003aeb8 36
#include "plx.h"
#include "scriptctx.h"

void scriptOpGetPlayerFieldE2(void)
{
    ScriptCtx *p = SCRIPT_CTX;
    p->arg[0] = PLX.b611a;
    p->pc += 1;
}
