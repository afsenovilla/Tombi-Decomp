// VERSION PCPX_96084 tomba 8003CEC4 156
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Area-0 scene step, the shape of func_8003CF60_pcpx96084.c: waits for the event word 0x300, then
// raises +0x88 by 32 up to 0x600 and sets the event state bytes (5, 2, 0, 0).
extern unsigned short D_800F4FDC;
extern unsigned char D_800F4FDD, D_800F4FDE, D_800F4FDF;

void func_8003CEC4_pcpx96084(unsigned char *o)
{
    int v;
    switch (o[6]) {
    case 0:
        if (D_800F4FDC == 0x300) o[6]++;
        break;
    case 1:
        v = *(int *)(o + 0x88) + 32;
        *(int *)(o + 0x88) = v;
        if (v > 0x600) {
            *(int *)(o + 0x88) = 0x600;
            o[6]++;
            *(unsigned char *)&D_800F4FDC = 5;
            D_800F4FDD = 2;
            D_800F4FDE = 0;
            D_800F4FDF = 0;
        }
        break;
    case 2:
        break;
    }
}
#endif
