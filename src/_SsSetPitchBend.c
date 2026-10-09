// FUNC 8006e090 184 MAIN0
// MATCHING 8006e090 184
// Portado de psx_tomba (psyq/libsnd/midibend.c, _SsSetPitchBend); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "libsnd_i.h"

void _SsSetPitchBend(s16 arg0, s16 arg1) {
    struct SeqStruct* score = &_ss_score[arg0][arg1];
    u8 channel = score->channel;
    u8* temp_v1;

    temp_v1 = score->read_pos++;
    _SsVmPitchBend(
        (s16)(arg0 | (arg1 << 8)), score->unk4c, score->programs[channel], *temp_v1);
    score->delta_value = _SsReadDeltaValue(arg0, arg1);
}
