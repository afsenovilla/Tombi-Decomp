// FUNC 80041240 576 MAIN0
/* score 98: structure matches; game has an extra copy "move a1,a2" of cs before "bgez" (neg branch uses copy, else branch uses a2) which shifts all temp regs by one; game also hoists (short)y before x&7 */
extern unsigned short *D_1F800278;
extern unsigned short D_1F800284;
extern short *func_8003F200(int, int);

typedef struct O { char pad[0x44]; short *h; } O;

int func_80041240(O *o, short x, short y)
{
    short *p;
    short n;
    int cnt;
    unsigned short a, b, c, d;
    int lo, hi;
    short cs, k, r;
    char xm;

    p = func_8003F200((short)x, o->h[1]);
    D_1F800278 = (unsigned short *)(p + 1);
    n = *p;
    if (n == 0) return 0;
    cnt = 0;
    xm = x & 7;
    do {
        a = *D_1F800278++;
        n--;
        if ((a & 0xc) == 0) {
            D_1F800278 += 3;
            cnt++;
            continue;
        }
        b = *D_1F800278++;
        c = *D_1F800278++;
        cs = c;
        d = *D_1F800278++;
        lo = d & 0xf;
        if (cs == 0) continue;
        hi = (d >> 4) & 0xf;
        if (cnt != 0 && (short)b + 0x10 < (short)y) break;
        cnt++;
        k = y - b;
        if (cs < 0) {
            if (k < cs) continue;
            if (k > 0) return 0;
        } else {
            if (cs < k) break;
            if (k < 0) continue;
        }
        D_1F800284 = (a & 0xe00) >> 9;
        if (a & 0x10) {
            if (a & 4) return 1;
            return (a >> 2) & 2;
        }
        r = xm - lo - hi * ((short)y - (short)b) / (short)c;
        if (a & 4) return r >= 0;
        if (a & 8) return (r < 1) << 1;
    } while (n != 0);
    return 0;
}
