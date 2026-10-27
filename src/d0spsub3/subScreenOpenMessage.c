// FUNC 800ed5fc 448 D0SPSUB3
// MATCHING 800ed5fc 448
typedef struct { short p0[4]; unsigned short off8; short pa[3]; unsigned short ids[1]; } Bank;
extern unsigned char SUBSCREEN_MESSAGE_BOX;
extern unsigned char D_80100A92, D_80100A9A, D_80100A9C, D_80100A9F;
extern int D_80100AA0, D_80100AA4, D_80100AA8;
extern short D_80100AAE, D_80100AB0, D_80100AB2, D_80100ABC;
extern void *D_80100B24;
extern unsigned short D_80100B46, D_80100B56, D_80100B62;
extern short D_80100B44, D_80100B48, D_80100B4A, D_80100B4C, D_80100B4E, D_80100B50, D_80100B52;
extern short D_80100B54, D_80100B58, D_80100B5A, D_80100B5C, D_80100B5E, D_80100B60;
extern short MESSAGE_SLOTS[][4];
extern Bank *WFM3_DATA[];

unsigned char *subScreenOpenMessage(int kind, int add, int bank)
{
    int i;
    int off;
    unsigned char *o;
    Bank **tb;
    do { i = 0; o = &SUBSCREEN_MESSAGE_BOX; } while (0);
    for (; i < 4; i++) {
        if (MESSAGE_SLOTS[i][0] == -1) {
            char *b;
            Bank **base;
            *o = 1;
            D_80100A92 = 0x1A;
            D_80100A9A = 0xA;
            D_80100AAE = 0x14;
            D_80100A9F = 0x32;
            D_80100AA0 = 0xA00000;
            D_80100AA4 = 0x780000;
            D_80100B44 = 4;
            D_80100B52 = 0x100;
            D_80100B54 = 0x30;
            D_80100B4A = -0x80;
            D_80100B4C = -0x30;
            D_80100B4E = 0x20;
            D_80100B50 = 0x48;
            D_80100B5E = 8;
            D_80100A9C = kind;
            D_80100ABC = 0;
            D_80100AA8 = 0;
            D_80100B46 = 0xFFFF;
            D_80100B48 = 0;
            D_80100B56 = 0xFFFF;
            D_80100B5A = i;
            D_80100B5C = 1;
            D_80100B60 = 6;
            D_80100B62 = 0xFFFF;
            D_80100AB0 = 1;
            D_80100AB2 = 0;
            base = WFM3_DATA;
            MESSAGE_SLOTS[i][0] = 0;
            tb = &base[bank];
            off = kind * 2 + 0x10;
            b = (char *)*tb;
            D_80100B58 = *(unsigned short *)(b + off) + add;
            b = (char *)*tb;
            b = b + ((Bank *)b)->off8;
            b += *(unsigned short *)(b + ((unsigned short)D_80100B58 << 1));
            D_80100B24 = b;
            return o;
        }
    }
}
