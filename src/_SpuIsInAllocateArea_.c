// FUNC 80076378 140 MAIN0
// MATCHING 80076378 140
// Portado de psx_tomba (psyq/libspu/s_sr.c, _SpuIsInAllocateArea_); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "libspu_internal.h"

s32 SpuSetReverb(s32 on_off);

int _SpuIsInAllocateArea(unsigned arg0);

int _SpuIsInAllocateArea_(unsigned arg0) {
    int i;

    arg0 <<= _spu_mem_mode_plus;
    if (_spu_memList == NULL) {
        return 0;
    }
    for (i = 0;; i++) {
        if (_spu_memList[i].addr & 0x80000000) {
            continue;
        }
        if (_spu_memList[i].addr & 0x40000000) {
            break;
        }
        if (arg0 <= (_spu_memList[i].addr & 0x0FFFFFFF)) {
            return 1;
        }
        if (arg0 < (_spu_memList[i].addr & 0x0FFFFFFF) + _spu_memList[i].size) {
            return 1;
        }
    }
    return 0;
}
