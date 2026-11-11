// VERSION PCPX_96084 tomba 8001C420 336
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
extern unsigned short *COLLISION_SEG_PTR;
// Collision segment slope test (PAL testCollisionSegmentSlopeB shape): the x position mod 8 against the packed
// span of the segment, the y offset scaled by the slope height over the segment width.

int func_8001C420_pcpx96084(short u, int a)
{
    unsigned short x, w, m;
    short sw;
    int sx, sa;
    short lo, hi; short s;
    short k; short t2; short u2, lo2, hi2;
    x = *COLLISION_SEG_PTR++;
    w = *COLLISION_SEG_PTR++;
    m = *COLLISION_SEG_PTR++;
    lo = m & 0xf;
    hi = (m >> 4) & 0xf;
    sw = w;
    s = lo;
    if (sw == 0) goto ret0;
    sa = (short)a;
    sx = (short)x;
    if (sx + 0x10 < sa) return 0;
    if ((unsigned short)(a - x - w - 1) > -sw) goto ret0;
    u = (short)u % 8;
    k = u;
    u2 = u;
    lo2 = lo;
    hi2 = hi;
    t2 = 0;
    if (u2 > lo2 + hi2) {
ret0:
        return 0;
    }
    if (u2 < lo2) {
        t2 = lo - u;
        k = s;
    }
    return (short)(k - lo - hi2 * (sa - sx) / sw - t2) <= 0;
}

#endif
