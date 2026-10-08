// FUNC 80040ac0 316 MAIN0
// MATCHING 80040ac0 316
extern unsigned short *DAT_1f800278;

unsigned func_80040AC0(int u, int a)
{
    unsigned short x;
    unsigned short w;
    unsigned short m;
    int lo;
    int hi;
    short sw;
    unsigned short t;
    unsigned short v;
    unsigned char cu;
    short cl;
    unsigned char ch;
    x = *DAT_1f800278++;
    w = *DAT_1f800278++;
    m = *DAT_1f800278++;
    lo = m & 0xf;
    hi = (m >> 4) & 0xf;
    sw = w;
    if (sw == 0) return 0;
    if ((short)a > (short)x + 0x10) return 0;
    if ((int)(unsigned short)(a - x - w - 1) > -sw) return 0;
    u &= 7;
    v = u;
    cu = u;
    cl = lo;
    t = 0;
    if (cu < cl) return 0;
    ch = hi;
    if (cl + ch < cu) {
        t = u - (lo + hi);
        v = lo + hi;
    }
    return (short)(t + (v - lo - ch * ((short)a - (short)x) / sw)) >= 0;
}
