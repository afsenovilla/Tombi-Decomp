// VERSION PCPX_96084 tomba 80035694 144
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Sprite setup, sibling of func_80035724_pcpx96084: depth 0xF always, scratchpad word 0x1F80016C.
extern short D_800A2B8C[];
extern unsigned short D_800A2B54[];
extern int D_1F80016C;

void func_80035694_pcpx96084(unsigned char *o)
{
    char pad[0];
    *(short *)(o + 0x1E) = 0xF;
    o[0xD] = 0;
    if (D_800A2B8C[o[3]] != 0) {
        o[0xD] = 1;
        *(short *)(o + 8) = ((D_800A2B8C[o[3]] + o[0xC]) << 6) | ((D_800A2B54[o[3]] >> 4) & 0x3F);
    }
    *(int *)(o + 0x3C) = D_1F80016C;
}
#endif
