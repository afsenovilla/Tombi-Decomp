// VERSION SLPS_01144 dspsub3 800EE89C 444
#if defined(VERSION_SLPS_01144)
// JP version of PAL d0spsub3 800ed5fc (subScreenOpenMessage.c)
typedef struct { short p0[4]; unsigned short off8; short pa[3]; unsigned short ids[1]; } Bank;
extern unsigned char D_800FF49C;
extern unsigned char D_800FF49E, D_800FF4A6, D_800FF4A8, D_800FF4AB;
extern int D_800FF4AC, D_800FF4B0, D_800FF4B4;
extern short D_800FF4BA, D_800FF4BC, D_800FF4BE, D_800FF4C8;
extern void *D_800FF530;
extern unsigned short D_800FF552, D_800FF562, D_800FF56E;
extern short D_800FF550, D_800FF554, D_800FF556, D_800FF558, D_800FF55A, D_800FF55C, D_800FF55E;
extern short D_800FF560, D_800FF564, D_800FF566, D_800FF568, D_800FF56A, D_800FF56C;
extern short D_800A6190[][4];
extern Bank *WFM3_DATA[];

unsigned char *func_800EE89C(int kind, int add, int bank)
{
    int i;
    int off;
    unsigned char *o;
    Bank **tb;
    i = 0;
    o = &D_800FF49C;
    for (; i < 4; i++) {
        if (D_800A6190[i][0] == -1) {
            char *b;
            Bank **base;
            *o = 1;
            D_800FF49E = 0x1A;
            D_800FF4A6 = 0xA;
            D_800FF4BA = 0x14;
            D_800FF4AB = 0x32;
            D_800FF4AC = 0xA00000;
            D_800FF4B0 = 0x780000;
            D_800FF550 = 4;
            D_800FF55E = 0x100;
            D_800FF560 = 0x30;
            D_800FF556 = -0x80;
            D_800FF558 = -0x30;
            D_800FF55A = 0x20;
            D_800FF4A8 = kind;
            D_800FF4C8 = 0;
            D_800FF4B4 = 0;
            D_800FF552 = 0xFFFF;
            D_800FF554 = 0;
            D_800FF55C = 0x48;
            D_800FF562 = 0xFFFF;
            D_800FF566 = i;
            D_800FF568 = 1;
            D_800FF56A = 8;
            D_800FF56C = 8;
            D_800FF56E = 0xFFFF;
            D_800FF4BC = 1;
            D_800FF4BE = 0;
            base = WFM3_DATA;
            D_800A6190[i][0] = 0;
            tb = &base[bank];
            off = kind * 2 + 0x10;
            b = (char *)*tb;
            D_800FF564 = *(unsigned short *)(b + off) + add;
            b = (char *)*tb;
            b = b + ((Bank *)b)->off8;
            b += *(unsigned short *)(b + ((unsigned short)D_800FF564 << 1));
            D_800FF530 = b;
            return o;
        }
    }
}
#endif
