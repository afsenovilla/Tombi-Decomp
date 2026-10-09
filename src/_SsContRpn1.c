// FUNC 8006d320 120 MAIN0
// MATCHING 8006d320 120
// Portado de psx_tomba (psyq/libsnd/cc_100.c, _SsContRpn1); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "libsnd_i.h"

void _SsContRpn1(s16 arg0, s16 arg1, u8 arg2) {
    struct SeqStruct* score = &_ss_score[arg0][arg1];

    score->unk13 = arg2;
    score->unk29 += 1;
    score->delta_value = _SsReadDeltaValue(arg0, arg1);
}
