// FUNC 8006c994 1044 MAIN0 (NOT matching: 143 words differ; frame 0xe0 vs 0xd8)
// Written after the algorithm of eternal-silver-sssc-decomp SLUS_006.28/src/libsnd_cc_6.c (MIT, PSY-Q 4.5); this PAL libsnd revision loops over all tones for NRPN msb 16 (added here), field offsets per this exe.
#define SKIP_ASM
#include "common.h"
#include "libsnd_i.h"

typedef struct { u8 pad[0x44]; void (*ccentry[20])(); } SsFCallEntry;
extern SsFCallEntry SsFCALL;

s32 _SsReadDeltaValue(s16, s16);
s16 SsUtSetVagAtr(s16, s16, s16, VagAtr*);

void _SsContDataEntry(s16 seq_no, s16 sep_no, u8 data)
{
    struct SeqStruct *score;
    ProgAtr progAtr;
    VagAtr vagAtr;
    u8 channel;
    s32 i;
    s16 tune;

    score = &_ss_score[seq_no][sep_no];
    channel = score->channel;
    SsUtGetProgAtr(score->unk4c, score->programs[channel], &progAtr);
    if (score->unk27 == 1 && score->unk10 == 0) {
        score->unk28 = data;
        score->unk27 = 0;
        score->unk10 = 1;
        score->delta_value = _SsReadDeltaValue(seq_no, sep_no);
    } else if (score->unk29 == 2) {
        if (score->unk14 == 0) {
            for (i = 0; i < progAtr.tones; i++) {
                SsUtGetVagAtr(score->unk4c, score->programs[channel], i, &vagAtr);
                switch (score->unk13) {
                case 0:
                    vagAtr.pbmin = vagAtr.pbmax = data & 0x7F;
                    break;
                case 1:
                    if (data < 0x40) {
                        tune = (data * 100 / 8192) * 8192;
                    }
                    if (data > 0x40 && data < 0x80) {
                        tune = (data * 100 / 8192) * 8192;
                    } else {
                        tune = 0;
                    }
                    vagAtr.shift += tune;
                    break;
                case 2:
                    if (data < 0x40) {
                        tune = data * 6400;
                    }
                    if (data >= 0x40 && data < 0x80) {
                        tune = data * 6400;
                    } else {
                        tune = 0;
                    }
                    vagAtr.center += tune;
                    break;
                }
                SsUtSetVagAtr(score->unk4c, score->programs[channel], i, &vagAtr);
            }
        }
        score->delta_value = _SsReadDeltaValue(seq_no, sep_no);
        score->unk29 = 0;
    } else if (score->unk2a == 2) {
        if (score->unk16 == 16) {
            for (i = 0; i < progAtr.tones; i++) {
                SsFCALL.ccentry[score->unk15](score->unk4c, score->programs[channel], i, vagAtr, score->unk15, data);
            }
        } else {
            SsFCALL.ccentry[score->unk15](score->unk4c, score->programs[channel], score->unk16, vagAtr, score->unk15, data);
        }
        score->delta_value = _SsReadDeltaValue(seq_no, sep_no);
        score->unk2a = 0;
    } else {
        score->delta_value = _SsReadDeltaValue(seq_no, sep_no);
    }
}
