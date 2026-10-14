// FUNC 8003840c 264 MAIN0
// MATCHING 8003840c 264
#include "scriptctx.h"
extern int SCRIPT_CODE;

void scriptOpBranch(unsigned char op)
{
    short tmp;
    int code = SCRIPT_CODE;
    ScriptCtx *o = SCRIPT_CTX;
    unsigned char r;
    unsigned char *p;
    int k;
    switch (op) {
    case 8:
        r = 1;
        break;
    case 9:
    case 11:
        r = o->cmpFlag == 0;
        break;
    case 10:
    case 12:
        r = o->cmpFlag != 0;
        break;
    case 14:
        r = o->cmpFlag == 2;
        break;
    case 16:
        r = o->cmpFlag != 1;
        break;
    case 13:
        r = o->cmpFlag == 1;
        break;
    case 15:
        r = o->cmpFlag != 2;
        break;
    }
    if (r) {
        p = (unsigned char *)(o->pc + code) + 1;
        for (k = 0; k < 2; k++)
            ((char *)&tmp)[k] = p[k];
        (*(short*)&o->pc) = tmp;
    } else {
        (*(short*)&o->pc) += 3;
    }
}
