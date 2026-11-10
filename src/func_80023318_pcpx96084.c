// VERSION PCPX_96084 tomba 80023318 228
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 form of openAreaBgm (src/openAreaBgm.c): stops / closes the current sequence, opens the area's
// sequence with its VAB (per-area pairs at D_800A24C0) and plays it at volume 60.
typedef struct { unsigned short seq, vab; } BGMDEF;
extern short D_800E518C;
extern unsigned short D_800DE0D8;
extern BGMDEF D_800A24C0[];
extern unsigned short D_800E2B34[];
extern void *D_800DE750[];
extern unsigned short D_800DE088;
void SsSeqStop(short a);
void startAreaBgmWithReverb(short a);
short SsSeqOpen(void *p, short v);
void SsSetMVol(short a, short b);
void SsSeqSetVol(short a, short b, short c);
void SsSeqPlay(short a, int b, int c);

void func_80023318_pcpx96084(void)
{
    unsigned short t;
    if (D_800E518C != -1) {
        SsSeqStop(D_800E518C);
        startAreaBgmWithReverb(D_800E518C);
        D_800E518C = -1;
    }
    t = D_800E2B34[D_800A24C0[D_800DE0D8].vab];
    D_800DE088 = t;
    D_800E518C = SsSeqOpen(D_800DE750[D_800A24C0[D_800DE0D8].seq], t);
    SsSetMVol(100, 100);
    SsSeqSetVol(D_800E518C, 60, 60);
    SsSeqPlay(D_800E518C, 1, 1);
}
#endif
