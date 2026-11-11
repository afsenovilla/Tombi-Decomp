// VERSION PCPX_96084 tomba 80017B8C 136
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Pad edges: pressed (new & ~old) and released (old & ~new) bits of both pads, the previous state kept.
extern unsigned short D_800DE760, D_800DE762;
extern unsigned short D_800DE764, D_800DE766;
extern unsigned short D_800E2B18, D_800E2B1A;
extern unsigned short D_800DE5D0, D_800E2B14;
extern unsigned short D_1F8002B0, D_1F8002B2;

void func_80017B8C_pcpx96084(void)
{
    unsigned short old1 = D_800DE760;
    unsigned short old2 = D_800DE762;
    unsigned short new1 = D_800E2B18;
    unsigned short new2 = D_800E2B1A;
    D_1F8002B0 = new1 & ~old1;
    D_800DE760 = new1;
    D_800DE764 = old1;
    D_800DE762 = new2;
    D_800DE766 = old2;
    D_800DE5D0 = new2 & ~old2;
    D_1F8002B2 = old1 & ~new1;
    D_800E2B14 = old2 & ~new2;
}
#endif
