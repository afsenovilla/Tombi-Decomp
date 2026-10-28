// FUNC 8006c574 72 MAIN0
// MATCHING 8006c574 72
// HEADERS tomba24
// Portado de psx_tomba (../psyq/libsnd/ssloop.c, SsSetLoop); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "libsnd_i.h"

void SsSetLoop(s16 access_num, s16 seq_num, s16 l_count) {
    struct SeqStruct* score = &_ss_score[access_num][seq_num];

    score->unk46 = l_count;
    score->unk48 = 0;
}

s16 SsIsEos(s16 access_num, s16 seq_num);
