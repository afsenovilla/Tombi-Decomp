// JPDEMO x00 800EBE6C 140
// Demo-only helper: counts +0xcf up to a per-level limit, then spawns an object of the current kind.
extern unsigned char D_8009B162;
extern short D_8006D698[];
extern unsigned char *func_80016F34(void);
void func_800EBE6C(unsigned char *o)
{
    unsigned char *p = &D_8009B162;
    unsigned char *n;
    short m;
    o[0xcf]++;
    m = D_8006D698[*p];
    if (m < (signed char)o[0xcf])
        o[0xcf] = m;
    n = func_80016F34();
    if (n != 0) {
        n[0] = 1;
        n[2] = *p;
        n[5] = 0;
        n[6] = 0;
    }
}
