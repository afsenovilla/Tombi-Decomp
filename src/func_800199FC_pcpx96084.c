// VERSION PCPX_96084 tomba 800199FC 52
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
extern char D_800DE0CC[];
extern void *memset(void *, int, int);

void func_800199FC_pcpx96084(void)
{
    memset(D_800DE0CC, 0, 12);
    *(short *)0x1F800126 = 0;
}
#endif
