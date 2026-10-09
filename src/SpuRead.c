// FUNC 80077260 96 MAIN0
// MATCHING 80077260 96
// Portado de psx_tomba (psyq/libspu/s_r.c, SpuRead); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "libspu_internal.h"

unsigned long SpuRead(unsigned char* addr, unsigned long size) {
    if (size > 0x7EFF0) {
        size = 0x7EFF0;
    }
    _spu_write(addr, size);
    if (_spu_transferCallback == NULL) {
        _spu_inTransfer = 0;
    }
    return size;
}
