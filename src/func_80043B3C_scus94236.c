// VERSION SCUS_94236 scus94236 80043B3C 496
#if defined(VERSION_SCUS_94236)
// USA version: JP C of src/func_800412B0_slps01144.c (func_800412B0_slps01144) (1.000 masked) [-O2 -G0] (tools/demo_sweep.py)
typedef struct S { char p[0x14]; short w14; unsigned short w16; } S;
extern unsigned short *D_1F800278;
extern short D_1F80027E, D_1F800280;
extern unsigned short D_1F800282;
#define COLLISION_SEG_PTR D_1F800278
#define COLLISION_HIT_HEIGHT D_1F80027E
int func_80043B3C_scus94236(S *o, int x, unsigned short y)
{
    char pad;
    short n;
    unsigned short fl, w;
    int a;
    short b;
    int lo, len, h;
    short d;
    int x7;
    short xs;
    unsigned short nv;
    n = *COLLISION_SEG_PTR++;
    xs = x;
    if (n == 0)
        return 0;
    __asm__("" : "=r"(xs) : "0"(xs));
    do {
        fl = *COLLISION_SEG_PTR++;
        n--;
        if (!(fl & 2)) goto skip;
        if (fl & 0x10) {
        skip:
            COLLISION_SEG_PTR += 3;
            continue;
        }
        a = *COLLISION_SEG_PTR++;
        nv = *COLLISION_SEG_PTR++;
        b = nv;
        w = *COLLISION_SEG_PTR++;
        lo = w & 0xf;
        len = (w >> 4) & 0xf;
        if (len == 0) continue;
        if ((short)y < (short)a - 0x10) continue;
        if ((xs & 7) < lo) continue;
        if (lo + len < (xs & 7)) continue;
        if (b == 0)
            h = a;
        else
            h = a + b * (((short)x - lo) % len) / len;
        d = h - y;
        if ((short)d > 0) {
            o->w14 = 0;
            COLLISION_HIT_HEIGHT = b;
            D_1F800280 = a;
            D_1F800282 = fl;
            o->w16 += d;
            return 1;
            do { } while (0);
        }
    } while (n != 0);
    return 0;
}
#endif
