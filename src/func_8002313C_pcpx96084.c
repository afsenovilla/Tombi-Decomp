// VERSION PCPX_96084 tomba 8002313C 112
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
extern short D_800E2474[];
extern short D_800E2476;
extern int D_800C8260;
extern short D_800E518C;
extern void SsSeqSetVol(short, short, short);

void func_8002313C_pcpx96084(void)
{
    short a = ++D_800E2474[0];
    short b = ++D_800E2476;
    if (a >= 127) D_800C8260 = 0;
    SsSeqSetVol(D_800E518C, a, b);
}
#endif
