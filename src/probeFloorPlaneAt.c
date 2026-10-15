// FUNC 80041ebc 608 MAIN0
// MATCHING 80041ebc 608
#include "spad.h"
typedef struct O { char p[0x44]; short *q; } O;
extern unsigned short *getCollisionPlaneAt(int, int);
extern unsigned short *DAT_1f800278;
int probeFloorPlaneAt(O *o, short a, short b)
{
    short n;
    int cnt;
    unsigned short flags, w;
    int h;
    short k, lo, nn, m;
    int x;
    char pad[8];
    DAT_1f800278 = getCollisionPlaneAt(a, o->q[1]);
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
            x = h;
            DAT_1f800278++;
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
            SPAD.s27e = k;
            SPAD.s280 = h;
            (*(short*)&SPAD.s282) = flags;
            SPAD.s284 = (flags & 0xe00) >> 9;
            return 1;
        }
    } while (n != 0);
    return 0;
}
