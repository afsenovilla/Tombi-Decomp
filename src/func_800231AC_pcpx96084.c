// VERSION PCPX_96084 tomba 800231AC 124
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
extern short D_800E2474[];
extern short D_800E2476;
extern int D_800C8264;
extern short D_800E518C;
extern void SsSeqSetVol(short, short, short);

void func_800231AC_pcpx96084(void)
{
    short a = --D_800E2474[0];
    short b = --D_800E2476;
    SsSeqSetVol(D_800E518C, a, b);
    if (D_800E2474[0] == 0) D_800C8264 = 0;
}
#endif
