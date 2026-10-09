// FUNC 8006ec20 80 MAIN0
// MATCHING 8006ec20 80
// Portado de psx_tomba (psyq/libsnd/sssmv.c, SsSetMVol); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "libsnd_i.h"

void SsSetMVol(short voll, short volr) {
    SpuCommonAttr attr;
    attr.mask = SPU_COMMON_MVOLL | SPU_COMMON_MVOLR;
    attr.mvol.left = 0x81 * voll;
    attr.mvol.right = 0x81 * volr;
    SpuSetCommonAttr(&attr);
}
