// FUNC 80072a10 1228 MAIN0
// MATCHING 80072a10 1228
// Ported from sotn-decomp src/main/psxsdk/libsnd/vmanager.c (MIT, (c) github.com/xeeynamo/sotn-decomp contributors), derived from Sony's PSY-Q libraries.
#define SKIP_ASM

#include "common.h"
#include "libsnd_i.h"

#ifdef VERSION_PC
extern s16 _svm_sreg_buf[];
extern char _svm_sreg_dirty[];
extern struct SpuVoice _svm_voice[];
extern s32 D_8003BD08;
extern s32 _svm_envx_hist[];
extern u32 _ss_spu_vm_rec[];
#else
static s16 _svm_sreg_buf[NUM_SPU_CHANNELS * 8];
static char _svm_sreg_dirty[NUM_SPU_CHANNELS];
static struct SpuVoice _svm_voice[NUM_SPU_CHANNELS];
static s32 D_8003BD08;
static s32 _svm_envx_hist[0x10];

static u32 _ss_spu_vm_rec[0x20];
#endif

extern u16* D_800982E8; /* port: VM_NO1.OBJ static pointer -> 0x1F801C00, PAL address from the retail lui/lw */
/* port: the pitch table static of vmanager.c is not used by vmNoiseOn and is defined elsewhere in PAL */

extern s16 _svm_vcf;
extern s16 _svm_orev1;
extern s16 _svm_orev2;
extern u8 _svm_auto_kof_mode;

inline u16 get_field_0x1a();

u8 SpuVmAlloc(s32 arg0);

#ifndef VERSION_PC
void SpuVmKeyOnNow(u8 vagCount, u16 arg2);
#endif

void SpuVmDoAllocate(void);

void vmNoiseOn(u8 arg0) {
    struct SeqStruct* score =
        &_ss_score[_svm_cur.field_16_vag_idx & 0xFF]
                  [(_svm_cur.field_16_vag_idx & 0xFF00) >> 8];
    s16 voice;
    u16 bitsUpper;
    u16 bitsLower;
    u32 voll_t, volr_t;
    u32 voll, volr;
    u16 cnd;
    u32 temp;

    cnd = D_800982E8[0x1AA / 2];

    voll_t = score->unk74 * 0x81;
    volr_t = score->unk76 * 0x81;

    voll_t = (voll_t * _svm_cur.field_A_mvol) / 0x7F;
    volr_t = (volr_t * _svm_cur.field_A_mvol) / 0x7F;

    voll_t = (voll_t * _svm_cur.field_D_vol) / 0x7F;
    volr_t = (volr_t * _svm_cur.field_D_vol) / 0x7F;

    temp = _svm_cur.field_E_pan;
    if (temp < 0x40) {
        voll = voll_t;
        volr = (volr_t * temp) / 0x3F;
    } else {
        voll = (voll_t * (0x7F - temp)) / 0x3F;
        volr = volr_t;
    }
    temp = _svm_cur.field_B_mpan;
    if (temp < 0x40) {
        volr = (volr * temp) / 0x3F;
    } else {
        voll = (voll * (0x7F - temp)) / 0x3F;
    }
    temp = _svm_cur.field_0x5;
    if (temp < 0x40) {
        volr = (temp * volr) / 0x3F;
    } else {
        voll = (voll * (0x7F - temp)) / 0x3F;
    }

    if (_svm_stereo_mono == 1) {
        if (voll < volr) {
            voll = volr;
        } else {
            volr = voll;
        }
    }

    cnd &= ~0x3F00;
    cnd |= ((_svm_cur.field_2_note - _svm_cur.field_10_centre) & 0x3F) << 8;
    D_800982E8[0x1AA / 2] = cnd;

    _svm_sreg_buf[arg0 * 8 + 0] = voll;
    _svm_sreg_buf[arg0 * 8 + 1] = volr;
    _svm_sreg_dirty[arg0] |= 3;
    if (arg0 < 0x10) {
        bitsLower = 1 << arg0;
        bitsUpper = 0;
    } else {
        bitsLower = 0;
        bitsUpper = 1 << (arg0 - 0x10);
    }
    _svm_voice[arg0].unk04 = 0xA;
    for (voice = 0; voice < spuVmMaxVoice; voice++) {
        _svm_voice[voice].unk1b &= 1;
    }
    _svm_voice[arg0].unk1b = 2;

    _svm_okon1 |= bitsLower;
    _svm_okon2 |= bitsUpper;

    _svm_okof1 &= ~_svm_okon1;
    _svm_okof2 &= ~_svm_okon2;

    if (_svm_cur.field_14_seq_sep_no & 4) {
        _svm_orev1 |= bitsLower;
        _svm_orev2 |= bitsUpper;
    } else {
        _svm_orev1 &= ~bitsLower;
        _svm_orev2 &= ~bitsUpper;
    }

    D_800982E8[0x194 / 2] = bitsLower;
    D_800982E8[0x196 / 2] = bitsUpper;
}

void vmNoiseOn2(u8 voice, u16 arg1, u16 arg2, u16 arg3, u16 arg4);

u16 note2pitch();

s32 note2pitch2(u16 arg0, u16 arg1);

void SePitchBend(u8 voice, s16 arg1);

inline u8 _SsVmSelectToneAndVag(u8* toneIdx, u8* vagIdx);

void SsUtVibrateOn(void);

void SsUtVibrateOff(void);

#define klabs(A) (((A) >= 0) ? (A) : -(A))

void SeAutoVol(s16 voice, s16 start_vol, s16 end_vol, s16 delta_time);

void SetAutoVol(s16 voice);

void SeAutoPan(s16 voice, s16 start_pan, s16 end_pan, s16 delta_time);

void SetAutoPan(s16 voice);

void SpuVmInit(u8 arg0);

void SpuVmNoiseOnWithAdsr(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void SpuVmNoiseOff(void);

void SpuVmNoiseOn(s32 arg0, s32 arg1);

s16 SpuVmPBVoice(
    s16 voice, s16 seq_sep_no, s16 vabId, s16 program, u16 amount);

s32 SpuVmPitchBend(s16 arg0, s16 arg1, s16 arg2, s16 arg3);

#ifndef VERSION_PC
void SpuVmFlush(void);
#endif

#ifndef VERSION_PC
s32 SpuVmKeyOn(s16 arg0, s16 vabId, s16 prog, u16 note, u16 voll, u16 volr);
#endif

s32 SpuVmKeyOff(s16 arg0, s16 vabId, s16 prog, u16 note);

void SpuVmSeKeyOn(s16 vabId, s16 prog, u16 note, s32 arg3, u16 voll, u16 volr);

void SpuVmSeKeyOff(s16 vabId, s16 prog, u16 note);

void KeyOnCheck(void);

s32 SpuVmSetSeqVol(s16 seq_sep_no, u16 voll, u16 volr, s16 arg3);

s32 SpuVmGetSeqVol(s16 arg0, s16* arg1, s16* arg2);

s16 SpuVmGetSeqLVol(s16 arg0);

s16 SpuVmGetSeqRVol(s16 arg0);

void SpuVmSeqKeyOff(s16 arg0);

s32 SpuVmSetProgVol(s16 vabId, s16 prog, u8 arg2);

s32 SpuVmGetProgVol(s16 vabId, s16 prog);

s32 SpuVmSetProgPan(s16 vabId, s16 prog, u8 pan);

s32 SpuVmGetProgPan(s16 vabId, s16 prog);

#ifndef VERSION_PC
s32 SpuVmSetVol(s16 seq_sep_no, s16 vabId, s16 prog, u16 arg3, u16 arg4);
#endif

s16 SsUtKeyOn(
    s16 vabId, s16 prog, s16 tone, s16 note, s16 fine, s16 voll, s16 volr);

short SsUtKeyOff(s16 voice, s16 vabId, s16 prog, s16 tone, s16 note);

s16 SsUtKeyOnV(s16 voice, s16 vabId, s16 prog, s16 tone, s16 note, s16 fine,
               s16 voll, s16 volr);

s32 SsUtKeyOffV(s16 voice);

s16 SsUtPitchBend(s16 voice, s16 vabId, s16 prog, s16 note, s16 pbend);

short SsUtChangePitch(short voice, short vabId, short prog, short old_note,
                      short old_fine, short new_note, short new_fine);

s16 SsUtChangeADSR(
    s16 voice, s16 vabId, s16 prog, s16 old_note, u16 adsr1, u16 adsr2);

s16 SsUtGetDetVVol(s16 voice, s16* voll, s16* volr);

s32 SsUtSetDetVVol(s16 voice, s16 arg1, s16 arg2);

s16 SsUtGetVVol(s16 voice, s16* voll, s16* volr);

s16 SsUtSetVVol(s16 voice, s16 voll, s16 volr);

s16 SsUtAutoVol(s16 voice, s16 start_vol, s16 end_vol, s16 delta_time);

s16 SsUtAutoPan(s16 voice, s16 start_pan, s16 end_pan, s16 delta_time);

void SsUtAllKeyOff(void);
