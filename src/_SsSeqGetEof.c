// FUNC 8006faec 452 MAIN0
// MATCHING 8006faec 452
// HEADERS tomba24
// Portado de psx_tomba (../psyq/libsnd/midiread.c, _SsSeqGetEof); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "libsnd_i.h"

void _SsSndNextSep(s16, s16);
void _SsVmSeqKeyOff(s16);
s32 _SsReadDeltaValue(s16, s16);
void _SsSeqGetEof();

typedef struct {
    void (*noteon)();
    void (*programchange)();
    void (*pitchbend)();
    void (*metaevent)();
    void (*control[13])();
    void (*ccentry[20])();
} _SsFCALL;
extern _SsFCALL SsFCALL;

void _SsSeqPlay(s16 arg0, s16 arg1);

void _SsSeqGetEof(access_num, seq_num)
short access_num;
short seq_num;
{
    struct SeqStruct* score = &_ss_score[access_num][seq_num];

    score->unk48++;
    if (score->unk46 == 0) {
        score->unk80 = 0;
        score->unk27 = 0;
        score->delta_value = 0;
        score->read_pos = score->next_sep_pos;
    } else if (score->unk48 < score->unk46) {
        score->unk80 = 0;
        score->unk27 = 0;
        score->delta_value = 0;
        score->read_pos = score->next_sep_pos;
        score->loop_pos = score->next_sep_pos;
    } else {
        _ss_score[access_num][seq_num].unk90 &= ~1;
        _ss_score[access_num][seq_num].unk90 &= ~8;
        _ss_score[access_num][seq_num].unk90 &= ~2;
        _ss_score[access_num][seq_num].unk90 |= 0x200;
        _ss_score[access_num][seq_num].unk90 |= 4;
        score->unk2b = 0;
        score->loop_pos = score->next_sep_pos;
        if (score->unk3C != 0xFF) {
            score->unk2b = 0;
            _SsSndNextSep(score->unk3C, score->unk0);
            _SsVmSeqKeyOff(access_num | (seq_num << 8));
        }
        _SsVmSeqKeyOff(access_num | (seq_num << 8));
        score->delta_value = score->unk70;
    }
}

void _SsGetSeqData(s16 access_num, s16 seq_num);
