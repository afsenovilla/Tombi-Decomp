// FUNC 8004094c 372 MAIN0
// MATCHING 8004094c 372
extern unsigned short *COLLISION_SEG_PTR;

int testCollisionSegment(int x, int y)
{
    int base;
    short amp;
    unsigned short v;
    int r; short n, lo;

    base = *COLLISION_SEG_PTR++;
    if ((unsigned short)(y - base + 16) > 32) {
        COLLISION_SEG_PTR += 2;
        return 0;
    }
    amp = *COLLISION_SEG_PTR++;
    if (amp == 0) {
        COLLISION_SEG_PTR++;
        x = base;
    } else {
        v = *COLLISION_SEG_PTR++;
        lo = v & 0xf;
        n = (v >> 4) & 0xf;
        if (n == 0) return 0;
        x = (short)x;
        r = (short)(x % 8);
        if (r < lo) return 0;
        if (lo + n < r) return 0;
        x = base + amp * ((x - lo) % n) / n;
    }
    return (short)x - (short)y < 1;
}
