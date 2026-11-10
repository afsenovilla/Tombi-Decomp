// VERSION PCPX_96084 tomba 80023514 76
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
extern short D_800E518C;
extern void SsSeqStop(short);
extern void startAreaBgmWithReverb(short);

void func_80023514_pcpx96084(void)
{
    if (D_800E518C != -1) {
        SsSeqStop(D_800E518C);
        startAreaBgmWithReverb(D_800E518C);
        D_800E518C = -1;
    }
}
#endif
