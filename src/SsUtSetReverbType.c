// FUNC 80071228 164 MAIN0
// MATCHING 80071228 164
// Portado de psx_tomba (psyq/libsnd/ut_rev.c, SsUtSetReverbType); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "libsnd_i.h"

s16 SsUtSetReverbType(s16 arg0) {
    s16 var_v1;
    s32 var_a1;

    var_a1 = 0;
    var_v1 = arg0;
    if (arg0 < 0) {
        var_a1 = 1;
        var_v1 = -arg0;
    }

    if ((u16)var_v1 < 10) {
        _svm_rattr.mask = 1;
        if (var_a1 != 0) {
            _svm_rattr.mode = var_v1 | 0x100;
        } else {
            _svm_rattr.mode = var_v1;
        }
        if (var_v1 == 0) {
            SpuSetReverb(0);
        }
        SpuSetReverbModeParam(&_svm_rattr);
        return var_v1;
    }
    return -1;
}

s16 SsUtGetReverbType(void);
