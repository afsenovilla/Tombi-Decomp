// VERSION PCPX_96084 tomba 80081DB0 192
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Variant of func_80081CF8_pcpx96084: the vertical test uses b's box (+0x70/+0x72).
#define U16(o, k) (*(unsigned short *)((char *)(o) + (k)))
#define S16(o, k) (*(short *)((char *)(o) + (k)))
#define PTR(o, k) (*(char **)((char *)(o) + (k)))
extern short D_800DE744;

int func_80081DB0_pcpx96084(char *a, char *b)
{
    char pad[0];
    if ((unsigned short)(U16(PTR(a, 0x44), 2) - U16(PTR(b, 0x44), 2) + 45) >= 91)
        return 0;
    if ((unsigned short)(U16(b, 0x6c) + (U16(a, 0xbc) - U16(PTR(b, 0x40), 2))) > S16(b, 0x6e))
        return 0;
    if ((unsigned short)(U16(b, 0x70) + (U16(a, 0xbe) - U16(b, 0x16))) > S16(b, 0x72))
        return 0;
    if (U16(a, 0x2e) & 1)
        D_800DE744 = S16(b, 0x6e) - U16(b, 0x6c);
    else
        D_800DE744 = -U16(b, 0x6c);
    return 1;
}
#endif
