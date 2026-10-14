// FUNC 8003ae94 36 MAIN0
// MATCHING 8003ae94 36
#include "scriptctx.h"
extern unsigned char D_8009D2B1;

void scriptOpGetAreaId(void)
{
    ScriptCtx *g = SCRIPT_CTX;
    g->arg[0] = D_8009D2B1;
    g->pc++;
}
