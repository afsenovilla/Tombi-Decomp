// FUNC 800387e0 244 MAIN0
// MATCHING 800387e0 244
#include "scriptctx.h"
extern unsigned char *SCRIPT_CODE;

void scriptOpUnaryVar(unsigned char op)
{
    ScriptCtx *b = SCRIPT_CTX;
    unsigned char *code = SCRIPT_CODE;
    unsigned char idx = code[b->pc + 1];
    ScriptCtx *q;
    int v;

    switch (op) {
    case 0x18:
        b->vars[idx] += 1;
        break;
    case 0x19:
        b->vars[idx] -= 1;
        break;
    case 0x1a:
        b->vars[idx] = ~b->vars[idx];
        break;
    case 0x1b:
        b->vars[idx] = -b->vars[idx];
        break;
    }
    v = b->vars[idx];
    q = SCRIPT_CTX;
    if (v == 0) {
        q->cmpFlag = 0;
    } else {
        q->cmpFlag = v < 0 ? 1 : 2;
    }
    (*(short*)&b->pc) = (*(short*)&b->pc) + 2;
}
