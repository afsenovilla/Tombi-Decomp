// FUNC 8006d01c 144 MAIN0
// MATCHING 8006d01c 144
// HEADERS tomba24
// Portado de psx_tomba (../psyq/libsnd/cc_91.c, _SsContExternal); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "libsnd_i.h"

s32 _SsReadDeltaValue(s16, s16);

void _SsContExternal(s16 access_num, s16 seq_num, u8 data) {
    struct SeqStruct* score = &_ss_score[access_num][seq_num];

    SsUtSetReverbDepth(data, data);
    score->delta_value = _SsReadDeltaValue(access_num, seq_num);
}
