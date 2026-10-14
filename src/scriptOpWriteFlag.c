// FUNC 8003a498 52 MAIN0
// MATCHING 8003a498 52
#include "scriptctx.h"
extern unsigned char SCRIPT_FLAGS[];

void scriptOpWriteFlag(void)
{
    ScriptCtx *g = SCRIPT_CTX;
    SCRIPT_FLAGS[g->arg[0]] = g->arg[1];
    g->pc++;
}
