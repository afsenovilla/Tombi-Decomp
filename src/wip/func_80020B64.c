// FUNC 80020b64 160 MAIN0
extern unsigned short D_8009C960;
extern unsigned short D_80078790[];
extern unsigned short D_8009C962;

int func_80020B64(int n)
{
    int i;
    int s = 0;
    int m;
    unsigned short *p;
    for (i = 0; i < D_8009C960; i++) s += D_80078790[i];
    p = &D_8009C962;
    s += *p;
    if (n >= 32) s++;
    m = n % 32;
    return *(int *)&p[s * 2 + 0x119] & (1 << m);
}
