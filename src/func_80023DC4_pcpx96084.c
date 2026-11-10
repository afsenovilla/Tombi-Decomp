// VERSION PCPX_96084 tomba 80023DC4 132
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
extern char D_800E5D40[];
extern char D_800AF23A[];
extern int D_1F8003E8, D_1F8003E4;
extern int func_80091D80(char *, char *);

int func_80023DC4_pcpx96084(void)
{
    int i;
    char *b;
    char *a;
    i = 0;
    b = D_800AF23A;
    a = D_800E5D40;
    for (; i < 39; i++) {
        if (func_80091D80(a, b) == 0) return -1;
        b += 64;
        a += 24;
    }
    D_1F8003E8 = 0;
    D_1F8003E4 = 0;
    return 0;
}
#endif
