// FUNC 800724bc 236 MAIN0
// MATCHING 800724bc 236
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

static u16* D_80032F10 = (u16*)0x1F801C00;
static u16 D_80032F14[] = {
    0x1000, 0x100E, 0x101D, 0x102C, 0x103B, 0x104A, 0x1059, 0x1068, 0x1078,
    0x1087, 0x1096, 0x10A5, 0x10B5, 0x10C4, 0x10D4, 0x10E3, 0x10F3, 0x1103,
    0x1113, 0x1122, 0x1132, 0x1142, 0x1152, 0x1162, 0x1172, 0x1182, 0x1193,
    0x11A3, 0x11B3, 0x11C4, 0x11D4, 0x11E5, 0x11F5, 0x1206, 0x1216, 0x1227,
    0x1238, 0x1249, 0x125A, 0x126B, 0x127C, 0x128D, 0x129E, 0x12AF, 0x12C1,
    0x12D2, 0x12E3, 0x12F5, 0x1306, 0x1318, 0x132A, 0x133C, 0x134D, 0x135F,
    0x1371, 0x1383, 0x1395, 0x13A7, 0x13BA, 0x13CC, 0x13DE, 0x13F1, 0x1403,
    0x1416, 0x1428, 0x143B, 0x144E, 0x1460, 0x1473, 0x1486, 0x1499, 0x14AC,
    0x14BF, 0x14D3, 0x14E6, 0x14F9, 0x150D, 0x1520, 0x1534, 0x1547, 0x155B,
    0x156F, 0x1583, 0x1597, 0x15AB, 0x15BF, 0x15D3, 0x15E7, 0x15FB, 0x1610,
    0x1624, 0x1638, 0x164D, 0x1662, 0x1676, 0x168B, 0x16A0, 0x16B5, 0x16CA,
    0x16DF, 0x16F4, 0x170A, 0x171F, 0x1734, 0x174A, 0x175F, 0x1775, 0x178B,
    0x17A1, 0x17B6, 0x17CC, 0x17E2, 0x17F9, 0x180F, 0x1825, 0x183B, 0x1852,
    0x1868, 0x187F, 0x1896, 0x18AC, 0x18C3, 0x18DA, 0x18F1, 0x1908, 0x191F,
    0x1937, 0x194E, 0x1965, 0x197D, 0x1995, 0x19AC, 0x19C4, 0x19DC, 0x19F4,
    0x1A0C, 0x1A24, 0x1A3C, 0x1A55, 0x1A6D, 0x1A85, 0x1A9E, 0x1AB7, 0x1ACF,
    0x1AE8, 0x1B01, 0x1B1A, 0x1B33, 0x1B4C, 0x1B66, 0x1B7F, 0x1B98, 0x1BB2,
    0x1BCC, 0x1BE5, 0x1BFF, 0x1C19, 0x1C33, 0x1C4D, 0x1C67, 0x1C82, 0x1C9C,
    0x1CB7, 0x1CD1, 0x1CEC, 0x1D07, 0x1D22, 0x1D3D, 0x1D58, 0x1D73, 0x1D8E,
    0x1DA9, 0x1DC5, 0x1DE0, 0x1DFC, 0x1E18, 0x1E34, 0x1E50, 0x1E6C, 0x1E88,
    0x1EA4, 0x1EC1, 0x1EDD, 0x1EFA, 0x1F16, 0x1F33, 0x1F50, 0x1F6D, 0x1F8A,
    0x1FA7, 0x1FC5, 0x1FE2, 0x2000};

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

void vmNoiseOn(u8 arg0);

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
s32 _SsVmKeyOn(s16 arg0, s16 vabId, s16 prog, u16 note, u16 voll, u16 volr);
#endif

s32 SpuVmKeyOff(s16 arg0, s16 vabId, s16 prog, u16 note);

void _SsVmSeKeyOn(s16 vabId, s16 prog, u16 note, s32 arg3, u16 voll, u16 volr) {
    u16 var_v1;
    u16 var_a1;

    if (voll == volr) {
        var_v1 = 0x40;
        var_a1 = voll;
    } else if (volr < voll) {
        var_a1 = voll;
        var_v1 = (volr * 0x40) / voll;
    } else {
        var_a1 = volr;
        var_v1 = 0x7F - ((voll * 0x40) / volr);
    }
    _SsVmKeyOn(0x21, vabId, prog, note, var_a1, var_v1);
}

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
