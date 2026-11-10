// VERSION PCPX_96084 tomba 80041A08 72
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
extern unsigned short D_800DE760;

void func_80041A08_pcpx96084(char *o)
{
    unsigned short f = *(unsigned short *)(o + 46) & 1;
    *(unsigned short *)(o + 46) = f;
    if (D_800DE760 & 0x2000) *(unsigned short *)(o + 46) = 0;
    else if (D_800DE760 & 0x8000) *(unsigned short *)(o + 46) = 1;
    else *(unsigned short *)(o + 46) = f | 2;
}
#endif
