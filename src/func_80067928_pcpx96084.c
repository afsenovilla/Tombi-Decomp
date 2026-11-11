// VERSION PCPX_96084 tomba 80067928 268
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Two sprite parts through D_800E2454 / func_8006897C: a fixed one (0x50, 0xB0) with the image at **(+0x4C),
// then the part of D_800AE334 with its frame from the table at +0x50.
#include "TOBJ.H"
extern short D_800E2454[];
extern short D_800E2456, D_800E2458, D_800E245A;
extern void *D_800E245C;
extern int D_800E2460;
extern unsigned short D_800AE334, D_800AE336, D_800AE338;
extern short D_800AE33A;
extern int D_1F800174;
extern void func_8006897C(void);

void func_80067928_pcpx96084(TObj *o)
{
    int *c = &D_1F800174;
    D_800E2454[0] = 0x50;
    D_800E2456 = 0xb0;
    D_800E2458 = *(unsigned short *)((char *)o + 0x10) + D_800AE338;
    D_800E245A = 0;
    D_800E245C = **(void ***)((char *)o + 0x4c);
    D_800E2460 = *c;
    func_8006897C();
    D_800E2454[0] = D_800AE334;
    D_800E2456 = D_800AE336;
    D_800E2458 = *(unsigned short *)((char *)o + 0x10) + D_800AE338;
    D_800E245A = 0;
    D_800E245C = (*(void ***)((char *)o + 0x50))[D_800AE33A];
    D_800E2460 = *c;
    func_8006897C();
}
#endif
