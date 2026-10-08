// FUNC 801274b0 76 X004
// MATCHING 801274b0 76

extern unsigned char D_8009CEA4[];

void func_801274B0(int n)
{
    int q = n / 8;
    int i = q + 0x58;
    D_8009CEA4[i] |= 1 << (n - q * 8);
}
