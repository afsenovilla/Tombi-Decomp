// FUNC 800382fc 272 MAIN0
extern unsigned char *DAT_8009f0f0;
extern unsigned char *DAT_8009d60c;

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

void FUN_800382fc(void)
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
    o = DAT_8009f0f0;
    code = DAT_8009d60c;
    n = code[*(unsigned short *)(o + 0x8a) + 1];
    q = buf;
    for (i = 0; i < n; i++) {
        s = code + (*(unsigned short *)(o + 0x8a) + pc);
        c = *s;
        buf[i] = rd(DAT_8009f0f0, c, s + 1);
        if (c == 0)
            pc += 2;
        else
            pc += 5;
    }
    for (i = 0; i < n; i++)
        *(int *)(o + 0x1190 + i * 4) = q[i];
    *(unsigned short *)(o + 0x8a) += pc;
}
