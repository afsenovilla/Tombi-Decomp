// FUNC 80071150 64 MAIN0
// MATCHING 80071150 64
// Portado de psx_tomba (psyq/libsnd/ut_rdel.c, SsUtSetReverbDelay); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "libsnd_i.h"

void SsUtSetReverbDelay(short delay) {
    _svm_rattr.mask = SPU_REV_DELAYTIME;
    _svm_rattr.delay = delay;
    SpuSetReverbModeParam(&_svm_rattr);
}
