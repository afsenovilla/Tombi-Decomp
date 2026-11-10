// VERSION PCPX_96084 tomba 80059B44 168
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Spawns a type 1 object (subtype 1, kind c | 0x80) at the position record q, with two halfword parameters.
extern unsigned char *func_8001A5D8_pcpx96084(void);

void func_80059B44_pcpx96084(short c, short *q, short a, short b)
{
    unsigned char *o = func_8001A5D8_pcpx96084();
    if (o != 0) {
        o[0] = 1;
        o[3] = 1;
        o[2] = 0;
        o[0xC] = c | 0x80;
        o[0xF] = 0;
        *(short *)(o + 0x2E) = 0;
        *(int *)(o + 0x10) = q[1] << 16;
        *(int *)(o + 0x14) = q[3] << 16;
        *(int *)(o + 0x18) = q[5] << 16;
        *(short *)(o + 0x80) = a;
        *(short *)(o + 0x82) = b;
    }
}
#endif
