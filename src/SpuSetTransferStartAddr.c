// FUNC 800772c0 88 MAIN0
// MATCHING 800772c0 88
// Portado de psx_tomba (psyq/libspu/s_stsa.c, SpuSetTransferStartAddr); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "libspu_internal.h"

unsigned long SpuSetTransferStartAddr(unsigned long addr) {
    unsigned _addr;
    _addr = addr;
    if (_addr - 0x1010 > 0x7EFE8) {
        return 0;
    }
    _addr = _spu_FsetRXXa(-1, _addr);
    _spu_tsa = (u16)_addr;
    return (u16)_addr << _spu_mem_mode_plus;
}
