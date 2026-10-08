// FUNC 80016fc0 124 MAIN0
// FLAGS -O2 -G0 -fno-cse-follow-jumps
// NOTE: solo difiere en 4 addiu (n-1 calculado desde t en vez de n); requiere -fno-cse-follow-jumps
void FUN_80016fc0(unsigned int a)
{
    unsigned int n;
    unsigned int v;
    unsigned int t;
    unsigned int *p;
    unsigned int *q;
    unsigned int *r;

    n = 0x327;
    p = (unsigned int *)(a & 0xffffff);
loop1:
    v = *p;
    q = p - 1;
    if (v != (unsigned int)q) {
        t = n--; if (t == 0) return;
        p = q;
        goto loop1;
    }
    r = p;
    t = n--; if (t == 0) return;
    p--;
loop3:
    v = *p;
    q = p - 1;
    if (v == (unsigned int)q) {
        t = n--; if (t == 0) return;
        p = (unsigned int *)v;
        goto loop3;
    }
    *r = (unsigned int)p;
    t = n--; if (t == 0) return;
    p--;
    goto loop1;
}
