// FUNC 80072370 332 MAIN0
// MATCHING 80072370 332
// HEADERS tomba24
// Portado de psx_tomba (../psyq/libsnd/vm_key.c, _SsVmKeyOff); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "libsnd_i.h"

int _SsVmKeyOn(unsigned short seq_sep_no, short vabId, short prog, unsigned short note,
                unsigned short voll, unsigned short pan);
int _SsVmKeyOff(short seq_sep_no, short vabId, short prog, unsigned short note);
void _SsVmKeyOffNow(int mode);
void vmNoiseOff(u8 voice);
extern struct SpuVoice _svm_voice[24];

extern u8 _SsVmSelectToneAndVag(u8* tones, u8* vags);
extern u8 _SsVmAlloc(int mode);
extern void _SsVmDoAllocate(void);
extern void vmNoiseOn(u8 voice);
extern u16 note2pitch(void);
extern void _SsVmKeyOnNow(u8 count, u16 pitch);

int _SsVmKeyOn(unsigned short seq_sep_no, short vabId, short prog, unsigned short note, unsigned short voll, unsigned short pan);

int _SsVmKeyOff(short seq_sep_no, short vabId, short prog, unsigned short note) {
    u8 i;
    int count;

    count = 0;
    for (i = 0; i < spuVmMaxVoice; i++) {
        if (_svm_voice[i].note == note && _svm_voice[i].prog == prog &&
            _svm_voice[i].unke == seq_sep_no && _svm_voice[i].vabId == vabId) {
            if (_svm_voice[i].unk0 == 0xFF) {
                vmNoiseOff(i);
            } else {
                _svm_cur.field_0x1a = i;
                _SsVmKeyOffNow(0);
            }
            count++;
        }
    }
    return count;
}

void _SsVmSeKeyOn(short vabId, short prog, unsigned short note, unsigned short pitch, unsigned short voll, unsigned short volr);

void _SsVmSeKeyOff(short vabId, short prog, unsigned short note);

void KeyOnCheck(void);
