// near-twin of PAL rewardAnimalDash (tools/jp_autofix.py)
// VERSION SLPS_01144 slps01144 80064D64 128
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
extern int D_8009DDB8, D_8009DDBC, D_8009DDC0, D_8009DDC4;
extern void func_800584CC(void);
extern void ExitCriticalSection(void);
extern void func_80064E1C_slps01144(void);
extern void func_80064E84_slps01144(void);
extern void func_80064F14(int, int *);
extern void func_80064F04(int, int *);

int func_80064D64(void)
{
    int *q = &D_8009DDBC;
    int *p;
    func_800584CC();
    p = q - 1;
    *q = (int)func_80064E1C_slps01144;
    D_8009DDC0 = (int)func_80064E84_slps01144;
    D_8009DDB8 = 0;
    D_8009DDC4 = 0;
    func_80064F14(1, p);
    func_80064F04(1, p);
    ExitCriticalSection();
    return 1;
}
#endif
