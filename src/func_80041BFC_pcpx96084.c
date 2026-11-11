// VERSION PCPX_96084 tomba 80041BFC 120
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
extern unsigned short D_800DE760;

void func_80041BFC_pcpx96084(char *o)
{
    if (*(unsigned short *)(o + 46) & 1) {
        if (D_800DE760 & 0x1000) {
            if (D_800DE760 & 0x8000) *(short *)(o + 46) = 5;
            else *(short *)(o + 46) = 7;
        } else *(short *)(o + 46) = 3;
    } else {
        if (D_800DE760 & 0x1000) {
            if (D_800DE760 & 0x2000) *(short *)(o + 46) = 4;
            else *(short *)(o + 46) = 6;
        } else *(short *)(o + 46) = 2;
    }
}
#endif
