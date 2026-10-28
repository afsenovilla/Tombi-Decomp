// FUNC 800734d4 512 MAIN0
// MATCHING 800734d4 512
// Written after eternal-silver-sssc-decomp SLUS_006.28/src/libsnd_vm_pb.c (MIT, PSY-Q 4.5), derived from Sony's PSY-Q libraries.
// port: PAL libsnd (older vm_pb.o): the note/tone loads and the bend sign tests follow the retail order.
#define SKIP_ASM
#include "common.h"
#include "libsnd_i.h"

extern struct SpuVoice _svm_voice[];
extern short _svm_sreg_buf[];     /* D_8009F150: 8 shadow SPU registers per voice */
extern unsigned char _svm_sreg_dirty[]; /* D_8009C918 */
unsigned short note2pitch2(unsigned short note, unsigned short fine);

short _SsVmPBVoice(short voice, short seq_sep_no, short vabId, short prog, int pitch)
{
    short bend;
    unsigned short tone;
    unsigned short note;
    unsigned short fine;
    int fake;

    bend = pitch - 64;
    if (_svm_voice[voice].unke == seq_sep_no && _svm_voice[voice].vabId == vabId &&
        _svm_voice[voice].prog == prog) {
        fake = (u8)_svm_cur.field_7_fake_program;
        tone = _svm_voice[voice].tone;
        note = _svm_voice[voice].note;
        fake <<= 4;
        tone += fake;
        if (bend > 0) {
            note += bend * _svm_tn[tone].pbmax / 63;
            fine = (bend * _svm_tn[tone].pbmax % 63) * 2;
        } else if (bend < 0) {
            note = note + bend * _svm_tn[tone].pbmin / 64 - 1;
            fine = (bend * _svm_tn[tone].pbmin % 64) * 2 + 127;
        } else {
            fine = 0;
        }
        _svm_cur.field_C_vag_idx = _svm_voice[voice].tone;
        _svm_cur.field_18_voice_idx = voice;
        _svm_sreg_buf[voice * 8 + 2] = note2pitch2(note, fine);
        _svm_sreg_dirty[voice] |= 4;
        return 1;
    }
    return 0;
}
