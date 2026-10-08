// FUNC 80039580 1092 MAIN0
// MATCHING 80039580 1092
typedef struct { char p0[0x84]; unsigned char *p84; unsigned char b88; char p89; unsigned short w8a; } S;
extern int D_8009D69C;
extern unsigned short D_800A603C;
extern unsigned short D_8009C960;
extern unsigned short D_8009C962;
extern S D_8009F840;
extern S *D_8009F0F0;
extern unsigned char *D_8009D60C;
extern unsigned int D_800A0A10, D_800A0A14;
extern int D_8009D4F0;
extern int func_80038CD0(unsigned char);
extern int func_8003AFB4(unsigned char);

static __inline__ unsigned char run(void)
{
    S *p;
    int r;
    unsigned char x;
    D_8009F0F0 = &D_8009F840;
    p = D_8009F0F0;
    D_8009D60C = D_8009F840.p84;
    if (D_8009F840.b88 == 2 && ++D_800A0A10 >= D_800A0A14) {
        D_8009F840.b88 = 1;
    }
    x = p->b88;
    if (x != 1) return x;
    do {
        S *q = D_8009F0F0;
        unsigned char *b = D_8009D60C;
        unsigned char c = b[q->w8a];
        if (c < 0x80) {
            r = func_80038CD0(c);
        } else {
            r = func_8003AFB4(c);
        }
    } while (r != 0);
    return p->b88;
}

void func_80039580(void)
{
    if (D_8009D69C == 0) return;
    if (D_800A603C == 0x505) return;
    switch (D_8009C960) {
    case 0:
        switch (D_8009C962) {
        case 0 ... 2:
            D_8009D4F0 = run();
        }
        break;
    case 1:
        switch (D_8009C962) {
        case 0 ... 4:
            D_8009D4F0 = run();
        }
        break;
    case 2:
        switch (D_8009C962) {
        case 0 ... 2:
            D_8009D4F0 = run();
        }
        break;
    case 0x13:
        switch (D_8009C962) {
        case 0 ... 1:
            D_8009D4F0 = run();
        }
        break;
    }
}
