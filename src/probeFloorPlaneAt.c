// FUNC 80041ebc 608 MAIN0
// MATCHING 80041ebc 608
// VERSION SLPM_80165 slpm80165 8003EA64 624
#if defined(VERSION_SLPM_80165)
// Demo version of PAL main0 80041EBC (func_80041EBC.c): returns 0 for a negative column first; 4-bit slope field (PAL
// 3 bits).
#include "spad.h"
typedef struct O { char p[0x44]; short *q; } O;
extern unsigned short *getCollisionPlaneAt(int, int);
extern unsigned short *COLLISION_SEG_PTR;
int func_80041EBC(O *o, short a, short b)
{
    short n;
    int cnt;
    unsigned short flags, w;
    int h;
    short k, lo, nn, m;
    int x;
    char pad[8];
    if (a < 0) return 0;
    COLLISION_SEG_PTR = getCollisionPlaneAt(a, o->q[1]);
    n = *(short *)COLLISION_SEG_PTR++;
    if (n == 0) return 0;
    cnt = 0;
    do {
        flags = *COLLISION_SEG_PTR++;
        n--;
        if (flags & 0x10) {
            COLLISION_SEG_PTR += 3;
            continue;
        }
        if (!(flags & 1)) {
            COLLISION_SEG_PTR += 3;
            cnt++;
            continue;
        }
        h = *COLLISION_SEG_PTR++;
        if (cnt != 0 && (short)h + 0x10 < (short)b) return 0;
        k = *(short *)COLLISION_SEG_PTR++;
        cnt++;
        if (k == 0) {
            x = h;
            COLLISION_SEG_PTR++;
        } else {
            w = *COLLISION_SEG_PTR++;
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
            SPAD.s284 = (flags & 0xf00) >> 8;
            return 1;
        }
    } while (n != 0);
    return 0;
}
#else
#include "spad.h"
typedef struct O { char p[0x44]; short *q; } O;
extern unsigned short *getCollisionPlaneAt(int, int);
extern unsigned short *COLLISION_SEG_PTR;
int probeFloorPlaneAt(O *o, short a, short b)
{
    short n;
    int cnt;
    unsigned short flags, w;
    int h;
    short k, lo, nn, m;
    int x;
    char pad[8];
    COLLISION_SEG_PTR = getCollisionPlaneAt(a, o->q[1]);
    n = *(short *)COLLISION_SEG_PTR++;
    if (n == 0) return 0;
    cnt = 0;
    do {
        flags = *COLLISION_SEG_PTR++;
        n--;
        if (flags & 0x10) {
            COLLISION_SEG_PTR += 3;
            continue;
        }
        if (!(flags & 1)) {
            COLLISION_SEG_PTR += 3;
            cnt++;
            continue;
        }
        h = *COLLISION_SEG_PTR++;
        if (cnt != 0 && (short)h + 0x10 < (short)b) return 0;
        k = *(short *)COLLISION_SEG_PTR++;
        cnt++;
        if (k == 0) {
            x = h;
            COLLISION_SEG_PTR++;
        } else {
            w = *COLLISION_SEG_PTR++;
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
#endif
