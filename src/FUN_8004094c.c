// FUNC 8004094c 372 MAIN0
// MATCHING 8004094c 372
extern unsigned short *DAT_1f800278;

int FUN_8004094c(int x, int y)
{
    int base;
    short amp;
    unsigned short v;
    int r; short n, lo;

    base = *DAT_1f800278++;
    if ((unsigned short)(y - base + 16) > 32) {
        DAT_1f800278 += 2;
        return 0;
    }
    amp = *DAT_1f800278++;
    if (amp == 0) {
        DAT_1f800278++;
        x = base;
    } else {
        v = *DAT_1f800278++;
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
