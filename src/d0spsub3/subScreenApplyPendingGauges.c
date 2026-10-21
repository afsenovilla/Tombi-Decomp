// FUNC 800e882c 292 D0SPSUB3
// MATCHING 800e882c 292
typedef struct { unsigned short a, b; } Pair;
extern Pair D_8007A024[];
extern int D_8009C984;
extern unsigned char D_8009C990, D_8009CD98, D_8009CD99, D_8009CD9A, D_8009CD9C, D_8009CD9D, D_8009CD9E;
extern unsigned char D_8009CE4A, D_8009CFFB, D_8009D089, D_8009D08A, D_8009D08B, D_8009D08C, D_8009D08D;
extern unsigned char D_8009D08E, D_8009D08F, D_8009D090, D_8009D096, D_8009D2B3;
extern char D_800A603B;
extern unsigned short D_800A6040, D_800A60FC;

static inline Pair *gp(int i) { return &D_8007A024[i]; }
typedef struct { char p[3]; char b3; char p4[4]; unsigned short w8; } Obj;
extern Obj D_800A6038;
void subScreenApplyPendingGauges(void)
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
        D_800A6038.w8 = gp(D_8009C990)->a;
        D_800A60FC = gp(D_8009D2B3)->b;
        D_800A603B = 0;
        D_8009CFFB = 0;
    }
}
