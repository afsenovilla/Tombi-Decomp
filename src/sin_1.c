// FUNC 800634b8 144 MAIN0
// MATCHING 800634b8 144
// Portado de psx_tomba (psyq/libgte/geo_00.c, sin_1); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"

extern s16 RSIN_TABLE[];

s32 rsin(s32 arg0);

s32 sin_1(s32 arg0) {
    if (arg0 < 0x801) {
        if (arg0 < 0x401) {
            return RSIN_TABLE[arg0];
        } else {
            return RSIN_TABLE[0x800 - arg0];
        }
    } else {
        if (arg0 < 0xC01) {
            return -1 * RSIN_TABLE[arg0 - 0x800];
        } else {
            return -1 * RSIN_TABLE[0x1000 - arg0];
        }
    }
}
