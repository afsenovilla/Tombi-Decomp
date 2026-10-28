// FUNC 8006fcb0 740 MAIN0
// MATCHING 8006fcb0 740
// HEADERS tomba24
// Portado de psx_tomba (../psyq/libsnd/midiread.c, _SsGetSeqData); licencia MIT del proyecto original.
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

void _SsSeqGetEof();

void _SsGetSeqData(s16 access_num, s16 seq_num) {
    struct SeqStruct* score = &_ss_score[access_num][seq_num];
    u8 data;
    u8 channel;
    u8 note;
    u8 velocity;

    data = *score->read_pos++;
    channel = data & 0xF;
    if (data & 0x80) {
        score->channel = channel;
        switch (data & 0xF0) {
        case 0x90:
            score->unk11 = 0x90;
            note = *score->read_pos++;
            velocity = *score->read_pos++;
            score->delta_value = _SsReadDeltaValue(access_num, seq_num);
            SsFCALL.noteon(access_num, seq_num, note, velocity);
            break;
        case 0xB0:
            score->unk11 = 0xB0;
            data = *score->read_pos++;
            SsFCALL.control[0](access_num, seq_num, data);
            break;
        case 0xC0:
            score->unk11 = 0xC0;
            data = *score->read_pos++;
            SsFCALL.programchange(access_num, seq_num, data);
            break;
        case 0xE0:
            score->unk11 = 0xE0;
            score->read_pos++;
            SsFCALL.pitchbend(access_num, seq_num);
            break;
        case 0xF0:
            score->unk11 = 0xFF;
            score->channel = channel;
            data = *score->read_pos++;
            if (data == 0x2F) {
                _SsSeqGetEof(access_num, seq_num, data);
            } else {
                SsFCALL.metaevent(access_num, seq_num, data);
            }
            break;
        }
    } else {
        switch (score->unk11) {
        case 0x90:
            velocity = *score->read_pos++;
            score->delta_value = _SsReadDeltaValue(access_num, seq_num);
            SsFCALL.noteon(access_num, seq_num, data, velocity);
            break;
        case 0xB0:
            SsFCALL.control[0](access_num, seq_num, data);
            break;
        case 0xC0:
            SsFCALL.programchange(access_num, seq_num, data);
            break;
        case 0xE0:
            SsFCALL.pitchbend(access_num, seq_num);
            break;
        case 0xFF:
            if (data == 0x2F) {
                _SsSeqGetEof(access_num, seq_num, data);
            } else {
                SsFCALL.metaevent(access_num, seq_num, data);
            }
            break;
        }
    }
}
