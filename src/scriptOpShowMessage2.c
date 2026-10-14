// FUNC 8003ac50 68 MAIN0
// MATCHING 8003ac50 68
#include "scriptctx.h"
extern void printInfoMessage(int, int);

void scriptOpShowMessage2(void)
{
    ScriptCtx *p = SCRIPT_CTX;
    printInfoMessage(p->arg[0], 2);
    p->pc += 1;
}
