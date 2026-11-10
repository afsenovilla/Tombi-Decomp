// VERSION PCPX_96084 tomba 8001FBC0 284
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Game init task loop: resets the task modes and the draw environment flags, then each frame: pad read, soft reset
// on the 0x900 combination, mode 0 / 1 handler, text window draw, one frame wait.
extern short *D_800DE0B0;
extern volatile unsigned char D_800E2B28;
extern volatile unsigned char D_800E14A5, D_800E14A6, D_800E14A7, D_800E21B5, D_800E21B6, D_800E21B7;
extern unsigned short D_800DE760;
extern int D_800E246C;
extern short D_1F8003F0;
extern char *D_800E2B20;
extern char D_800E5154[];
extern void func_80017B8C_pcpx96084(void), exitTask(void);
extern void func_8001FCDC_pcpx96084(void), func_8001FDA4_pcpx96084(void);
extern void func_8001E4C8_pcpx96084(char *, char *);
extern void ThreadWaitFrames(int);

// A goto loop (no loop.c hoisting: retail reloads 0x900 every frame) and the constant 1 in one variable shared by
// the reset stores and the mode compare (retail keeps it in s0); volatile stores keep the retail store order.
void func_8001FBC0_pcpx96084(void)
{
    int one = 1;
    volatile short *p;
    D_800E2B28 = 1;
    p = D_800DE0B0;
    p[0x48 / 2] = 0;
    p[0x4a / 2] = 0;
    p[0x4c / 2] = 0;
    p[0x4e / 2] = 0;
    D_800E14A5 = 0;
    D_800E14A6 = 0;
    D_800E14A7 = 0;
    D_800E21B5 = 0;
    D_800E21B6 = 0;
    D_800E21B7 = 0;
loop:
    func_80017B8C_pcpx96084();
    if ((D_800DE760 & 0x900) == 0x900) {
        D_800E246C = one;
        D_1F8003F0 = one;
        exitTask();
    }
    {
        unsigned short m = D_800DE0B0[0x48 / 2];
        if (m == 0) goto mode0;
        if (m == one) goto mode1;
        goto draw;
    mode0:
        func_8001FCDC_pcpx96084();
        goto draw;
    mode1:
        func_8001FDA4_pcpx96084();
    }
draw:
    func_8001E4C8_pcpx96084(D_800E2B20, D_800E5154);
    ThreadWaitFrames(1);
    goto loop;
}
#endif
