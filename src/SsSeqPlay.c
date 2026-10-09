// FUNC 8006ea90 56 MAIN0
// MATCHING 8006ea90 56
// Portado de psx_tomba (psyq/libsnd/ssplay.c, SsSeqPlay); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "libsnd_i.h"

void SsSeqPlay(short seq_access_num, char play_mode, short l_count) {
    Snd_SetPlayMode(seq_access_num, 0, play_mode, l_count);
}

void SsSepPlay( short sep_access_num, short seq_num, char play_mode, short l_count);

void SsQuit(void);
