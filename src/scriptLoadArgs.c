// FUNC 800382fc 272 MAIN0
// MATCHING 800382fc 272
#include "scriptctx.h"
extern unsigned char *SCRIPT_CODE;

static __inline__ int rd(unsigned char *g, unsigned char c, unsigned char *t)
{
    int tmp;
    int k;
    if (c == 0)
        return *(int *)(g + *t * 4 + 0x1090);
    for (k = 0; k < 4; k++)
        ((char *)&tmp)[k] = *t++;
    return tmp;
}

void scriptLoadArgs(void)
{
    int buf[16];
    unsigned char *o;
    unsigned char *code;
    int *q;
    int pc;
    int i, n;
    unsigned char c;
    unsigned char *s;
    pc = 2;
    o = SCRIPT_CTX;
    code = SCRIPT_CODE;
    n = code[*(unsigned short *)(o + 0x8a) + 1];
    q = buf;
    for (i = 0; i < n; i++) {
        s = code + (*(unsigned short *)(o + 0x8a) + pc);
        c = *s++;
        buf[i] = rd(SCRIPT_CTX, c, s);
        if (c == 0)
            pc += 2;
        else
            pc += 5;
    }
    for (i = 0; i < n; i++)
        *(int *)(o + 0x1190 + i * 4) = q[i];
    *(unsigned short *)(o + 0x8a) += pc;
}
