// VERSION SLPM_80165 slpm80165 8003D6B8 344
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of testCollisionSegmentSlopeA (src/testCollisionSegmentSlopeA.c): short first argument, signed
// modulo 8 (PAL: & 7).
extern unsigned short *COLLISION_SEG_PTR;

unsigned testCollisionSegmentSlopeA(short u, int a)
{
    unsigned short x;
    unsigned short w;
    unsigned short m;
    int lo;
    int hi;
    short sw;
    unsigned short t;
    unsigned short v;
    short cu;
    short cl;
    unsigned char ch;
    x = *COLLISION_SEG_PTR++;
    w = *COLLISION_SEG_PTR++;
    m = *COLLISION_SEG_PTR++;
    lo = m & 0xf;
    hi = (m >> 4) & 0xf;
    sw = w;
    if (sw == 0) return 0;
    if ((short)a > (short)x + 0x10) return 0;
    if ((int)(unsigned short)(a - x - w - 1) > -sw) return 0;
    u = u % 8;
    v = u;
    cu = u;
    cl = lo;
    t = 0;
    if (cu < cl) return 0;
    ch = hi;
    if (cl + ch < cu) {
        t = u - (lo + hi);
        v = lo + hi;
    }
    return (short)(t + (v - lo - ch * ((short)a - (short)x) / sw)) >= 0;
}
#endif
