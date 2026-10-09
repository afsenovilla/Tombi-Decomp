// FUNC 800635e8 100 MAIN0
// MATCHING 800635e8 100
// Portado de psx_tomba (psyq/libgte/fog_01.c, SetFogNear); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"

void SetFogNear(long a, long h) {
    SetDQA(-(a * 0x140) / h);
    SetDQB(0x01400000);
}
