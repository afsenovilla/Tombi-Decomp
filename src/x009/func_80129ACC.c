// FUNC 80129acc 152 X009
// MATCHING 80129acc 152
extern unsigned char D_8009C93A;
extern unsigned char D_8009C938;
extern unsigned char D_800A60A1;
extern unsigned char D_800A6100;
extern unsigned short *D_800A6078;
extern short D_800A604E;

static __inline__ short chk(void)
{
    if (D_8009C93A == 0) return 0;
    if (D_8009C938 != 0) return 0;
    if (D_800A60A1 == 0 && D_800A6100 == 0) return 0;
    if ((unsigned short)(D_800A6078[1] - 0x1034) >= 0xc8) return 0;
    if (D_800A604E < -0x1d0) return 0;
    return 1;
}

int func_80129ACC(void)
{
    return chk();
}
