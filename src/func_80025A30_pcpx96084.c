// VERSION PCPX_96084 tomba 80025A30 80
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
extern unsigned short D_800A2538[][3];
extern unsigned char D_800E6144[3];

void func_80025A30_pcpx96084(int i)
{
    D_800E6144[0] = D_800A2538[i][0];
    D_800E6144[1] = D_800A2538[i][1];
    D_800E6144[2] = D_800A2538[i][2];
}
#endif
