// VERSION PCPX_96084 tomba 8002357C 88
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
typedef struct { int mask; int mode; short depthL, depthR; int delay, feedback; } RevAttr;
extern void SpuSetReverbModeParam(RevAttr *);
extern void SpuSetReverbDepth(RevAttr *);
extern void SpuSetReverbVoice(int, int);
extern void SpuSetReverb(int);

void func_8002357C_pcpx96084(int mode, int depth)
{
    RevAttr r;
    r.mask = 7;
    r.mode = mode | 0x100;
    r.depthL = depth;
    r.depthR = depth;
    SpuSetReverbModeParam(&r);
    SpuSetReverbDepth(&r);
    SpuSetReverbVoice(1, 0xffffff);
    SpuSetReverb(1);
}
#endif
