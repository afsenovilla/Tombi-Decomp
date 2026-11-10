// VERSION PCPX_96084 tomba 800233FC 280
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 form of openAreaBgm (src/openAreaBgm.c): stops / closes the current sequence, opens the area's
// sequence with its VAB (per-area pairs at D_800A24C0) and starts it at volume 0 with the fade-in on.
typedef struct { unsigned short seq, vab; } BGMDEF;
extern short D_800E518C;
extern unsigned short D_800DE0D8;
extern BGMDEF D_800A24C0[];
extern unsigned short D_800E2B34[];
extern void *D_800DE750[];
extern unsigned short D_800DE088;
extern short D_800E2474, D_800E2476;
extern int D_800C8260, D_800C8264;
void SsSeqStop(short a);
void startAreaBgmWithReverb(short a);
short SsSeqOpen(void *p, short v);
void SsSetMVol(short a, short b);
void SsSeqSetVol(short a, short b, short c);
void SsSeqPlay(short a, int b, int c);

void func_800233FC_pcpx96084(void)
{
    unsigned short t;
    if (D_800E518C != -1) {
        SsSeqStop(D_800E518C);
        startAreaBgmWithReverb(D_800E518C);
        D_800E518C = -1;
    }
    {
        short *v = &D_800E2474;
        *v = 0;
        D_800E2476 = 0;
    t = D_800E2B34[D_800A24C0[D_800DE0D8].vab];
    D_800DE088 = t;
    D_800E518C = SsSeqOpen(D_800DE750[D_800A24C0[D_800DE0D8].seq], t);
    SsSetMVol(100, 100);
    SsSeqSetVol(D_800E518C, *v, D_800E2476);
    SsSeqPlay(D_800E518C, 1, 1);
    }
    D_800C8260 = 1;
    D_800C8264 = 0;
}
#endif
