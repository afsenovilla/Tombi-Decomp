// VERSION PCPX_96084 tomba 8001ECD4 264
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Front-end task loop: pad read, soft reset on the 0x900 combination, the mode handler (0..4), the text window
// draw, one frame wait.
extern short *D_800DE0B0;
extern unsigned short D_800DE760;
extern int D_800E246C;
extern char *D_800E2B20;
extern char D_800E5154[];
extern void func_80017B8C_pcpx96084(void), exitTask(void);
extern void func_8001EDDC(void), func_8001F2C0_pcpx96084(void), func_8001F3AC(void), func_8001F9D8_pcpx96084(void), func_8001FACC_pcpx96084(void);
extern void func_8001E4C8_pcpx96084(char *, char *);
extern void ThreadWaitFrames(int);

void func_8001ECD4_pcpx96084(void)
{
    D_800DE0B0[0x4a / 2] = 0;
    for (;;) {
        func_80017B8C_pcpx96084();
        if ((D_800DE760 & 0x900) == 0x900) {
            D_800E246C = 1;
            exitTask();
        }
        switch ((unsigned short)D_800DE0B0[0x48 / 2]) {
        case 0:
            func_8001EDDC();
            break;
        case 1:
            func_8001F2C0_pcpx96084();
            break;
        case 2:
            func_8001F3AC();
            break;
        case 3:
            func_8001F9D8_pcpx96084();
            break;
        case 4:
            func_8001FACC_pcpx96084();
            break;
        }
        func_8001E4C8_pcpx96084(D_800E2B20, D_800E5154);
        ThreadWaitFrames(1);
    }
}
#endif
