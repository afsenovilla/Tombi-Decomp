// VERSION SLPS_01144 dspsub 800EEE88 448
#if defined(VERSION_SLPS_01144)
// JP version of PAL d0spsub 800ede64 (subScreenOpenMessage.c)
typedef struct { short p0[4]; unsigned short off8; short pa[3]; unsigned short ids[1]; } Bank;
extern unsigned char D_8010A4C4;
extern unsigned char D_8010A4C6, D_8010A4CE, D_8010A4D0, D_8010A4D3;
extern int D_8010A4D4, D_8010A4D8, D_8010A4DC;
extern short D_8010A4E2, D_8010A4E4, D_8010A4E6, D_8010A4F0;
extern void *D_8010A558;
extern unsigned short D_8010A57A, D_8010A58A, D_8010A596;
extern short D_8010A578, D_8010A57C, D_8010A57E, D_8010A580, D_8010A582, D_8010A584, D_8010A586;
extern short D_8010A588, D_8010A58C, D_8010A58E, D_8010A590, D_8010A592, D_8010A594;
extern short D_800A6190[][4];
extern Bank *WFM3_DATA[];

unsigned char *func_800EEE88(int kind, int add, int bank)
{
    int i;
    int off;
    unsigned char *o;
    Bank **tb;
    i = 0;
    o = &D_8010A4C4;
    for (; i < 4; i++) {
        if (D_800A6190[i][0] == -1) {
            char *b;
            Bank **base;
            *o = 1;
            D_8010A4C6 = 0x1A;
            D_8010A4CE = 0xA;
            D_8010A4E2 = 0x14;
            D_8010A4D3 = 0x32;
            D_8010A4D4 = 0xA00000;
            D_8010A4D8 = 0x780000;
            D_8010A578 = 4;
            D_8010A586 = 0x100;
            D_8010A588 = 0x30;
            D_8010A57E = -0x80;
            D_8010A580 = -0x30;
            D_8010A582 = 0x20;
            D_8010A4D0 = kind;
            D_8010A4F0 = 0;
            D_8010A4DC = 0;
            D_8010A57A = 0xFFFF;
            D_8010A57C = 0;
            D_8010A584 = 0x48;
            D_8010A58A = 0xFFFF;
            D_8010A58E = i;
            D_8010A590 = 1;
            D_8010A592 = 8;
            D_8010A594 = 8;
            D_8010A596 = 0xFFFF;
            D_8010A4E4 = 1;
            D_8010A4E6 = 0;
            base = WFM3_DATA;
            D_800A6190[i][0] = 0;
            tb = &base[bank];
            off = kind * 2 + 0x10;
            b = (char *)*tb;
            D_8010A58C = *(unsigned short *)(b + off) + add;
            b = (char *)*tb;
            b = b + ((Bank *)b)->off8;
            b += *(unsigned short *)(b + ((unsigned short)D_8010A58C << 1));
            D_8010A558 = b;
            return o;
        }
    }
    return 0;
}
#endif
