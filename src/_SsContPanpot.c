// FUNC 8006ce9c 208 MAIN0
// MATCHING 8006ce9c 208
// HEADERS tomba24
// Portado de psx_tomba (../psyq/libsnd/cc_10.c, _SsContPanpot); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "libsnd_i.h"

s32 _SsReadDeltaValue(s16, s16);
void _SsVmSetVol(s16 seq_sep_no, s16 vabId, s16 prog, u16 vol, u16 pan);

void _SsContPanpot(s16 access_num, s16 seq_num, u8 data) {
    struct SeqStruct* score = &_ss_score[access_num][seq_num];
    u8 ch = score->channel;

    _SsVmSetVol(access_num | (seq_num << 8), score->unk4c, score->programs[ch],
                score->vol[ch], data);
    score->panpot[ch] = data;
    score->delta_value = _SsReadDeltaValue(access_num, seq_num);
}
