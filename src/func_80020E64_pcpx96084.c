// VERSION PCPX_96084 tomba 80020E64 64
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
extern unsigned short D_800DE0D8;
extern unsigned short D_800DE0DA;
extern unsigned short *D_800A1AA4[];
extern char *D_800DE0B0;

void func_80020E64_pcpx96084(void)
{
    unsigned short v = D_800A1AA4[D_800DE0D8][D_800DE0DA];
    *(short *)(D_800DE0B0 + 78) = 0;
    *(short *)(D_800DE0B0 + 76) = v;
}
#endif
