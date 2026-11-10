// VERSION PCPX_96084 tomba 800676E4 120
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
extern short D_800E2454, D_800E2456, D_800E2458, D_800E245A;
extern int D_800E245C, D_800E2460;
extern int D_1F800174;
extern void func_8006897C(void);

void func_800676E4_pcpx96084(char *o)
{
    D_800E2454 = 0x20;
    D_800E2456 = 0x10;
    D_800E2458 = *(unsigned short *)(o + 16);
    D_800E245A = 0;
    D_800E245C = (*(int **)(o + 68))[*(short *)(o + 18)];
    D_800E2460 = D_1F800174;
    func_8006897C();
}
#endif
