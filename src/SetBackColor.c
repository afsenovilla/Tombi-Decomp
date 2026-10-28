// FUNC 80063f9c 32 MAIN0
// MATCHING 80063f9c 32
// HEADERS tomba24
// Portado de psx_tomba (../psyq/libgte/reg10.c, SetBackColor); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "psyq/libgte.h"
#include "psyq/inline_c.h"

void SetBackColor(long rbk, long gbk, long bbk) {
    rbk <<= 4;
    gbk <<= 4;
    bbk <<= 4;
    gte_ldbkdir(rbk, gbk, bbk);
}
