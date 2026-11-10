// VERSION SLPM_80165 slpm80165 8005D730 80
#if defined(VERSION_SLPM_80165)
// Demo version: PCPX_96084 C of src/func_80093E34_pcpx96084.c (func_80093E34_pcpx96084, twin score 1.000) [-O2 -G0] (tools/demo_sweep.py)
extern int D_800E07AC;
extern int D_800CCB94;
extern void ResetCallback(void);
extern void func_80093EFC_pcpx96084(int, int *);
extern void ChangeClearPAD(int);

void func_80093E34_pcpx96084(int a)
{
    D_800E07AC = a;
    D_800CCB94 = -1;
    ResetCallback();
    func_80093EFC_pcpx96084(0x20000001, &D_800CCB94);
    ChangeClearPAD(0);
}
#endif
