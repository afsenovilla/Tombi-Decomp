// FUNC 80074690 112 MAIN0
// MATCHING 80074690 112
// Portado de psx_tomba (psyq/libsnd/vs_vfb.c, SsVabFakeBody); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "libsnd_i.h"

s16 SsVabFakeBody(s16 vabid) {
    if (vabid >= 0 && vabid <= 0x10 && _svm_vab_used[vabid] == 2) {
        _spu_setInTransfer(0);
        _svm_vab_used[vabid] = 1;
        return vabid;
    }
    return -1;
}
