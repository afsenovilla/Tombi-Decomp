// VERSION SLPM_80165 slpm80165 8003F028 312
#if defined(VERSION_SLPM_80165)
// Demo version of PAL main0 8004232C (probeFlaggedSegmentA.c): returns 0 for a negative column first.
extern short *getCollisionPlaneAt(int a, int b);
extern unsigned short *DAT_1f800278;

int probeFlaggedSegmentA(short a, int y, short b)
{
    short n;
    unsigned short w, x0, h;
    int d;
    short *q;
    if (a < 0) return 0;
    q = getCollisionPlaneAt(a, b);
    DAT_1f800278 = (unsigned short *)(q + 1);
    n = *q;
    if (n == 0) return 0;
    for (;;) {
        w = *DAT_1f800278++;
        n--;
        if (w & 0x4000) {
            if (w & 0x10) goto take;
        }
        DAT_1f800278 += 3;
        goto test;
take:
        x0 = *DAT_1f800278++;
        h = *DAT_1f800278++;
        DAT_1f800278++;
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
