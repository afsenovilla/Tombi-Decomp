// FUNC 8006f90c 164 MAIN0
// MATCHING 8006f90c 164
// Portado de psx_tomba (psyq/libsnd/pause.c, _SsSndPause); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "libsnd_i.h"

void _SsSndPause(s16 arg0, s16 arg1) {
    struct SeqStruct* score = &_ss_score[arg0][arg1];
    _SsVmSeqKeyOff((s16)(arg0 | arg1 << 8));
    score->unk2b = 0;
    _ss_score[arg0][arg1].unk90 &= ~2;
}
