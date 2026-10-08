// FUNC 80059d44 468 MAIN0
// MATCHING 80059d44 468
typedef struct { char p0[0xc]; unsigned char b0c, b0d, b0e, b0f, b10, b11, b12, b13; short s14, s16, s18, s1a; } P;
extern P *G164;
extern int G1e0;
extern unsigned char G3d1, D36c4;
extern void FUN_80060008(void *, int, int, int, int), FUN_8005e580(int, void *), FUN_8005e634(void *, int);
extern int FUN_8005efe4(void);

void FUN_80059d44(unsigned char a)
{
    P *p;
    void *r;
    int t, m;
    r = G164;
    FUN_80060008(r, 0, 0, 0, 0);
    FUN_8005e580(G1e0 + 0x10, r);
    p = G164;
    r = G164 = (P *)((char *)p + 0xc);
    p->s18 = 0x140;
    p->s1a = 0x100;
    p->b0f = 3;
    p->s14 = 0;
    p->s16 = 0;
    p->b13 = 0x60;
    FUN_8005e634(r, 1);
    if (D36c4 == 2) {
        p->b10 = 0;
        p->b11 = 0;
    } else {
        p->b10 = a;
        p->b11 = a;
    }
    p->b12 = a;
    FUN_8005e580(G1e0 + 0x10, r);
    r = G164 = (P *)((char *)G164 + 0x10);
    if (G3d1 == 1 || (unsigned)(D36c4 - 1) < 2) {
        if (FUN_8005efe4() == 1) m = 0x80;
        else if (FUN_8005efe4() == 2) m = 0x80;
        else m = 0x20;
    } else {
        if (FUN_8005efe4() == 1) m = 0x100;
        else if (FUN_8005efe4() == 2) m = 0x100;
        else m = 0x40;
    }
    FUN_80060008(r, 0, 0, m, 0);
    FUN_8005e580(G1e0 + 0x10, r);
    G164 = (P *)((char *)G164 + 0xc);
}
