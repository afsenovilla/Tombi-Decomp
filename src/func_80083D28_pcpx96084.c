// VERSION PCPX_96084 tomba 80083D28 96
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
extern unsigned short D_800DE0DA;
extern unsigned char *D_1F8001D8[];
extern void func_800834D0(unsigned char *, int);

void func_80083D28_pcpx96084(void)
{
    unsigned char *p;
    for (p = D_1F8001D8[D_800DE0DA]; p[2] < 0xFF; p += 16) func_800834D0(p, -1);
}
#endif
