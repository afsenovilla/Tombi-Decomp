// FUNC 8006e648 120 MAIN0
// MATCHING 8006e648 120
// Portado de psx_tomba (psyq/libsnd/midiprog.c, _SsSetProgramChange); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "libsnd_i.h"

void _SsSetProgramChange(s16 arg0, s16 arg1, u8 arg2) {
    struct SeqStruct* score = &_ss_score[arg0][arg1];
    score->programs[score->channel] = arg2;
    score->delta_value = _SsReadDeltaValue(arg0, arg1);
}
