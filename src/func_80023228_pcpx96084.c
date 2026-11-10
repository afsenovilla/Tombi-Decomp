// VERSION PCPX_96084 tomba 80023228 240
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// BGM fades: fade-in (D_800C8260) raises both channel volumes by one up to 127, fade-out (D_800C8264) lowers them
// to 0.
extern int D_800C8260, D_800C8264;
extern short D_800E2474, D_800E2476;
extern short D_800E518C;
void SsSeqSetVol(short a, short b, short c);

void func_80023228_pcpx96084(void)
{
    if (D_800C8260 != 0) {
        short l = ++D_800E2474;
        short r = ++D_800E2476;
        if (l >= 127) D_800C8260 = 0;
        SsSeqSetVol(D_800E518C, l, r);
    }
    if (D_800C8264 != 0) {
        short l = --D_800E2474;
        short r = --D_800E2476;
        SsSeqSetVol(D_800E518C, l, r);
        if (D_800E2474 == 0) D_800C8264 = 0;
    }
}
#endif
