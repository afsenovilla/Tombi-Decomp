// FUNC 800773c0 44 MAIN0
// MATCHING 800773c0 44
// Ported from sotn-decomp src/main/psxsdk/libspu/s_it.c (MIT, (c) github.com/xeeynamo/sotn-decomp contributors), derived from Sony's PSY-Q libraries.
#define SKIP_ASM

#include "common.h"
#include "libspu_internal.h"

void _spu_setInTransfer(s32 arg0) {
    if (arg0 == 1) {
        _spu_inTransfer = 0;
    } else {
        _spu_inTransfer = 1;
    }
}

s32 _spu_getInTransfer(void);
