/* Sound, first step (docs/PORT.md "Sound"): a software SPU and a high-level libsnd.
 *
 * SPU: 512 KB sound RAM and 24 voices: SPU ADPCM blocks (filters, loop start / loop end / repeat flags), 4.12 pitch
 * counter with linear interpolation, the PS1 ADSR envelope (attack / decay / sustain / release with the hardware's
 * linear and exponential rate rules), left/right volume.  It is mixed once per vblank (882 samples at 44.1 kHz on
 * PAL) into a ring the SDL audio callback drains (port_sound_pull) and, with --dump-audio PATH, into a WAV file.
 * No reverb, noise or pitch modulation yet.  Carried over from port/shims/sound.c.
 *
 * libsnd is replaced at its API (the sequencer core of the game's libsnd is still partly asm only): SsVabFakeHead
 * keeps a copy of the VAB header (programs, tones, VAG sizes) of a bank whose body the game wrote to sound RAM
 * (SpuSetTransferStartAddr + the transfer at 0x80077260), SsSeqOpen/SsSeqPlay play a Sony SEQ (pQES: MIDI events
 * with running status, tempo meta events, loop points CC99 = 20 / 30) on free voices, and SsUtKeyOnV/KeyOffV play
 * sound effects on the voice the game names.  Voice gain follows libsnd: velocity, channel volume (CC7) and
 * expression (CC11), program and tone volume, VAB master volume, sequence and master volume; pan from CC10 and
 * the tone/program pans. */
#include <stdio.h>
#include "port_runtime.h"

typedef unsigned char u8; typedef signed char s8; typedef unsigned short u16; typedef short s16;
typedef unsigned int u32; typedef int s32;

/* ----------------------------------------------------------------------------------------------- SPU */
#define SPU_RAM_SIZE 0x80000
#define NVOICES 24
#define RATE 44100
static u8 spu_ram[SPU_RAM_SIZE];
static u32 spu_xfer_addr;

enum { ENV_OFF, ENV_ATTACK, ENV_DECAY, ENV_SUSTAIN, ENV_RELEASE };
typedef struct {
    u32 start, loop, cur;            /* byte addresses in sound RAM */
    int loop_set;                    /* a loop-start flag was seen (else the start address repeats) */
    u32 counter;                     /* 4.12 position inside the current 28 sample block */
    u16 pitch;
    s16 buf[28];
    s16 prev;                        /* last sample of the previous block (interpolation) */
    int s1, s2;                      /* ADPCM history */
    int vol_l, vol_r;                /* 0..0x3FFF */
    u16 adsr1, adsr2;
    int env_phase, env_level, env_wait;
    int owner;                       /* -1 free, 0..: sequence slot, 100: sound effect (game named voice) */
    int ch, key;                     /* sequence channel and key of the note */
    unsigned age;
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
    if (b[1] & 4) { v->loop = v->cur; v->loop_set = 1; }
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
    v->loop = v->start; v->loop_set = 0;
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

static void mix(s16 *out, int n) {
    for (int i = 0; i < n; i++) {
        int l = 0, r = 0;
        for (int k = 0; k < NVOICES; k++) {
            Voice *v = &voices[k];
            if (v->env_phase == ENV_OFF) continue;
            int idx = v->counter >> 12, fr = v->counter & 0xFFF;
            int a = idx == 0 ? v->prev : v->buf[idx - 1], b = v->buf[idx];
            int s = a + (((b - a) * fr) >> 12);
            s = (s * v->env_level) >> 15;
            l += (s * v->vol_l) >> 14;
            r += (s * v->vol_r) >> 14;
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
        l /= 2; r /= 2;
        out[2 * i] = l > 32767 ? 32767 : l < -32768 ? -32768 : l;
        out[2 * i + 1] = r > 32767 ? 32767 : r < -32768 ? -32768 : r;
    }
}

/* ------------------------------------------------------------------------------------- audio output */
const char *port_audio_path;          /* --dump-audio PATH */
static FILE *audio_fd;
static int audio_failed;
static u32 audio_bytes;
/* the output ring the SDL audio thread drains: a quarter second; when the game runs ahead the oldest samples are
   dropped (headless runs never pull: the ring just wraps) */
#define RING_FRAMES 16384
static volatile short ring[RING_FRAMES * 2];
static volatile unsigned ring_w, ring_r;
int port_sound_pull(short *out, int frames) {
    int got = 0;
    for (; got < frames; got++) {
        if (ring_r == ring_w) break;
        unsigned i = ring_r % RING_FRAMES;
        out[2 * got] = ring[2 * i]; out[2 * got + 1] = ring[2 * i + 1];
        __sync_synchronize();
        ring_r++;
    }
    for (int k = got; k < frames; k++) out[2 * k] = out[2 * k + 1] = 0;
    return got;
}
static void ring_push(const s16 *s, int n) {
    for (int k = 0; k < n; k++) {
        if (ring_w - ring_r >= RING_FRAMES) ring_r = ring_w - RING_FRAMES + 1;
        unsigned i = ring_w % RING_FRAMES;
        ring[2 * i] = s[2 * k]; ring[2 * i + 1] = s[2 * k + 1];
        __sync_synchronize();
        ring_w++;
    }
}
static void wav_header(u8 *h, u32 data) {
    static const u8 t[44] = { 'R','I','F','F',0,0,0,0,'W','A','V','E','f','m','t',' ',16,0,0,0,1,0,2,0,
                              0x44,0xAC,0,0, 0x10,0xB1,2,0, 4,0,16,0, 'd','a','t','a',0,0,0,0 };
    for (int i = 0; i < 44; i++) h[i] = t[i];
    u32 riff = data + 36;
    for (int i = 0; i < 4; i++) { h[4 + i] = riff >> (8 * i); h[40 + i] = data >> (8 * i); }
}
void port_audio_close(void) {
    if (!audio_fd) return;
    u8 h[44];
    wav_header(h, audio_bytes);
    fseek(audio_fd, 0, SEEK_SET); fwrite(h, 1, 44, audio_fd);
    fclose(audio_fd);
    audio_fd = 0;
    port_printf("[port] audio: %u samples written to %s\n", audio_bytes / 4, port_audio_path);
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
    for (u32 i = 0; i < n && spu_xfer_addr + i < SPU_RAM_SIZE; i++) spu_ram[spu_xfer_addr + i] = p[i];
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
    for (u32 i = 0; i < size; i++) vab_store[id][i] = h[i];
    vb->hdr = vab_store[id];
    vb->used = 1; vb->ps = ps; vb->vs = vs; vb->mvol = h[0x18];
    vb->base = sbaddr;
    const u16 *sizes = (const u16 *)(vb->hdr + 0x20 + 0x800 + ps * 0x200);
    u32 a = sbaddr;
    for (int k = 1; k <= vs; k++) { vb->vag_addr[k] = a; a += sizes[k] * 8; }
    for (int p = 0, g = 0; p < 128; p++) vb->tone_base[p] = PROG(vb, p)[0] ? 16 * g++ : -1;
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

static Voice *alloc_voice(int owner) {
    Voice *best = 0;
    for (int k = 0; k < NVOICES; k++)
        if (voices[k].env_phase == ENV_OFF) return &voices[k];
    for (int k = 0; k < NVOICES; k++)       /* steal the oldest releasing, else the oldest note of a sequence */
        if (voices[k].owner != 100 && voices[k].env_phase == ENV_RELEASE && (!best || voices[k].age < best->age)) best = &voices[k];
    if (!best)
        for (int k = 0; k < NVOICES; k++)
            if (voices[k].owner != 100 && (!best || voices[k].age < best->age)) best = &voices[k];
    return best;
}

/* ---------------------------------------------------------------------------------- sequencer */
#define NSEQ 8
typedef struct {
    int used, playing, vab;
    const u8 *data, *pos, *loop_pos, *start;
    int res;
    u32 tempo;                       /* us per quarter note */
    int status;
    int wait;                        /* ticks until the next event */
    u32 tick_frac;                   /* fractional ticks (1/1000000 units) */
    int loops;                       /* SsSeqPlay l_count: 0 = forever */
    int vol_l, vol_r;                /* SsSeqSetVol 0..127 */
    int nrpn;
    struct { int prog, vol, pan, expr, bend; } ch[16];
} Seq;
static Seq seqs[NSEQ];
static int master_l = 127, master_r = 127;

static int read_var(const u8 **p) {
    int v = 0, b;
    do { b = *(*p)++; v = (v << 7) | (b & 0x7F); } while (b & 0x80);
    return v;
}
short SsSeqOpen(u8 *d, short vab) {
    int id;
    if (d[0] != 'p' || d[1] != 'Q' || d[2] != 'E' || d[3] != 'S') return -1;
    for (id = 0; id < NSEQ && seqs[id].used; id++) ;
    if (id == NSEQ) return -1;
    Seq *s = &seqs[id];
    for (u32 i = 0; i < sizeof *s; i++) ((u8 *)s)[i] = 0;
    s->used = 1; s->vab = vab; s->data = d;
    s->res = d[8] << 8 | d[9];
    s->tempo = d[10] << 16 | d[11] << 8 | d[12];
    s->start = d + 15;
    s->vol_l = s->vol_r = 127;
    if (port_verbose) port_printf("[port] sound: SEQ %d at %p: resolution %d, tempo %u us, bank %d\n", id, d, s->res, s->tempo, vab);
    return id;
}
static void seq_notes_off(int id, int now) {
    for (int k = 0; k < NVOICES; k++)
        if (voices[k].owner == id) {
            if (now) { voices[k].env_phase = ENV_OFF; voices[k].owner = -1; }
            else voice_key_off(&voices[k]);
        }
}
static void seq_rewind(Seq *s) {
    s->pos = s->start; s->status = 0; s->tick_frac = 0;
    s->wait = read_var(&s->pos);
    for (int c = 0; c < 16; c++) { s->ch[c].vol = 127; s->ch[c].pan = 64; s->ch[c].expr = 127; s->ch[c].bend = 0x2000; s->ch[c].prog = 0; }
}
void SsSeqPlay(short id, char mode, short loops) {
    if (port_verbose) port_printf("[port] sound: SsSeqPlay(%d, %d, %d)\n", id, mode, loops);
    if (id < 0 || id >= NSEQ || !seqs[id].used) return;
    Seq *s = &seqs[id];
    if (!s->pos) seq_rewind(s);
    s->playing = mode != 0;
    s->loops = loops;
}
void SsSeqStop(short id) { if (id >= 0 && id < NSEQ) { seqs[id].playing = 0; seqs[id].pos = 0; seq_notes_off(id, 0); } }
void SsSeqClose(short id) { if (id >= 0 && id < NSEQ) { SsSeqStop(id); seqs[id].used = 0; } }
void SsSeqSetVol(short id, short l, short r) { if (id >= 0 && id < NSEQ) { seqs[id].vol_l = l; seqs[id].vol_r = r; } }
void SsSetMVol(short l, short r) { master_l = l; master_r = r; }
short SsIsEos(short id, short sep) { return id >= 0 && id < NSEQ && seqs[id].playing; }

static void pan_volumes(int gain /* 0..0x3FFF */, int pan, int *l, int *r) {
    if (pan < 0) pan = 0;
    if (pan > 127) pan = 127;
    *l = pan < 64 ? gain : gain * (127 - pan) / 63;
    *r = pan > 64 ? gain : gain * pan / 64;
}
static void seq_note_on(int id, Seq *s, int c, int key, int vel) {
    Vab *vb = (s->vab >= 0 && s->vab < NVAB && vabs[s->vab].used) ? &vabs[s->vab] : 0;
    if (port_verbose > 2) port_printf("[port] sound: note on seq %d ch %d key %d vel %d prog %d bank %d (%s)\n", id, c, key, vel,
                                      s->ch[c].prog, s->vab, vb ? "ok" : "closed");
    if (!vb) return;
    int prog = s->ch[c].prog, tb = vb->tone_base[prog];
    if (tb < 0) return;
    u8 *pa = PROG(vb, prog);
    for (int t = 0; t < pa[0] && t < 16; t++) {
        u8 *ta = TONE(vb, tb + t);
        int vag = *(u16 *)(ta + 0x16);
        if (key < ta[6] || key > ta[7] || vag <= 0 || vag > vb->vs) continue;
        Voice *v = alloc_voice(id);
        if (!v) return;
        int bend = s->ch[c].bend - 0x2000;
        int range = bend >= 0 ? ta[13] : ta[12];
        int fine = bend * (range ? range : 2) * 128 / 0x2000;
        /* gain: velocity, CC7, CC11, program, tone, bank master, sequence and master volume (all 0..127) */
        long long g = 0x3FFFll * vel * s->ch[c].vol * s->ch[c].expr * pa[1] * ta[2] * vb->mvol;
        g /= 127ll * 127 * 127 * 127 * 127 * 127;
        int pan = s->ch[c].pan + (ta[3] - 64) + (pa[4] - 64);
        int l, r;
        pan_volumes((int)g, pan, &l, &r);
        l = l * s->vol_l / 127 * master_l / 127;
        r = r * s->vol_r / 127 * master_r / 127;
        voice_key_on(v, vb->vag_addr[vag], note_pitch(key, fine, ta[4], ta[5]), *(u16 *)(ta + 0x10), *(u16 *)(ta + 0x12), l, r);
        v->owner = id; v->ch = c; v->key = key;
        if (port_verbose > 1) port_printf("[port] sound: seq %d ch %d key %d vel %d prog %d tone %d vag %d pitch %04x vol %d/%d\n",
                                          id, c, key, vel, prog, t, vag, v->pitch, l, r);
    }
}
static void seq_note_off(int id, int c, int key) {
    for (int k = 0; k < NVOICES; k++)
        if (voices[k].owner == id && voices[k].ch == c && voices[k].key == key && voices[k].env_phase != ENV_RELEASE)
            voice_key_off(&voices[k]);
}
/* runs the events due now; returns 0 when the sequence ended */
static int seq_events(int id, Seq *s) {
    while (s->wait <= 0) {
        int st = *s->pos;
        if (st == 0xFF) {
            int ty = s->pos[1], ln = s->pos[2];
            const u8 *body = s->pos + 3;
            s->pos += 3 + ln;
            if (ty == 0x51 && ln == 3) s->tempo = body[0] << 16 | body[1] << 8 | body[2];
            if (ty == 0x2F) {                     /* end of track */
                if (s->loops == 1) return 0;
                if (s->loops > 1) s->loops--;
                seq_notes_off(id, 0);
                seq_rewind(s);
                continue;
            }
        } else {
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
                else if (a == 11) s->ch[c].expr = b;
                else if (a == 99) {
                    if (b == 20) s->loop_pos = s->pos;    /* loop start (after this event) */
                    else if (b == 30 && s->loop_pos) {    /* loop end: back to the start */
                        s->pos = s->loop_pos;
                        s->wait = read_var(&s->pos);
                        continue;
                    }
                }
                break;
            }
        }
        s->wait = read_var(&s->pos);
    }
    return 1;
}
static void seq_frame(int id, Seq *s, u32 us) {
    /* ticks this frame = us * res / tempo */
    s->tick_frac += us * (u32)s->res;
    while (s->tick_frac >= s->tempo && s->playing) {
        s->tick_frac -= s->tempo;
        s->wait--;
        if (!seq_events(id, s)) { s->playing = 0; seq_notes_off(id, 0); }
    }
}

/* ---------------------------------------------------------------------------------- sound effects */
short SsUtKeyOnV(short voice, short vabid, short prog, short tone, short note, short fine, short voll, short volr) {
    if (voice < 0 || voice >= NVOICES || vabid < 0 || vabid >= NVAB || !vabs[vabid].used) return -1;
    Vab *vb = &vabs[vabid];
    if (prog < 0 || prog >= 128 || vb->tone_base[prog] < 0) return -1;
    u8 *pa = PROG(vb, prog), *ta = TONE(vb, vb->tone_base[prog] + (tone & 15));
    int vag = *(u16 *)(ta + 0x16);
    if (vag <= 0 || vag > vb->vs) return -1;
    long long g = 0x3FFFll * pa[1] * ta[2] * vb->mvol / (127ll * 127 * 127);
    int l, r, pan = ta[3] + (pa[4] - 64);
    pan_volumes((int)g, pan, &l, &r);
    l = l * voll / 127; r = r * volr / 127;
    Voice *v = &voices[voice];
    voice_key_on(v, vb->vag_addr[vag], note_pitch(note, fine, ta[4], ta[5]), *(u16 *)(ta + 0x10), *(u16 *)(ta + 0x12), l, r);
    v->owner = 100; v->ch = -1; v->key = note;
    return voice;
}
short SsUtKeyOffV(short voice) {
    if (voice < 0 || voice >= NVOICES) return -1;
    voice_key_off(&voices[voice]);
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
    for (int k = 0; k < NVOICES; k++)
        if (bits & (1u << k)) { if (on) voices[k].env_phase = voices[k].env_phase; else voice_key_off(&voices[k]); }
}
int SpuGetAllKeysStatus(u8 *st) {
    for (int k = 0; k < NVOICES; k++)
        st[k] = voices[k].env_phase == ENV_OFF ? 0 : voices[k].env_phase == ENV_RELEASE ? 2 : 1;
    return 0;
}

/* ---------------------------------------------------------------------------------- the sample clock */
/* Audio is produced on its own clock.  The sequencer ticks every 1e6/hz us of *audio time* (RATE/hz samples: the
   libsnd tick is the vblank timer on the console), so the tempo depends only on the sample count.  Headless the
   count advances RATE/hz samples per vblank (deterministic dumps); paced (--realtime, --window) it follows the wall
   clock: each vblank mixes the samples that became due since the previous one, however many vblanks actually ran.
   A stall longer than four frames (the viewer pipe blocked, a slow load) re-anchors the clock instead of producing a
   burst that would sit in the player's queue and delay every later sound. */
static s16 out[2 * 2048];
static int seq_left;                       /* samples until the next sequencer tick */
static int seq_frac;                       /* RATE % hz remainder carried between ticks */
static unsigned long long audio_epoch_us;  /* wall clock when samples_done was 0 */
static unsigned long long samples_done;    /* samples produced on the wall clock since the epoch */
static int samples_second;                 /* --show-fps: samples produced since the last report */
int port_sound_samples_last_second(void) { int n = samples_second; samples_second = 0; return n; }
static void audio_out(int n) {
    if (port_audio_path && !audio_fd && !audio_failed) {
        audio_fd = fopen(port_audio_path, "wb");
        if (!audio_fd) { port_printf("[port] cannot create %s\n", port_audio_path); audio_failed = 1; }
        else { u8 h[44]; wav_header(h, 0); fwrite(h, 1, 44, audio_fd); }
    }
    if (audio_fd) { fwrite(out, 4, n, audio_fd); audio_bytes += n * 4; }
    if (port_window) ring_push(out, n);
}
static void produce(int n) {
    samples_second += n;
    while (n > 0) {
        if (seq_left <= 0) {
            for (int i = 0; i < NSEQ; i++) if (seqs[i].used && seqs[i].playing) seq_frame(i, &seqs[i], 1000000u / (u32)port_hz);
            seq_left += RATE / port_hz;
            seq_frac += RATE % port_hz;
            if (seq_frac >= port_hz) { seq_frac -= port_hz; seq_left++; }
        }
        int k = n < seq_left ? n : seq_left;
        if (k > 2048) k = 2048;
        mix(out, k);
        audio_out(k);
        n -= k; seq_left -= k;
    }
}
void port_sound_vblank(void) {
    if (!port_realtime) { produce(RATE / port_hz + (port_frame % port_hz < RATE % port_hz)); return; }
    unsigned long long now = port_time_us();
    if (!audio_epoch_us) { audio_epoch_us = now; samples_done = 0; }
    unsigned long long due = (now - audio_epoch_us) * RATE / 1000000ull;
    if (due > samples_done + 4u * RATE / port_hz) {        /* stalled: drop to real time, as the vblank schedule does */
        if (port_verbose) port_printf("[port] frame %d: audio clock re-anchored (%d samples late)\n", port_frame, (int)(due - samples_done));
        audio_epoch_us = now - samples_done * 1000000ull / RATE;
        due = samples_done;
    }
    if (due > samples_done) { produce((int)(due - samples_done)); samples_done = due; }
}
