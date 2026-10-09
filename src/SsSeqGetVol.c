// FUNC 800706ac 52 MAIN0
// MATCHING 800706ac 52
// Portado de psx_tomba (psyq/libsnd/ssvol.c, SsSeqGetVol); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "libsnd_i.h"

void _SsSndSetVol(s32 arg0, s32 arg1, u16 arg2, u16 arg3);

void SsSeqSetVol(short arg0, short arg1, short arg2);

void SsSepSetVol(s16 sep_access_num, s16 seq_num, s16 voll, s16 volr);

void SsSeqGetVol(s16 access_num, s16 seq_num, s16* voll, s16* volr) {
    _SsVmGetSeqVol(access_num | (seq_num << 8), voll, volr);
}
