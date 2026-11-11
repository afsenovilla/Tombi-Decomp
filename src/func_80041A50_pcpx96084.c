// VERSION PCPX_96084 tomba 80041A50 152
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 setFacingAndUpDownFromPad (src/setFacingAndUpDownFromPad.c, demo branch): the pad word is D_800DE760,
// read once for the facing.
extern unsigned short D_800DE760;

void func_80041A50_pcpx96084(char *o)
{
    unsigned short v = *(unsigned short *)(o + 0x2e) & 1;
    unsigned short p;
    volatile unsigned short *q;
    *(unsigned short *)(o + 0x2e) = v;
    p = D_800DE760;
    if (p & 0x2000) {
        *(short *)(o + 0x2e) = 0;
    } else {
        *(unsigned short *)(o + 0x2e) = (p & 0x8000) ? 1 : (v | 2);
    }
    q = (volatile unsigned short *)&D_800DE760;
    if (*q & 0x1000)
        *(unsigned short *)(o + 0x2e) |= 8;
    if (*q & 0x4000)
        *(unsigned short *)(o + 0x2e) |= 4;
}
#endif
