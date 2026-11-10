// VERSION PCPX_96084 tomba 8001C6E8 196
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Collision segment test: reads three halfwords (top, height, packed x span) from COLLISION_SEG_PTR; 1 when y is
// inside the segment and x mod 8 inside the span.
extern unsigned short *COLLISION_SEG_PTR;
int func_8001C6E8_pcpx96084(int x, short y)
{
    unsigned short a, b, c;
    int lo, w, e;
    int m;
    a = *COLLISION_SEG_PTR++;
    b = *COLLISION_SEG_PTR++;
    c = *COLLISION_SEG_PTR++;
    lo = c & 0xf;
    w = (c >> 4) & 0xf;
    e = b + a;
    if ((short)a > y && y > (short)e) {
        m = (short)((short)x % 8);
        { int l = lo; asm("" : "=r"(l) : "0"(l)); if (l < m && m < l + w) return 1; }
    }
    return 0;
}

#endif
