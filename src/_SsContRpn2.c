// FUNC 8006d398 120 MAIN0
// MATCHING 8006d398 120
// Portado de psx_tomba (psyq/libsnd/cc_101.c, _SsContRpn2); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "libsnd_i.h"

void _SsContRpn2(s16 arg0, s16 arg1, u8 arg2) {
    struct SeqStruct* score = &_ss_score[arg0][arg1];

    score->unk14 = arg2;
    score->unk29 += 1;
    score->delta_value = _SsReadDeltaValue(arg0, arg1);
}
