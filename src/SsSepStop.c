// FUNC 80070294 44 MAIN0
// MATCHING 80070294 44
// Portado de psx_tomba (psyq/libsnd/ssstop.c, SsSepStop); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "libsnd_i.h"

void _SsSndStop(s16 arg0, s16 arg1);

void SsSeqStop(short seq_access_num);

void SsSepStop(short sep_access_num, short arg1) {
    _SsSndStop(sep_access_num, arg1);
}
