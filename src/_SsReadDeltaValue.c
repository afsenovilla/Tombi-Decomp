// FUNC 8006e6c0 184 MAIN0
// MATCHING 8006e6c0 184
// Portado de psx_tomba (psyq/libsnd/miditime.c, _SsReadDeltaValue); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "libsnd_i.h"

s32 _SsReadDeltaValue(s16 arg0, s16 arg1) {
    struct SeqStruct* score = &_ss_score[arg0][arg1];
    s32 temp_v0;
    u8 temp_v1_2;
    u32 var_a0;

    var_a0 = *score->read_pos++;
    if (var_a0 == 0) {
        return 0;
    }

    // variable length quantity decoding (midi)
    if (var_a0 & 0x80) {
        var_a0 &= 0x7F;
        do {
            temp_v1_2 = *score->read_pos++;
            var_a0 = (var_a0 << 7) + (temp_v1_2 & 0x7F);
        } while (temp_v1_2 & 0x80);
    }

    temp_v0 = var_a0 * 10;
    score->unk80 += temp_v0;
    return temp_v0;
}
