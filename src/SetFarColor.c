// FUNC 80063fbc 32 MAIN0
// MATCHING 80063fbc 32
// HEADERS tomba24
// Portado de psx_tomba (../psyq/libgte/reg11.c, SetFarColor); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "psyq/libgte.h"
#include "psyq/inline_c.h"

void SetFarColor(long rfc, long gfc, long bfc) {
    rfc <<= 4;
    gfc <<= 4;
    bfc <<= 4;
    gte_ldfcdir(rfc, gfc, bfc);
}
