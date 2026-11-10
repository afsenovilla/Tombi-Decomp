// VERSION PCPX_96084 tomba 80055F70 316
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 form of weapon8to9Init (src/weapon8to9Init.c): state at +0xA4, constant CLUT words, depth -7.
extern int func_8008A79C_pcpx96084(void);
extern short *D_800CCCD8;
extern short D_800DE67C[], D_800DE67E[], D_800DE680[];
extern unsigned short *D_800F5018, *D_800F501C;
extern unsigned short D_800F4FEE[];

static __inline__ short getm(void)
{
    if (func_8008A79C_pcpx96084() == 1) return 0x80;
    if (func_8008A79C_pcpx96084() == 2) return 0x80;
    return 0x20;
}

void func_80055F70_pcpx96084(unsigned char *o)
{
    short *p;
    D_800CCCD8 = (short *)(o + 0xa4);
    if (o[6] == 0) {
        if (func_8008A79C_pcpx96084() != 1)
            func_8008A79C_pcpx96084();
        p = D_800CCCD8;
        *(short *)(o + 0xa8) = 0;
        p[4] = getm();
        D_800CCCD8[3] = (0x1e0 << 6) | (0x3e0 >> 4);
        D_800CCCD8[5] = (0x1e0 << 6) | (0x3e0 >> 4);
        D_800DE67C[0] = 0xf;
        D_800DE67E[0] = 8;
        D_800DE680[0] = 0x5a;
        *(signed char *)(o + 0xf) = -7;
        *(short *)(o + 0xa) = 0;
        (*(unsigned short **)(o + 0x40))[1] = D_800F5018[1];
        *(unsigned short *)(o + 0x16) = D_800F4FEE[0] - 0x10;
        (*(unsigned short **)(o + 0x44))[1] = D_800F501C[1];
        *(short *)(o + 0x80) = 0;
        *(short *)(o + 0x82) = 0;
        *(short *)(o + 0x2e) = 0;
        o[1] = 1;
        o[6] = 0;
    }
}
#endif
