// VERSION PCPX_96084 tomba 80067FD0 204
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Area plate sprite: position (0x40, 0x1C), x by area 0 / 1, image from the per-area-and-section index table
// D_800AE6A0 (7 sections per area) into D_800AE684, drawn by func_80068DD4.
extern unsigned short D_800DE0D8;
extern volatile unsigned short D_800DE0DA;
extern short D_800E2454, D_800E2456, D_800E2458;
extern volatile short D_800E245A;
extern void *D_800E2464;
extern void *D_800E2468;
extern short D_800AE6A0[][7];
extern void *D_800AE684[];
extern char D_800AB130[];
extern void func_80068DD4(void);

void func_80067FD0_pcpx96084(void)
{
    D_800E2454 = 0x40;
    D_800E2456 = 0x1c;
    switch (D_800DE0D8) {
    case 0:
        D_800E2458 = 0xd;
        break;
    case 1:
        D_800E2458 = 0xc;
        break;
    }
    D_800E245A = 0;
    D_800E2464 = D_800AE684[D_800AE6A0[D_800DE0D8][D_800DE0DA]];
    D_800E2468 = D_800AB130;
    func_80068DD4();
}
#endif
