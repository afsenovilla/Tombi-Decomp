// FUNC 80063548 160 MAIN0
// MATCHING 80063548 160
// Ported from sotn-decomp src/main/psxsdk/libgte/geo_01.c (MIT, (c) github.com/xeeynamo/sotn-decomp contributors), derived from Sony's PSY-Q libraries.
#define SKIP_ASM

#include "common.h"

extern s16 RSIN_TABLE[];

int rcos(int a) {
    if (a < 0) {
        a = -a;
    }
    a &= 0xFFF;

    if (a <= 0x800) {
        if (a <= 0x400) {
            return RSIN_TABLE[0x400 - a];
        } else {
            return -RSIN_TABLE[a - 0x400];
        }
    } else {
        if (a <= 0xC00) {
            return -RSIN_TABLE[0xC00 - a];
        } else {
            return RSIN_TABLE[a - 0xC00];
        }
    }
}
