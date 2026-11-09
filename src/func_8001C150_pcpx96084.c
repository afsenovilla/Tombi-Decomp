// VERSION PCPX_96084 tomba 8001C150 376
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 version of PAL main0 8004094C (src/testCollisionSegment.c): the low nibble is an unsigned char here, and
// the segment pointer is scratchpad 0x1F800388.
extern unsigned short *D_1F800388;
#define COLLISION_SEG_PTR D_1F800388

int func_8001C150_pcpx96084(int x, int y)
{
    int base;
    short amp;
    unsigned short v;
    int r; short n;
    unsigned char lo;

    base = *COLLISION_SEG_PTR++;
    if ((unsigned short)(y - base + 16) > 32) {
        COLLISION_SEG_PTR += 2;
        return 0;
    }
    amp = *COLLISION_SEG_PTR++;
    if (amp == 0) {
        COLLISION_SEG_PTR++;
        x = base;
    } else {
        v = *COLLISION_SEG_PTR++;
        lo = v & 0xf;
        n = (v >> 4) & 0xf;
        if (n == 0) return 0;
        x = (short)x;
        r = (short)(x % 8);
        if (r < lo) return 0;
        if (lo + n < r) return 0;
        x = base + amp * ((x - lo) % n) / n;
    }
    return (short)x - (short)y < 1;
}
#endif
