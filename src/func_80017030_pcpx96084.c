// VERSION PCPX_96084 tomba 80017030 172
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 FlipFrame (src/FlipFrame.c, JP branch): the buffer index is the scratchpad halfword 0x1F8002A8.
extern unsigned short D_1F8002A8;
extern char *D_800E2B20, *D_800E2B2C;
extern char D_800E07D8[];
extern void PutDispEnv(void *), PutDrawEnv(void *), func_800175DC_pcpx96084(void *), DrawOTag(void *);

void func_80017030_pcpx96084(void)
{
    int n = D_1F8002A8 ^ 1;
    int idx = (short)n * 0xd10;
    D_1F8002A8 = n;
    D_800E2B2C = D_800E2B20;
    D_800E2B20 = D_800E07D8 + idx;
    PutDispEnv(D_800E2B20 + 0xca0);
    PutDrawEnv(D_800E2B20 + 0xcb4);
    func_800175DC_pcpx96084(D_800E2B2C + 0xc9c);
    DrawOTag(D_800E2B2C + 0xc9c);
}
#endif
