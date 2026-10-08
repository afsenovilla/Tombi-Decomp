// FUNC 800216c4 88 MAIN0
// MATCHING 800216c4 88
extern unsigned short G[];
extern unsigned char *TAB[];

void FUN_800216c4(void)
{
    unsigned char *p = TAB[G[0]] + G[1] * 2;
    unsigned short *s = G + 0x4b2;
    s[p[0]] |= 1 << p[1];
}
