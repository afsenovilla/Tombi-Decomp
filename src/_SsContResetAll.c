// FUNC 8006d410 196 MAIN0
// MATCHING 8006d410 196
// Portado de psx_tomba (psyq/libsnd/cc_121.c, _SsContResetAll); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "libsnd_i.h"

void _SsContResetAll(s16 arg0, s16 arg1) {
    struct SeqStruct* score = &_ss_score[arg0][arg1];

    SsUtReverbOff();
    _SsVmDamperOff();
    score->programs[score->channel] = score->channel;
    score->unk13 = 0;
    score->unk14 = 0;
    score->vol[score->channel] = 0x7f;
    score->panpot[score->channel] = 0x40;
    score->delta_value = _SsReadDeltaValue(arg0, arg1);
}
