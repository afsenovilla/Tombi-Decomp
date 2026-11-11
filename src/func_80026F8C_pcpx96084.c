// VERSION PCPX_96084 tomba 80026F8C 60
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 relative of PAL stepScrollClearX (src/stepScrollClearX.c): steps the word +0x20 toward 0, 1 while it moved.
typedef struct S { char pad[0x20]; int v; char pad2[0x6e - 0x24]; char a; char b; } S;

int func_80026F8C_pcpx96084(S *s)
{
    int v = s->v;
    if (v != 0) {
        if (v > 0) {
            v -= 0x80;
        } else {
            v += 0x80;
        }
        s->v = v;
        return 1;
    }
    return 0;
}
#endif
