// VERSION PCPX_96084 tomba 80070E54 156
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
extern void func_8003357C(short x, int k, short y, int z);

void func_80070E54_pcpx96084(unsigned char *o)
{
    if (*(short *)(*(char **)(o + 0x40) + 2) > 2370) {
        o[0] = 2;
        o[4] = 3;
        func_8003357C(*(unsigned short *)(*(char **)(o + 0x40) + 2) + 8, 0x2E, *(short *)(*(char **)(o + 0x44) + 2), 0);
        func_8003357C(*(unsigned short *)(*(char **)(o + 0x40) + 2) - 8, 0x2E, *(short *)(*(char **)(o + 0x44) + 2), 0);
    }
}
#endif
