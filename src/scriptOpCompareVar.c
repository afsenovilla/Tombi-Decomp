// FUNC 800386f4 236 MAIN0
// MATCHING 800386f4 236
#include "scriptctx.h"
extern unsigned char *SCRIPT_CODE;

static __inline__ int rd(unsigned char *g, unsigned char c, unsigned char *t)
{
    int tmp;
    int k;
    if (c == 0)
        return *(int *)(g + *t * 4 + 0x1090);
    for (k = 0; k < 4; k++)
        ((char *)&tmp)[k] = t[k];
    return tmp;
}

void scriptOpCompareVar(unsigned char m)
{
    ScriptCtx *o = SCRIPT_CTX;
    unsigned char *code = SCRIPT_CODE;
    int a, b;
    unsigned char *p;
    a = rd(o, 0, &code[o->pc + 1]);
    p = (unsigned char *)(o->pc + (int)code);
    p += 2;
    b = rd(SCRIPT_CTX, m, p);
    if (a == b)
        o->cmpFlag = 0;
    else if (a < b)
        o->cmpFlag = 1;
    else
        o->cmpFlag = 2;
    if (m == 0)
        o->pc += 3;
    else
        o->pc += 6;
}
