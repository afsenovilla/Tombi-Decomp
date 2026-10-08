// FUNC 80039580 1092 MAIN0
typedef struct { char p0[0x84]; unsigned char *p84; unsigned char b88; char p89; unsigned short w8a; } S;
extern int D_8009D69C;
extern unsigned short D_800A603C;
extern unsigned short D_8009C960;
extern unsigned short D_8009C962;
extern S D_8009F840;
extern S *D_8009F0F0;
extern unsigned char *D_8009D60C[];
extern unsigned int D_800A0A10, D_800A0A14;
extern int D_8009D4F0;
extern int func_80038CD0(void);
extern int func_8003AFB4(void);

static __inline__ void run(void)
{
    S *p = &D_8009F840;
    int r;
    D_8009F0F0 = p;
    D_8009D60C[0] = p->p84;
    if (p->b88 == 2 && ++D_800A0A10 >= D_800A0A14) {
        p->b88 = 1;
    }
    if (p->b88 == 1) {
        do {
            if (D_8009D60C[0][D_8009F0F0->w8a] < 0x80) {
                r = func_80038CD0();
            } else {
                r = func_8003AFB4();
            }
        } while (r != 0);
    }
    D_8009D4F0 = p->b88;
}

void func_80039580(void)
{
    if (D_8009D69C == 0) return;
    if (D_800A603C == 0x505) return;
    switch (D_8009C960) {
    case 0:
        switch (D_8009C962) {
        case 0: case 1: case 2:
            run();
        }
        break;
    case 1:
        switch (D_8009C962) {
        case 0: case 1: case 2: case 3: case 4:
            run();
        }
        break;
    case 2:
        switch (D_8009C962) {
        case 0: case 1: case 2:
            run();
        }
        break;
    case 0x13:
        switch (D_8009C962) {
        case 0: case 1:
            run();
        }
        break;
    }
}
