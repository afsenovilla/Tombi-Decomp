// FUNC 800205d8 436 MAIN0
int FUN_800205d8(int a, int b)
{
    int r, q;
    if (a == 0) {
        if (b < 0) return 0x40;
        return 0xc0;
    }
    if (b == 0) return (a < 1) << 7;
    if (a > 0) {
        if (b < 1) {
            if (b + a > 0) return -((b << 16) / a >> 11);
            goto L1;
        }
        if (b - a > 0) goto L2;
        q = (b << 16) / a >> 11;
        r = 0x100;
        if (q == 0) return 0;
    } else {
        if (b < 1) {
            if (b - a <= 0) {
L1:
                return ((a << 16) / b >> 11) + 0x40;
            }
        } else {
            if (b + a > 0) {
L2:
                return ((a << 16) / b >> 11) + 0xc0;
            }
        }
        q = (b << 16) / a >> 11;
        r = 0x80;
    }
    return r - q;
}
