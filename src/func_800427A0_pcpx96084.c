// VERSION PCPX_96084 tomba 800427A0 148
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
extern char *D_800CCCCC;

void func_800427A0_pcpx96084(char *o)
{
    short a = *(short *)(o + 0xAA);
    int n = *(unsigned short *)(D_800CCCCC + 0x20);
    short b;
    b = a < 0 ? -a : a;
    switch (n) {
    case 0 ... 12:
        *(short *)(o + 0x7E) += 32;
        break;
    default:
        *(short *)(o + 0x7E) += 32 - (b >> 6);
        break;
    }
    if (*(short *)(o + 0x7E) > 1920) *(short *)(o + 0x7E) = 1920;
    if (*(short *)(o + 0x7E) < -1920) *(short *)(o + 0x7E) = -1920;
}
#endif
