// VERSION SLPS_01144 x10 80116458 360
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"

typedef struct { unsigned short a; short p0[1]; unsigned short b; short p1[1]; unsigned short c; short p2[1]; unsigned short d; } G;

void func_80116458_slps01144(TObj *o0)
{
    register TObj *o asm("$11") = o0;
    char *p;
    unsigned short *q;
    short n;
    register unsigned a asm("$5");
    register unsigned b asm("$6");
    register unsigned c asm("$3");
    register unsigned d asm("$4");
    register unsigned x asm("$8");
    register unsigned y asm("$9");
    unsigned t;
    register unsigned v asm("$2");
    register unsigned u asm("$3");

    switch (o->state) {
    case 0:
        o->state++;
        o->timer = 10;
        o->w22 = 0;
        break;
    case 1:
        if (--o->timer != 0) break;
        o->timer = 10;
        o->w22++;
        p = (char *)o->da0;
        n = *(int *)p;
        p += 4;
        while (n) {
            p += 0x28;
            n--;
        }
        n = *(short *)p;
        q = (unsigned short *)(p + 20);
        do {
            a = q[-6]; b = q[-4]; c = q[-2]; d = q[0];
            { t = a & 0xff; q[-6] = t; t = b & 0xff; q[-4] = t; t = c & 0xff; q[-2] = t; t = d & 0xff; q[0] = t; }
            a >>= 8; b >>= 8; c >>= 8; d >>= 8;
            if (o->w22 & 1) { a += 0x60; b += 0x60; x = c + 0x60; y = d + 0x60; }
            else { a -= 0x60; b -= 0x60; x = c - 0x60; y = d - 0x60; }
            n--;
            u = a << 8; v = q[-6]; v |= u; q[-6] = v; u = b << 8; v = q[-4]; v |= u; q[-4] = v; u = x << 8; v = q[-2]; v |= u; q[-2] = v; u = y << 8; v = q[0]; v |= u; q[0] = v;
            q += 26;
        } while (n);
        break;
    }
}
#endif
