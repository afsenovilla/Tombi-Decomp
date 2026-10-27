// FUNC 80040d30 376 MAIN0
// MATCHING 80040d30 376
extern unsigned short *COLLISION_SEG_PTR;

int testCollisionSegmentStep(int x, int y)
{
    int h;
    unsigned short w;
    short k, m;
    short lo, n;
    h = *COLLISION_SEG_PTR++;
    if ((unsigned short)(y - h + 0x10) > 0x20) {
        COLLISION_SEG_PTR += 2;
        return 0;
    }
    k = *(short *)COLLISION_SEG_PTR++;
    if (k == 0) {
        COLLISION_SEG_PTR++;
        x = h;
    } else {
        w = *COLLISION_SEG_PTR++;
        lo = w & 0xf;
        n = (w >> 4) & 0xf;
        if (n == 0) return 0;
        x = (short)x;
        m = x % 8;
        if (m < lo) return 0;
        if (lo + n < m) return 0;
        x = h + k * ((x - lo) % n) / n;
    }
    return (short)x - (short)y >= 0;
}
