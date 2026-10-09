// FUNC 8007026c 40 MAIN0
// MATCHING 8007026c 40
// Portado de psx_tomba (psyq/libsnd/ssstop.c, SsSeqStop); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "libsnd_i.h"

void _SsSndStop(s16 arg0, s16 arg1);

void SsSeqStop(short seq_access_num) { _SsSndStop(seq_access_num, 0); }

void SsSepStop(short sep_access_num, short arg1);
