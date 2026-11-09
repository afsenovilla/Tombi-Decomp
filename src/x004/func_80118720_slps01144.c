// VERSION SLPS_01144 x04 80118720 624
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"

typedef struct {
    char p0[4]; unsigned short uv0; char p1[2]; unsigned short uv1; char p2[2];
    unsigned short uv2; char p3[2]; unsigned short uv3; char p4[8]; short flag; char p5[24];
} FT4;

void func_80118720(TObj *o_)
{
    register TObj *o asm("$9") = o_;
    short n;
    register FT4 *f asm("$3");
    FT4 *g;
    register short *s asm("$5");
    register unsigned short *q asm("$7");
    unsigned short *p;
    register unsigned short st asm("$5");
    unsigned short a2, b2, c2, d2;
    register int t asm("$2");

    p = (unsigned short *)o->da0;
    p += 2;
    f = (FT4 *)(p + 2); asm("":"=r"(f):"0"(f));
    n = *p;
    g = f;
    st = o->state;
    switch ((unsigned char)st) {
    case 0:
        /* st + 1 through a $2-pinned temp: a hard-register dest is no "birthing" insn, so sched1 does not pull it
           down next to the `s` setup (adjust_priority); the loop constants are explicit locals set before `s`, so
           `s = &f->flag` has the higher luid and is placed after them (as the hoisted li's of loop.c are not) */
        { register unsigned char u asm("$2"); u = st + 1; o->state = u; }
        o->timer = 10;
        o->w22 = 0;
        { int k0 = 0xd0, k1 = 0xff; short one = 1;
        s = &f->flag;
        do {
            if (*((unsigned char *)f + 4) == k0 || *((unsigned char *)f + 4) == k1)
                *s = 0;
            else
                *s = one;
            s += 26;
            f++;
        } while (--n);
        }
        break;
    case 1:
        if (--o->timer == 0) {
            o->timer = 10;
            o->w22++;
            q = &f->uv3;
            do {
                switch (g->flag) {
                case 0: {
                    unsigned short a, b, c, d, aS, bS, cS, dS;
                    a = q[-6]; b = q[-4]; c = q[-2]; d = q[0];
                    t = a & 0xff; q[-6] = t;
                    t = b & 0xff; q[-4] = t;
                    t = c & 0xff; q[-2] = t;
                    t = d & 0xff; q[0] = t;
aS = a >> 8; bS = b >> 8; cS = c >> 8; dS = d >> 8;
if (o->w22 & 1) { aS += 0x60; bS += 0x60; cS += 0x60; dS += 0x60; }
else { aS -= 0x60; bS -= 0x60; cS -= 0x60; dS -= 0x60; }
q[-6] |= aS << 8;
q[-4] |= bS << 8;
q[-2] |= cS << 8;
q[0] |= dS << 8;
                } break;
                case 1: {
                    unsigned short a, b, c, d;
                    a = q[-6]; b = q[-4]; c = q[-2]; d = q[0];
                    t = a & 0xff00; q[-6] = t;
                    t = b & 0xff00; q[-4] = t;
                    t = c & 0xff00; q[-2] = t;
                    t = d & 0xff00; q[0] = t;
                    a &= 0xff; b &= 0xff; c &= 0xff; d &= 0xff;
                    if (o->w22 & 1) { a2 = a + 0x30; b2 = b + 0x30; c2 = c + 0x30; d2 = d + 0x30; }
                    else { a2 = a - 0x30; b2 = b - 0x30; c2 = c - 0x30; d2 = d - 0x30; }
                    q[-6] |= a2;
                    q[-4] |= b2;
                    q[-2] |= c2;
                    q[0] |= d2;
                } break;
                }
                g++;
                q += 26;
            } while (--n);
        }
        break;
    }
}
#endif
