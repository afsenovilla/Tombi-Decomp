// FUNC 80073f60 1412 MAIN0
// Written after the algorithm of eternal-silver-sssc-decomp SLUS_006.28/src/libsnd_vm_vol.c (MIT, PSY-Q 4.5); PAL has no _snd_vmask test.
#define SKIP_ASM
#include "common.h"
#include "libsnd_i.h"

extern struct SpuVoice _svm_voice[];
extern short _svm_sreg_buf[];
extern unsigned char _svm_sreg_dirty[];

int _SsVmSetVol(short seq_sep_no, short vabId, short prog, unsigned short vol, unsigned short pan)
{
    struct SeqStruct *score;
    int count;
    short voice;
    int v;
    unsigned int vl, vr, l, r;
    unsigned char pv;

    count = 0;
    score = &_ss_score[seq_sep_no & 0xFF][(seq_sep_no & 0xFF00) >> 8];
    _SsVmVSetUp(vabId, prog);
    _svm_cur.field_16_vag_idx = seq_sep_no;
    if (pan == 0) pan = 1;
    if (vol == 0) vol = 1;
    for (voice = 0; voice < spuVmMaxVoice; voice++) {
        if (_svm_voice[voice].unke == seq_sep_no && _svm_voice[voice].prog == prog &&
            _svm_voice[voice].vabId == vabId) {
            if (score->vol[score->channel] != vol && score->vol[score->channel] == 0) {
                score->vol[score->channel] = 1;
            }
            v = _svm_voice[voice].unk8 * vol / 127;
            v = v * (_svm_vh->mvol * 0x3FFF) / 16129;
            vl = (unsigned int)(v * _svm_pg[prog].mvol *
                  _svm_tn[_svm_voice[voice].unk10 * 16 + _svm_voice[voice].tone].vol) / 16129;
            vr = vl * score->unk76 / 127;
            vl = vl * score->unk74 / 127;
            pv = _svm_tn[_svm_voice[voice].unk10 * 16 + _svm_voice[voice].tone].pan;
            if (pv < 64) {
                l = vl;
                r = vr * pv / 63;
            } else {
                l = vl * (127 - pv) / 63;
                r = vr;
            }
            pv = _svm_pg[_svm_voice[voice].prog].mpan;
            if (pv < 64) r = r * pv / 63; else l = l * (127 - pv) / 63;
            pv = pan;
            if (pv < 64) r = r * pv / 63; else l = l * (127 - pv) / 63;
            if (_svm_stereo_mono == 1) {
                if (l < r) l = r; else r = l;
            }
            l = l * l / 16383;
            r = r * r / 16383;
            _svm_sreg_buf[voice * 8 + 0] = l;
            _svm_sreg_buf[voice * 8 + 1] = r;
            _svm_sreg_dirty[voice] |= 3;
            count++;
        }
    }
    return count;
}
