// VERSION PCPX_96084 tomba 8003F4A4 128
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
extern unsigned char D_800F4FE7;
extern char *D_800F501C;

void func_8003F4A4_pcpx96084(unsigned char *o)
{
    if (o[3] == 0) {
        *(short *)(o + 0x6C) = 0x58;
        *(short *)(o + 0x6E) = 0x60;
        *(short *)(o + 0x70) = 4;
        *(short *)(o + 0x72) = 8;
        *(short *)(o + 0x0A) = 0x11;
        o[0xF] = D_800F4FE7 + 1;
        *(int *)(o + 0x84) = 0;
        *(int *)(o + 0x88) = 0;
        *(int *)(o + 0x8C) = 0;
        *(short *)(*(char **)(o + 0x44) + 2) = *(short *)(D_800F501C + 2);
        o[4]++;
    }
}
#endif
