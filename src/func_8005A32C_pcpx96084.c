// VERSION PCPX_96084 tomba 8005A32C 180
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Distance to the current object along the scroll axis (0x1F800128); when non-zero and the flag is clear, +0x64
// = 7 * distance + 0x1000 and the flags come back with bit 0 set.
extern short D_1F800128;
extern char *D_800DE544;

unsigned short func_8005A32C_pcpx96084(char *o)
{
    short d;
    if (*(unsigned short *)(o + 0xA) < 4) {
        switch (D_1F800128) {
        case 0:
            d = *(unsigned short *)(D_800DE544 + 0x56) - *(unsigned short *)(o + 0x1A);
            break;
        case 1:
            d = *(unsigned short *)(o + 0x12) - *(unsigned short *)(D_800DE544 + 0x4E);
            break;
        }
        if (d != 0 && !(*(unsigned short *)(o + 0xA) & 1)) {
            *(int *)(o + 0x64) = d * 7 + 0x1000;
            return *(unsigned short *)(o + 0xA) | 1;
        }
    }
    return *(unsigned short *)(o + 0xA);
}
#endif
