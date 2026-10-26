// FUNC 800e999c 1852 D0SPSUB2
typedef struct { int d[3]; } B12;
typedef struct { char c[2]; } SJ;
typedef struct { char d[64]; } B64;
typedef struct { char d[32]; } B32;
typedef struct { char d[0x180]; } B384;
typedef struct { char d[0x988]; } BGame;
extern char D_800E1E28[];
extern char D_800E1E29, D_800E1E2A, D_800E1E2B;
extern char D_800E2028[];
extern char D_8009C960[];
extern int D_8009D2B4[], D_800A6048[];
extern short D_8009C982[];
extern char D_8009C972[], D_8009D2E4[], D_8009D2E5[], D_8009D2E6[], D_8009C992[];
extern unsigned short BUTTON_CONFIG_INDEX, D_8009D612;
extern unsigned char D_8009D67C, D_8009F0E8;
extern unsigned int D_8009C968;
extern unsigned char D_8009C980, D_8009C973;
extern B64 *D_800F6A34[];
extern B32 *D_800F6A2C[];
extern B384 *D_800F6A24[];
extern SJ *D_800F66B4[];
extern short D_1F8003D8;
extern void *memset(void *p, int c, int n);
extern void memCardStoreChecksum(void *p, int n);

void func_800E999C(int slot)
{
    char *h = D_800E1E28;
    unsigned char dg[5];
    unsigned char *q;
    int i, hh, mm;
    memset(h, 0, 0x2000);
    *(B12 *)D_8009D2B4 = *(B12 *)D_800A6048;
    D_8009C982[0] = 0;
    D_8009C972[0] = 1;
    h[0] = 'S';
    h[1] = 'C';
    h[2] = 0x11;
    h[3] = 1;
    D_8009D2E6[0] = BUTTON_CONFIG_INDEX;
    D_8009D2E4[0] = D_8009D67C;
    D_8009D2E5[0] = D_8009F0E8;
    D_8009C992[0] = D_8009D612;
    *(B64 *)(h + 4) = *D_800F6A34[D_1F8003D8];
    dg[1] = slot % 10;
    dg[0] = slot / 10 % 10;
    for (q = dg, i = 0; i < 2; i++) {
        *(SJ *)&h[0x16 + i * 2] = *D_800F66B4[*q++];
    }
    hh = D_8009C968 / 180000 % 1000;
    mm = D_8009C968 / 3000 - D_8009C968 / 180000 * 60;
    dg[0] = hh / 100 % 10;
    dg[4] = mm % 10;
    dg[1] = hh / 10 - hh / 100 * 10;
    dg[2] = hh % 10;
    dg[3] = mm / 10 % 10;
    for (q = dg, i = 0; i < 6; i++) {
        if (i != 3) {
            *(SJ *)&h[0x1E + i * 2] = *D_800F66B4[*q++];
        }
    }
    dg[1] = D_8009C980 % 10;
    dg[0] = (unsigned char)(D_8009C980 / 10) % 10;
    for (q = dg, i = 0; i < 2; i++) {
        *(SJ *)&h[0x30 + i * 2] = *D_800F66B4[*q++];
    }
    dg[1] = D_8009C973 % 10;
    dg[0] = (unsigned char)(D_8009C973 / 10) % 10;
    for (q = dg, i = 0; i < 2; i++) {
        *(SJ *)&h[0x40 + i * 2] = *D_800F66B4[*q++];
    }
    memset(h + 0x44, 0, 0x1C);
    *(B32 *)(h + 0x60) = *D_800F6A2C[D_1F8003D8];
    *(B384 *)(h + 0x80) = *D_800F6A24[D_1F8003D8];
    *(BGame *)D_800E2028 = *(BGame *)D_8009C960;
    memCardStoreChecksum(D_800E2028, 0xA00);
}
