// FUNC 80072914 252 MAIN0
// MATCHING 80072914 252
// Portado de psx_tomba (psyq/libsnd/vm_aloc1.c, note2pitch2); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"

extern unsigned long SpuSetNoiseVoice(long on_off, unsigned long voice_bit);
struct SpuVoice {
    s16 vag_idx;
    s16 unk2;
    s16 unk04;
    u16 key_stat;
    s16 voll1;
    char pan;
    char unkb;
    s16 note;
    s16 seq_sep_no;
    s16 fake_program;
    s16 prog;
    s16 tone;
    s16 vabId;
    s16 priority;
    u8 pad4[1];
    u8 unk1b;
    s16 auto_vol;
    s16 unk1e;
    s16 unk20;
    s16 unk22;
    s16 start_vol;
    s16 end_vol;
    s16 auto_pan;
    s16 unk2a;
    s16 unk2c;
    s16 unk2e;
    s16 start_pan;
    s16 end_pan;
};
extern struct SpuVoice _svm_voice[24];
extern char spuVmMaxVoice;

struct struct_svm {
    char prog_tones;
    char vabId;
    char note;
    char fine;
    char volume;
    char pan;
    char prog;
    char fake_program;
    char field_8_unknown;
    char field_0x9;
    char mvol;
    char mpan;
    char tone;
    char tone_vol;
    char tone_pan;
    char tone_prior;
    char tone_center;
    unsigned char tone_shift;
    char tone_min;
    char tone_max;
    u8 tone_mode;
    u8 pad;
                          short seq_sep_no;
    short tone_vag_idx;
    short voice;
    short voiceOffset;
    short field_0x1e;
};
extern struct struct_svm _svm_cur;
extern unsigned short pitch_table[];
typedef struct VagAtr {
    unsigned char prior;
    unsigned char mode;
    unsigned char vol;
    unsigned char pan;
    unsigned char center;
    unsigned char shift;
    unsigned char min;
    unsigned char max;
    unsigned char vibW;
    unsigned char vibT;
    unsigned char porW;
    unsigned char porT;
    unsigned char pbmin;
    unsigned char pbmax;
    unsigned char reserved1;
    unsigned char reserved2;
    unsigned short adsr1;
    unsigned short adsr2;
    short prog;
    short vag;
    short reserved[4];
} VagAtr;
extern VagAtr* _svm_tn;

char _SsVmAlloc(short voice);

unsigned short note2pitch(void);

unsigned short note2pitch2(unsigned short note, unsigned short fine) {
    VagAtr* tn;
    int octaveBase;
    int octave;
    short step;
    short shift;
    int semitones;
    int noteIndex;
    unsigned short pitch;
    int tone;

    tone = _svm_cur.tone + (_svm_cur.fake_program * 16);
    step = (fine + _svm_tn[tone].shift) / 8;
    octaveBase = 0;
    if (step >= 16) {
        octaveBase = 1;
        step -= 16;
    }
    semitones = (short)(octaveBase + (note + 60 - _svm_tn[tone].center));
    octave = semitones / 12;
    semitones = (short)(semitones - octave * 12);
    noteIndex = semitones * 16;
    pitch = pitch_table[noteIndex + step];
    shift = octave - 5;
    if (shift > 0) {
        pitch <<= shift;
    } else if (shift < 0) {
        pitch >>= -shift;
    }
    return pitch;
}
