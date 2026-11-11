// VERSION SLPM_80165 slpm80165 8005D970 128
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo-only library setup: installs a priority-1 interrupt entry (two handlers) with SysDeqIntRP / SysEnqIntRP
// inside a critical section, like _padStartCom does for priority 2.
typedef struct { int next; void (*f1)(void); void (*f2)(void); int pad; } IntRP;
extern IntRP D_800892A0;
extern int D_800892A0_next asm("D_800892A0"); /* the link word written as its own global */
extern void func_8005DA28(void);
extern void func_8005DA90(void);
extern int EnterCriticalSection(void);
extern int ExitCriticalSection(void);
extern int SysDeqIntRP(int, IntRP *);
extern int SysEnqIntRP(int, IntRP *);

int func_8005D970(void)
{
    EnterCriticalSection();
    D_800892A0.f1 = func_8005DA28;
    D_800892A0.f2 = func_8005DA90;
    D_800892A0_next = 0;
    D_800892A0.pad = 0;
    SysDeqIntRP(1, &D_800892A0);
    SysEnqIntRP(1, &D_800892A0);
    ExitCriticalSection();
    return 1;
}
#endif
