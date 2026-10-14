// FUNC 8003a570 48 MAIN0
// MATCHING 8003a570 48
#include "globals.h"
#include "scriptctx.h"


void scriptOpReadGlobal(void)
{
    ScriptCtx *p = SCRIPT_CTX;
    p->arg[0] = SCRIPT_GLOBALS[p->arg[0]];
    p->pc++;
}
