// FUNC 8007009c 104 MAIN0
// MATCHING 8007009c 104
// Portado de psx_tomba (psyq/libsnd/replay.c, _SsSndReplay); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "libsnd_i.h"

void _SsSndReplay(s16 arg0, s16 arg1) {
    struct SeqStruct* p;

    p = &_ss_score[arg0][arg1];
    p->unk2b = 1;
    _ss_score[arg0][arg1].unk90 &= ~8;
}
