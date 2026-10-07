// FUNC 800216c4 88 MAIN0
extern unsigned short G[];
extern unsigned char *TAB[];

void FUN_800216c4(void)
{
    unsigned char *p = TAB[G[0]] + G[1] * 2;
    unsigned short *f = (unsigned short *)((char *)G + 0x964);
    f[p[0]] |= 1 << p[1];
}
