// VERSION PCPX_96084 tomba 8003CF60 116
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
extern unsigned short D_800F4FDC;

void func_8003CF60_pcpx96084(unsigned char *o)
{
    int v;
    switch (o[6]) {
    case 0:
        if (D_800F4FDC == 0x300) o[6]++;
        break;
    case 1:
        v = *(int *)(o + 0x88) - 32;
        *(int *)(o + 0x88) = v;
        if (v < 0xa00) {
            *(int *)(o + 0x88) = 0xa00;
            o[6]++;
        }
        break;
    case 2:
        break;
    }
}
#endif
