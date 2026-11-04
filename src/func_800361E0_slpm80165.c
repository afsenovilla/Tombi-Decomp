// VERSION SLPM_80165 slpm80165 800361E0 180
#if defined(VERSION_SLPM_80165)
// Demo version: JP C of src/func_80038F70_slps01144.c (func_80038F70_slps01144) (1.000 masked) [-O2 -G0] (tools/demo_sweep.py)
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
void func_800361E0_slpm80165(unsigned char m)
{
    ScriptCtx *o = SCRIPT_CTX;
    unsigned char *code = SCRIPT_CODE;
    unsigned char dst;
    unsigned char *p;
    dst = code[o->pc + 1];
    p = (unsigned char *)(o->pc + (int)code);
    p += 2;
    o->vars[dst] = rd((unsigned char *)SCRIPT_CTX, m, p);
    if (m == 0)
        o->pc += 3;
    else
        o->pc += 6;
}
#endif
