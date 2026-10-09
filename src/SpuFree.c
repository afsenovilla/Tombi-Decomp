// FUNC 80075f80 124 MAIN0
// MATCHING 80075f80 124
// Portado de psx_tomba (psyq/libspu/s_m_f.c, SpuFree); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "libspu_internal.h"

void SpuFree(unsigned long arg0) {
    s32 i;

    for (i = 0; i < D_80097CA4; i++) {
        if (_spu_memList[i].addr & 0x40000000) {
            break;
        }
        if (_spu_memList[i].addr == arg0) {
            _spu_memList[i].addr |= 0x80000000;
            break;
        }
    }
    _spu_gcSPU();
}
