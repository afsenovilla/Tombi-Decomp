// FUNC 8006eb20 256 MAIN0
// MATCHING 8006eb20 256
// Portado de psx_tomba (psyq/libsnd/playmode.c, Snd_SetPlayMode); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "libsnd_i.h"

void Snd_SetPlayMode(s16 arg0, s16 arg1, u8 arg2, s16 arg3) {
    struct SeqStruct* score = &_ss_score[arg0][arg1];

    _ss_score[arg0][arg1].unk90 &= ~0x200;
    _ss_score[arg0][arg1].unk90 &= ~4;
    score->unk46 = arg3;

    if (arg2 == 1) {
        _ss_score[arg0][arg1].unk90 |= 1;
        score->unk48 = 0;
        score->unk2b = 1;
        _SsVmSetSeqVol(arg0 | (arg1 << 8), score->unk74, score->unk76, 0);
    } else if (arg2 == 0) {
        _ss_score[arg0][arg1].unk90 |= 2;
    }
}
