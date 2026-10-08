// FUNC 800386f4 236 MAIN0
// MATCHING 800386f4 236
extern unsigned char *DAT_8009f0f0;
extern unsigned char *DAT_8009d60c;

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

void func_800386F4(unsigned char m)
{
    unsigned char *o = DAT_8009f0f0;
    unsigned char *code = DAT_8009d60c;
    int a, b;
    unsigned char *p;
    a = rd(o, 0, &code[*(unsigned short *)(o + 0x8a) + 1]);
    p = (unsigned char *)(*(unsigned short *)(o + 0x8a) + (int)code);
    p += 2;
    b = rd(DAT_8009f0f0, m, p);
    if (a == b)
        o[0x89] = 0;
    else if (a < b)
        o[0x89] = 1;
    else
        o[0x89] = 2;
    if (m == 0)
        *(unsigned short *)(o + 0x8a) += 3;
    else
        *(unsigned short *)(o + 0x8a) += 6;
}
