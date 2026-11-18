/* Software MDEC: the libpress calls of the movie player (moviePlayerTask / MovieDecodeFrame / mdecSliceCallback)
 * on a C model of the PlayStation's decoder.
 *
 * The data path is the hardware's, so the game's own buffers and buffer sizes work unchanged:
 *   DecDCTvlc(bs, buf)   the STR frame (a "BS" bitstream, version 2 or 3) becomes MDEC run-length codes in buf:
 *                        word 0 is the decode command (0x3800xxxx, xxxx = words of codes, a multiple of 32), then
 *                        16-bit codes: per block a DC code (q scale << 10 | dc), AC codes (run << 10 | level) and
 *                        the end-of-block code 0xFE00; the padding is 0xFE00 too.
 *   DecDCTin(buf, mode)  hands those codes to the decoder (mode bit 0: 24-bit output instead of 16-bit, bit 1:
 *                        bit 15 of the 16-bit pixels set), as MDEC_in does with the command word's bits 27 / 25.
 *   DecDCTout(buf, n)    takes the next n words of decoded pixels: whole 16x16 macroblocks (128 words each at
 *                        16 bits per pixel, 192 at 24), row-major inside the macroblock, in the order of the stream
 *                        (the columns of the picture top to bottom, left to right).  The movie player asks for one
 *                        16x224 column per call and LoadImages it from the DMA callback, which asks for the next
 *                        column: the callback runs when the words are there, re-entrant calls are queued.
 * The decoder: dequantisation with the default MDEC tables (q scale from the DC code), an integer IDCT, YCbCr to
 * RGB as the hardware does it (one chroma sample per 2x2 luma), clamped to 5-bit (16-bit output) or 8-bit
 * channels.  The streaming side (the St* ring the frames come from) is in psyq.c; port_st_frame_dims tells
 * DecDCTvlc the picture size of the frame it was handed, so it decodes exactly the frame's macroblocks. */
#include <string.h>
#include "port_runtime.h"

typedef unsigned char u8; typedef unsigned short u16; typedef short s16; typedef int s32; typedef unsigned int u32;

int port_st_frame_dims(const void *bs, int *w, int *h, int *bytes);   /* psyq.c: the frame StGetNext handed out */

/* ------------------------------------------------------------------ tables */
static const u8 zigzag[64] = {
     0,  1,  8, 16,  9,  2,  3, 10, 17, 24, 32, 25, 18, 11,  4,  5,
    12, 19, 26, 33, 40, 48, 41, 34, 27, 20, 13,  6,  7, 14, 21, 28,
    35, 42, 49, 56, 57, 50, 43, 36, 29, 22, 15, 23, 30, 37, 44, 51,
    58, 59, 52, 45, 38, 31, 39, 46, 53, 60, 61, 54, 47, 55, 62, 63,
};
/* the default quantisation matrix (raster order; the MDEC's copy, iq_y / iq_c of libpress, is this one in
   zigzag order with 2 for the DC entry) */
static const u8 quant_raster[64] = {
     8, 16, 19, 22, 26, 27, 29, 34,
    16, 16, 22, 24, 27, 29, 34, 37,
    19, 22, 26, 27, 29, 34, 34, 38,
    22, 22, 26, 27, 29, 34, 37, 40,
    22, 26, 27, 29, 32, 35, 40, 48,
    26, 27, 29, 32, 35, 40, 48, 58,
    26, 27, 29, 34, 38, 46, 56, 69,
    27, 29, 35, 38, 46, 56, 69, 83,
};
static u8 iq_y[64], iq_c[64];            /* zigzag order, as the hardware holds them */
static s32 idct_cos[8][8];               /* C(u) cos((2x+1)u pi/16) / 2, 12 fractional bits */

/* The AC run/level codes (the MPEG-1 table B.14 as the MDEC bitstream uses it: "11" is run 0 level 1 for every
   coefficient, "10" is end of block, "000001" escapes a 6-bit run and a 10-bit level); a sign bit follows. */
static const struct { const char *code; u8 run, level; } ac_codes[] = {
    /* run 0 */ {"11",0,1},{"0100",0,2},{"00101",0,3},{"0000110",0,4},{"00100110",0,5},{"00100001",0,6},
    {"0000001010",0,7},{"000000011101",0,8},{"000000011000",0,9},{"000000010011",0,10},{"000000010000",0,11},
    {"0000000011010",0,12},{"0000000011001",0,13},{"0000000011000",0,14},{"0000000010111",0,15},
    {"00000000011111",0,16},{"00000000011110",0,17},{"00000000011101",0,18},{"00000000011100",0,19},
    {"00000000011011",0,20},{"00000000011010",0,21},{"00000000011001",0,22},{"00000000011000",0,23},
    {"00000000010111",0,24},{"00000000010110",0,25},{"00000000010101",0,26},{"00000000010100",0,27},
    {"00000000010011",0,28},{"00000000010010",0,29},{"00000000010001",0,30},{"00000000010000",0,31},
    {"000000000011000",0,32},{"000000000010111",0,33},{"000000000010110",0,34},{"000000000010101",0,35},
    {"000000000010100",0,36},{"000000000010011",0,37},{"000000000010010",0,38},{"000000000010001",0,39},
    {"000000000010000",0,40},
    /* run 1 */ {"011",1,1},{"000110",1,2},{"00100101",1,3},{"0000001100",1,4},{"000000011011",1,5},
    {"0000000010110",1,6},{"0000000010101",1,7},{"000000000011111",1,8},{"000000000011110",1,9},
    {"000000000011101",1,10},{"000000000011100",1,11},{"000000000011011",1,12},{"000000000011010",1,13},
    {"000000000011001",1,14},{"0000000000010011",1,15},{"0000000000010010",1,16},{"0000000000010001",1,17},
    {"0000000000010000",1,18},
    /* run 2 */ {"0101",2,1},{"0000100",2,2},{"0000001011",2,3},{"000000010100",2,4},{"0000000010100",2,5},
    /* run 3 */ {"00111",3,1},{"00100100",3,2},{"000000011100",3,3},{"0000000010011",3,4},
    /* run 4 */ {"00110",4,1},{"0000001111",4,2},{"000000010010",4,3},
    /* run 5 */ {"000111",5,1},{"0000001001",5,2},{"0000000010010",5,3},
    /* run 6 */ {"000101",6,1},{"000000011110",6,2},{"0000000000010100",6,3},
    /* run 7 */ {"000100",7,1},{"000000010101",7,2},
    /* run 8 */ {"0000111",8,1},{"000000010001",8,2},
    /* run 9 */ {"0000101",9,1},{"0000000010001",9,2},
    /* run 10 */ {"00100111",10,1},{"0000000010000",10,2},
    /* run 11 */ {"00100011",11,1},{"0000000000011010",11,2},
    /* run 12 */ {"00100010",12,1},{"0000000000011001",12,2},
    /* run 13 */ {"00100000",13,1},{"0000000000011000",13,2},
    /* run 14 */ {"0000001110",14,1},{"0000000000010111",14,2},
    /* run 15 */ {"0000001101",15,1},{"0000000000010110",15,2},
    /* run 16 */ {"0000001000",16,1},{"0000000000010101",16,2},
    /* run 17..21 */ {"000000011111",17,1},{"000000011010",18,1},{"000000011001",19,1},{"000000010111",20,1},
    {"000000010110",21,1},
    /* run 22..26 */ {"0000000011111",22,1},{"0000000011110",23,1},{"0000000011101",24,1},{"0000000011100",25,1},
    {"0000000011011",26,1},
    /* run 27..31 */ {"0000000000011111",27,1},{"0000000000011110",28,1},{"0000000000011101",29,1},
    {"0000000000011100",30,1},{"0000000000011011",31,1},
};
/* the version-3 DC size codes (MPEG-1 dct_dc_size): luma then chroma */
static const struct { const char *code; u8 size; } dc_luma[] = {
    {"00",1},{"01",2},{"100",0},{"101",3},{"110",4},{"1110",5},{"11110",6},{"111110",7},{"1111110",8},
};
static const struct { const char *code; u8 size; } dc_chroma[] = {
    {"00",0},{"01",1},{"10",2},{"110",3},{"1110",4},{"11110",5},{"111110",6},{"1111110",7},{"11111110",8},
};
#define AC_EOB 0xFE
#define AC_ESC 0xFD
/* lookup on the next 16 bits: length (0 = no code), run (AC_EOB / AC_ESC for the two specials), level */
static u8 ac_len[65536], ac_run[65536], ac_lvl[65536];
static u8 dcl_len[256], dcl_size[256], dcc_len[256], dcc_size[256];   /* on the next 8 bits */

static void lut_fill16(u8 *len, u8 *a, u8 *b, const char *code, u8 va, u8 vb) {
    int l = (int)strlen(code); u32 v = 0;
    for (const char *c = code; *c; c++) v = v * 2 + (*c - '0');
    u32 lo = v << (16 - l), hi = lo + (1u << (16 - l));
    for (u32 i = lo; i < hi; i++) { len[i] = (u8)l; a[i] = va; b[i] = vb; }
}
static void lut_fill8(u8 *len, u8 *a, const char *code, u8 va) {
    int l = (int)strlen(code); u32 v = 0;
    for (const char *c = code; *c; c++) v = v * 2 + (*c - '0');
    u32 lo = v << (8 - l), hi = lo + (1u << (8 - l));
    for (u32 i = lo; i < hi; i++) { len[i] = (u8)l; a[i] = va; }
}
static int tables_ready;
static void tables_init(void) {
    if (tables_ready) return;
    tables_ready = 1;
    for (unsigned i = 0; i < sizeof ac_codes / sizeof ac_codes[0]; i++) {
        u32 v = 0; const char *c = ac_codes[i].code; int l = (int)strlen(c);
        for (; *c; c++) v = v * 2 + (*c - '0');
        if (ac_len[v << (16 - l)]) port_printf("[mdec] AC table: code %s is not prefix-free\n", ac_codes[i].code);
        lut_fill16(ac_len, ac_run, ac_lvl, ac_codes[i].code, ac_codes[i].run, ac_codes[i].level);
    }
    lut_fill16(ac_len, ac_run, ac_lvl, "10", AC_EOB, 0);
    lut_fill16(ac_len, ac_run, ac_lvl, "000001", AC_ESC, 0);
    for (unsigned i = 0; i < sizeof dc_luma / sizeof dc_luma[0]; i++) lut_fill8(dcl_len, dcl_size, dc_luma[i].code, dc_luma[i].size);
    for (unsigned i = 0; i < sizeof dc_chroma / sizeof dc_chroma[0]; i++) lut_fill8(dcc_len, dcc_size, dc_chroma[i].code, dc_chroma[i].size);
    for (int k = 0; k < 64; k++) iq_y[k] = iq_c[k] = k == 0 ? 2 : quant_raster[zigzag[k]];
    /* cos((2x+1) u pi / 16) C(u) / 2 in 12-bit fixed point, from the values of cos(k pi/16), k = 0..7
       (0.5 * 4096 = 2048; C(0) = 1/sqrt 2) */
    static const s32 cosk[8] = { 4096, 4017, 3784, 3406, 2896, 2276, 1567, 799 };   /* cos(k pi/16) * 4096 */
    for (int u = 0; u < 8; u++)
        for (int x = 0; x < 8; x++) {
            int k = ((2 * x + 1) * u) % 32;          /* cos(k pi/16), k in 0..31 */
            s32 c = k < 8 ? cosk[k] : k < 16 ? -cosk[16 - k] : k < 24 ? -cosk[k - 16] : cosk[32 - k];
            if (k == 8 || k == 24) c = 0;
            if (u == 0) c = (s32)(c * 2896 / 4096);  /* C(0) = 1 / sqrt 2 */
            idct_cos[u][x] = c / 2;                   /* the 1/4 of the 2-D transform, half per pass */
        }
}

/* ------------------------------------------------------------------ bit reader (16-bit words, MSB first) */
typedef struct { const u8 *p, *end; u32 win; int n; int over; } BR;
static void br_fill(BR *b) {
    while (b->n <= 16) {
        u32 w = 0;
        if (b->p + 2 <= b->end) { w = b->p[0] | (u32)b->p[1] << 8; b->p += 2; }
        else b->over += 16;                           /* past the data: zeros (the caller stops on that) */
        b->win |= w << (16 - b->n);
        b->n += 16;
    }
}
static u32 br_peek(BR *b, int k) { br_fill(b); return b->win >> (32 - k); }
static void br_skip(BR *b, int k) { b->win <<= k; b->n -= k; }
static u32 br_get(BR *b, int k) { u32 v = br_peek(b, k); br_skip(b, k); return v; }

/* ------------------------------------------------------------------ DecDCTvlc: BS frame -> run-length codes */
static int vlc_warned;
/* one 8x8 block: writes its codes, returns 0 at an undecodable code */
static int vlc_block(BR *b, int ver, int chroma, int *dc_pred, int q, u16 **out, u16 *out_end) {
    u16 *o = *out;
    int dc;
    if (ver >= 3) {
        u32 pk = br_peek(b, 8);
        int len = chroma ? dcc_len[pk] : dcl_len[pk], size = chroma ? dcc_size[pk] : dcl_size[pk];
        if (!len) return 0;
        br_skip(b, len);
        int diff = 0;
        if (size) {
            u32 v = br_get(b, size);
            diff = (v & (1u << (size - 1))) ? (int)v : (int)v - (1 << size) + 1;
        }
        *dc_pred += diff * 4;                        /* the differences are in units of 4 */
        dc = *dc_pred;
    } else {
        u32 v = br_get(b, 10);
        dc = (int)(v << 22) >> 22;
    }
    if (o >= out_end) return 0;
    *o++ = (u16)((q << 10) | (dc & 0x3FF));
    for (int k = 0; k < 64;) {
        u32 pk = br_peek(b, 16);
        int len = ac_len[pk];
        if (!len) return 0;
        br_skip(b, len);
        int run, level;
        if (ac_run[pk] == AC_EOB) { if (o >= out_end) return 0; *o++ = 0xFE00; *out = o; return 1; }
        if (ac_run[pk] == AC_ESC) {
            run = (int)br_get(b, 6);
            level = (int)(br_get(b, 10) << 22) >> 22;
        } else {
            run = ac_run[pk]; level = ac_lvl[pk];
            if (br_get(b, 1)) level = -level;
        }
        k += run + 1;
        if (o >= out_end) return 0;
        *o++ = (u16)((run << 10) | (level & 0x3FF));
    }
    /* 64 coefficients without an end-of-block code: close the block as the hardware would read it */
    if (o >= out_end) return 0;
    *o++ = 0xFE00; *out = o;
    return 1;
}
void DecDCTvlc(u32 *bs, u32 *buf) {
    tables_init();
    const u16 *h = (const u16 *)bs;
    int nwords = h[0], q = h[2] & 0x3F, ver = h[3];
    int w = 0, hgt = 0, bytes = 0, mbs = 0, known = port_st_frame_dims(bs, &w, &hgt, &bytes);
    if (known) mbs = ((w + 15) / 16) * ((hgt + 15) / 16);      /* 0: the end-of-stream placeholder, nothing to decode */
    BR b; b.p = (const u8 *)bs + 8; b.end = bytes > 8 ? (const u8 *)bs + bytes : (const u8 *)bs + 0x10000; b.win = 0; b.n = 0; b.over = 0;
    u16 *o = (u16 *)(buf + 1), *out_end = o + 2 * 0x3FE0;     /* the game's VLC buffers are 64 KB */
    int dc[3] = { 0, 0, 0 }, done = 0, ok = 1;
    for (; (known ? done < mbs : 1) && ok; done++) {
        u16 *mark = o;
        for (int blk = 0; blk < 6 && ok; blk++) {
            int chroma = blk < 2;          /* Cr, Cb, Y1, Y2, Y3, Y4 */
            ok = vlc_block(&b, ver, chroma, &dc[blk < 2 ? blk : 2], q, &o, out_end);
            if (ok && b.over > 16) ok = 0;  /* read past the end of the frame data */
        }
        if (!ok) { o = mark; break; }
    }
    if (mbs && done < mbs && vlc_warned++ < 5)
        port_printf("[mdec] frame %dx%d version %d: %d of %d macroblocks decoded (%d bytes of data)\n", w, hgt, ver, done, mbs, bytes);
    /* pad to the next multiple of 32 words, then to the size the frame header announces (what the real DecDCTvlc
       emits and what DecDCTin hands the decoder) */
    int hw = (int)(o - (u16 *)(buf + 1));
    int words = (hw + 1) / 2;
    words = (words + 31) & ~31;
    if (nwords > words && nwords <= 0x3FE0) words = nwords;
    while (o < (u16 *)(buf + 1) + 2 * words) *o++ = 0xFE00;
    buf[0] = 0x38000000u | (u32)words;
}
int DecDCTBufSize(u32 *bs) { return *(u16 *)bs; }

/* ------------------------------------------------------------------ the decoder */
static const u16 *in_p, *in_end;       /* the run-length codes of DecDCTin */
static int in_depth, in_bit15, in_signed;
static void (*out_cb)(void);
static int in_cb, have_pending; static u32 *pending_buf; static int pending_n;

typedef struct { u8 iq_y[64], iq_c[64]; s16 dct[64]; } DECDCTENV;
void DecDCTReset(int mode) { tables_init(); in_p = in_end = 0; (void)mode; }
DECDCTENV *DecDCTGetEnv(DECDCTENV *env) { tables_init(); memcpy(env->iq_y, iq_y, 64); memcpy(env->iq_c, iq_c, 64); memset(env->dct, 0, sizeof env->dct); return env; }
DECDCTENV *DecDCTPutEnv(DECDCTENV *env) { tables_init(); memcpy(iq_y, env->iq_y, 64); memcpy(iq_c, env->iq_c, 64); return env; }
void DecDCTin(u32 *buf, int mode) {
    tables_init();
    if (mode & 1) buf[0] &= ~0x08000000u; else buf[0] |= 0x08000000u;
    if (mode & 2) buf[0] |= 0x02000000u; else buf[0] &= ~0x02000000u;
    u32 cmd = buf[0];
    in_depth = (cmd >> 27) & 3; in_signed = (cmd >> 26) & 1; in_bit15 = (cmd >> 25) & 1;
    in_p = (const u16 *)(buf + 1); in_end = in_p + 2 * (cmd & 0xFFFF);
}
int DecDCTinSync(int mode) { (void)mode; return 0; }
int DecDCToutSync(int mode) { (void)mode; return 0; }
void *DecDCToutCallback(void *f) { void *o = (void *)out_cb; out_cb = (void (*)(void))f; return o; }
void *DecDCTinCallback(void *f) { (void)f; return 0; }

/* one block of codes -> 64 coefficients (raster order) -> 64 samples (-128..127) */
static int rl_block(const u8 *qt, s32 *blk, s32 *px) {
    memset(blk, 0, 64 * sizeof *blk);
    while (in_p < in_end && *in_p == 0xFE00) in_p++;
    if (in_p >= in_end) return 0;
    u32 n = *in_p++;
    int qs = (n >> 10) & 0x3F, val = (int)(n << 22) >> 22;
    val = qs ? val * qt[0] : val * 2;
    val = val < -0x400 ? -0x400 : val > 0x3FF ? 0x3FF : val;
    blk[0] = val;
    for (int k = 0; in_p < in_end;) {
        n = *in_p++;
        if (n == 0xFE00) break;
        k += ((n >> 10) & 0x3F) + 1;
        if (k > 63) break;
        val = (int)(n << 22) >> 22;
        val = qs ? (val * qt[k] * qs + 4) >> 3 : val * 2;
        val = val < -0x400 ? -0x400 : val > 0x3FF ? 0x3FF : val;
        blk[qs ? zigzag[k] : k] = val;
    }
    /* separable IDCT: rows (frequency u along x) then columns */
    s32 t[64];
    for (int v = 0; v < 8; v++) {
        const s32 *row = blk + v * 8;
        for (int x = 0; x < 8; x++) {
            s32 s = 0;
            for (int u = 0; u < 8; u++) if (row[u]) s += row[u] * idct_cos[u][x];
            t[v * 8 + x] = (s + 2048) >> 12;
        }
    }
    for (int x = 0; x < 8; x++)
        for (int y = 0; y < 8; y++) {
            s32 s = 0;
            for (int v = 0; v < 8; v++) if (t[v * 8 + x]) s += t[v * 8 + x] * idct_cos[v][y];
            s = (s + 2048) >> 12;
            px[y * 8 + x] = s < -128 ? -128 : s > 127 ? 127 : s;
        }
    return 1;
}
/* one macroblock: 16x16 pixels into out (16- or 24-bit); returns the words written */
static int decode_mb(u8 *out) {
    s32 blk[64], cr[64], cb[64], y[4][64];
    int ok = rl_block(iq_c, blk, cr);
    ok &= rl_block(iq_c, blk, cb);
    for (int i = 0; i < 4; i++) ok &= rl_block(iq_y, blk, y[i]);
    if (!ok) {   /* out of codes: black, as a stalled hardware output would be read */
        memset(cr, 0, sizeof cr); memset(cb, 0, sizeof cb);
        for (int i = 0; i < 4; i++) for (int j = 0; j < 64; j++) y[i][j] = -128;
    }
    u8 *o = out;
    for (int py = 0; py < 16; py++)
        for (int px = 0; px < 16; px++) {
            int Y = y[(py >> 3) * 2 + (px >> 3)][(py & 7) * 8 + (px & 7)];
            int c = (py >> 1) * 8 + (px >> 1);
            int Cr = cr[c], Cb = cb[c];
            int r = Y + ((1436 * Cr) >> 10), g = Y - ((352 * Cb + 731 * Cr) >> 10), b = Y + ((1815 * Cb) >> 10);
            if (in_signed) {
                r = r < -128 ? -128 : r > 127 ? 127 : r; g = g < -128 ? -128 : g > 127 ? 127 : g; b = b < -128 ? -128 : b > 127 ? 127 : b;
                r &= 0xFF; g &= 0xFF; b &= 0xFF;
            } else {
                r += 128; g += 128; b += 128;
                r = r < 0 ? 0 : r > 255 ? 255 : r; g = g < 0 ? 0 : g > 255 ? 255 : g; b = b < 0 ? 0 : b > 255 ? 255 : b;
            }
            if (in_depth == 2) { *o++ = (u8)r; *o++ = (u8)g; *o++ = (u8)b; }
            else { u16 p = (u16)((r >> 3) | (g >> 3) << 5 | (b >> 3) << 10 | in_bit15 << 15); *o++ = (u8)p; *o++ = (u8)(p >> 8); }
        }
    return (int)(o - out) / 4;
}
static void decode_words(u32 *buf, int n) {
    u8 *o = (u8 *)buf; int left = n;
    int mb_words = in_depth == 2 ? 192 : 128;
    while (left >= mb_words) { decode_mb(o); o += mb_words * 4; left -= mb_words; }
    if (left > 0) {   /* a partial macroblock: the hardware would hold the rest for the next request */
        u8 tmp[768]; decode_mb(tmp); memcpy(o, tmp, (size_t)left * 4);
    }
}
void DecDCTout(u32 *buf, int n) {
    if (in_cb) { pending_buf = buf; pending_n = n; have_pending = 1; return; }
    for (;;) {
        decode_words(buf, n);
        if (!out_cb) return;
        in_cb = 1; have_pending = 0;
        out_cb();
        in_cb = 0;
        if (!have_pending) return;
        buf = pending_buf; n = pending_n;
    }
}
