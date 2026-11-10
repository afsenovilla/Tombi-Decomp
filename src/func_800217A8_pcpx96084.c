// VERSION PCPX_96084 tomba 800217A8 72
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
extern unsigned char D_800DE21C[];
extern void func_800217F0(void);

int func_800217A8_pcpx96084(int i)
{
    D_800DE21C[i] = 4;
    func_800217F0();
    return D_800DE21C[i];
}
#endif
