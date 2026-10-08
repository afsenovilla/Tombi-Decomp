// FUNC 8003f1bc 68 MAIN0
// MATCHING 8003f1bc 68
extern char *D_8007B5AC[];
char *func_8003F1BC(short idx, unsigned char n)
{
    char *p;
    int o;
    p = D_8007B5AC[n];
    o = *(unsigned short *)(p + 8);
    p = p + o;
    o = *(unsigned short *)(p + (idx / 8 << 1));
    return p + o;
}
