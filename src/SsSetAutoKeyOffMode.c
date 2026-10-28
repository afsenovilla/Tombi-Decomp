// FUNC 800745a8 16 MAIN0
// MATCHING 800745a8 16
// HEADERS tomba24
// Portado de psx_tomba (../psyq/libsnd/vm_vsu.c, SsSetAutoKeyOffMode); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"

#define NUM_VAB 16
typedef struct VabHdr {
    long form;
    long ver;
    long id;
    unsigned long fsize;
    unsigned short reserved0;
    unsigned short ps;
    unsigned short ts;
    unsigned short vs;
    unsigned char mvol;
    unsigned char pan;
    unsigned char attr1;
    unsigned char attr2;
    unsigned long reserved1;
} VabHdr;
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
extern short kMaxPrograms;
extern struct struct_svm _svm_cur;
extern u8 _svm_vab_used[16];
extern VabHdr* _svm_vab_vh[16];
extern ProgAtr* _svm_vab_pg[16];
extern VagAtr* _svm_vab_tn[16];
extern VabHdr* _svm_vh;
extern ProgAtr* _svm_pg;
extern VagAtr* _svm_tn;

int _SsVmVSetUp(short vabId, short prog);

extern char _svm_auto_kof_mode;

void SsSetAutoKeyOffMode(short mode) {
    _svm_auto_kof_mode = mode;
}
