// VERSION PCPX_96084 tomba 80094074 128
// FLAGS -O2 -G0
#if defined(VERSION_PCPX_96084)
// Installs the interrupt handler record at D_800CCB98 (handler func_8009412C_pcpx96084, verifier
// func_80094194_pcpx96084) at priority 1.
extern void *D_800CCB9C[];
extern void *D_800CCBA0;
extern int D_800CCB98, D_800CCBA4;
extern void EnterCriticalSection(void);
extern void ExitCriticalSection(void);
extern int SysDeqIntRP(int, void *);
extern int SysEnqIntRP(int, void *);
extern void func_8009412C_pcpx96084(void);
extern void func_80094194_pcpx96084(void);

int func_80094074_pcpx96084(void)
{
    void **p;
    EnterCriticalSection();
    p = D_800CCB9C;
    p[0] = func_8009412C_pcpx96084;
    D_800CCBA0 = func_80094194_pcpx96084;
    D_800CCB98 = 0;
    D_800CCBA4 = 0;
    SysDeqIntRP(1, p - 1);
    SysEnqIntRP(1, p - 1);
    ExitCriticalSection();
    return 1;
}
#endif
