// VERSION PCPX_96084 tomba 8003CDCC 60
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
void func_8003CDCC_pcpx96084(unsigned char *o)
{
    *(int *)(o + 0x88) = 0x1000;
    *(short *)(o + 0x6c) = 0;
    *(short *)(o + 0x6e) = 0;
    *(short *)(o + 0x70) = 0;
    *(short *)(o + 0x72) = 0;
    *(int *)(o + 0x84) = 0;
    *(int *)(o + 0x8c) = 0;
    *(short *)(o + 10) = 0x11;
    o[6] = 0;
    o[4]++;
}
#endif
