// FUNC 8003a4cc 48 MAIN0
// MATCHING 8003a4cc 48
#include "scriptctx.h"
extern unsigned char SCRIPT_FLAGS[];

void scriptOpReadFlag(void)
{
    ScriptCtx *g = SCRIPT_CTX;
    int v = SCRIPT_FLAGS[g->arg[0]];
    g->pc++;
    g->arg[0] = v;
}
