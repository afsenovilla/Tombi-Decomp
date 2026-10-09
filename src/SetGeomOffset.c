// FUNC 80063fdc 32 MAIN0
// MATCHING 80063fdc 32
// Portado de psx_tomba (psyq/libgte/reg12.c, SetGeomOffset); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"

void SetGeomOffset(long ofx, long ofy) {
    __asm__ volatile("sll $4, %0, 16; sll $5, %1, 16; ctc2 $4, $24; ctc2 $5, $25" : : "r"(ofx), "r"(ofy));
}

__asm__("nop");
__asm__("nop");
