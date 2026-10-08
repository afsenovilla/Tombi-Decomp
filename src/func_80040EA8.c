// FUNC 80040ea8 208 MAIN0
// MATCHING 80040ea8 208
extern unsigned short *D_1f800278;
static __inline__ short inrange(short m, char lo, int w)
{
    if (m < lo) return 0;
    if (lo + w < m) return 0;
    return 1;
}
int func_80040EA8(int x, short y)
{
    unsigned short a, b, c;
    int lo, w, e;
    a = *D_1f800278++;
    b = *D_1f800278++;
    c = *D_1f800278++;
    lo = c & 0xf;
    w = (c >> 4) & 0xf;
    e = b + a;
    if ((short)a >= y && y >= (short)e) {
        return inrange((short)x % 8, lo, w);
    }
    return 0;
}
