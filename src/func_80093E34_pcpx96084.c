// VERSION PCPX_96084 tomba 80093E34 80
// FLAGS -O2 -G0
#if defined(VERSION_PCPX_96084)
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
