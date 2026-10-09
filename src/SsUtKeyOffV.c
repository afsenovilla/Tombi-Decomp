// FUNC 800710d8 120 MAIN0
// MATCHING 800710d8 120
// Portado de psx_tomba (psyq/libsnd/ut_keyv.c, SsUtKeyOffV); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"

#define NUM_VOICES 24
void _SsVmKeyOffNow(int mode);
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
extern s32 _snd_ev_flag;
extern struct struct_svm _svm_cur;
typedef struct ProgAtr {
    unsigned char tones;
    unsigned char mvol;
    unsigned char prior;
    unsigned char mode;
    unsigned char mpan;
    char reserved0;
    short attr;
    unsigned long reserved1;
    unsigned long reserved2;
} ProgAtr;
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
extern ProgAtr* _svm_pg;
extern VagAtr* _svm_tn;
void vmNoiseOn(char voice);
unsigned short note2pitch2(unsigned short note, unsigned short fine);
void _SsVmKeyOnNow(unsigned short vagCount, unsigned short pitch);
int _SsVmVSetUp(short vabId, short prog);
void _SsVmDoAllocate(void);

short SsUtKeyOnV(short voice, short vabId, short prog, short tone, short note, short fine, short voll, short volr);

short SsUtKeyOffV(short voice) {
    if (_snd_ev_flag == 1) {
        return -1;
    }
    _snd_ev_flag = 1;
    if (voice >= 0 && voice < NUM_VOICES) {
        _svm_cur.voice = voice;
        _SsVmKeyOffNow(0);
        _snd_ev_flag = 0;
        return 0;
    }
    _snd_ev_flag = 0;
    return -1;
}
