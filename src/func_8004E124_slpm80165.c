// VERSION SLPM_80165 slpm80165 8004E124 184
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of func_80081CF8_pcpx96084 (src/func_80081CF8_pcpx96084.c): the point is at +0xD4/+0xD6 (PrePre 8: +0xBC/+0xBE), the
// edge offset goes to the scratchpad (0x1F8003BC).
// A box test in the style of boxesOverlap (src/boxesOverlap.c) of the point (+0xBC, +0xBE) of a against b; on a hit
// D_1F8003BC gets the offset to b's edge on a's facing side.
#define U16(o, k) (*(unsigned short *)((char *)(o) + (k)))
#define S16(o, k) (*(short *)((char *)(o) + (k)))
#define PTR(o, k) (*(char **)((char *)(o) + (k)))
extern short D_1F8003BC;

int func_8004E124(char *a, char *b)
{
    char pad[0];
    if ((unsigned short)(U16(PTR(a, 0x44), 2) - U16(PTR(b, 0x44), 2) + 45) >= 91)
        return 0;
    if ((unsigned short)(U16(b, 0x6c) + (U16(a, 0xd4) - U16(PTR(b, 0x40), 2))) > S16(b, 0x6e))
        return 0;
    if ((unsigned short)(U16(a, 0xd6) - U16(b, 0x16) + 8) >= 17)
        return 0;
    if (U16(a, 0x2e) & 1)
        D_1F8003BC = S16(b, 0x6e) - U16(b, 0x6c);
    else
        D_1F8003BC = -U16(b, 0x6c);
    return 1;
}
#endif
