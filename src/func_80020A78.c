// FUNC 80020a78 116 MAIN0
// MATCHING 80020a78 116
extern unsigned short D_8009C960;
extern int D_8009C994[][8];

int func_80020A78(int n)
{
    int q = n / 32;
    int r = n % 32;
    int k;
    if (D_8009C960 == 0x10)
        k = 7;
    else if (D_8009C960 == 0x11)
        k = 0xc;
    else
        k = D_8009C960;
    return D_8009C994[k][q] & (1 << r);
}
