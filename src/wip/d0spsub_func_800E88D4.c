// FUNC 800e88d4 292 D0SPSUB
typedef struct { unsigned short a, b; } Pair;
extern Pair D_8007A024[];
extern int D_8009C984;
extern unsigned char D_8009C990, D_8009CD98, D_8009CD99, D_8009CD9A, D_8009CD9C, D_8009CD9D, D_8009CD9E;
extern unsigned char D_8009CE4A, D_8009CFFB, D_8009D089, D_8009D08A, D_8009D08B, D_8009D08C, D_8009D08D;
extern unsigned char D_8009D08E, D_8009D08F, D_8009D090, D_8009D096, D_8009D2B3;
extern char D_800A603B;
extern unsigned short D_800A6040, D_800A60FC;

void func_800E88D4(void)
{
    if (D_8009CFFB == 1) {
        D_8009CD9C = D_8009D089;
        D_8009CD98 = D_8009D08A;
        D_8009CD9D = D_8009D08B;
        D_8009CD99 = D_8009D08C;
        D_8009CD9E = D_8009D08D;
        D_8009CD9A = D_8009D08E;
        D_8009C984 = D_8009D08F;
        D_8009D2B3 = D_8009D090;
        if (D_8009D096 != 0 || D_8009CE4A == 0xFF) D_8009C984 = D_8009D08F | 0x40;
        D_800A6040 = D_8007A024[D_8009C990].a;
        D_800A603B = 0;
        D_8009CFFB = 0;
        D_800A60FC = D_8007A024[D_8009D2B3].b;
    }
}
