// VERSION PCPX_96084 tomba 800411A8 172
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
extern void func_8001AC5C_pcpx96084(unsigned char *o);
extern void func_8001A9F8_pcpx96084(unsigned char *o);

void func_800411A8_pcpx96084(unsigned char *o)
{
    unsigned char s = o[4];
    switch (s) {
    case 0:
        *(short *)(o + 0x1E) = 12;
        o[0xD] = 1;
        o[0xF] = 0;
        o[0xE] = 1;
        o[4]++;
        break;
    case 1:
        o[1] = 1;
        func_8001AC5C_pcpx96084(o);
        break;
    case 2:
        o[4] = s + 1;
        break;
    case 3:
        func_8001A9F8_pcpx96084(o);
        break;
    }
}
#endif
