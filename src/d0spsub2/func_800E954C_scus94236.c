// VERSION SCUS_94236 dspsub2 800E954C 1800
#if defined(VERSION_SCUS_94236)
// USA version of PAL d0spsub2 800e999c (memory card save header)
typedef struct { int d[3]; } B12;
typedef struct { char c[2]; } SJ;
typedef struct { char d[64]; } B64;
typedef struct { char d[32]; } B32;
typedef struct { char d[0x180]; } B384;
typedef struct { char d[0x988]; } BGame;
extern char D_800E1188[];
extern char D_800E1189, D_800E118A, D_800E118B;
extern char D_800E1388[];
extern char D_8009BCC8[];
extern int D_8009C61C[], D_800A53A8[];
extern short D_8009BCEA[];
extern char D_8009BCDA[], D_8009C64C[], D_8009C64D[], D_8009C64E[], D_8009BCFA[];
extern unsigned short D_8009E744, D_8009C97A;
extern unsigned char D_8009C9E4, D_8009E450;
extern unsigned int D_8009BCD0;
extern unsigned char D_8009BCE8, D_8009BCDB;
extern B64 D_800E74F4;
extern B32 D_800F55CC;
extern B384 D_800F544C;
extern SJ *D_800F5424[];
extern void *func_8005B814(void *p, int c, int n);
extern void memCardStoreChecksum(void *p, int n);

void func_800E954C(int slot_)
{
    int slot;
    char *h = D_800E1188;
    unsigned char dg[5];
    int i;
    unsigned char *p;
    { register int t asm("$17"); t = slot_; asm("" : "=r"(t) : "0"(t)); slot = t; }
    func_8005B814(h, 0, 0x2000);
    *(B12 *)D_8009C61C = *(B12 *)D_800A53A8;
    D_8009BCEA[0] = 0;
    D_8009BCDA[0] = 1;
    h[0] = 'S';
    h[1] = 'C';
    h[2] = 0x13;
    h[3] = 1;
    D_8009C64E[0] = D_8009E744;
    D_8009C64C[0] = D_8009C9E4;
    D_8009C64D[0] = D_8009E450;
    D_8009BCFA[0] = D_8009C97A;
    *(B64 *)(h + 4) = D_800E74F4;
    {
        int s10 = slot / 10;
        dg[1] = slot % 10;
        dg[0] = s10 % 10;
        p = dg; for (i = 0; i < 2; i++) {
            *(SJ *)&h[0x14 + i * 2] = *D_800F5424[*p++];
        }
    }
    {
        unsigned int hh = D_8009BCD0 / 216000;
        int h1 = hh % 1000;
        int m = D_8009BCD0 / 3600 - hh * 60;
        {
            int m10, h100, h10; unsigned char d4, d1, d2;
            h100 = h1 / 100; h10 = h1 / 10; m10 = m / 10;
            d4 = m - m10 * 10;
            d1 = h10 - h100 * 10;
            d2 = h1 - h10 * 10;
            h100 %= 10;
            dg[0] = h100;
            m10 %= 10;
            dg[3] = m10;
            dg[4] = d4;
            dg[1] = d1;
            dg[2] = d2;
        }
    }
    {
        p = dg; for (i = 0; i < 6; i++) {
            if (i != 3) *(SJ *)&h[0x1C + i * 2] = *D_800F5424[*p++];
        }
    }
    {
        unsigned char q10 = D_8009BCE8 / 10;
        dg[1] = D_8009BCE8 % 10;
        dg[0] = q10 % 10;
        p = dg; for (i = 0; i < 2; i++) {
            *(SJ *)&h[0x2E + i * 2] = *D_800F5424[*p++];
        }
    }
    {
        unsigned char q10 = D_8009BCDB / 10;
        dg[1] = D_8009BCDB % 10;
        dg[0] = q10 % 10;
        p = dg; for (i = 0; i < 2; i++) {
            *(SJ *)&h[0x3E + i * 2] = *D_800F5424[*p++];
        }
    }
    func_8005B814(h + 0x44, 0, 0x1C);
    {
        register char *g asm("$16");
        g = h;
        asm("" : "=r"(g) : "0"(g));
        *(B32 *)(g + 0x60) = D_800F55CC;
        *(B384 *)(g + 0x80) = D_800F544C;
    }
    *(BGame *)D_800E1388 = *(BGame *)D_8009BCC8;
    memCardStoreChecksum(D_800E1388, 0xA00);
}
#endif
