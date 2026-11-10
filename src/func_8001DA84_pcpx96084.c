// VERSION PCPX_96084 tomba 8001DA84 312
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Byte-identical copy of func_8001D94C_pcpx96084 (PAL main0 8004245C probeFlaggedSegmentB with an early return).
extern short *func_8001B480(int a, int b);
extern unsigned short *COLLISION_SEG_PTR;

int func_8001DA84_pcpx96084(short a, int y, short b)
{
    short n;
    unsigned short w, x0, h;
    int d;
    short *q;
    if (a < 0) return 0;
    q = func_8001B480(a, b);
    COLLISION_SEG_PTR = (unsigned short *)(q + 1);
    n = *q;
    if (n == 0) return 0;
    for (;;) {
        w = *COLLISION_SEG_PTR++;
        n--;
        if (w & 0x4000) {
            if (w & 0x10) goto take;
        }
        COLLISION_SEG_PTR += 3;
        goto test;
take:
        x0 = *COLLISION_SEG_PTR++;
        h = *COLLISION_SEG_PTR++;
        COLLISION_SEG_PTR++;
        d = y - x0 - h;
        if ((unsigned short)(d - 1) > -(short)h) {
            if ((short)d > 0) return 0;
        } else {
            x0 += 8;
            if ((unsigned short)(y - x0 - h - 1) > -(short)h) return 2;
            return 1;
        }
test:
        if (n == 0) break;
    }
    return 0;
}
#endif
