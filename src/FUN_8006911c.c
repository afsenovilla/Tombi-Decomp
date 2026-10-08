// FUNC 8006911c 248 MAIN0
// MATCHING 8006911c 248
// CC gcc-2.8.1
typedef struct C { int *tab; char p[0xe3-4]; unsigned char n; char p1; unsigned short s; } C;
extern C *(*DAT_800981c0)(void);
unsigned FUN_8006911c(int unused, int k, int idx)
{
    C *c = DAT_800981c0();
    unsigned r;
    switch (k) {
    case 1: r = ((unsigned char *)c)[0xe8]; break;
    case 2: r = c->s; break;
    case 3: r = ((unsigned char *)c)[0xe4]; break;
    case 4:
        if (idx < 0) r = c->n;
        else if (idx < c->n) r = ((unsigned short *)c->tab)[idx];
        else r = 0;
        break;
    case 100: r = ((int *)c)[0x13]; break;
    default: r = 0;
    }
    return r;
}
