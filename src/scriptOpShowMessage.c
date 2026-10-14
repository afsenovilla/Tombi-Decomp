// FUNC 8003ab34 68 MAIN0
// MATCHING 8003ab34 68
#include "scriptctx.h"
extern void printInfoMessage(int, int);
void scriptOpShowMessage(void)
{
    ScriptCtx *g = SCRIPT_CTX;
    printInfoMessage(g->arg[0], 2);
    g->pc++;
}
