// VERSION PCPX_96084 tomba 800177D4 68
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
extern char *D_800DE0B0;
extern void setTaskEntry(void *, int);
extern void func_80017988_pcpx96084(int);

void func_800177D4_pcpx96084(int fn)
{
    setTaskEntry(D_800DE0B0 + 12, fn);
    func_80017988_pcpx96084(*(int *)(D_800DE0B0 + 12));
}
#endif
