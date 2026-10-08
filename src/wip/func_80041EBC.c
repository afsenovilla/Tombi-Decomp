// FUNC 80041ebc 608 MAIN0
// matchcheck: 3 words differ (x lands in v0, game a2). ncheck does not expand div checks: verify with matchcheck
typedef struct O { char p[0x44]; short *q; } O;
extern unsigned short *func_8003F200(int, int);
extern unsigned short *DAT_1f800278;
extern short D_1F80027E, D_1F800280, D_1F800282, D_1F800284;
int func_80041EBC(O *o, short a, short b)
{
    short n;
    int cnt;
    unsigned short flags, w;
    int h;
    short k, lo, nn, m;
    int x;
    char pad[8];
    DAT_1f800278 = func_8003F200(a, o->q[1]);
    n = *(short *)DAT_1f800278++;
    if (n == 0) return 0;
    cnt = 0;
    do {
        flags = *DAT_1f800278++;
        n--;
        if (flags & 0x10) {
            DAT_1f800278 += 3;
            continue;
        }
        if (!(flags & 1)) {
            DAT_1f800278 += 3;
            cnt++;
            continue;
        }
        h = *DAT_1f800278++;
        if (cnt != 0 && (short)h + 0x10 < (short)b) return 0;
        k = *(short *)DAT_1f800278++;
        cnt++;
        if (k == 0) {
            DAT_1f800278++;
            x = h;
        } else {
            w = *DAT_1f800278++;
            lo = w & 0xf;
            nn = (w >> 4) & 0xf;
            if (nn == 0) continue;
            m = a % 8;
            if (m < lo) continue;
            if (lo + nn < m) continue;
            x = h + k * ((a - lo) % nn) / nn;
        }
        if ((short)(x - b) <= 0) {
            D_1F80027E = k;
            D_1F800280 = h;
            D_1F800282 = flags;
            D_1F800284 = (flags & 0xe00) >> 9;
            return 1;
        }
    } while (n != 0);
    return 0;
}
