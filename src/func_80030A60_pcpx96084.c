// VERSION PCPX_96084 tomba 80030A60 184
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Part init: anchor at the current position, +0x20 timer 60, four texture rows moved down by 72 * kind (+0xC).
void func_80030A60_pcpx96084(unsigned char *o)
{
    o[0xE] = 1;
    *(int *)(o + 0x30) = *(short *)(*(char **)(o + 0x40) + 2);
    *(int *)(o + 0x34) = *(short *)(o + 0x16);
    *(short *)(o + 0x20) = 0x3C;
    *(short *)(o + 0x22) = 0;
    o[5] = o[0xC] & 1;
    *(short *)(o + 0x9A) += o[0xC] * 72;
    *(short *)(o + 0xA2) += o[0xC] * 72;
    *(short *)(o + 0xAA) += o[0xC] * 72;
    *(short *)(o + 0xB2) += o[0xC] * 72;
    *(short *)(o + 0xBE) = 0x80;
    *(short *)(o + 0xBA) = -8;
}
#endif
