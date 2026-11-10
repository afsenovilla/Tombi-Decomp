// VERSION PCPX_96084 tomba 80035724 172
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
extern short D_800A2B8C[];
extern unsigned short D_800A2B54[];
extern int D_1F80015C;

void func_80035724_pcpx96084(unsigned char *o)
{
    char pad[0];
    if (o[3] >= 21 && o[3] <= 26) *(short *)(o + 0x1E) = 0xF;
    else *(short *)(o + 0x1E) = 9;
    o[0xD] = 0;
    if (D_800A2B8C[o[3]] != 0) {
        o[0xD] = 1;
        *(short *)(o + 8) = ((D_800A2B8C[o[3]] + o[0xC]) << 6) | ((D_800A2B54[o[3]] >> 4) & 0x3F);
    }
    *(int *)(o + 0x3C) = D_1F80015C;
}
#endif
