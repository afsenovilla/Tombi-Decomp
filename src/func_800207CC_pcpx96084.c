// VERSION PCPX_96084 tomba 800207CC 220
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Area mode 2 (gameplay) steps: room setup and player placement, then the per-frame update; step 2 goes to mode 6.
extern short *D_800DE0B0;
extern char D_800E5154[];
extern int D_800E43D4;
extern void func_80019720_pcpx96084(void), func_800199FC_pcpx96084(void), func_8001E084(char *);
extern void func_80018EC8(void), func_80018F9C(void), func_8008299C_pcpx96084(void), playerPushOutOfObjectBoxThunk(void);

void func_800207CC(void)
{
    short *t = D_800DE0B0;
    switch ((unsigned short)t[0x4e / 2]) {
    case 0:
        func_80019720_pcpx96084();
        func_800199FC_pcpx96084();
        func_8001E084(D_800E5154);
        D_800E43D4 = 1;
        func_80018EC8();
        func_80018F9C();
        func_8008299C_pcpx96084();
        D_800DE0B0[0x4e / 2]++;
        break;
    case 1:
        playerPushOutOfObjectBoxThunk();
        break;
    case 2:
        t[0x4c / 2] = 6;
        t[0x4e / 2] = 0;
    }
}
#endif
