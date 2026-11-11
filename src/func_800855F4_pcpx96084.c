// VERSION PCPX_96084 tomba 800855F4 92
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
extern char D_800E5154[];
extern char D_800AF1B8[];
extern int D_800CA588;
extern char *D_800DE0B0;
extern void func_8001E084(void *);
extern void func_8001EA78_pcpx96084(void *, void *);

void func_800855F4_pcpx96084(void)
{
    func_8001E084(D_800E5154);
    func_8001EA78_pcpx96084(D_800E5154, D_800AF1B8);
    D_800CA588 = 0;
    *(short *)(D_800DE0B0 + 72) = 1;
    *(short *)(D_800DE0B0 + 74) = 0;
}
#endif
