// VERSION PCPX_96084 tomba 80041B80 124
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
extern unsigned short D_800DE760;

void func_80041B80_pcpx96084(char *o)
{
    unsigned short f = *(unsigned short *)(o + 46) & 1;
    *(unsigned short *)(o + 46) = f;
    if (D_800DE760 & 0x1000) {
        if (D_800DE760 & 0x8000) *(short *)(o + 46) = 5;
        else if (D_800DE760 & 0x2000) *(short *)(o + 46) = 4;
        else if (f) *(short *)(o + 46) = 7;
        else *(short *)(o + 46) = 6;
    } else {
        if (D_800DE760 & 0x8000) *(short *)(o + 46) = 3;
        else if (D_800DE760 & 0x2000) *(short *)(o + 46) = 2;
        else if (f) *(short *)(o + 46) = 3;
        else *(short *)(o + 46) = 2;
    }
}
#endif
