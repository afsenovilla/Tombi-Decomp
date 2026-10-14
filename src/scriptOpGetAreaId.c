// FUNC 8003ae94 36 MAIN0
// MATCHING 8003ae94 36
#include "gv2.h"
#include "scriptctx.h"

void scriptOpGetAreaId(void)
{
    ScriptCtx *g = SCRIPT_CTX;
    g->arg[0] = GV2.bb1;
    g->pc++;
}
