// VERSION PCPX_96084 tomba 8006781C 268
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Two sprite parts through D_800E2454 / func_8006897C: a fixed one (0x50, 0xC8) with the image at **(+0x48),
// then the part of D_800AE31C with its frame from the table at +0x50.
#include "TOBJ.H"
extern short D_800E2454[];
extern short D_800E2456, D_800E2458, D_800E245A;
extern void *D_800E245C;
extern int D_800E2460;
extern unsigned short D_800AE31C, D_800AE31E, D_800AE320;
extern short D_800AE322;
extern int D_1F800174;
extern void func_8006897C(void);

void func_8006781C_pcpx96084(TObj *o)
{
    int *c = &D_1F800174;
    D_800E2454[0] = 0x50;
    D_800E2456 = 0xc8;
    D_800E2458 = *(unsigned short *)((char *)o + 0x10) + D_800AE320;
    D_800E245A = 0;
    D_800E245C = **(void ***)((char *)o + 0x48);
    D_800E2460 = *c;
    func_8006897C();
    D_800E2454[0] = D_800AE31C;
    D_800E2456 = D_800AE31E;
    D_800E2458 = *(unsigned short *)((char *)o + 0x10) + D_800AE320;
    D_800E245A = 0;
    D_800E245C = (*(void ***)((char *)o + 0x50))[D_800AE322];
    D_800E2460 = *c;
    func_8006897C();
}
#endif
