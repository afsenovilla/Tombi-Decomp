// VERSION PCPX_96084 tomba 8003CE64 96
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
extern unsigned short D_800DE0DA;

void func_8003CE64_pcpx96084(unsigned char *o)
{
    *(short *)(o + 0x6c) = 0;
    *(short *)(o + 0x6e) = 0;
    *(short *)(o + 0x70) = 0;
    *(short *)(o + 0x72) = 0;
    *(int *)(o + 0x84) = 0;
    o[4]++;
    if (D_800DE0DA == 1) {
        *(int *)(o + 0x88) = 0x600;
        o[6] = 2;
    } else {
        *(int *)(o + 0x88) = 0x1000;
        o[6] = 0;
    }
    *(int *)(o + 0x8c) = 0;
    *(short *)(o + 10) = 0x11;
}
#endif
