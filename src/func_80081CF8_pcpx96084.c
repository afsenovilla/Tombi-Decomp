// VERSION PCPX_96084 tomba 80081CF8 184
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// A box test in the style of boxesOverlap (src/boxesOverlap.c) of the point (+0xBC, +0xBE) of a against b; on a hit
// D_800DE744 gets the offset to b's edge on a's facing side.
#define U16(o, k) (*(unsigned short *)((char *)(o) + (k)))
#define S16(o, k) (*(short *)((char *)(o) + (k)))
#define PTR(o, k) (*(char **)((char *)(o) + (k)))
extern short D_800DE744;

int func_80081CF8_pcpx96084(char *a, char *b)
{
    char pad[0];
    if ((unsigned short)(U16(PTR(a, 0x44), 2) - U16(PTR(b, 0x44), 2) + 45) >= 91)
        return 0;
    if ((unsigned short)(U16(b, 0x6c) + (U16(a, 0xbc) - U16(PTR(b, 0x40), 2))) > S16(b, 0x6e))
        return 0;
    if ((unsigned short)(U16(a, 0xbe) - U16(b, 0x16) + 8) >= 17)
        return 0;
    if (U16(a, 0x2e) & 1)
        D_800DE744 = S16(b, 0x6e) - U16(b, 0x6c);
    else
        D_800DE744 = -U16(b, 0x6c);
    return 1;
}
#endif
