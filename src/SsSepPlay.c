// FUNC 8006eac8 56 MAIN0
// MATCHING 8006eac8 56
// Portado de psx_tomba (psyq/libsnd/ssplay.c, SsSepPlay); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "libsnd_i.h"

void SsSeqPlay(short seq_access_num, char play_mode, short l_count);

void SsSepPlay(
    short sep_access_num, short seq_num, char play_mode, short l_count) {
    Snd_SetPlayMode(sep_access_num, seq_num, play_mode, l_count);
}

void SsQuit(void);
