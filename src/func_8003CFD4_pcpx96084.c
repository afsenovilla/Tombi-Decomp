// VERSION PCPX_96084 tomba 8003CFD4 156
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Area-0 scene step, the shape of func_8003CF60_pcpx96084.c: waits for the player at X 2851..2999, then
// raises +0x88 by 8 up to 0x600.
extern short D_800F4FEE;
extern char *D_800F5018;

void func_8003CFD4_pcpx96084(unsigned char *o)
{
    int v;
    switch (o[6]) {
    case 0:
        if (D_800F4FEE > -320 && *(unsigned short *)(D_800F5018 + 2) >= 2851 && *(unsigned short *)(D_800F5018 + 2) < 3000) o[6]++;
        break;
    case 1:
        v = *(int *)(o + 0x88) + 8;
        *(int *)(o + 0x88) = v;
        if (v > 0x600) {
            *(int *)(o + 0x88) = 0x600;
            o[6]++;
        }
        break;
    case 2:
        break;
    }
}
#endif
