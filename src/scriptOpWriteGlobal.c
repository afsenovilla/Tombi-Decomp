// FUNC 8003a5a0 52 MAIN0
// MATCHING 8003a5a0 52
#include "globals.h"
#include "scriptctx.h"
void scriptOpWriteGlobal(void)
{
    ScriptCtx *g = SCRIPT_CTX;
    SCRIPT_GLOBALS[g->arg[0]] = g->arg[1];
    g->pc++;
}
