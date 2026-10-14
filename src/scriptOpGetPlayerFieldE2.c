// FUNC 8003aeb8 36 MAIN0
// MATCHING 8003aeb8 36
#include "scriptctx.h"
extern unsigned char D_800A611A;

void scriptOpGetPlayerFieldE2(void)
{
    ScriptCtx *p = SCRIPT_CTX;
    p->arg[0] = D_800A611A;
    p->pc += 1;
}
