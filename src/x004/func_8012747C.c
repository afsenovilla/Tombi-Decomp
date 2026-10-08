// FUNC 8012747c 52 X004
// MATCHING 8012747c 52
extern unsigned char D_8009CEFC[];
int func_8012747C(int n)
{
    int q = n / 8; int r = n - q * 8; return D_8009CEFC[q] & (1 << r);
}
