// FUNC 800ede64 452 D0SPSUB
// MATCHING 800ede64 452
typedef struct { short p0[4]; unsigned short off8; short pa[3]; unsigned short ids[1]; } Bank;
extern unsigned char D_8010A200;
extern unsigned char D_8010A202, D_8010A20A, D_8010A20C, D_8010A20F;
extern int D_8010A210, D_8010A214, D_8010A218;
extern short D_8010A21E, D_8010A220, D_8010A222, D_8010A22C;
extern void *D_8010A294;
extern unsigned short D_8010A2B6, D_8010A2C6, D_8010A2D2;
extern short D_8010A2B4, D_8010A2B8, D_8010A2BA, D_8010A2BC, D_8010A2BE, D_8010A2C0, D_8010A2C2;
extern short D_8010A2C4, D_8010A2C8, D_8010A2CA, D_8010A2CC, D_8010A2CE, D_8010A2D0;
extern short D_800A4648[][4];
extern Bank *D_1F800398[];

unsigned char *func_800EDE64(int kind, int add, int bank)
{
    int i;
    int off;
    unsigned char *o;
    Bank **tb;
    do { i = 0; o = &D_8010A200; } while (0);
    for (; i < 4; i++) {
        if (D_800A4648[i][0] == -1) {
            char *b;
            Bank **base;
            *o = 1;
            D_8010A202 = 0x1A;
            D_8010A20A = 0xA;
            D_8010A21E = 0x14;
            D_8010A20F = 0x32;
            D_8010A210 = 0xA00000;
            D_8010A214 = 0x780000;
            D_8010A2B4 = 4;
            D_8010A2C2 = 0x100;
            D_8010A2C4 = 0x30;
            D_8010A2BA = -0x80;
            D_8010A2BC = -0x30;
            D_8010A2BE = 0x20;
            D_8010A2C0 = 0x48;
            D_8010A2CE = 8;
            D_8010A20C = kind;
            D_8010A22C = 0;
            D_8010A218 = 0;
            D_8010A2B6 = 0xFFFF;
            D_8010A2B8 = 0;
            D_8010A2C6 = 0xFFFF;
            D_8010A2CA = i;
            D_8010A2CC = 1;
            D_8010A2D0 = 6;
            D_8010A2D2 = 0xFFFF;
            D_8010A220 = 1;
            D_8010A222 = 0;
            base = D_1F800398;
            D_800A4648[i][0] = 0;
            tb = &base[bank];
            off = kind * 2 + 0x10;
            b = (char *)*tb;
            D_8010A2C8 = *(unsigned short *)(b + off) + add;
            b = (char *)*tb;
            b = b + ((Bank *)b)->off8;
            b += *(unsigned short *)(b + ((unsigned short)D_8010A2C8 << 1));
            D_8010A294 = b;
            return o;
        }
    }
    return 0;
}
