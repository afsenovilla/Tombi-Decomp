// VERSION PCPX_96084 tomba 8005A280 172
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// func_80059B44_pcpx96084.c with +2 = 2: spawns a type 1 object (subtype 1, kind c | 0x80) at the position record q, with two halfword parameters.
extern unsigned char *func_8001A5D8_pcpx96084(void);

void func_8005A280_pcpx96084(short c, short *q, short a, short b)
{
    unsigned char *o = func_8001A5D8_pcpx96084();
    if (o != 0) {
        o[0] = 1;
        o[2] = 2;
        o[3] = 1;
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
