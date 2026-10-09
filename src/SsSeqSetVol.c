// FUNC 80070644 48 MAIN0
// MATCHING 80070644 48
// Portado de psx_tomba (psyq/libsnd/ssvol.c, SsSeqSetVol); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "libsnd_i.h"

void _SsSndSetVol(s32 arg0, s32 arg1, u16 arg2, u16 arg3);

void SsSeqSetVol(short arg0, short arg1, short arg2) {
    _SsVmSetSeqVol(arg0, arg1, arg2, 1);
}

void SsSepSetVol(s16 sep_access_num, s16 seq_num, s16 voll, s16 volr);

void SsSeqGetVol(s16 access_num, s16 seq_num, s16* voll, s16* volr);
