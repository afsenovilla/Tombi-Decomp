// FUNC 80041480 568 MAIN0
// MATCHING 80041480 568
/* Slope/floor probe over the collision plane segments (US psx_tomba main/game/collision.c func_80044694 plus the PAL `(a & 0x10)` skip; the `a & 0x10` return further down is dead in both). */
typedef struct O_44694 { char pad[0x44]; short *h; } O_44694;
extern unsigned short *getCollisionPlaneAt(int, int);
extern unsigned short *D_1F800278;
extern unsigned char D_1F800284;

int probeSlopeFloorAt(O_44694 *o, short x, short y)
{
    short *p;
    short n;
    int cnt;
    unsigned short a;
    unsigned short b;
    unsigned short c;
    unsigned short d;
    int lo;
    int hi;
    int cs;
    short k;
    short r;
    short cc;
    char xm;
    int ys;

    p = (short *)getCollisionPlaneAt((short)x, o->h[1]);
    D_1F800278 = (unsigned short *)(p + 1);
    n = *p;
    if (n == 0) return 0;
    cnt = 0;
    ys = (short)y;
    xm = x & 7;
    while (n != 0) {
        a = *D_1F800278++;
        n--;
        if ((a & 0xc) == 0 || (a & 0x10)) {
            D_1F800278 += 3;
            cnt++;
            continue;
        }
        b = *D_1F800278++;
        c = *(volatile unsigned short *)D_1F800278++;
        cs = (short)c;
        d = *D_1F800278++;
        if (cs == 0) continue;
        lo = d & 0xf;
        hi = (d >> 4) & 0xf;
        if (cnt != 0 && (short)b + 0x10 < ys) break;
        cnt++;
        k = y - b;
        cc = cs;
        if (cc < 0) {
            if (k < cc) continue;
            if (k > 0) return 0;
        } else {
            if (cs < k) break;
            if (k < 0) continue;
        }
        if (a & 0x10) {
            if (a & 4) return 1;
            return (a >> 2) & 2;
        }
        r = xm - lo - hi * (ys - (short)b) / (short)c;
        if (a & 4) return r >= 0;
        if (a & 8) return (r < 1) << 1;
    }
    return 0;
}
