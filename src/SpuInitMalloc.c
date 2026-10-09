// FUNC 8007595c 84 MAIN0
// MATCHING 8007595c 84
// Portado de psx_tomba (psyq/libspu/s_m_init.c, SpuInitMalloc); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "libspu_internal.h"

s32 SpuInitMalloc(s32 num, s8* top) {
    if (num > 0) {
        _spu_memList = top;
        _spu_memList[0].addr = 0x40001010;
        _spu_memList[0].size = (0x10000 << _spu_mem_mode_plus) - 0x1010;
        _spu_AllocLastNum = 0;
        D_80097CA4 = num;
        return num;
    }
    return 0;
}
