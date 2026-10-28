// FUNC 8006c914 128 MAIN0
// MATCHING 8006c914 128
// HEADERS tomba24
// Portado de psx_tomba (../psyq/libsnd/cc_0.c, _SsContBankChange); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "libsnd_i.h"

s32 _SsReadDeltaValue(s16, s16);

void _SsContBankChange(s16 access_num, s16 seq_num) {
    struct SeqStruct* score = &_ss_score[access_num][seq_num];

    score->unk4c = *score->read_pos++;
    score->delta_value = _SsReadDeltaValue(access_num, seq_num);
}
