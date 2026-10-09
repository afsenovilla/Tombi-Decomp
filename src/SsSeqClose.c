// FUNC 8006c350 36 MAIN0
// MATCHING 8006c350 36
// Portado de psx_tomba (psyq/libsnd/ssclose.c, SsSeqClose); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "libsnd_i.h"

void _SsClose(s16 seq_sep_num);

void SsSeqClose(short seq_access_num) { _SsClose(seq_access_num); }

void SsSepClose(short sep_access_num);

void SsEnd(void);
