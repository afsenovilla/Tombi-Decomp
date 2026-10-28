/* Sound (docs/PORT.md "Audio"): a software SPU and a re-implementation of the libsnd layer the game uses.
 *
 * SPU: 512 KB sound RAM, 24 voices (SPU ADPCM blocks with their filter / loop flags, a 4.12 pitch counter with
 * linear interpolation, the hardware ADSR envelope with its linear and exponential rate rules, left/right voice
 * volume), the main volume, the CD input (XA audio of the movies, port_sound_xa_sector) with its own volume, and
 * the hardware reverb (the SPU's own algorithm at 22.05 kHz on a work buffer, with Sony's preset tables as the
 * game sets them through SpuSetReverbModeParam / Depth / Voice).  No noise generator or pitch modulation (the
 * game's banks do not use them).
 *
 * libsnd is replaced at its API with the real library's arithmetic: SsVabFakeHead keeps the VAB header of a
 * bank whose body the game wrote to sound RAM; SsSeqOpen/Play run a Sony SEQ (pQES: MIDI events with running
 * status, NRPN loop points CC99 = 20 / 30 with their CC6 count, tempo and end-of-track meta events) on the
 * voices below SsSetReservedVoice; SsUtKeyOnV/KeyOffV play sound effects on the voice the game names.  The
 * sequencer tick is the console's: SoundInit sets SsSetTickMode(SS_TICK60), a 60 Hz root-counter interrupt on a
 * PAL console (VSync on NTSC), and every tick advances a sequence by `step` tenths of a MIDI tick where
 * step = round(resolution * bpm * 10 / 3600) and bpm = round(60e6 / tempo), exactly as _SsInitSoundSeq / _SsSeqPlay
 * do, so the port's tempo (and its rounding) is the console's.  Voice gain is libsnd's: velocity x CC7, the VAB /
 * program / tone volumes, the sequence volume, the three pans, then squared (_SsVmKeyOnNow), re-derived for the
 * sounding notes when SsSeqSetVol changes (the game's fades).
 *
 * Clock: audio time is the sample count, never the wall clock.  With an audio device (runtime/sdl.c) the mixer is
 * pulled by the device's callback (port_sound_pull): the sequencer ticks every 735 samples the device consumes,
 * so a slow or stalled game thread cannot change the tempo and nothing is ever dropped or skipped; the game
 * thread's calls into this file take the device lock (port_audio_lock).  Without a device (headless, --dump-audio,
 * a failed device) port_sound_vblank mixes 44100/hz samples per vblank, which makes the dumps deterministic. */
#include <stdio.h>
#include <string.h>
#include "port_runtime.h"

typedef unsigned char u8; typedef unsigned short u16; typedef short s16;
typedef unsigned int u32; typedef int s32;

#define LOCK() port_audio_lock()
#define UNLOCK() port_audio_unlock()

/* ----------------------------------------------------------------------------------------------- SPU */
#define SPU_RAM_SIZE 0x80000
#define NVOICES 24
#define RATE 44100
#define TICK_HZ 60                          /* the libsnd tick (SsSetTickMode(SS_TICK60) in SoundInit) */
#define TICK_SAMPLES (RATE / TICK_HZ)       /* 735 */
static u8 spu_ram[SPU_RAM_SIZE];
static u32 spu_xfer_addr;
static int main_l = 0x3FFF, main_r = 0x3FFF;    /* SPU main volume (SpuSetCommonAttr), 0..0x3FFF */
static int cd_l = 0x7FFF, cd_r = 0x7FFF;        /* CD input volume, 0..0x7FFF */
static int cd_muted;                            /* CdlMute */
extern short _svm_stereo_mono;                  /* libsnd's SsSetMono / SsSetStereo (matched C, in the data image) */
#define stereo_mono (_svm_stereo_mono == 1)

enum { ENV_OFF, ENV_ATTACK, ENV_DECAY, ENV_SUSTAIN, ENV_RELEASE };
typedef struct {
    u32 start, loop, cur;            /* byte addresses in sound RAM */
    u32 counter;                     /* 4.12 position inside the current 28 sample block */
    u16 pitch;
    s16 buf[28];
    s16 prev;                        /* last sample of the previous block (interpolation) */
    int s1, s2;                      /* ADPCM history */
    int vol_l, vol_r;                /* SPU voice volume, 0..0x3FFF */
    u16 adsr1, adsr2;
    int env_phase, env_level, env_wait;
    int owner;                       /* -1 free, 0..: sequence slot, 100: sound effect (game named voice) */
    int ch, key;                     /* sequence channel and key of the note */
    int prior;                       /* tone priority (libsnd allocation) */
    unsigned age;
    /* the libsnd gain inputs, kept so SsSeqSetVol can re-derive the volume of a sounding note */
    int g_vol, g_pan, g_mvol, g_pvol, g_ppan, g_tvol, g_tpan;
} Voice;
static Voice voices[NVOICES];
static unsigned voice_clock;

static const int adpcm_f0[5] = { 0, 60, 115, 98, 122 }, adpcm_f1[5] = { 0, 0, -52, -55, -60 };

static void decode_block(Voice *v) {
    const u8 *b = spu_ram + (v->cur & (SPU_RAM_SIZE - 16));
    int shift = b[0] & 15, f = (b[0] >> 4) & 7;
    if (shift > 12) shift = 9;
    if (f > 4) f = 4;
    for (int i = 0; i < 28; i++) {
        int nib = (b[2 + i / 2] >> ((i & 1) * 4)) & 15;
        int s = (s16)(nib << 12) >> shift;
        s += (v->s1 * adpcm_f0[f] + v->s2 * adpcm_f1[f] + 32) >> 6;
        if (s > 32767) s = 32767;
        if (s < -32768) s = -32768;
        v->buf[i] = (s16)s;
        v->s2 = v->s1; v->s1 = s;
    }
    if (b[1] & 4) v->loop = v->cur;   /* loop start flag: the repeat address */
}

/* one envelope step for a phase: exp / decreasing / shift / step per the SPU's rate rules */
static void env_tick(Voice *v, int exp, int dec, int shift, int step) {
    if (v->env_wait > 0) { v->env_wait--; return; }
    int s = dec ? -8 + step : 7 - step;
    int wait = 1 << (shift > 11 ? shift - 11 : 0);
    s <<= (shift < 11 ? 11 - shift : 0);
    if (exp && !dec && v->env_level > 0x6000) wait <<= 2;
    if (exp && dec) s = (s * v->env_level) >> 15;
    v->env_wait = wait - 1;
    v->env_level += s;
    if (v->env_level > 0x7FFF) v->env_level = 0x7FFF;
    if (v->env_level < 0) v->env_level = 0;
}
static void env_step(Voice *v) {
    u16 a1 = v->adsr1, a2 = v->adsr2;
    switch (v->env_phase) {
    case ENV_ATTACK:
        env_tick(v, a1 >> 15, 0, (a1 >> 10) & 31, (a1 >> 8) & 3);
        if (v->env_level >= 0x7FFF) { v->env_phase = ENV_DECAY; v->env_wait = 0; }
        break;
    case ENV_DECAY:
        env_tick(v, 1, 1, (a1 >> 4) & 15, 0);
        if (v->env_level <= (int)(((a1 & 15) + 1) << 11)) { v->env_phase = ENV_SUSTAIN; v->env_wait = 0; }
        break;
    case ENV_SUSTAIN:
        env_tick(v, a2 >> 15, (a2 >> 14) & 1, (a2 >> 8) & 31, (a2 >> 6) & 3);
        break;
    case ENV_RELEASE:
        env_tick(v, (a2 >> 5) & 1, 1, a2 & 31, 0);
        if (v->env_level == 0) { v->env_phase = ENV_OFF; v->owner = -1; }
        break;
    }
}

static void voice_key_on(Voice *v, u32 addr, u16 pitch, u16 adsr1, u16 adsr2, int vl, int vr) {
    v->start = v->cur = addr & (SPU_RAM_SIZE - 8);
    v->loop = v->start;
    v->counter = 0; v->s1 = v->s2 = 0; v->prev = 0;
    v->pitch = pitch > 0x3FFF ? 0x3FFF : pitch;
    v->adsr1 = adsr1; v->adsr2 = adsr2;
    v->env_phase = ENV_ATTACK; v->env_level = 0; v->env_wait = 0;
    v->vol_l = vl; v->vol_r = vr;
    v->age = ++voice_clock;
    decode_block(v);
}
static void voice_key_off(Voice *v) {
    if (v->env_phase != ENV_OFF) { v->env_phase = ENV_RELEASE; v->env_wait = 0; }
}

/* ---------------------------------------------------------------------------------------- reverb */
/* The SPU reverb (the hardware's algorithm, run every other sample on a work buffer).  The registers are Sony's
   preset table (libspu's rev_param table in MAIN0: 10 modes, the same values the SPU documentation lists) as
   SpuSetReverbModeParam copies them, with the ECHO / DELAY delay and feedback rewrites of that function. */
enum { R_dAPF1, R_dAPF2, R_vIIR, R_vCOMB1, R_vCOMB2, R_vCOMB3, R_vCOMB4, R_vWALL, R_vAPF1, R_vAPF2, R_mLSAME, R_mRSAME,
       R_mLCOMB1, R_mRCOMB1, R_mLCOMB2, R_mRCOMB2, R_dLSAME, R_dRSAME, R_mLDIFF, R_mRDIFF, R_mLCOMB3, R_mRCOMB3,
       R_mLCOMB4, R_mRCOMB4, R_dLDIFF, R_dRDIFF, R_mLAPF1, R_mRAPF1, R_mLAPF2, R_mRAPF2, R_vLIN, R_vRIN };
static const u16 rev_preset[10][32] = {
    { 0 },
    { 0x007D, 0x005B, 0x6D80, 0x54B8, 0xBED0, 0x0000, 0x0000, 0xBA80, 0x5800, 0x5300, 0x04D6, 0x0333, 0x03F0, 0x0227, 0x0374, 0x01EF,
      0x0334, 0x01B5, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x01B4, 0x0136, 0x00B8, 0x005C, 0x8000, 0x8000 },
    { 0x0033, 0x0025, 0x70F0, 0x4FA8, 0xBCE0, 0x4410, 0xC0F0, 0x9C00, 0x5280, 0x4EC0, 0x03E4, 0x031B, 0x03A4, 0x02AF, 0x0372, 0x0266,
      0x031C, 0x025D, 0x025C, 0x018E, 0x022F, 0x0135, 0x01D2, 0x00B7, 0x018F, 0x00B5, 0x00B4, 0x0080, 0x004C, 0x0026, 0x8000, 0x8000 },
    { 0x00B1, 0x007F, 0x70F0, 0x4FA8, 0xBCE0, 0x4510, 0xBEF0, 0xB4C0, 0x5280, 0x4EC0, 0x0904, 0x076B, 0x0824, 0x065F, 0x07A2, 0x0616,
      0x076C, 0x05ED, 0x05EC, 0x042E, 0x050F, 0x0305, 0x0462, 0x02B7, 0x042F, 0x0265, 0x0264, 0x01B2, 0x0100, 0x0080, 0x8000, 0x8000 },
    { 0x00E3, 0x00A9, 0x6F60, 0x4FA8, 0xBCE0, 0x4510, 0xBEF0, 0xA680, 0x5680, 0x52C0, 0x0DFB, 0x0B58, 0x0D09, 0x0A3C, 0x0BD9, 0x0973,
      0x0B59, 0x08DA, 0x08D9, 0x05E9, 0x07EC, 0x04B0, 0x06EF, 0x03D2, 0x05EA, 0x031D, 0x031C, 0x0238, 0x0154, 0x00AA, 0x8000, 0x8000 },
    { 0x01A5, 0x0139, 0x6000, 0x5000, 0x4C00, 0xB800, 0xBC00, 0xC000, 0x6000, 0x5C00, 0x15BA, 0x11BB, 0x14C2, 0x10BD, 0x11BC, 0x0DC1,
      0x11C0, 0x0DC3, 0x0DC0, 0x09C1, 0x0BC4, 0x07C1, 0x0A00, 0x06CD, 0x09C2, 0x05C1, 0x05C0, 0x041A, 0x0274, 0x013A, 0x8000, 0x8000 },
    { 0x033D, 0x0231, 0x7E00, 0x5000, 0xB400, 0xB000, 0x4C00, 0xB000, 0x6000, 0x5400, 0x1ED6, 0x1A31, 0x1D14, 0x183B, 0x1BC2, 0x16B2,
      0x1A32, 0x15EF, 0x15EE, 0x1055, 0x1334, 0x0F2D, 0x11F6, 0x0C5D, 0x1056, 0x0AE1, 0x0AE0, 0x07A2, 0x0464, 0x0232, 0x8000, 0x8000 },
    { 0x0001, 0x0001, 0x7FFF, 0x7FFF, 0x0000, 0x0000, 0x0000, 0x8100, 0x0000, 0x0000, 0x1FFF, 0x0FFF, 0x1005, 0x0005, 0x0000, 0x0000,
      0x1005, 0x0005, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1004, 0x1002, 0x0004, 0x0002, 0x8000, 0x8000 },
    { 0x0001, 0x0001, 0x7FFF, 0x7FFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1FFF, 0x0FFF, 0x1005, 0x0005, 0x0000, 0x0000,
      0x1005, 0x0005, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1004, 0x1002, 0x0004, 0x0002, 0x8000, 0x8000 },
    { 0x0017, 0x0013, 0x70F0, 0x4FA8, 0xBCE0, 0x4510, 0xBEF0, 0x8500, 0x5F80, 0x54C0, 0x0371, 0x02AF, 0x02E5, 0x01DF, 0x02B0, 0x01D7,
      0x0358, 0x026A, 0x01D6, 0x011E, 0x012D, 0x00B1, 0x011F, 0x0059, 0x01A0, 0x00E3, 0x0058, 0x0040, 0x0028, 0x0014, 0x8000, 0x8000 },
};
/* work area of each mode in bytes (libspu's _spu_rev_startaddr table: 0x80000 - start * 8) */
static const u32 rev_size[10] = { 0x10, 0x26C0, 0x1F40, 0x4840, 0x6FE0, 0xADE0, 0xF6C0, 0x18040, 0x18040, 0x3C00 };
#define REV_MAX 0x18040
static s16 rev_buf[REV_MAX / 2];
static u16 rev[32];                   /* the reverb registers of the current mode */
static int rev_mode;
static u32 rev_bytes = 0x10, rev_pos;
static int rev_on;                    /* SpuSetReverb */
static int rev_vol_l, rev_vol_r;      /* reverb output volume (SpuSetReverbDepth) */
static u32 rev_voices;                /* SpuSetReverbVoice */
static int rev_delay, rev_feedback;
static int rev_in_l, rev_in_r, rev_cnt, rev_out_l, rev_out_r, rev_prev_l, rev_prev_r;

static int rev_rd(int units, int back) {          /* sample at the register address (8-byte units), `back` samples earlier */
    long long a = ((long long)rev_pos + (long long)units * 8 - (long long)back * 2) % (long long)rev_bytes;
    if (a < 0) a += rev_bytes;
    return rev_buf[a >> 1];
}
static void rev_wr(int units, int val) {
    long long a = ((long long)rev_pos + (long long)units * 8) % (long long)rev_bytes;
    if (a < 0) a += rev_bytes;
    rev_buf[a >> 1] = val > 32767 ? 32767 : val < -32768 ? -32768 : val;
}
#define RV(i) ((int)(s16)rev[i])
#define RA(i) ((int)rev[i])
static int clamp16(long long v) { return v > 32767 ? 32767 : v < -32768 ? -32768 : (int)v; }
static void rev_step(int in_l, int in_r) {          /* one 22.05 kHz reverb cycle */
    int Lin = (RV(R_vLIN) * in_l) >> 15, Rin = (RV(R_vRIN) * in_r) >> 15;
    rev_wr(RA(R_mLSAME), (((Lin + ((rev_rd(RA(R_dLSAME), 0) * RV(R_vWALL)) >> 15) - rev_rd(RA(R_mLSAME), 1)) * RV(R_vIIR)) >> 15) + rev_rd(RA(R_mLSAME), 1));
    rev_wr(RA(R_mRSAME), (((Rin + ((rev_rd(RA(R_dRSAME), 0) * RV(R_vWALL)) >> 15) - rev_rd(RA(R_mRSAME), 1)) * RV(R_vIIR)) >> 15) + rev_rd(RA(R_mRSAME), 1));
    rev_wr(RA(R_mLDIFF), (((Lin + ((rev_rd(RA(R_dRDIFF), 0) * RV(R_vWALL)) >> 15) - rev_rd(RA(R_mLDIFF), 1)) * RV(R_vIIR)) >> 15) + rev_rd(RA(R_mLDIFF), 1));
    rev_wr(RA(R_mRDIFF), (((Rin + ((rev_rd(RA(R_dLDIFF), 0) * RV(R_vWALL)) >> 15) - rev_rd(RA(R_mRDIFF), 1)) * RV(R_vIIR)) >> 15) + rev_rd(RA(R_mRDIFF), 1));
    long long L = ((long long)RV(R_vCOMB1) * rev_rd(RA(R_mLCOMB1), 0) + (long long)RV(R_vCOMB2) * rev_rd(RA(R_mLCOMB2), 0) +
                   (long long)RV(R_vCOMB3) * rev_rd(RA(R_mLCOMB3), 0) + (long long)RV(R_vCOMB4) * rev_rd(RA(R_mLCOMB4), 0)) >> 15;
    long long R = ((long long)RV(R_vCOMB1) * rev_rd(RA(R_mRCOMB1), 0) + (long long)RV(R_vCOMB2) * rev_rd(RA(R_mRCOMB2), 0) +
                   (long long)RV(R_vCOMB3) * rev_rd(RA(R_mRCOMB3), 0) + (long long)RV(R_vCOMB4) * rev_rd(RA(R_mRCOMB4), 0)) >> 15;
    int Lout = clamp16(L), Rout = clamp16(R), t;
    t = rev_rd(RA(R_mLAPF1) - RA(R_dAPF1), 0); Lout -= (RV(R_vAPF1) * t) >> 15; rev_wr(RA(R_mLAPF1), Lout); Lout = clamp16(((long long)Lout * RV(R_vAPF1) >> 15) + t);
    t = rev_rd(RA(R_mRAPF1) - RA(R_dAPF1), 0); Rout -= (RV(R_vAPF1) * t) >> 15; rev_wr(RA(R_mRAPF1), Rout); Rout = clamp16(((long long)Rout * RV(R_vAPF1) >> 15) + t);
    t = rev_rd(RA(R_mLAPF2) - RA(R_dAPF2), 0); Lout -= (RV(R_vAPF2) * t) >> 15; rev_wr(RA(R_mLAPF2), Lout); Lout = clamp16(((long long)Lout * RV(R_vAPF2) >> 15) + t);
    t = rev_rd(RA(R_mRAPF2) - RA(R_dAPF2), 0); Rout -= (RV(R_vAPF2) * t) >> 15; rev_wr(RA(R_mRAPF2), Rout); Rout = clamp16(((long long)Rout * RV(R_vAPF2) >> 15) + t);
    rev_out_l = clamp16((long long)Lout * rev_vol_l >> 15);
    rev_out_r = clamp16((long long)Rout * rev_vol_r >> 15);
    rev_pos += 2;
    if (rev_pos >= rev_bytes) rev_pos = 0;
}
static void rev_set_mode(int mode, int clear) {
    if (mode < 0 || mode > 9) return;
    rev_mode = mode;
    memcpy(rev, rev_preset[mode], sizeof rev);
    rev_bytes = rev_size[mode];
    if (mode == 7 || mode == 8) {                   /* ECHO / DELAY: SpuSetReverbModeParam derives the delays and the wall */
        int d = rev_delay;
        rev[R_mLSAME] = ((d << 13) / 0x7F) - RA(R_dAPF1);
        rev[R_mRSAME] = ((d << 12) / 0x7F) - RA(R_dAPF2);
        rev[R_mLCOMB1] = ((d << 12) / 0x7F) + RA(R_mRCOMB1);
        rev[R_dLSAME] = ((d << 12) / 0x7F) + RA(R_dRSAME);
        rev[R_mLAPF1] = ((d << 12) / 0x7F) + RA(R_mLAPF2);
        rev[R_mRAPF1] = ((d << 12) / 0x7F) + RA(R_mRAPF2);
        rev[R_vWALL] = (rev_feedback * 0x8100) / 0x7F;
    }
    if (clear) { memset(rev_buf, 0, sizeof rev_buf); rev_pos = 0; rev_in_l = rev_in_r = rev_cnt = rev_out_l = rev_out_r = rev_prev_l = rev_prev_r = 0; }
    if (port_verbose) port_printf("[port] sound: reverb mode %d (%u byte work area)%s\n", mode, rev_bytes, clear ? ", cleared" : "");
}

/* --------------------------------------------------------------------------------- XA (CD audio) */
/* CD-XA ADPCM of the movie streams: every audio sector (Mode 2 Form 2, submode bit 2) carries 18 groups of 128
   bytes; a group is 16 bytes of sound parameters (byte 4 + u for unit u: filter in bits 4-5, shift in bits 0-3)
   and 112 bytes holding 8 units of 28 4-bit samples (unit u: nibble u & 1 of byte u >> 1 of each 4-byte word);
   stereo alternates left / right units.  psyq.c hands the sectors over as the stream ring reads them (the drive's
   pace), they are decoded into a FIFO at the stream's rate (37.8 or 18.9 kHz), and the mixer resamples the FIFO
   to 44.1 kHz through the CD volume.  Playback starts once two sectors are queued (about 0.1 s of cushion for the
   ring's per-vblank delivery) and falls silent, then re-primes, if the FIFO runs dry. */
#define XA_FIFO 32768
static s16 xa_fifo[XA_FIFO][2];
static unsigned xa_w, xa_r;
static int xa_rate = 37800, xa_prime = 4032, xa_primed;
static int xa_s1[2], xa_s2[2];
static u32 xa_phase;
static int xa_sectors, xa_underruns, xa_8bit_warned, xa_ended;
static unsigned long long samples_total;        /* samples rendered since the start (the audio clock) */
static const int xa_f0[4] = { 0, 60, 115, 98 }, xa_f1[4] = { 0, 0, -52, -55 };

static void xa_push(int l, int r) {
    if (xa_w - xa_r >= XA_FIFO) xa_r++;             /* full (the mixer is not consuming): drop the oldest */
    xa_fifo[xa_w % XA_FIFO][0] = l; xa_fifo[xa_w % XA_FIFO][1] = r;
    xa_w++;
}
static void xa_unit(const u8 *grp, int u, int ch, s16 *out) {
    int h = grp[4 + u], shift = h & 15, f = (h >> 4) & 3;
    if (shift > 12) shift = 9;
    int s1 = xa_s1[ch], s2 = xa_s2[ch];
    for (int k = 0; k < 28; k++) {
        int b = grp[16 + k * 4 + (u >> 1)];
        int nib = (u & 1) ? b >> 4 : b & 15;
        int s = (s16)(nib << 12) >> shift;
        s += (s1 * xa_f0[f] + s2 * xa_f1[f] + 32) >> 6;
        if (s > 32767) s = 32767;
        if (s < -32768) s = -32768;
        out[k] = s; s2 = s1; s1 = s;
    }
    xa_s1[ch] = s1; xa_s2[ch] = s2;
}
static void xa_sector(int coding, const u8 *payload) {
    if (coding & 0x30) {
        if (!xa_8bit_warned++) port_printf("[port] sound: 8-bit XA audio (coding %02x) is not supported\n", coding);
        return;
    }
    int stereo = coding & 1;
    xa_rate = (coding & 4) ? 18900 : 37800;
    xa_prime = stereo ? 4032 : 8064;
    s16 a[28], b[28];
    for (int g = 0; g < 18; g++) {
        const u8 *grp = payload + g * 128;
        if (stereo)
            for (int u = 0; u < 8; u += 2) {
                xa_unit(grp, u, 0, a); xa_unit(grp, u + 1, 1, b);
                for (int k = 0; k < 28; k++) xa_push(a[k], b[k]);
            }
        else
            for (int u = 0; u < 8; u++) {
                xa_unit(grp, u, 0, a);
                for (int k = 0; k < 28; k++) xa_push(a[k], a[k]);
            }
    }
    xa_sectors++; xa_ended = 0;
}
static void xa_reset(void) {
    xa_w = xa_r = 0; xa_primed = 0; xa_phase = 0; xa_ended = 0;
    xa_s1[0] = xa_s1[1] = xa_s2[0] = xa_s2[1] = 0;
}
/* the next 44.1 kHz frame of CD audio (0 when nothing is queued) */
static void xa_next(int *l, int *r) {
    unsigned avail = xa_w - xa_r;
    *l = *r = 0;
    if (!xa_primed) { if (avail < (unsigned)xa_prime) return; xa_primed = 1; }
    if (avail == 0) {
        xa_primed = 0;
        if (!xa_ended) { xa_underruns++; if (port_verbose) port_printf("[port] sound: XA underrun at sample %llu (frame %d)\n", samples_total, port_frame); }
        return;
    }
    const s16 *c = xa_fifo[xa_r % XA_FIFO], *n = avail > 1 ? xa_fifo[(xa_r + 1) % XA_FIFO] : c;
    *l = c[0] + (int)(((long long)(n[0] - c[0]) * xa_phase) / RATE);
    *r = c[1] + (int)(((long long)(n[1] - c[1]) * xa_phase) / RATE);
    xa_phase += xa_rate;
    while (xa_phase >= RATE) { xa_phase -= RATE; if (xa_w - xa_r) xa_r++; }
}
void port_sound_xa_sector(int coding, const unsigned char *payload) { LOCK(); xa_sector(coding, payload); UNLOCK(); }
void port_sound_xa_reset(void) { LOCK(); xa_reset(); UNLOCK(); }
void port_sound_xa_end(void) { xa_ended = 1; }       /* the stream stopped: what is queued plays out */
void port_sound_cd_mute(int mute) { cd_muted = mute; }
int port_sound_xa_stats(int *underruns) { *underruns = xa_underruns; return xa_sectors; }

/* ------------------------------------------------------------------------------------------- mixer */
static void mix(s16 *out, int n) {
    for (int i = 0; i < n; i++) {
        int l = 0, r = 0, wl = 0, wr = 0;
        for (int k = 0; k < NVOICES; k++) {
            Voice *v = &voices[k];
            if (v->env_phase == ENV_OFF) continue;
            int idx = v->counter >> 12, fr = v->counter & 0xFFF;
            int a = idx == 0 ? v->prev : v->buf[idx - 1], b = v->buf[idx];
            int s = a + (((b - a) * fr) >> 12);
            s = (s * v->env_level) >> 15;
            int sl = (s * v->vol_l) >> 14, sr = (s * v->vol_r) >> 14;
            l += sl; r += sr;
            if (rev_voices & (1u << k)) { wl += sl; wr += sr; }
            env_step(v);
            v->counter += v->pitch;
            while (v->counter >= 28 << 12 && v->env_phase != ENV_OFF) {
                u8 fl = spu_ram[(v->cur & (SPU_RAM_SIZE - 16)) + 1];
                v->counter -= 28 << 12;
                v->prev = v->buf[27];
                if (fl & 1) {                         /* loop end: jump to the loop address, or stop */
                    v->cur = v->loop;
                    if (!(fl & 2)) { v->env_phase = ENV_OFF; v->env_level = 0; v->owner = -1; break; }
                } else v->cur = (v->cur + 16) & (SPU_RAM_SIZE - 1);
                decode_block(v);
            }
        }
        if (xa_w != xa_r || xa_primed) {
            int cl, cr;
            xa_next(&cl, &cr);
            if (!cd_muted) { l += (cl * cd_l) >> 15; r += (cr * cd_r) >> 15; }
        }
        if (rev_on) {
            rev_in_l += wl; rev_in_r += wr;
            if (++rev_cnt == 2) {
                rev_prev_l = rev_out_l; rev_prev_r = rev_out_r;
                rev_step(clamp16(rev_in_l / 2), clamp16(rev_in_r / 2));
                rev_in_l = rev_in_r = 0; rev_cnt = 0;
                l += rev_out_l; r += rev_out_r;
            } else { l += (rev_prev_l + rev_out_l) >> 1; r += (rev_prev_r + rev_out_r) >> 1; }
        }
        l = (l * main_l) >> 14; r = (r * main_r) >> 14;
        out[2 * i] = l > 32767 ? 32767 : l < -32768 ? -32768 : l;
        out[2 * i + 1] = r > 32767 ? 32767 : r < -32768 ? -32768 : r;
    }
}

/* ------------------------------------------------------------------------------------- audio output */
const char *port_audio_path;          /* --dump-audio PATH */
static FILE *audio_fd;
static int audio_failed;
static u32 audio_bytes;
static void wav_header(u8 *h, u32 data) {
    static const u8 t[44] = { 'R','I','F','F',0,0,0,0,'W','A','V','E','f','m','t',' ',16,0,0,0,1,0,2,0,
                              0x44,0xAC,0,0, 0x10,0xB1,2,0, 4,0,16,0, 'd','a','t','a',0,0,0,0 };
    for (int i = 0; i < 44; i++) h[i] = t[i];
    u32 riff = data + 36;
    for (int i = 0; i < 4; i++) { h[4 + i] = riff >> (8 * i); h[40 + i] = data >> (8 * i); }
}
void port_audio_close(void) {
    LOCK();
    if (audio_fd) {
        u8 h[44];
        wav_header(h, audio_bytes);
        fseek(audio_fd, 0, SEEK_SET); fwrite(h, 1, 44, audio_fd);
        fclose(audio_fd);
        audio_fd = 0;
        port_printf("[port] audio: %u samples written to %s\n", audio_bytes / 4, port_audio_path);
    }
    UNLOCK();
    if (port_verbose && xa_sectors) port_printf("[port] sound: %d XA sectors played, %d underruns\n", xa_sectors, xa_underruns);
}

/* ---------------------------------------------------------------------------------- VAB banks */
#define NVAB 16
typedef struct {
    int used;
    u8 *hdr;                         /* copy of the VAB header */
    int ps, vs, mvol;
    u32 base;                        /* sound RAM address of the body */
    u32 vag_addr[256];
    int tone_base[128];              /* first tone record of each program (-1: no tones) */
} Vab;
static Vab vabs[NVAB];
static u8 vab_store[NVAB][0x20 + 0x800 + 128 * 0x200 + 0x200];

#define PROG(vb, p) ((vb)->hdr + 0x20 + (p) * 16)
#define TONE(vb, t) ((vb)->hdr + 0x20 + 0x800 + (t) * 32)

void SpuSetTransferStartAddr(u32 a) { spu_xfer_addr = a & (SPU_RAM_SIZE - 1); }
u32 SpuRead(u8 *p, u32 n) {           /* 0x80077260: really the main RAM -> sound RAM transfer (_spu_write) */
    if (n > 0x7EFF0) n = 0x7EFF0;
    LOCK();
    for (u32 i = 0; i < n && spu_xfer_addr + i < SPU_RAM_SIZE; i++) spu_ram[spu_xfer_addr + i] = p[i];
    UNLOCK();
    return n;
}
int SpuIsTransferCompleted(int f) { return 1; }

short SsVabFakeHead(u8 *h, short vabid, u32 sbaddr) {
    int id = vabid;
    if (h[0] != 'p' || h[1] != 'B' || h[2] != 'A' || h[3] != 'V') return -1;
    if (id < 0 || id >= NVAB) for (id = 0; id < NVAB && vabs[id].used; id++) ;
    if (id >= NVAB) return -1;
    Vab *vb = &vabs[id];
    int ps = *(u16 *)(h + 0x12), vs = *(u16 *)(h + 0x16);
    if (ps > 128 || vs > 254) return -1;
    u32 size = 0x20 + 0x800 + ps * 0x200 + 0x200;
    LOCK();
    for (u32 i = 0; i < size; i++) vab_store[id][i] = h[i];
    vb->hdr = vab_store[id];
    vb->used = 1; vb->ps = ps; vb->vs = vs; vb->mvol = h[0x18];
    vb->base = sbaddr;
    const u16 *sizes = (const u16 *)(vb->hdr + 0x20 + 0x800 + ps * 0x200);
    u32 a = sbaddr;
    for (int k = 1; k <= vs; k++) { vb->vag_addr[k] = a; a += sizes[k] * 8; }
    for (int p = 0, g = 0; p < 128; p++) vb->tone_base[p] = PROG(vb, p)[0] ? 16 * g++ : -1;
    UNLOCK();
    if (port_verbose) port_printf("[port] sound: VAB %d: %d programs, %d VAGs at SPU %05x..%05x\n", id, ps, vs, sbaddr, a);
    return id;
}
short SsVabFakeBody(short id) { return id; }
void SsVabClose(short id) { if (id >= 0 && id < NVAB) vabs[id].used = 0; }

static u16 note_pitch(int note, int fine, int center, int shift) {
    /* 0x1000 plays the sample at 44.1 kHz on its center note; fine and shift in 1/128 semitone */
    static const u16 semi[12] = { 0x1000, 0x10F3, 0x11F6, 0x1306, 0x1428, 0x155B, 0x16A0, 0x17F9, 0x1966, 0x1AE9,
                                  0x1C82, 0x1E34 };
    int c = (note - center) * 128 + fine + shift;
    int oct = c >= 0 ? c / (12 * 128) : -((-c + 12 * 128 - 1) / (12 * 128));
    int rem = c - oct * 12 * 128, st = rem / 128, f = rem % 128;
    u32 p = semi[st];
    u32 nx = st == 11 ? 0x2000 : semi[st + 1];
    p = p + (((nx - p) * f) >> 7);
    if (oct > 0) p <<= oct; else p >>= -oct;
    return p > 0x3FFF ? 0x3FFF : p;
}

/* libsnd's voice gain (_SsVmKeyOnNow): volume (velocity x CC7, or the effect's volume) through the VAB master,
   program and tone volumes, the sequence volume (seq_l < 0: a sound effect), the tone, program and channel pans,
   then squared.  All inputs 0..127; the result is the SPU voice volume 0..0x3FFF. */
static void libsnd_gain(int volume, int pan, int mvol, int pvol, int ppan, int tvol, int tpan, int seq_l, int seq_r, int *outl, int *outr) {
    int vol = (volume * (mvol * 16383)) / 16129;
    int chR = ((vol * pvol) * tvol) / 16129, chL;
    vol = chR;
    if (seq_l >= 0) { vol = (chR * seq_l) / 127; chR = (chR * seq_r) / 127; }
    if (tpan < 64) { chR = (chR * tpan) / 63; chL = vol; } else chL = (vol * (127 - tpan)) / 63;
    if (ppan < 64) chR = (chR * ppan) / 63; else chL = (chL * (127 - ppan)) / 63;
    if (pan < 64) chR = (chR * pan) / 63; else chL = (chL * (127 - pan)) / 63;
    if (stereo_mono) { if (chL < chR) chL = chR; else chR = chL; }
    *outl = (chL * chL) / 16383;
    *outr = (chR * chR) / 16383;
}

/* ---------------------------------------------------------------------------------- sequencer */
#define NSEQ 8
typedef struct {
    int used, playing, vab;
    const u8 *data, *start, *pos, *loop_pos;
    int res, bpm;
    int step;                        /* libsnd unk70: tenths of a MIDI tick per 60 Hz tick */
    int delta;                       /* libsnd delta_value: tenths of a MIDI tick to the next event */
    int status;
    int loops;                       /* SsSeqPlay l_count: 0 = forever */
    int nrpn, loop_count, after_loop_start;      /* NRPN loop: CC99 (20 / 30), its CC6 count (127 = forever) */
    int vol_l, vol_r;                /* SsSeqSetVol 0..127 */
    struct { int prog, vol, pan, bend; } ch[16];
} Seq;
static Seq seqs[NSEQ];
static int max_voice = NVOICES;      /* SsSetReservedVoice: the sequencer allocates below it */

static int read_delta(Seq *s) {      /* _SsReadDeltaValue: a MIDI variable-length quantity, in tenths */
    int v = 0, b;
    do { b = *s->pos++; v = (v << 7) | (b & 0x7F); } while (b & 0x80);
    return v * 10;
}
static void seq_set_tempo(Seq *s, int bpm, int at_open) {
    s->bpm = bpm;
    int x = s->res * bpm * 10;
    if (at_open) {                   /* _SsInitSoundSeq: rounded to nearest */
        s->step = x / (TICK_HZ * 60);
        if (x % (TICK_HZ * 60) > TICK_HZ * 30) s->step++;
    } else s->step = x / (TICK_HZ * 60);     /* _SsSndTempo / the tempo meta event: truncated */
    if (s->step <= 0) s->step = 1;
}
static void seq_rewind(Seq *s) {
    s->pos = s->start; s->status = 0;
    s->delta = read_delta(s);
    s->loop_pos = s->pos;
    s->nrpn = 0; s->loop_count = 0; s->after_loop_start = 0;
    for (int c = 0; c < 16; c++) { s->ch[c].vol = 127; s->ch[c].pan = 64; s->ch[c].bend = 0x2000; s->ch[c].prog = c; }
}
static short seq_open(u8 *d, short vab) {
    int id;
    if (d[0] != 'p' || d[1] != 'Q' || d[2] != 'E' || d[3] != 'S') return -1;
    for (id = 0; id < NSEQ && seqs[id].used; id++) ;
    if (id == NSEQ) return -1;
    Seq *s = &seqs[id];
    memset(s, 0, sizeof *s);
    s->used = 1; s->vab = vab; s->data = d;
    s->res = d[8] << 8 | d[9];
    u32 tempo = d[10] << 16 | d[11] << 8 | d[12];
    if (tempo == 0) tempo = 500000;
    int bpm = 60000000 / tempo;
    if (60000000 % tempo > tempo / 2) bpm++;
    seq_set_tempo(s, bpm, 1);
    s->start = d + 15;
    s->vol_l = s->vol_r = 127;
    seq_rewind(s);
    if (port_verbose) port_printf("[port] sound: SEQ %d at %p: resolution %d, tempo %u us (%d bpm, step %d/10 ticks per 1/60 s), bank %d\n",
                                  id, d, s->res, tempo, bpm, s->step, vab);
    return id;
}
static void seq_notes_off(int id, int now) {
    for (int k = 0; k < NVOICES; k++)
        if (voices[k].owner == id) {
            if (now) { voices[k].env_phase = ENV_OFF; voices[k].owner = -1; }
            else voice_key_off(&voices[k]);
        }
}
static void seq_apply_vol(int id, Seq *s) {         /* _SsVmSetSeqVol: the sounding notes follow the sequence volume */
    for (int k = 0; k < NVOICES; k++) {
        Voice *v = &voices[k];
        if (v->owner != id || v->env_phase == ENV_OFF) continue;
        libsnd_gain(v->g_vol, v->g_pan, v->g_mvol, v->g_pvol, v->g_ppan, v->g_tvol, v->g_tpan, s->vol_l, s->vol_r, &v->vol_l, &v->vol_r);
    }
}

/* _SsVmAlloc: a free voice below the reserved ones, else the lowest-priority voice not above the new note's
   priority (the oldest among equals, releasing ones first); none: the note is dropped */
static Voice *alloc_voice(int prior) {
    Voice *best = 0;
    for (int k = 0; k < max_voice; k++)
        if (voices[k].env_phase == ENV_OFF) return &voices[k];
    for (int k = 0; k < max_voice; k++) {
        Voice *v = &voices[k];
        if (v->owner == 100 || v->prior > prior) continue;
        int vr = v->env_phase == ENV_RELEASE, br = best ? best->env_phase == ENV_RELEASE : 0;
        if (!best || v->prior < best->prior || (v->prior == best->prior && (vr > br || (vr == br && v->age < best->age)))) best = v;
    }
    return best;
}
static void seq_note_on(int id, Seq *s, int c, int key, int vel) {
    Vab *vb = (s->vab >= 0 && s->vab < NVAB && vabs[s->vab].used) ? &vabs[s->vab] : 0;
    if (!vb) return;
    int prog = s->ch[c].prog, tb = vb->tone_base[prog];
    if (tb < 0) return;
    u8 *pa = PROG(vb, prog);
    int volume = (vel * s->ch[c].vol) / 127;       /* _SsVmKeyOn */
    for (int t = 0; t < pa[0] && t < 16; t++) {
        u8 *ta = TONE(vb, tb + t);
        int vag = *(u16 *)(ta + 0x16);
        if (key < ta[6] || key > ta[7] || vag <= 0 || vag > vb->vs) continue;
        Voice *v = alloc_voice(ta[0]);
        if (!v) return;
        int bend = s->ch[c].bend - 0x2000;
        int range = bend >= 0 ? ta[13] : ta[12];
        int fine = bend * (range ? range : 2) * 128 / 0x2000;
        int l, r;
        v->g_vol = volume; v->g_pan = s->ch[c].pan; v->g_mvol = vb->mvol; v->g_pvol = pa[1]; v->g_ppan = pa[4]; v->g_tvol = ta[2]; v->g_tpan = ta[3];
        libsnd_gain(volume, s->ch[c].pan, vb->mvol, pa[1], pa[4], ta[2], ta[3], s->vol_l, s->vol_r, &l, &r);
        voice_key_on(v, vb->vag_addr[vag], note_pitch(key, fine, ta[4], ta[5]), *(u16 *)(ta + 0x10), *(u16 *)(ta + 0x12), l, r);
        v->owner = id; v->ch = c; v->key = key; v->prior = ta[0];
        if (port_verbose > 1) port_printf("[port] sound: seq %d ch %d key %d vel %d prog %d tone %d vag %d voice %d pitch %04x vol %d/%d at sample %llu\n",
                                          id, c, key, vel, prog, t, vag, (int)(v - voices), v->pitch, l, r, samples_total);
    }
}
static void seq_note_off(int id, int c, int key) {
    for (int k = 0; k < NVOICES; k++)
        if (voices[k].owner == id && voices[k].ch == c && voices[k].key == key && voices[k].env_phase != ENV_RELEASE)
            voice_key_off(&voices[k]);
}
/* _SsGetSeqData: one event, then the delta to the next one into s->delta; 0 when the sequence ended */
static int seq_event(int id, Seq *s) {
    int st = *s->pos;
    if (st == 0xFF) {
        int ty = s->pos[1], ln = s->pos[2];
        const u8 *body = s->pos + 3;
        s->pos += 3 + ln;
        if (ty == 0x51 && ln == 3) {
            u32 tempo = body[0] << 16 | body[1] << 8 | body[2];
            if (tempo) seq_set_tempo(s, 60000000 / tempo, 0);
        }
        if (ty == 0x2F) {                     /* end of track (_SsSeqGetEof): l_count plays, 0 = forever */
            if (s->loops == 1) return 0;
            if (s->loops > 1) s->loops--;
            seq_notes_off(id, 0);
            seq_rewind(s);
            return 1;
        }
        s->delta = read_delta(s);
        return 1;
    }
    if (st & 0x80) { s->status = st; s->pos++; }
    int c = s->status & 15, a = s->pos[0], b = 0;
    switch (s->status >> 4) {
    case 0xC: case 0xD: s->pos += 1; break;
    default: b = s->pos[1]; s->pos += 2; break;
    }
    switch (s->status >> 4) {
    case 0x9: if (b) seq_note_on(id, s, c, a, b); else seq_note_off(id, c, a); break;
    case 0x8: seq_note_off(id, c, a); break;
    case 0xC: s->ch[c].prog = a; break;
    case 0xE: s->ch[c].bend = a | b << 7; break;
    case 0xB:
        if (a == 7) s->ch[c].vol = b;
        else if (a == 10) s->ch[c].pan = b;
        else if (a == 6) { if (s->after_loop_start) { s->loop_count = b; s->after_loop_start = 0; } }
        else if (a == 99) {                            /* _SsContNrpn2 */
            s->nrpn = b;
            if (b == 20) {                             /* loop start: the loop resumes after this event's delta */
                s->after_loop_start = 1;
                s->delta = read_delta(s);
                s->loop_pos = s->pos;
                return 1;
            }
            if (b == 30) {                             /* loop end */
                if (s->loop_count == 0) { s->delta = read_delta(s); return 1; }
                if (s->loop_count < 0x7F) {
                    s->loop_count--;
                    s->delta = read_delta(s);
                    if (s->loop_count) s->pos = s->loop_pos;
                    return 1;
                }
                read_delta(s);                         /* forever: back at once */
                s->delta = 0;
                s->pos = s->loop_pos;
                return 1;
            }
        }
        break;
    }
    s->delta = read_delta(s);
    return 1;
}
/* _SsSeqPlay, once per 60 Hz tick */
static void seq_tick(int id, Seq *s) {
    int a = s->delta - s->step;
    if (a > 0) { s->delta = a; return; }
    int acc = s->delta;
    for (;;) {
        if (!seq_event(id, s)) { s->playing = 0; seq_notes_off(id, 0); return; }
        if (s->delta == 0) continue;
        acc += s->delta;
        if (acc >= s->step) break;
    }
    s->delta = acc - s->step;
}
static void seq_stop(int id) {
    if (id < 0 || id >= NSEQ) return;
    seqs[id].playing = 0;
    seq_notes_off(id, 0);
}

short SsSeqOpen(u8 *d, short vab) { LOCK(); short id = seq_open(d, vab); UNLOCK(); return id; }
void SsSeqPlay(short id, char mode, short loops) {
    if (port_verbose) port_printf("[port] sound: SsSeqPlay(%d, %d, %d)\n", id, mode, loops);
    if (id < 0 || id >= NSEQ || !seqs[id].used) return;
    LOCK();
    Seq *s = &seqs[id];
    if (mode == 1) { s->playing = 1; s->loops = loops; }     /* SSPLAY_PLAY */
    else if (mode == 0) s->playing = 0;                      /* SSPLAY_PAUSE */
    UNLOCK();
}
void SsSeqStop(short id) { LOCK(); seq_stop(id); UNLOCK(); }
void SsSeqClose(short id) { LOCK(); seq_stop(id); if (id >= 0 && id < NSEQ) seqs[id].used = 0; UNLOCK(); }
void SsSeqSetVol(short id, short l, short r) {
    if (id < 0 || id >= NSEQ) return;
    l = l < 0 ? 0 : l > 127 ? 127 : l;
    r = r < 0 ? 0 : r > 127 ? 127 : r;
    LOCK();
    seqs[id].vol_l = l; seqs[id].vol_r = r;
    seq_apply_vol(id, &seqs[id]);
    UNLOCK();
}
void SsSeqGetVol(short id, short *l, short *r) { if (id >= 0 && id < NSEQ) { *l = seqs[id].vol_l; *r = seqs[id].vol_r; } }
short SsIsEos(short id, short sep) { return id >= 0 && id < NSEQ && seqs[id].playing; }
void SsSetMVol(short l, short r) {   /* libsnd: the SPU main volume, 0x81 * vol */
    l = l < 0 ? 0 : l > 127 ? 127 : l;
    r = r < 0 ? 0 : r > 127 ? 127 : r;
    main_l = 0x81 * l; main_r = 0x81 * r;
}
char SsSetReservedVoice(char n) { if (n <= 0 || n > NVOICES) return -1; max_voice = n; return n; }

/* ---------------------------------------------------------------------------------- sound effects */
short SsUtKeyOnV(short voice, short vabid, short prog, short tone, short note, short fine, short voll, short volr) {
    if (voice < 0 || voice >= NVOICES || vabid < 0 || vabid >= NVAB || !vabs[vabid].used) return -1;
    Vab *vb = &vabs[vabid];
    if (prog < 0 || prog >= 128 || vb->tone_base[prog] < 0) return -1;
    u8 *pa = PROG(vb, prog), *ta = TONE(vb, vb->tone_base[prog] + (tone & 15));
    int vag = *(u16 *)(ta + 0x16);
    if (vag <= 0 || vag > vb->vs) return -1;
    int volume, pan;                                   /* libsnd SsUtKeyOnV: the larger volume, the pan from the ratio */
    voll = voll < 0 ? 0 : voll > 127 ? 127 : voll;
    volr = volr < 0 ? 0 : volr > 127 ? 127 : volr;
    if (voll == volr) { volume = voll; pan = 64; }
    else if (voll > volr) { volume = voll; pan = (volr << 6) / voll; }
    else { volume = volr; pan = 127 - (voll << 6) / volr; }
    int l, r;
    libsnd_gain(volume, pan, vb->mvol, pa[1], pa[4], ta[2], ta[3], -1, -1, &l, &r);
    LOCK();
    Voice *v = &voices[voice];
    v->g_vol = volume; v->g_pan = pan; v->g_mvol = vb->mvol; v->g_pvol = pa[1]; v->g_ppan = pa[4]; v->g_tvol = ta[2]; v->g_tpan = ta[3];
    voice_key_on(v, vb->vag_addr[vag], note_pitch(note, fine, ta[4], ta[5]), *(u16 *)(ta + 0x10), *(u16 *)(ta + 0x12), l, r);
    v->owner = 100; v->ch = -1; v->key = note; v->prior = ta[0];
    UNLOCK();
    if (port_verbose > 1) port_printf("[port] sound: sfx voice %d vab %d prog %d tone %d note %d vol %d/%d -> %d/%d\n", voice, vabid, prog, tone, note, voll, volr, l, r);
    return voice;
}
short SsUtKeyOffV(short voice) {
    if (voice < 0 || voice >= NVOICES) return -1;
    LOCK(); voice_key_off(&voices[voice]); UNLOCK();
    return 0;
}
short SsUtChangePitch(short voice, short vabid, short prog, short old_note, short old_fine, short new_note, short new_fine) {
    if (voice < 0 || voice >= NVOICES || vabid < 0 || vabid >= NVAB || !vabs[vabid].used) return -1;
    Vab *vb = &vabs[vabid];
    if (prog < 0 || prog >= 128 || vb->tone_base[prog] < 0) return -1;
    u8 *ta = TONE(vb, vb->tone_base[prog]);
    voices[voice].pitch = note_pitch(new_note, new_fine, ta[4], ta[5]);
    return 0;
}
void SpuSetKey(int on, u32 bits) {
    LOCK();
    for (int k = 0; k < NVOICES; k++)
        if ((bits & (1u << k)) && !on) voice_key_off(&voices[k]);
    UNLOCK();
}
int SpuGetAllKeysStatus(u8 *st) {
    for (int k = 0; k < NVOICES; k++)
        st[k] = voices[k].env_phase == ENV_OFF ? 0 : voices[k].env_phase == ENV_RELEASE ? 2 : 1;
    return 0;
}

/* ------------------------------------------------------------------------------ libspu attributes */
typedef struct { short left, right; } SpuVol;
typedef struct { u32 mask; SpuVol mvol, mvolmode, mvolx, cd_vol; int cd_reverb, cd_mix; SpuVol ext_vol; int ext_reverb, ext_mix; } SpuCommon;
typedef struct { u32 mask; int mode; SpuVol depth; int delay, feedback; } SpuReverb;
void SpuSetCommonAttr(SpuCommon *a) {
    u32 m = a->mask ? a->mask : ~0u;
    if (m & 0x01) main_l = a->mvol.left & 0x7FFF;      /* SPU_COMMON_MVOLL */
    if (m & 0x02) main_r = a->mvol.right & 0x7FFF;
    if (m & 0x40) cd_l = a->cd_vol.left;                /* SPU_COMMON_CDVOLL */
    if (m & 0x80) cd_r = a->cd_vol.right;
    if (main_l > 0x3FFF) main_l = 0x3FFF;
    if (main_r > 0x3FFF) main_r = 0x3FFF;
    if (cd_l < 0) cd_l = 0;
    if (cd_r < 0) cd_r = 0;
    if (port_verbose) port_printf("[port] sound: SpuSetCommonAttr mask %x: main %d/%d cd %d/%d\n", a->mask, main_l, main_r, cd_l, cd_r);
}
long SpuSetReverbModeParam(SpuReverb *a) {
    u32 m = a->mask ? a->mask : ~0u;
    LOCK();
    if (m & 1) {                                        /* SPU_REV_MODE */
        int mode = a->mode & 0xFF, clear = (a->mode & 0x100) != 0;
        if (mode > 9) { UNLOCK(); return -1; }
        if (mode == 7 || mode == 8) {                   /* ECHO / DELAY: libspu's defaults unless the call sets them */
            rev_delay = (m & 8) ? a->delay : 0x7F;                  /* SPU_REV_DELAYTIME */
            rev_feedback = (m & 0x10) ? a->feedback : (mode == 7 ? 0x7F : 0);   /* SPU_REV_FEEDBACK */
        }
        rev_set_mode(mode, clear);
        rev_vol_l = rev_vol_r = 0;                      /* a mode change zeroes the depth (libspu) */
    } else {
        if ((rev_mode == 7 || rev_mode == 8) && (m & 0x18)) {
            if (m & 8) rev_delay = a->delay;
            if (m & 0x10) rev_feedback = a->feedback;
            rev_set_mode(rev_mode, 0);
        }
        if (m & 2) rev_vol_l = a->depth.left;           /* SPU_REV_DEPTHL */
        if (m & 4) rev_vol_r = a->depth.right;          /* SPU_REV_DEPTHR */
    }
    UNLOCK();
    return 0;
}
long SpuSetReverbDepth(SpuReverb *a) {
    u32 m = a->mask ? a->mask : ~0u;
    if (m & 2) rev_vol_l = a->depth.left;
    if (m & 4) rev_vol_r = a->depth.right;
    if (port_verbose) port_printf("[port] sound: reverb depth %d/%d\n", rev_vol_l, rev_vol_r);
    return 0;
}
u32 SpuSetReverbVoice(int on_off, u32 bits) {
    if (on_off == 8) rev_voices = bits & 0xFFFFFF;      /* SPU_BIT */
    else if (on_off) rev_voices |= bits & 0xFFFFFF;
    else rev_voices &= ~bits;
    return rev_voices;
}
int SpuSetReverb(int on) {
    if (on == 2) return rev_on;                         /* SPU_CHECK */
    rev_on = on != 0;
    if (port_verbose) port_printf("[port] sound: reverb %s\n", rev_on ? "on" : "off");
    return rev_on;
}
u32 SpuReserveReverbWorkArea(int on) { return on; }
u32 SpuClearReverbWorkArea(int mode) { LOCK(); memset(rev_buf, 0, sizeof rev_buf); UNLOCK(); return 0; }
/* initialisation the game's SoundInit runs (libsnd / libspu internals that have nothing to set up here) */
void _SsInit(void) {}
void SpuInitHot(void) {}
void SsStart(void) {}                              /* 8006eed8 (was named SpuInitHot by Ghidra) */
void _SpuInit(int hot) {}
void SpuQuit(void) {}
void SsSetTableSize(void *table, short s_max, short t_max) {}
void SsSetTickMode(int mode) { if (port_verbose) port_printf("[port] sound: SsSetTickMode(%d) (the sequencer ticks at %d Hz)\n", mode, TICK_HZ); }
void SsSetAutoKeyOffMode(short mode) {}

/* ---------------------------------------------------------------------------------- the sample clock */
static s16 out[2 * 2048];
static int tick_left;                      /* samples until the next sequencer tick */
static int samples_second;                 /* --show-fps: samples produced since the last report */
static int pulled;                         /* an audio device drives the mixer through port_sound_pull */
int port_sound_samples_last_second(void) { int n = samples_second; samples_second = 0; return n; }
static void wav_out(const s16 *buf, int n) {
    if (port_audio_path && !audio_fd && !audio_failed) {
        audio_fd = fopen(port_audio_path, "wb");
        if (!audio_fd) { port_printf("[port] cannot create %s\n", port_audio_path); audio_failed = 1; }
        else { u8 h[44]; wav_header(h, 0); fwrite(h, 1, 44, audio_fd); }
    }
    if (audio_fd) { fwrite(buf, 4, n, audio_fd); audio_bytes += n * 4; }
}
/* n stereo frames of audio time: the sequencer ticks every TICK_SAMPLES of them */
static void render(s16 *dst, int n) {
    samples_second += n;
    while (n > 0) {
        if (tick_left <= 0) {
            for (int i = 0; i < NSEQ; i++) if (seqs[i].used && seqs[i].playing) seq_tick(i, &seqs[i]);
            tick_left += TICK_SAMPLES;
        }
        int k = n < tick_left ? n : tick_left;
        mix(dst, k);
        wav_out(dst, k);
        dst += 2 * k; n -= k; tick_left -= k; samples_total += k;
    }
}
void port_sound_pulled(int on) { pulled = on; }
int port_sound_pull(short *dst, int frames) {       /* the audio device's callback (holds the device lock) */
    int done = 0;
    while (frames > 0) {
        int k = frames < 2048 ? frames : 2048;
        render(out, k);
        memcpy(dst, out, k * 4);
        dst += 2 * k; frames -= k; done += k;
    }
    return done;
}
void port_sound_vblank(void) {                       /* no device: 44100/hz samples per vblank, whatever the wall clock does */
    if (pulled) return;
    int n = RATE / port_hz + (port_frame % port_hz < RATE % port_hz);
    while (n > 0) {
        int k = n < 2048 ? n : 2048;
        render(out, k);
        n -= k;
    }
}
