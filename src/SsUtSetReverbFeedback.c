// FUNC 800712dc 64 MAIN0
// MATCHING 800712dc 64
// Portado de psx_tomba (psyq/libsnd/ut_rfb.c, SsUtSetReverbFeedback); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "libsnd_i.h"

s32 SpuSetReverb(s32);

void SsUtSetReverbFeedback(s16 feedback) {
    _svm_rattr.mask = SPU_REV_FEEDBACK;
    _svm_rattr.feedback = feedback;
    SpuSetReverbModeParam(&_svm_rattr);
}

void SsUtReverbOff(void);

void SsUtReverbOn(void);
