// VERSION PCPX_96084 tomba 80031A34 280
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 form of PAL updateYellowChick (src/updateYellowChick.c): CLUT word computed inline (x 0x280), halfword at +0x0A, scratchpad word 0x1F80015C.
typedef struct P { char p[0x24]; void *anim; char q[0x3c - 0x28]; int w3c; } P;
extern short D_800A2ADC[];
extern void *D_80013FC8[];
extern int D_1F80015C;
extern int func_80018550(char *);
extern void func_800180D8_pcpx96084(char *);
extern void AnimLoadDuration(char *);
extern void func_8001A87C_pcpx96084(char *);

void func_80031A34_pcpx96084(char *o)
{
    unsigned char s = o[4];

    switch (s) {
    case 0:
        o[4] = s + 1;
        *(short *)(o + 0x1e) = 9;
        *(short *)(o + 8) = (D_800A2ADC[(unsigned char)o[0xc]] << 6) | (0x280 >> 4);
        o[0xd] = 1;
        *(signed char *)&o[0xf] = -0x14;
        *(short *)(o + 0xa) = 0;
        *(short *)(o + 0x2e) = 0;
        ((P *)o)->w3c = D_1F80015C;
        ((P *)o)->anim = D_80013FC8[(unsigned char)o[3]];
        AnimLoadDuration(o);
        break;
    case 1:
        if (func_80018550(o) != 0)
            func_800180D8_pcpx96084(o);
        break;
    case 2:
        o[4] = s + 1;
        break;
    case 3:
        func_8001A87C_pcpx96084(o);
        break;
    }
}
#endif
