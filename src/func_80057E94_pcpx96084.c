// VERSION PCPX_96084 tomba 80057E94 44
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 random generator: the PAL rand (src/rand.c) without the final & 0x7FFF; seed D_800A84B8.
extern unsigned long D_800A84B8;

int func_80057E94_pcpx96084(void)
{
    D_800A84B8 = D_800A84B8 * 0x41C64E6D + 12345;
    return D_800A84B8 >> 16;
}
#endif
