// FUNC 80040d30 376 MAIN0
// MATCHING 80040d30 376
extern unsigned short *DAT_1f800278;

int FUN_80040d30(int x, int y)
{
    int h;
    unsigned short w;
    short k, m;
    short lo, n;
    h = *DAT_1f800278++;
    if ((unsigned short)(y - h + 0x10) > 0x20) {
        DAT_1f800278 += 2;
        return 0;
    }
    k = *(short *)DAT_1f800278++;
    if (k == 0) {
        DAT_1f800278++;
        x = h;
    } else {
        w = *DAT_1f800278++;
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
