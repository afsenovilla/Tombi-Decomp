// FUNC 8007460c 132 MAIN0
// MATCHING 8007460c 132
// Portado de psx_tomba (psyq/libsnd/vs_vab.c, SsVabClose); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "libsnd_i.h"

void SsVabClose(s16 vabid) {
    if ((vabid >= 0 && vabid < 0x10) && (_svm_vab_used[vabid] == 1)) {
        SpuFree(_svm_vab_start[vabid]);
        _svm_vab_used[vabid] = 0;
        _svm_vab_count -= 1;
    }
}
