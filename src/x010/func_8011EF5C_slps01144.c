// VERSION SLPS_01144 x10 8011EF5C 740
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#define U8(p, o) (*(unsigned char *)((p) + (o)))
#define U16(p, o) (*(unsigned short *)((p) + (o)))
#define S16(p, o) (*(short *)((p) + (o)))
#define PTR(p, o) (*(unsigned char **)((p) + (o)))
extern short pushOutOfBoxX(unsigned char *, unsigned char *);
extern void decreaseObjectTimer(unsigned char *, int);

void func_8011EF5C_slps01144(unsigned char *a, unsigned char *b)
{
    short dy, d, t, tt, w, e0, e1, x, s2;
    if (b[3] == 0) {
        if (pushOutOfBoxX(a, b) == 0) return;
        goto tail;
    }
    if ((unsigned short)(U16(PTR(a, 0x44), 2) - U16(PTR(b, 0x44), 2) + 0x2d) >= 0x5b) return;
    t = w = U16(b, 0x6c) + U16(a, 0x6c);
    d = U16(PTR(a, 0x40), 2) - U16(PTR(b, 0x40), 2);
    tt = d;
    e0 = S16(b, 0x6e);
    e1 = S16(a, 0x6e);
    if ((unsigned short)(d + w) > e0 + e1) return;
    { unsigned short q = U16(a, 0x16) - U16(b, 0x16);
    unsigned short r = q + (U16(b, 0x70) + U16(a, 0x70));
    dy = q;
    if (r > S16(a, 0x72) + S16(b, 0x72)) return; }
    s2 = tt; x = t;
    if (d < 0) {
        tt = -d;
        t = -w;
    } else {
        t = (e0 - U16(b, 0x6c)) + (e1 - U16(a, 0x6c));
        x = t;
    }
    if ((unsigned short)(x - tt) < 5) {
        U16(PTR(a, 0x40), 2) = U16(PTR(b, 0x40), 2) + t;
        goto tail;
    }
    if ((short)dy <= 0) {
        if (U8(a, 0xac) == 2) {
            short v = U16(b, 0x16) - (U16(b, 0x70) + U16(a, 0x70));
            U16(a, 0x14) = 0;
            U8(a, 0x69) = 1;
            U16(a, 0x16) = v;
            return;
        }
        if ((a[0] & 2) == 0) {
            if (U8(b, 0x6a) != 0) {
                unsigned char *p = PTR(b, 0x90), *q = PTR(b, 0x94);
                U8(b, 0x6a) = 0;
                U8(b, 0x69) = 1;
                U8(p, 0x69) = 1;
                U8(q, 0x69) = 1;
                b[0] = 2;
                p[0] = 2;
                q[0] = 2;
                a[0] = 2;
                a[4] = 2;
                a[5] = 1;
                a[6] = 0;
                decreaseObjectTimer(a, 4);
                return;
            }
            goto blk;
        }
        {
        short v1 = U16(b, 0x70), v2 = U16(b, 0x16);
        U8(a, 0x69) = 1;
        U16(a, 0x14) = 0;
        U16(a, 0x16) = v2 - (v1 + U16(a, 0x70));
        }
        if (U8(a, 0xa6) == 0) {
            if (s2 >= 0) {
                U8(a, 0xbe) = 8;
                S16(a, 0xb0) = 2;
            } else {
                U8(a, 0xbe) = 9;
                S16(a, 0xb0) = -2;
            }
        }
        return;
    }
    U16(a, 0x16) = U16(b, 0x16) + (U16(b, 0x72) - U16(b, 0x70) + (U16(a, 0x72) - U16(a, 0x70)));
    if (S16(a, 0x7e) < 0) S16(a, 0x7e) = 0;
tail:
    if (a[0] & 2) return;
blk:
    asm volatile("" ::: "memory");
    a[0] = 2;
    {
        int x1 = S16(PTR(b, 0x40), 2);
        int y1 = S16(PTR(a, 0x40), 2);
        a[4] = 2;
        a[5] = 0;
        a[6] = 0;
        U16(a, 0x2e) = y1 < x1;
    }
    decreaseObjectTimer(a, 2);
}
#endif
