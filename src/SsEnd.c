// FUNC 8006c398 196 MAIN0
// MATCHING 8006c398 196
// Portado de psx_tomba (psyq/libsnd/ssclose.c, SsEnd); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "libsnd_i.h"

void _SsClose(s16 seq_sep_num);

void SsSeqClose(short seq_access_num);

void SsSepClose(short sep_access_num);

void SsEnd(void) {
    if (_snd_seq_tick_env.unk4 == 0) {
        _snd_seq_tick_env.unk17 = 0;
        if (_snd_seq_tick_env.unk18 != 0x7F) {
            EnterCriticalSection();
            if (_snd_seq_tick_env.unk16 != 0) {
                VSyncCallback(NULL);
                _snd_seq_tick_env.unk16 = 0;
            } else if (_snd_seq_tick_env.unk18 == 0) {
                InterruptCallback(0, _snd_seq_tick_env.unk12);
                _snd_seq_tick_env.unk12 = 0;
            } else {
                InterruptCallback(6, NULL);
            }
            ExitCriticalSection();
            _snd_seq_tick_env.unk18 = 0x7F;
        }
    }
}
