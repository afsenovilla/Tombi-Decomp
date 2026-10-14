// FUNC 80038514 152 MAIN0
// MATCHING 80038514 152

#include "scriptctx.h"
extern unsigned char *SCRIPT_CODE;

void func_80038514(void)
{
    ScriptCtx *p = SCRIPT_CTX;
    unsigned char *code = SCRIPT_CODE;
    unsigned short tmp;
    int i;
    unsigned char *s;
    if (--p->vars[code[p->pc + 1]] > 0) {
        s = (unsigned char *)(p->pc + (int)code) + 2;
        for (i = 0; i < 2; i++) {
            ((unsigned char *)&tmp)[i] = *s++;
        }
        p->pc = tmp;
    } else {
        p->pc += 4;
    }
}
