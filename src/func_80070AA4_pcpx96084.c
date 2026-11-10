// VERSION PCPX_96084 tomba 80070AA4 136
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
extern short D_1F800382;
extern short D_1F800386;

void func_80070AA4_pcpx96084(char *o)
{
    char pad[0];
    short a = D_1F800382;
    if (a < 0) a = -a;
    if (a > 8) a = 8;
    if (D_1F800382 < 0) a = -a;
    *(int *)(o + 0x8C) = -a & 0xFF;
    *(short *)(o + 0xAE) = a;
    *(short *)(o + 0xAA) = D_1F800386;
    *(short *)(o + 0xB2) = (-a << 2) & 0xFF;
}
#endif
