// FUNC 800637a0 156 MAIN0
// MATCHING 800637a0 156
// Ported from dcb_decomp src/main/psyq/libgte_geo_01.c (MIT, (c) 2026 ReGame Labs), derived from Sony's PSY-Q libraries.
#define SKIP_ASM
#include "common.h"

/* rsin_tbl and the labels splat put inside it */
extern short D_80071058[];
extern short D_80070858[];
extern short D_8006F858[];

int rcos(int a);

long csqrt_1(long a);
long func_8005FC54(long a);

int csqrt(int a) {
    long n;
    long s;
    long m;

    if (a == 0) {
        return 0;
    }
    n = 8 - func_8005FC54(a);
    if (n >= 0) {
        s = n >> 1;
        m = a >> (s * 2);
    } else {
        s = (n >> 1) + 1;
        m = a << -(s * 2);
    }
    s -= 6;
    if (s >= 0) {
        return csqrt_1(m) << s;
    }
    return csqrt_1(m) >> -s;
}

extern long D_80071868[];

int catan(int a);
