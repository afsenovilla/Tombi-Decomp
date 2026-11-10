// VERSION PCPX_96084 tomba 80042BB0 80
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
extern unsigned short D_800DE760;

void func_80042BB0_pcpx96084(char *o)
{
    unsigned short f = *(unsigned short *)(o + 46);
    if (f & 8) {
        *(short *)(o + 170) = 0;
        *(short *)(o + 174) = 0;
    } else {
        *(short *)(o + 174) = (f & 7) << 5;
        if (D_800DE760 & 0x80) *(short *)(o + 170) = 0x200;
        else *(short *)(o + 170) = 0x100;
    }
}
#endif
