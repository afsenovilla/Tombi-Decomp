// FUNC 8006347c 60 MAIN0
// MATCHING 8006347c 60
// Ported from sotn-decomp src/main/psxsdk/libgte/geo_00.c (MIT, (c) github.com/xeeynamo/sotn-decomp contributors), derived from Sony's PSY-Q libraries.
#define SKIP_ASM

#include "common.h"

extern s16 rsin_tbl[];

int rsin(int a) {
    if (a < 0) {
        return -sin_1(-a & 0xFFF);
    } else {
        return sin_1(a & 0xFFF);
    }
}

int sin_1(int a);
