// VERSION PCPX_96084 tomba 800200FC 68
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
extern char *D_800DE0B0;
extern int D_800E43D4;
extern void func_80019720_pcpx96084(void);
extern void func_800199FC_pcpx96084(void);

void func_800200FC_pcpx96084(void)
{
    (*(unsigned short *)(D_800DE0B0 + 78))++;
    func_80019720_pcpx96084();
    func_800199FC_pcpx96084();
    D_800E43D4 = 0;
}
#endif
