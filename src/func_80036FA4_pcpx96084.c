// VERSION PCPX_96084 tomba 80036FA4 164
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
extern void func_800183FC(unsigned char *o, int n);
extern void func_8001A8C8_pcpx96084(void);

void func_80036FA4_pcpx96084(unsigned char *o)
{
    unsigned char s = o[4];
    switch (s) {
    case 0:
        *(short *)(o + 0x6C) = 0;
        *(short *)(o + 0x6E) = 0;
        *(short *)(o + 0x70) = 0;
        *(short *)(o + 0x72) = 0;
        o[4]++;
        break;
    case 1:
        func_800183FC(o, 0x168);
        break;
    case 2:
        o[4] = s + 1;
        break;
    case 3:
        func_8001A8C8_pcpx96084();
        break;
    }
}
#endif
