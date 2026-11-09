// near-twin of PAL blackCrowContact (tools/jp_autofix.py)
// VERSION SLPS_01144 x03 8011F04C 400
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#define U8(p, o) (*(unsigned char *)((p) + (o)))
#define U16(p, o) (*(unsigned short *)((p) + (o)))
#define S16(p, o) (*(short *)((p) + (o)))
#define PTR(p, o) (*(unsigned char **)((p) + (o)))
extern short objectBoxOverlapSideFacing(unsigned char *, unsigned char *);
extern void decreaseObjectTimer(unsigned char *, int);
extern void spawnImpactEffect(int, int, int, int);
extern unsigned char DEBUG_INVINCIBLE;

void func_8011F04C(unsigned char *a, unsigned char *b)
{
    short r = objectBoxOverlapSideFacing(a, b);
    if (r == -1)
        return;
    if (U16(b, 0xb4) == 0) goto fall;
    switch (U8(a, 0xac)) {
    case 1:
        if (r == 2) {
            b[0] = 2;
            b[4] = 2;
            b[5] = 1;
            b[6] = 0;
            U16(b, 0x2e) = U16(a, 0x2e) & 1;
            PTR(a, 0xd4) = b;
            U8(a, 0xac) = 2;
            spawnImpactEffect(2, S16(a, 0x12), S16(a, 0x16), S16(a, 0x1a));
            break;
        }
    case 0:
    case 3:
    fall:
        if ((b[0] & 2) == 0) {
            if ((a[0] & 2) == 0) {
                int ah, bh;
                a[0] = 2;
                bh = S16(PTR(b, 0x40), 2);
                ah = S16(PTR(a, 0x40), 2);
                a[4] = 2;
                a[5] = 0;
                a[6] = 0;
                U16(a, 0x2e) = ah < bh;
                decreaseObjectTimer(a, 1);
            }
        }
        break;
    case 2:
        if (b[0] != 3) {
            int ah, bh;
            b[0] = 3;
            ah = S16(PTR(a, 0x40), 2);
            bh = S16(PTR(b, 0x40), 2);
            S16(b, 0x7a) = bh < ah;
            b[4] = 2;
            b[5] = 0;
            b[6] = 0;
        }
        break;
    }
}
#endif
