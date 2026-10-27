// FUNC 8004232c 304 MAIN0
// MATCHING 8004232c 304
// VERSION SLPM_80165 slpm80165 8003EEF0 312
#if defined(VERSION_SLPM_80165)
// Demo version of PAL main0 8004232C (probeFlaggedSegmentA.c): returns 0 for a negative column first.
extern short *getCollisionPlaneAt(int a, int b);
extern unsigned short *COLLISION_SEG_PTR;

int probeFlaggedSegmentA(short a, int y, short b)
{
    short n;
    unsigned short w, x0, h;
    int d;
    short *q;
    if (a < 0) return 0;
    q = getCollisionPlaneAt(a, b);
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
#else
extern short *getCollisionPlaneAt(int a, int b);
extern unsigned short *COLLISION_SEG_PTR;

int probeFlaggedSegmentA(short a, int y, short b)
{
    short n;
    unsigned short w, x0, h;
    int d;
    short *q;
    q = getCollisionPlaneAt(a, b);
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
