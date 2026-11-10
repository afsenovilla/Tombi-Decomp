// VERSION PCPX_96084 tomba 80018290 100
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
void func_80018290_pcpx96084(char *o)
{
    unsigned short v = *(unsigned short *)(*(char **)(o + 0x24) + 4);
    short dx = v & 0xFF;
    short dy = v >> 8;
    short d = dx;
    if (*(unsigned short *)(o + 0x2E) & 1) d = -dx;
    *(unsigned short *)(*(char **)(o + 0x40) + 2) += d;
    *(unsigned short *)(o + 0x16) += dy;
    *(unsigned short *)(o + 0x2C) = *(unsigned short *)(*(char **)(o + 0x24) + 6) & 0x3FFF;
}
#endif
