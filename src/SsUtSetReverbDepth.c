// FUNC 80071190 152 MAIN0
// MATCHING 80071190 152
// Portado de psx_tomba (psyq/libsnd/ut_rdep.c, SsUtSetReverbDepth); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "libsnd_i.h"

void SsUtSetReverbDepth(short ldepth, short rdepth) {
    _svm_rattr.mask = 6;
    _svm_rattr.depth.left = (ldepth * 0x7FFF) / 127;
    _svm_rattr.depth.right = (rdepth * 0x7FFF) / 127;
    SpuSetReverbModeParam(&_svm_rattr);
}
