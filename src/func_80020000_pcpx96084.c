// VERSION PCPX_96084 tomba 80020000 252
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 form of PAL main0 80019F98 (gameplayStateDispatcher.c): seven modes, no game-over mode.
extern unsigned short *D_800DE0B0;
extern short D_800E2B44;
extern short D_800E61DC;
extern short D_800E2B30;
extern void func_800201EC(void), func_8002043C(void), func_800207CC(void), func_800208C8(void), func_80020A88(void), func_80020EA4(void), func_80021048(void);
void func_80020000_pcpx96084(void)
{
    short a;
    switch (D_800DE0B0[0x4c / 2]) {
    case 0: func_800201EC(); break;
    case 1: func_8002043C(); break;
    case 2: func_800207CC(); break;
    case 3: func_800208C8(); break;
    case 4: func_80020A88(); break;
    case 5: func_80020EA4(); break;
    case 6: func_80021048(); break;
    }
    a = D_800E2B44;
    if (-1 < a && D_800E2B30 != 0) {
        D_800E2B44 = -1;
        D_800DE0B0[0x4c / 2] = a;
        D_800DE0B0[0x4e / 2] = D_800E61DC;
    }
}
#endif
