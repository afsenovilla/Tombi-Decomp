// FUNC 8006c374 36 MAIN0
// MATCHING 8006c374 36
// Portado de psx_tomba (psyq/libsnd/ssclose.c, SsSepClose); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "libsnd_i.h"

void _SsClose(s16 seq_sep_num);

void SsSeqClose(short seq_access_num);

void SsSepClose(short sep_access_num) { _SsClose(sep_access_num); }

void SsEnd(void);
