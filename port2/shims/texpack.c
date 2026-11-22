/* Texture dumping and replacement packs (port only, opt-in; docs/PORT.md "Texture packs").
 *
 *   --dump-textures DIR   every texture the game draws with is written once as DIR/<folder>/<key>.png (RGBA) and
 *                         described in DIR/textures.jsonl
 *   --texture-pack DIR    (OpenGL renderer) a PNG of the pack whose file name is such a key is drawn instead of the
 *                         original texels, at any integer scale of the original (1x .. 16x: HD packs)
 *
 * The logical texture.  Every host write to VRAM (LoadImage, LoadClut, the GP0 A0 upload) is remembered as an "upload":
 * its rectangle and the 64-bit FNV-1a of its words.  A textured primitive samples a box of its texture page (the
 * bounding box of its u/v, after the texture window); the smallest live upload that contains that box in VRAM is its
 * texture, and the key is FNV-1a(upload hash, depth, the CLUT words for 4/8-bit pages).  So a page uploaded with the
 * same pixels always gets the same name, whatever the area, the frame or the VRAM position it went to, and each
 * palette it is drawn with is its own image (a palette animation gives keys the pack does not have: the original is
 * drawn).  The image is the whole upload decoded with that CLUT (4-bit: 4 texels per VRAM word, 8-bit: 2).
 * When no single upload holds the box (a page assembled from several uploads, a sprite across two of them) the box
 * itself is the texture: key "sub_" + FNV-1a(depth, size, texels, CLUT), image = just the box.  Boxes in VRAM the
 * game draws into (anything that was the drawing area while textured primitives were drawn) are never keyed.
 *
 * Uploads go stale when a fill, a copy or a write that does not fit inside them touches them (a smaller write inside a
 * larger upload is a sub-upload: both stay, the smaller one wins for boxes inside it); a VRAM copy of a whole upload
 * (MoveImage) carries its key along.
 *
 * Pixels: 5-bit channels -> 8 bits (k << 3 | k >> 2); alpha 0 for the word 0x0000 (transparent on the PS1), 128 for a
 * word with bit 15 (STP) set (drawn semi-transparent by semi-transparent primitives), 255 otherwise.  Replacements are
 * read back with the same classes: alpha < 64 transparent, 64..191 STP, >= 192 opaque.
 *
 * The PNG writer uses fixed-Huffman deflate with a simple LZ77 matcher, the reader is a small inflate (8-bit grey,
 * RGB, RGBA, palette, grey+alpha; no interlace): no zlib dependency, like the frame dumps of gpu.c. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#ifdef _WIN32
#include <direct.h>
#endif
#include "port_runtime.h"
#include "gpu_backend.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

const char *port_texdump_dir, *port_texpack_dir;
int port_texpack_on;                 /* either is set (gpu.c calls the hooks below only then) */
unsigned port_gl_rep_texture(const u8 *rgba, int w, int h);   /* gpu_gl.c: 0 when the GL backend is not running */

/* FNV-1a over n bytes, continuing from h */
static u64 fnv(u64 h, const void *p, size_t n) {
    const u8 *b = p;
    while (n--) {
        h ^= *b++;
        h *= 0x100000001B3ull;
    }
    return h;
}
#define FNV0 0xCBF29CE484222325ull

/* ------------------------------------------------------------------ uploads */
#define MAX_UP 2048
#define MAX_HOLES 8
typedef struct { short x, y, w, h; u64 hash; u16 *data; int nkeys; u8 live, nholes; short hole[MAX_HOLES][4]; } Up;
static Up ups[MAX_UP];
static int n_ups;
static unsigned epoch;               /* bumped on every VRAM write the uploads see: the CLUT hash cache key */

/* the rectangles a and b overlap */
static int ov(int ax, int ay, int aw, int ah, int bx, int by, int bw, int bh) {
    return ax < bx + bw && bx < ax + aw && ay < by + bh && by < ay + ah;
}
static int inside(int ax, int ay, int aw, int ah, int bx, int by, int bw, int bh) {   /* a inside b */
    return ax >= bx && ay >= by && ax + aw <= bx + bw && ay + ah <= by + bh;
}
static void up_kill(Up *u) {
    u->live = 0;
    free(u->data);
    u->data = 0;
}
static Up *up_new(void) {
    for (int i = 0; i < n_ups; i++)
        if (!ups[i].live) return &ups[i];
    if (n_ups < MAX_UP) return &ups[n_ups++];
    up_kill(&ups[0]);
    return &ups[0];          /* (never seen: VRAM holds far fewer) */
}
/* the host wrote (x, y, w, h) with src (LoadImage), or changed it otherwise (src 0: a fill) */
void port_texpack_write(int x, int y, int w, int h, const u16 *src) {
    if (w <= 0 || h <= 0) return;
    epoch++;
    if (x + w > 1024 || y + h > 512) src = 0;  /* wrapping: only invalidate */
    for (int i = 0; i < n_ups; i++) {
        Up *u = &ups[i];
        if (!u->live || !ov(x, y, w, h, u->x, u->y, u->w, u->h)) continue;
        if (x <= u->x && y <= u->y && x + w >= u->x + u->w && y + h >= u->y + u->h) {
            up_kill(u);
            continue;
        }
        /* a part of it changed: that part is a hole (boxes over it are not this upload any more), the rest stays */
        if (u->nholes == MAX_HOLES) {
            up_kill(u);
            continue;
        }
        short *new_hole = u->hole[u->nholes++];
        new_hole[0] = (short)x;
        new_hole[1] = (short)y;
        new_hole[2] = (short)w;
        new_hole[3] = (short)h;
    }
    if (!src && getenv("TEXPACK_LOG")) port_printf("[texpack] f%d inval %d,%d %dx%d\n", port_frame, x, y, w, h);
    if (!src) return;
    if (getenv("TEXPACK_LOG")) port_printf("[texpack] f%d upload %d,%d %dx%d\n", port_frame, x, y, w, h);
    Up *u = up_new();
    u->x = (short)x;
    u->y = (short)y;
    u->w = (short)w;
    u->h = (short)h;
    u->live = 1;
    u->nkeys = 0;
    u->nholes = 0;
    u->hash = fnv(FNV0, src, (size_t)w * h * 2);
    u->data = 0;
    if (port_texdump_dir) {
        u->data = malloc((size_t)w * h * 2);
        if (u->data) memcpy(u->data, src, (size_t)w * h * 2);
    }
}
/* a VRAM to VRAM copy: invalidates the destination; a copy of a whole upload carries the upload (and its key) along */
void port_texpack_copy(int sx, int sy, int dx, int dy, int w, int h) {
    if (getenv("TEXPACK_LOG"))
        port_printf("[texpack] f%d copy %d,%d -> %d,%d %dx%d\n", port_frame, sx, sy, dx, dy, w, h);
    Up *src = 0;
    for (int i = 0; i < n_ups; i++)
        if (ups[i].live && ups[i].x == sx && ups[i].y == sy && ups[i].w == w && ups[i].h == h) {
            src = &ups[i];
            break;
        }
    Up keep;
    if (src) {
        keep = *src;
        if (keep.data) {
            keep.data = malloc((size_t)w * h * 2);
            if (keep.data) memcpy(keep.data, src->data, (size_t)w * h * 2);
        }
    }
    port_texpack_write(dx, dy, w, h, 0);
    if (src && dx + w <= 1024 && dy + h <= 512) {
        Up *u = up_new();
        *u = keep;
        u->x = (short)dx;
        u->y = (short)dy;
        u->nkeys = 0;
    } else if (src)
        free(keep.data);
}

/* ------------------------------------------------------------------ CLUT hashes */
/* (cached per position until VRAM changes) */
static u64 clut_hash(const u16 *vram, int cx, int cy, int n) {
    static struct { int cx, cy, n; unsigned ep; u64 h; } cache[64];
    int slot = ((cx >> 4) * 7 + cy * 13 + n) & 63;
    if (cache[slot].ep == epoch + 1 && cache[slot].cx == cx && cache[slot].cy == cy && cache[slot].n == n)
        return cache[slot].h;
    u16 pal[256];
    for (int i = 0; i < n; i++) pal[i] = vram[(cy & 511) * 1024 + ((cx + i) & 1023)];
    u64 h = fnv(FNV0, pal, (size_t)n * 2);
    cache[slot].cx = cx;
    cache[slot].cy = cy;
    cache[slot].n = n;
    cache[slot].ep = epoch + 1;
    cache[slot].h = h;
    return h;
}

/* ------------------------------------------------------------------ PNG out (RGBA8, fixed-Huffman deflate) */
static u32 crc_tab[256];
static u32 crc32(u32 c, const u8 *p, size_t n) {
    if (!crc_tab[1])
        for (u32 i = 0; i < 256; i++) {
            u32 k = i;
            for (int j = 0; j < 8; j++) k = k & 1 ? 0xEDB88320u ^ (k >> 1) : k >> 1;
            crc_tab[i] = k;
        }
    c = ~c;
    while (n--) c = crc_tab[(c ^ *p++) & 255] ^ (c >> 8);
    return ~c;
}
static void be32(u8 *p, u32 v) {
    p[0] = (u8)(v >> 24);
    p[1] = (u8)(v >> 16);
    p[2] = (u8)(v >> 8);
    p[3] = (u8)v;
}
/* one PNG chunk: length, type, data, CRC */
static void chunk(FILE *fd, const char *type, const u8 *data, u32 n) {
    u8 chunk_hdr[8];
    be32(chunk_hdr, n);
    memcpy(chunk_hdr + 4, type, 4);
    fwrite(chunk_hdr, 1, 8, fd);
    u32 c = crc32(0, chunk_hdr + 4, 4);
    if (n) {
        fwrite(data, 1, n, fd);
        c = crc32(c, data, n);
    }
    be32(chunk_hdr, c);
    fwrite(chunk_hdr, 1, 4, fd);
}
typedef struct { u8 *p; size_t n; u32 bits; int nb; } BitW;
static void putb(BitW *w, u32 v, int n) {          /* LSB first */
    w->bits |= v << w->nb;
    w->nb += n;
    while (w->nb >= 8) {
        w->p[w->n++] = (u8)w->bits;
        w->bits >>= 8;
        w->nb -= 8;
    }
}
static void puth(BitW *w, u32 code, int n) {       /* a Huffman code: MSB first */
    u32 reversed = 0;
    for (int i = 0; i < n; i++) reversed |= ((code >> i) & 1) << (n - 1 - i);
    putb(w, reversed, n);
}
/* a literal / length symbol (0..287) in the fixed Huffman code */
static void put_lit(BitW *w, int s) {
    if (s < 144)
        puth(w, 0x30 + s, 8);
    else if (s < 256)
        puth(w, 0x190 + s - 144, 9);
    else if (s < 280)
        puth(w, s - 256, 7);
    else
        puth(w, 0xC0 + s - 280, 8);
}
static const u16 len_base[29] = { 3,4,5,6,7,8,9,10,11,13,15,17,19,23,27,31,35,43,51,59,67,83,99,115,131,163,195,227,258 };
static const u8 len_extra[29] = { 0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,0 };
static const u16 dist_base[30] = { 1,2,3,4,5,7,9,13,17,25,33,49,65,97,129,193,257,385,513,769,1025,1537,2049,3073,4097,6145,8193,12289,16385,24577 };
static const u8 dist_extra[30] = { 0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13 };
/* one fixed-Huffman deflate block of in (n bytes) into out; returns its size.  LZ77 matches from hash chains of
   3-byte prefixes (head: the last position of each hash, prev: the one before each position), 32 tries a byte */
static size_t deflate_fixed(const u8 *in, size_t n, u8 *out) {
    static int head[1 << 15];
    static int *prev;
    static size_t prev_cap;
    BitW w = { out, 0, 0, 0 };
    if (n > prev_cap) {
        free(prev);
        prev = malloc(n * sizeof *prev);
        prev_cap = prev ? n : 0;
    }
    for (int i = 0; i < (1 << 15); i++) head[i] = -1;
    putb(&w, 1, 1);
    putb(&w, 1, 2);                 /* final block, fixed Huffman */
    size_t i = 0;
    while (i < n) {
        int best = 0,
            best_dist = 0;
        if (i + 3 <= n && prev) {
            u32 bucket = ((u32)in[i] << 10 ^ (u32)in[i + 1] << 5 ^ in[i + 2]) & 0x7FFF;
            int cand = head[bucket],
                tries = 32;
            while (cand >= 0 && i - (size_t)cand <= 32768 && tries--) {
                int match_len = 0,
                    max = n - i < 258 ? (int)(n - i) : 258;
                while (match_len < max && in[cand + match_len] == in[i + match_len]) match_len++;
                if (match_len > best) {
                    best = match_len;
                    best_dist = (int)(i - cand);
                    if (match_len == max) break;
                }
                cand = prev[cand];
            }
            prev[i] = head[bucket];
            head[bucket] = (int)i;
        }
        if (best >= 3) {
            int len_code = 28;
            while (len_base[len_code] > best) len_code--;
            put_lit(&w, 257 + len_code);
            putb(&w, best - len_base[len_code], len_extra[len_code]);
            int dist_code = 29;
            while (dist_base[dist_code] > best_dist) dist_code--;
            puth(&w, dist_code, 5);
            putb(&w, best_dist - dist_base[dist_code], dist_extra[dist_code]);
            for (int k = 1; k < best; k++) {          /* index the skipped positions too */
                size_t j = i + k;
                if (j + 3 <= n) {
                    u32 bucket = ((u32)in[j] << 10 ^ (u32)in[j + 1] << 5 ^ in[j + 2]) & 0x7FFF;
                    prev[j] = head[bucket];
                    head[bucket] = (int)j;
                }
            }
            i += best;
        } else {
            put_lit(&w, in[i]);
            i++;
        }
    }
    put_lit(&w, 256);
    if (w.nb) putb(&w, 0, 8 - w.nb);
    return w.n;
}
static int write_png_rgba(const char *path, const u8 *rgba, int w, int h) {
    size_t stride = (size_t)w * 4 + 1,
           total = stride * h;
    u8 *raw = malloc(total),
       *zbuf = malloc(total + total / 8 + 64);
    if (!raw || !zbuf) {
        free(raw);
        free(zbuf);
        return -1;
    }
    for (int y = 0; y < h; y++) {                   /* filter 1 (Sub): runs of the same colour become zeros */
        u8 *r = raw + y * stride;
        const u8 *s = rgba + (size_t)y * w * 4;
        r[0] = 1;
        for (int x = 0; x < w * 4; x++) r[1 + x] = (u8)(s[x] - (x >= 4 ? s[x - 4] : 0));
    }
    /* the zlib stream: header, the deflate block, the Adler-32 of the raw rows */
    zbuf[0] = 0x78;
    zbuf[1] = 0x01;
    size_t zlen = 2 + deflate_fixed(raw, total, zbuf + 2);
    u32 a = 1,
        b = 0;
    for (size_t i = 0; i < total; i++) {
        a = (a + raw[i]) % 65521;
        b = (b + a) % 65521;
    }
    be32(zbuf + zlen, (b << 16) | a);
    zlen += 4;
    FILE *fd = fopen(path, "wb");
    if (!fd) {
        free(raw);
        free(zbuf);
        return -1;
    }
    static const u8 sig[8] = { 137, 'P', 'N', 'G', 13, 10, 26, 10 };
    fwrite(sig, 1, 8, fd);
    u8 ihdr[13];
    be32(ihdr, (u32)w);
    be32(ihdr + 4, (u32)h);
    ihdr[8] = 8;
    ihdr[9] = 6;
    ihdr[10] = ihdr[11] = ihdr[12] = 0;
    chunk(fd, "IHDR", ihdr, 13);
    chunk(fd, "IDAT", zbuf, (u32)zlen);
    chunk(fd, "IEND", 0, 0);
    fclose(fd);
    free(raw);
    free(zbuf);
    return 0;
}

/* ------------------------------------------------------------------ PNG in (inflate) */
typedef struct { const u8 *p; size_t n, pos; u32 bits; int nb; int err; } BitR;
/* the next n bits, LSB first (0 and err set past the end) */
static int getb(BitR *r, int n) {
    while (r->nb < n) {
        if (r->pos >= r->n) {
            r->err = 1;
            return 0;
        }
        r->bits |= (u32)r->p[r->pos++] << r->nb;
        r->nb += 8;
    }
    int v = (int)(r->bits & ((1u << n) - 1));
    r->bits >>= n;
    r->nb -= n;
    return v;
}
/* a canonical Huffman code: the number of codes of each length, the symbols in code order */
typedef struct { short count[16], sym[320]; } Huff;
static void huff_build(Huff *h, const u8 *len, int n) {
    short offs[16];
    memset(h->count, 0, sizeof h->count);
    for (int i = 0; i < n; i++) h->count[len[i]]++;
    h->count[0] = 0;
    offs[1] = 0;
    for (int i = 1; i < 15; i++) offs[i + 1] = (short)(offs[i] + h->count[i]);
    for (int i = 0; i < n; i++)
        if (len[i]) h->sym[offs[len[i]]++] = (short)i;
}
static int huff_dec(BitR *r, const Huff *h) {
    int code = 0,
        first = 0,
        idx = 0;
    for (int l = 1; l < 16; l++) {
        code |= getb(r, 1);
        int c = h->count[l];
        if (code - c < first) return h->sym[idx + (code - first)];
        idx += c;
        first += c;
        first <<= 1;
        code <<= 1;
        if (r->err) return -1;
    }
    r->err = 1;
    return -1;
}
/* a zlib stream (n bytes, the 2-byte header skipped, the checksum not checked) into a new buffer of outcap bytes;
   *outn: the bytes it holds.  0 on bad data */
static u8 *inflate_z(const u8 *in, size_t n, size_t outcap, size_t *outn) {
    BitR r = { in + 2, n > 2 ? n - 2 : 0, 0, 0, 0, 0 };
    u8 *dst = malloc(outcap ? outcap : 1);
    size_t dst_len = 0;
    int last;
    if (!dst) return 0;
    do {
        last = getb(&r, 1);
        int type = getb(&r, 2);
        if (type == 0) {
            /* a stored block */
            r.bits = 0;
            r.nb = 0;
            if (r.pos + 4 > r.n) {
                r.err = 1;
                break;
            }
            int len = r.p[r.pos] | r.p[r.pos + 1] << 8;
            r.pos += 4;
            if (r.pos + len > r.n || dst_len + len > outcap) {
                r.err = 1;
                break;
            }
            memcpy(dst + dst_len, r.p + r.pos, len);
            dst_len += len;
            r.pos += len;
            continue;
        }
        Huff lit_huff, dist_huff;
        u8 lens[320];
        if (type == 1) {
            for (int i = 0; i < 288; i++) lens[i] = i < 144 ? 8 : i < 256 ? 9 : i < 280 ? 7 : 8;
            huff_build(&lit_huff, lens, 288);
            for (int i = 0; i < 30; i++) lens[i] = 5;
            huff_build(&dist_huff, lens, 30);
        } else if (type == 2) {
            /* dynamic Huffman codes: the code-length code first, then the literal and distance code lengths */
            static const u8 ord[19] = { 16,17,18,0,8,7,9,6,10,5,11,4,12,3,13,2,14,1,15 };
            int n_lit = getb(&r, 5) + 257,
                n_dist = getb(&r, 5) + 1,
                n_codelen = getb(&r, 4) + 4;
            u8 codelen_lens[19] = { 0 };
            Huff codelen_huff;
            for (int i = 0; i < n_codelen; i++) codelen_lens[ord[i]] = (u8)getb(&r, 3);
            huff_build(&codelen_huff, codelen_lens, 19);
            int i = 0;
            while (i < n_lit + n_dist && !r.err) {
                int s = huff_dec(&r, &codelen_huff);
                if (s < 16)
                    lens[i++] = (u8)s;
                else {
                    int rep = 0;
                    u8 v = 0;
                    if (s == 16) {
                        if (!i) {
                            r.err = 1;
                            break;
                        }
                        v = lens[i - 1];
                        rep = 3 + getb(&r, 2);
                    } else if (s == 17)
                        rep = 3 + getb(&r, 3);
                    else
                        rep = 11 + getb(&r, 7);
                    if (i + rep > n_lit + n_dist) {
                        r.err = 1;
                        break;
                    }
                    while (rep--) lens[i++] = v;
                }
            }
            if (r.err) break;
            huff_build(&lit_huff, lens, n_lit);
            huff_build(&dist_huff, lens + n_lit, n_dist);
        } else {
            r.err = 1;
            break;
        }
        for (;;) {
            int s = huff_dec(&r, &lit_huff);
            if (r.err || s < 0) {
                r.err = 1;
                break;
            }
            if (s < 256) {
                if (dst_len >= outcap) {
                    r.err = 1;
                    break;
                }
                dst[dst_len++] = (u8)s;
                continue;
            }
            if (s == 256) break;
            s -= 257;
            if (s >= 29) {
                r.err = 1;
                break;
            }
            int len = len_base[s] + getb(&r, len_extra[s]);
            int d = huff_dec(&r, &dist_huff);
            if (d < 0 || d >= 30) {
                r.err = 1;
                break;
            }
            size_t dist = dist_base[d] + getb(&r, dist_extra[d]);
            if (dist > dst_len || dst_len + len > outcap) {
                r.err = 1;
                break;
            }
            for (int k = 0; k < len; k++, dst_len++) dst[dst_len] = dst[dst_len - dist];
        }
    } while (!last && !r.err);
    if (r.err) {
        free(dst);
        return 0;
    }
    *outn = dst_len;
    return dst;
}
static u32 rd32(const u8 *p) { return (u32)p[0] << 24 | (u32)p[1] << 16 | (u32)p[2] << 8 | p[3]; }
/* read_png_rgba of a palette PNG (colour type 3): its indices, w * h bytes (caller frees) */
static u8 *png_last_idx;
static u8 *read_png_rgba(const char *path, int *pw, int *ph) {
    free(png_last_idx);
    png_last_idx = 0;
    FILE *f = fopen(path, "rb");
    if (!f) return 0;
    fseek(f, 0, SEEK_END);
    long file_len = ftell(f);
    fseek(f, 0, SEEK_SET);
    u8 *buf = file_len > 8 ? malloc((size_t)file_len) : 0;
    if (!buf || fread(buf, 1, (size_t)file_len, f) != (size_t)file_len) {
        fclose(f);
        free(buf);
        return 0;
    }
    fclose(f);
    u8 *idat = 0, *out = 0, *raw = 0;
    size_t in_n = 0;
    int w = 0,
        h = 0,
        color_type = -1,
        bit_depth = 0,
        interlace_method = 0;
    u8 pal[256][4];
    int npal = 0;
    for (int i = 0; i < 256; i++) {
        pal[i][0] = pal[i][1] = pal[i][2] = 0;
        pal[i][3] = 255;
    }
    if (memcmp(buf, "\x89PNG\r\n\x1a\n", 8)) goto done;
    for (size_t off = 8; off + 12 <= (size_t)file_len;) {
        u32 n = rd32(buf + off);
        const u8 *t = buf + off + 4, *d = buf + off + 8;
        if (off + 12 + n > (size_t)file_len) break;
        if (!memcmp(t, "IHDR", 4) && n >= 13) {
            w = (int)rd32(d);
            h = (int)rd32(d + 4);
            bit_depth = d[8];
            color_type = d[9];
            interlace_method = d[12];
        } else if (!memcmp(t, "PLTE", 4)) {
            npal = (int)(n / 3);
            for (int i = 0; i < npal && i < 256; i++) {
                pal[i][0] = d[i * 3];
                pal[i][1] = d[i * 3 + 1];
                pal[i][2] = d[i * 3 + 2];
            }
        } else if (!memcmp(t, "tRNS", 4) && color_type == 3) {
            for (u32 i = 0; i < n && i < 256; i++) pal[i][3] = d[i];
        } else if (!memcmp(t, "IDAT", 4)) {
            u8 *grown = realloc(idat, in_n + n);
            if (!grown) goto done;
            idat = grown;
            memcpy(idat + in_n, d, n);
            in_n += n;
        } else if (!memcmp(t, "IEND", 4))
            break;
        off += 12 + n;
    }
    int n_channels = color_type == 0   ? 1
                     : color_type == 2 ? 3
                     : color_type == 3 ? 1
                     : color_type == 4 ? 2
                     : color_type == 6 ? 4
                                       : 0;
    /* palette PNGs may pack 2/4/16 colours per byte */
    int low_depth = color_type == 3 && (bit_depth == 1 || bit_depth == 2 || bit_depth == 4);
    if (!idat || w <= 0 || h <= 0 || w > 16384 || h > 16384 || (bit_depth != 8 && !low_depth) || !n_channels
        || interlace_method) {
        port_printf("[texpack] %s: unsupported PNG (needs 8 bits per channel, not interlaced)\n", path);
        goto done;
    }
    size_t stride = low_depth ? ((size_t)w * bit_depth + 7) / 8 : (size_t)w * n_channels,
           rn;
    raw = inflate_z(idat, in_n, (stride + 1) * h, &rn);
    if (!raw || rn != (stride + 1) * h) {
        port_printf("[texpack] %s: bad PNG data\n", path);
        goto done;
    }
    for (int y = 0; y < h; y++) {                   /* unfilter in place */
        /* c: this row, p: the row above (0 for the first) */
        u8 *r = raw + y * (stride + 1),
           filter_type = r[0],
           *c = r + 1,
           *p = y ? raw + (y - 1) * (stride + 1) + 1 : 0;
        for (size_t x = 0; x < stride; x++) {
            int a = x >= (size_t)n_channels ? c[x - n_channels] : 0,
                b = p ? p[x] : 0,
                up_left = p && x >= (size_t)n_channels ? p[x - n_channels] : 0;
            switch (filter_type) {
            case 1:
                c[x] = (u8)(c[x] + a);
                break;
            case 2:
                c[x] = (u8)(c[x] + b);
                break;
            case 3:
                c[x] = (u8)(c[x] + ((a + b) >> 1));
                break;
            case 4: {
                int paeth = a + b - up_left,
                    pa = abs(paeth - a),
                    pb = abs(paeth - b),
                    pc = abs(paeth - up_left);
                c[x] = (u8)(c[x] + (pa <= pb && pa <= pc ? a : pb <= pc ? b : up_left));
                break;
            }
            default:
                break;
            }
        }
    }
    /* expand to one index byte per pixel, rows keep their filter byte slot */
    if (low_depth) {
        u8 *ex = malloc(((size_t)w + 1) * h);
        if (!ex) goto done;
        for (int y = 0; y < h; y++) {
            const u8 *r = raw + y * (stride + 1) + 1;
            u8 *o = ex + (size_t)y * (w + 1) + 1;
            for (int x = 0; x < w; x++) {
                int bit = x * bit_depth;
                o[x] = (u8)((r[bit >> 3] >> (8 - bit_depth - (bit & 7))) & ((1 << bit_depth) - 1));
            }
        }
        free(raw);
        raw = ex;
        stride = (size_t)w;
    }
    out = malloc((size_t)w * h * 4);
    if (!out) goto done;
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++) {
            const u8 *s = raw + y * (stride + 1) + 1 + (size_t)x * n_channels;
            u8 *o = out + ((size_t)y * w + x) * 4;
            switch (color_type) {
            case 0:
                o[0] = o[1] = o[2] = s[0];
                o[3] = 255;
                break;
            case 2:
                o[0] = s[0];
                o[1] = s[1];
                o[2] = s[2];
                o[3] = 255;
                break;
            case 3:
                memcpy(o, pal[s[0]], 4);
                break;
            case 4:
                o[0] = o[1] = o[2] = s[0];
                o[3] = s[1];
                break;
            default:
                memcpy(o, s, 4);
                break;
            }
        }
    if (color_type == 3 && (png_last_idx = malloc((size_t)w * h)))
        for (int y = 0; y < h; y++) memcpy(png_last_idx + (size_t)y * w, raw + y * (stride + 1) + 1, (size_t)w);
    *pw = w;
    *ph = h;
done:
    free(buf);
    free(idat);
    free(raw);
    return out;
}

/* ------------------------------------------------------------------ the pack: key -> file */
/* state 0 not loaded, 1 ok, 2 indexed, -1 unusable */
typedef struct { u64 key; char *path; int state; PortTexRep rep; u8 *idx; int iw, ih; } PackEnt;
/* open-addressing hash table of the pack's files by key */
static PackEnt *pack_ents;
static u32 pack_cap, pack_count;
static int pack_scanned, pack_has_sub;
static PackEnt *pack_find(u64 key) {
    if (!pack_cap) return 0;
    u32 slot_mask = pack_cap - 1,
        i = (u32)(key ^ (key >> 32)) & slot_mask;
    while (pack_ents[i].path) {
        if (pack_ents[i].key == key) return &pack_ents[i];
        i = (i + 1) & slot_mask;
    }
    return 0;
}
static void pack_add(u64 key, const char *path) {
    if (pack_count * 2 >= pack_cap) {
        /* grow (double) and re-insert the old entries */
        u32 old_cap = pack_cap;
        PackEnt *old_ents = pack_ents;
        pack_cap = old_cap ? old_cap * 2 : 1024;
        pack_ents = calloc(pack_cap, sizeof *pack_ents);
        pack_count = 0;
        for (u32 i = 0; i < old_cap; i++)
            if (old_ents[i].path) pack_add(old_ents[i].key, old_ents[i].path);
        free(old_ents);
    }
    if (pack_find(key)) return;                        /* the first file found wins */
    u32 slot_mask = pack_cap - 1,
        i = (u32)(key ^ (key >> 32)) & slot_mask;
    while (pack_ents[i].path) i = (i + 1) & slot_mask;
    pack_ents[i].key = key;
    pack_ents[i].path = strdup(path);
    pack_ents[i].state = 0;
    pack_count++;
}
/* "<16 hex>.png" or "sub_<16 hex>.png" (any case), also after a descriptive prefix ending in "__"
   ("tombi_corre__0123456789abcdef.png", tools/dump_all_textures.py); the "sub" keys get a separate space: key ^
   SUBMARK */
#define SUBMARK 0x5355425355425355ull
/* "r" keys (tools/dump_all_textures.py, computed from the disc files without running the game): FNV-1a(upload hash,
   depth, page part) without the CLUT, so one image replaces that page part with whatever palette it is drawn; an exact
   key wins over it */
#define ANYMARK 0x414E59414E59414Eull
/* "r<key>_U_V_W_H": a sprite rectangle of an "r" page part */
typedef struct { u64 key; int u, v, w, h; char *path; } Crop;
static Crop *crops;
static int n_crops, cap_crops;
/* a file name that starts with a key: 0 none, 1 an upload key, 2 a "sub" key, 3 an "r" crop (crop: u, v, w, h) */
static int parse_key(const char *n, u64 *key, int *crop) {
    int sub = 0,
        any = 0;
    u64 value = 0;
    if (!strncmp(n, "sub_", 4) || !strncmp(n, "SUB_", 4)) {
        sub = 1;
        n += 4;
    } else if (n[0] == 'r' || n[0] == 'R') {          /* "r<16 hex>": the page part, any palette */
        any = 1;
        n++;
    }
    for (int i = 0; i < 16; i++) {
        char c = n[i];
        int v = c >= '0' && c <= '9'   ? c - '0'
                : c >= 'a' && c <= 'f' ? c - 'a' + 10
                : c >= 'A' && c <= 'F' ? c - 'A' + 10
                                       : -1;
        if (v < 0) return 0;
        value = value << 4 | (u64)v;
    }
    n += 16;
    crop[0] = -1;
    if (any && *n == '_') {
        int u, v, w, h, used = 0;
        if (sscanf(n, "_%d_%d_%d_%d%n", &u, &v, &w, &h, &used) != 4 || u < 0 || v < 0 || w <= 0 || h <= 0
            || u + w > 256 || v + h > 256)
            return 0;
        n += used;
        crop[0] = u;
        crop[1] = v;
        crop[2] = w;
        crop[3] = h;
    }
    if (strcmp(n, ".png") && strcmp(n, ".PNG")) return 0;
    *key = sub ? value ^ SUBMARK : any ? value ^ ANYMARK : value;
    return sub ? 2 : crop[0] >= 0 ? 3 : 1;
}
/* "<16 hex>.png", "sub_<16 hex>.png", "r<16 hex>.png", "r<16 hex>_U_V_W_H.png" (any case), alone or after a
   descriptive prefix ending in "__" ("tombi_corre__r0123456789abcdef.png", tools/dump_all_textures.py); sub and r keys
   live in their own spaces */
static int parse_name(const char *n, u64 *key, int *crop) {
    int name_kind = parse_key(n, key, crop);
    for (const char *q = strstr(n, "__"); q; q = strstr(q + 1, "__")) {
        int r = parse_key(q + 2, key, crop);
        if (r) name_kind = r;
    }
    return name_kind;
}
/* the pack's files under dir (8 levels of folders at most) into the key table and the crop list */
static void scan(const char *dir, int depth) {
    DIR *d = opendir(dir);
    struct dirent *e;
    if (!d) {
        if (!depth) port_printf("[texpack] cannot open %s\n", dir);
        return;
    }
    while ((e = readdir(d))) {
        if (e->d_name[0] == '.') continue;
        char entry_path[1024];
        u64 found_key;
        int name_kind, crop_rect[4];
        snprintf(entry_path, sizeof entry_path, "%s/%s", dir, e->d_name);
        if ((name_kind = parse_name(e->d_name, &found_key, crop_rect))) {
            if (name_kind == 3) {
                if (n_crops == cap_crops) {
                    Crop *grown_crops = realloc(crops, sizeof *crops * (cap_crops = cap_crops ? cap_crops * 2 : 256));
                    if (!grown_crops) continue;
                    crops = grown_crops;
                }
                crops[n_crops].key = found_key;
                crops[n_crops].u = crop_rect[0];
                crops[n_crops].v = crop_rect[1];
                crops[n_crops].w = crop_rect[2];
                crops[n_crops].h = crop_rect[3];
                crops[n_crops++].path = strdup(entry_path);
            } else
                pack_add(found_key, entry_path);
            if (name_kind == 2) pack_has_sub = 1;
            continue;
        }
        struct stat st;
        if (depth < 8 && !stat(entry_path, &st) && S_ISDIR(st.st_mode)) scan(entry_path, depth + 1);
    }
    closedir(d);
}

/* ------------------------------------------------------------------ dumping */
/* open-addressing hash set of the keys already dumped (0 = empty slot) */
static u64 *dumped;
static u32 dumped_cap, dumped_n;
static int dumped_add(u64 h) {                       /* 1 when new */
    if (!h) h = 1;
    if (dumped_n * 2 >= dumped_cap) {
        /* grow (double) and re-insert the old keys */
        u32 old_cap = dumped_cap;
        u64 *old_set = dumped;
        dumped_cap = old_cap ? old_cap * 2 : 4096;
        dumped = calloc(dumped_cap, 8);
        dumped_n = 0;
        for (u32 i = 0; i < old_cap; i++)
            if (old_set[i]) dumped_add(old_set[i]);
        free(old_set);
    }
    u32 slot_mask = dumped_cap - 1,
        i = (u32)(h ^ (h >> 29)) & slot_mask;
    while (dumped[i]) {
        if (dumped[i] == h) return 0;
        i = (i + 1) & slot_mask;
    }
    dumped[i] = h;
    dumped_n++;
    return 1;
}
static void mkdir1(const char *p) {
#ifdef _WIN32
    _mkdir(p);
#else
    mkdir(p, 0777);
#endif
}
static FILE *index_f;
/* the upload of the image being dumped (0 for a sub box): its hash goes to the index */
static const Up *dump_up;
static int n_dumped, dump_full_warned;
/* a PS1 word as RGBA8 (the alpha classes of the header comment) */
static inline void to_rgba8(u8 *o, u16 c) {
    int r = c & 31,
        g = (c >> 5) & 31,
        b = (c >> 10) & 31;
    o[0] = (u8)(r << 3 | r >> 2);
    o[1] = (u8)(g << 3 | g >> 2);
    o[2] = (u8)(b << 3 | b >> 2);
    o[3] = c == 0 ? 0 : (c & 0x8000) ? 128 : 255;
}
/* decodes words (ww x wh, stride ws) at depth with pal into w x h RGBA starting at texel column u0 of the first word */
static void decode(u8 *o, const u16 *words, int ws, int depth, const u16 *pal, int u0, int w, int h) {
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++) {
            int u = u0 + x;
            u16 c;
            if (depth == 0)
                c = pal[(words[y * ws + (u >> 2)] >> ((u & 3) * 4)) & 15];
            else if (depth == 1)
                c = pal[(words[y * ws + (u >> 1)] >> ((u & 1) * 8)) & 255];
            else
                c = words[y * ws + u];
            to_rgba8(o + ((size_t)y * w + x) * 4, c);
        }
}
/* writes the image of a key once (DIR/areaNN/<key>.png) and its line in DIR/textures.jsonl */
static void dump(u64 key, int sub, const u16 *words, int ws, int u0, int w, int h, const GpuTex *t, const u16 *vram,
                 int vx, int vy, int vw, int vh, int ou, int ov) {
    if (!dumped_add(key)) return;
    if (n_dumped >= 20000) {
        if (!dump_full_warned++) port_printf("[texpack] --dump-textures: 20000 images, not dumping more\n");
        return;
    }
    const u8 *game = port_ram_ptr(0x8009C960u);
    int area = *(const u16 *)(game + 0x948),
        sect = *(const u16 *)(game + 0x94A);
    char folder[64], name[64], path[1200];
    snprintf(folder, sizeof folder, "area%02x", area & 0xFF);
    snprintf(name, sizeof name, "%s%016llx.png", sub ? "sub_" : "", (unsigned long long)key);
    snprintf(path, sizeof path, "%s/%s", port_texdump_dir, folder);
    mkdir1(path);
    snprintf(path, sizeof path, "%s/%s/%s", port_texdump_dir, folder, name);
    struct stat st;
    if (!stat(path, &st)) return;                    /* an earlier run has it */
    u16 pal[256];
    int pal_count = t->depth == 0 ? 16 : t->depth == 1 ? 256 : 0;
    for (int i = 0; i < pal_count; i++) pal[i] = vram[(t->cy & 511) * 1024 + ((t->cx + i) & 1023)];
    u8 *rgba = malloc((size_t)w * h * 4);
    if (!rgba) return;
    decode(rgba, words, ws, t->depth, pal, u0, w, h);
    if (write_png_rgba(path, rgba, w, h)) {
        free(rgba);
        return;
    }
    free(rgba);
    n_dumped++;
    if (!index_f) {
        char index_path[1100];
        snprintf(index_path, sizeof index_path, "%s/textures.jsonl", port_texdump_dir);
        index_f = fopen(index_path, "ab");
    }
    if (index_f) {
        fprintf(index_f, "{\"file\": \"%s/%s\", \"kind\": \"%s\", \"depth\": %d, \"size\": [%d, %d], "
                "\"vram\": [%d, %d, %d, %d], "
                "\"page\": [%d, %d], \"origin\": [%d, %d], \"clut\": [%d, %d], "
                "\"area\": %d, \"section\": %d, \"frame\": %d",
                folder, name, sub ? "sub" : "upload", t->depth == 0 ? 4 : t->depth == 1 ? 8 : 16, w, h, vx, vy, vw, vh,
                t->tx, t->ty, ou, ov, pal_count ? t->cx : -1, pal_count ? t->cy : -1, area, sect, port_frame);
        /* the whole upload: tools/dump_all_textures.py names it after the disc resource */
        if (dump_up && !sub)
            fprintf(index_f, ", \"upload\": \"%016llx\", \"uprect\": [%d, %d, %d, %d]",
                    (unsigned long long)dump_up->hash, dump_up->x, dump_up->y, dump_up->w, dump_up->h);
        fputs("}\n", index_f);
        fflush(index_f);
    }
}

/* ------------------------------------------------------------------ per primitive */
static void window(int *lo, int *hi, int mask, int off) {   /* the texel range [lo, hi] after the texture window */
    if (!mask) return;
    int m = mask * 8,
        base = (off & mask) * 8,
        free_bits = ~m & 255;
    if (*hi - *lo >= free_bits || (*lo & ~free_bits) != (*hi & ~free_bits)) {
        *lo = base;
        *hi = base | free_bits;
        return;
    }
    *lo = (*lo & free_bits) | base;
    *hi = (*hi & free_bits) | base;
}
static int n_rep_loaded;
/* the replacement of an exact key (loaded on first use), placed at (ou, ov) of the page; 0 = none */
static const PortTexRep *use(u64 key, int ou, int ov, int w, int h) {
    PackEnt *e = pack_find(key);
    if (!e || e->state < 0) return 0;
    if (e->state == 0) {
        int iw, ih;
        u8 *rgba = read_png_rgba(e->path, &iw, &ih);
        e->state = -1;
        if (!rgba) return 0;
        int img_scale = iw / w;
        if (img_scale < 1 || img_scale > 16 || iw != img_scale * w || ih != img_scale * h) {
            port_printf("[texpack] %s: %dx%d is not an integer scale (1..16) of the original %dx%d: ignored\n",
                        e->path, iw, ih, w, h);
            free(rgba);
            return 0;
        }
        unsigned tex = port_gl_rep_texture(rgba, iw, ih);
        free(rgba);
        if (!tex) return 0;
        e->rep.gltex = tex;
        e->rep.ou = ou;
        e->rep.ov = ov;
        e->rep.w = w;
        e->rep.h = h;
        e->state = 1;
        if (n_rep_loaded++ < 50 || port_verbose) port_printf("[texpack] %s (%dx)\n", e->path, img_scale);
    }
    /* (the same key always means the same texels; only the place in the page can differ for a moved upload) */
    e->rep.ou = ou;
    e->rep.ov = ov;
    return &e->rep;
}
/* An "r" image (any palette) of a 4/8-bit page part saved as a palette PNG holds CLUT indices
   (tools/dump_all_textures.py writes them so): each palette the game draws the part with is applied to the indices
   and gets its own GL texture (cached by CLUT hash), so one indexed image replaces the part under every palette,
   palette animations included.  "r<key>_U_V_W_H" images are sprite rectangles of that part (each exported with its
   own palette for editing): they are pasted over the part (over the "r" page image when there is one, else over the
   original indices read from VRAM) at the same scale.  Indexed images paste indices, RGB/RGBA ones paste colours that
   are drawn as they are under every palette. */
/* state 1 rgba (rep), 2 composed */
typedef struct { u64 key; int state, iw, ih, s; u8 *idx, *ov; PortTexRep rep; } AnyPage;
static AnyPage any_pg[1024];
static struct { u64 k; PortTexRep rep; } any_var[4096];
static int crop_cmp(const void *a, const void *b) {
    u64 x = ((const Crop *)a)->key,
        y = ((const Crop *)b)->key;
    return x < y ? -1 : x > y;
}
static int crops_sorted;
/* the crops of key k: their count, *first the index of the first (binary search in the sorted list) */
static int crops_of(u64 k, int *first) {
    if (!n_crops) return 0;
    if (!crops_sorted) {
        qsort(crops, n_crops, sizeof *crops, crop_cmp);
        crops_sorted = 1;
    }
    int lo = 0,
        hi = n_crops;
    while (lo < hi) {
        int m = (lo + hi) / 2;
        if (crops[m].key < k)
            lo = m + 1;
        else
            hi = m;
    }
    int n = 0;
    while (lo + n < n_crops && crops[lo + n].key == k) n++;
    *first = lo;
    return n;
}
/* the "r" page part of key k (VRAM (vx, vy), w x h texels, np palette entries), loaded and composed on first use;
   0 = none */
static AnyPage *any_load(u64 k, int depth, int vx, int vy, int w, int h, int np) {
    u32 m = 1023,
        i = (u32)(k ^ (k >> 32)) & m,
        tries = 0;
    while (any_pg[i].key && any_pg[i].key != k && tries++ < 1024) i = (i + 1) & m;
    AnyPage *a = &any_pg[i];
    if (a->key == k) return a->state > 0 ? a : 0;
    if (a->key) return 0;
    a->key = k;
    a->state = -1;
    PackEnt *e = pack_find(k);
    int first_crop = 0,
        crop_count = np ? crops_of(k, &first_crop) : 0;
    if (!e && !crop_count) return 0;
    u8 *rgba = 0;
    int iw = 0,
        ih = 0,
        img_scale = 0;
    if (e) {
        rgba = read_png_rgba(e->path, &iw, &ih);
        img_scale = rgba ? iw / w : 0;
        if (!rgba || img_scale < 1 || img_scale > 16 || iw != img_scale * w || ih != img_scale * h) {
            if (rgba)
                port_printf("[texpack] %s: %dx%d is not an integer scale (1..16) of the original %dx%d: ignored\n",
                            e->path, iw, ih, w, h);
            free(rgba);
            rgba = 0;
            img_scale = 0;
        }
    }
    if (!img_scale && crop_count) {                    /* the scale of the first readable crop */
        for (int c = 0; c < crop_count && !img_scale; c++) {
            Crop *cr = &crops[first_crop + c];
            int crop_w, crop_h;
            u8 *crop_rgba = read_png_rgba(cr->path, &crop_w, &crop_h);
            if (crop_rgba && crop_w % cr->w == 0 && crop_w / cr->w >= 1 && crop_w / cr->w <= 16
                && crop_h == crop_w / cr->w * cr->h)
                img_scale = crop_w / cr->w;
            free(crop_rgba);
        }
    }
    if (!img_scale) return 0;
    /* a plain RGBA "r" image: one texture for every palette */
    if (!np || (e && rgba && !png_last_idx && !crop_count)) {
        if (!rgba) return 0;
        unsigned tex = port_gl_rep_texture(rgba, iw, ih);
        free(rgba);
        if (!tex) return 0;
        a->rep.gltex = tex;
        a->rep.w = w;
        a->rep.h = h;
        a->state = 1;
        a->s = img_scale;
        if (n_rep_loaded++ < 50 || port_verbose) port_printf("[texpack] %s (%dx)\n", e->path, img_scale);
        return a;
    }
    int W = w * img_scale,
        H = h * img_scale;
    a->idx = calloc((size_t)W * H, 1);
    if (!a->idx) {
        free(rgba);
        return 0;
    }
    a->iw = W;
    a->ih = H;
    a->s = img_scale;
    if (rgba && png_last_idx)
        memcpy(a->idx, png_last_idx, (size_t)W * H);
    else {                                             /* the original indices from VRAM, upscaled */
        const u16 *vr = port_vram();
        int sh = depth == 0 ? 2 : 1,
            texels_per_word = 1 << sh;
        for (int y = 0; y < h; y++)
            for (int x = 0; x < w; x++) {
                u16 vram_word = vr[((vy + y) & 511) * 1024 + ((vx + x / texels_per_word) & 1023)];
                u8 texel_idx = (u8)((vram_word >> ((x & (texels_per_word - 1)) << (4 - sh))) & (np - 1));
                for (int yy = 0; yy < img_scale; yy++)
                    memset(a->idx + (size_t)(y * img_scale + yy) * W + x * img_scale, texel_idx, (size_t)img_scale);
            }
        /* an RGBA page image: colours over everything, crops still paste */
        if (rgba) {
            a->ov = rgba;
            rgba = 0;
        }
    }
    free(rgba);
    for (int c = 0; c < crop_count; c++) {
        Crop *cr = &crops[first_crop + c];
        int crop_w, crop_h;
        u8 *crop_rgba = read_png_rgba(cr->path, &crop_w, &crop_h);
        if (!crop_rgba || crop_w != cr->w * img_scale || crop_h != cr->h * img_scale || cr->u + cr->w > w
            || cr->v + cr->h > h) {
            if (crop_rgba)
                port_printf("[texpack] %s: %dx%d is not the %dx scale of %dx%d inside the %dx%d page part: ignored\n",
                            cr->path, crop_w, crop_h, img_scale, cr->w, cr->h, w, h);
            free(crop_rgba);
            continue;
        }
        for (int y = 0; y < crop_h; y++) {
            size_t o = (size_t)(cr->v * img_scale + y) * W + cr->u * img_scale;
            if (png_last_idx)
                memcpy(a->idx + o, png_last_idx + (size_t)y * crop_w, (size_t)crop_w);
            else {
                if (!a->ov && !(a->ov = calloc((size_t)W * H, 4))) break;
                memcpy(a->ov + o * 4, crop_rgba + (size_t)y * crop_w * 4, (size_t)crop_w * 4);
                for (int x = 0; x < crop_w; x++)
                    if (a->ov[(o + x) * 4 + 3] == 0) a->ov[(o + x) * 4 + 3] = 1;   /* (alpha 0 = no overlay: keep 1) */
            }
        }
        free(crop_rgba);
        if (n_rep_loaded++ < 50 || port_verbose) port_printf("[texpack] %s (%dx, sprite)\n", cr->path, img_scale);
    }
    a->state = 2;
    return a;
}
/* the "r" replacement of a page part (key0: its key without the CLUT) under the CLUT of t (hash ch): one GL texture
   per palette, cached in any_var; 0 = none */
static const PortTexRep *use_any(u64 key0, const GpuTex *t, u64 ch, int np, int vx, int vy, int ou, int ov, int w,
                                 int h) {
    AnyPage *a = any_load(key0 ^ ANYMARK, t->depth, vx, vy, w, h, np);
    if (!a) return 0;
    if (a->state == 1) {
        a->rep.ou = ou;
        a->rep.ov = ov;
        return &a->rep;
    }
    u64 var_key = fnv(key0, &ch, 8);
    if (!var_key) var_key = 1;
    u32 m = 4095,
        i = (u32)(var_key ^ (var_key >> 32)) & m,
        tries = 0;
    while (any_var[i].k && any_var[i].k != var_key && tries++ < 4096) i = (i + 1) & m;
    if (any_var[i].k == var_key) {
        any_var[i].rep.ou = ou;
        any_var[i].rep.ov = ov;
        return &any_var[i].rep;
    }
    if (any_var[i].k) return 0;                       /* (table full) */
    const u16 *vram = port_vram();
    u16 pal[256];
    for (int c = 0; c < np; c++) pal[c] = vram[(t->cy & 511) * 1024 + ((t->cx + c) & 1023)];
    size_t n = (size_t)a->iw * a->ih;
    u8 *rgba = malloc(n * 4);
    if (!rgba) return 0;
    for (size_t q = 0; q < n; q++) {
        if (a->ov && a->ov[q * 4 + 3]) {
            memcpy(rgba + q * 4, a->ov + q * 4, 4);
            if (rgba[q * 4 + 3] == 1) rgba[q * 4 + 3] = 0;
        } else
            to_rgba8(rgba + q * 4, pal[a->idx[q] & (np - 1)]);
    }
    unsigned tex = port_gl_rep_texture(rgba, a->iw, a->ih);
    free(rgba);
    if (!tex) return 0;
    any_var[i].k = var_key;
    any_var[i].rep.gltex = tex;
    any_var[i].rep.w = w;
    any_var[i].rep.h = h;
    any_var[i].rep.ou = ou;
    any_var[i].rep.ov = ov;
    return &any_var[i].rep;
}
/* the texture of a primitive that samples texels (u0, v0) .. (u1, v1) of page t: dumps it (--dump-textures) and
   returns its replacement (--texture-pack), 0 = none */
const PortTexRep *port_texpack_box(const GpuTex *t, int u0, int v0, int u1, int v1) {
    const GpuState *g = &port_gs;
    if (u1 < u0) u1 = 255;
    if (v1 < v0) v1 = 255;
    if (!pack_scanned && port_texpack_dir) {
        pack_scanned = 1;
        scan(port_texpack_dir, 0);
        port_printf("[texpack] %s: %u replacement images, %d sprite rectangles\n", port_texpack_dir, pack_count,
                    n_crops);
    }
    if (port_texdump_dir && !index_f && !n_dumped) mkdir1(port_texdump_dir);
    int want_rep = port_texpack_dir && (pack_count || n_crops) && port_gpu_be == &port_gpu_gl;
    if (!want_rep && !port_texdump_dir) return 0;
    window(&u0, &u1, g->twmx, g->twox);
    window(&v0, &v1, g->twmy, g->twoy);
    int sh = t->depth == 0 ? 2 : t->depth == 1 ? 1 : 0,
        texels_per_word = 1 << sh;
    /* the box in VRAM words */
    int vx = t->tx + (u0 >> sh),
        vx2 = t->tx + (u1 >> sh),
        vy = t->ty + v0,
        vy2 = t->ty + v1;
    if (vx2 > 1023 || vy2 > 511) return 0;
    int vw = vx2 - vx + 1,
        vh = vy2 - vy + 1;
    const u16 *vram = port_vram();
    /* the drawing area is not a texture: uploads under it are gone, boxes in it are not keyed */
    int dx1 = g->x1,
        dy1 = g->y1,
        dw = g->x2 - g->x1 + 1,
        dh = g->y2 - g->y1 + 1,
        drawing = !g->wide && dw > 0 && dh > 0;
    /* the smallest live upload that holds the box and has no hole over it */
    Up *best = 0;
    for (int i = 0; i < n_ups; i++) {
        Up *u = &ups[i];
        if (!u->live) continue;
        if (drawing && ov(u->x, u->y, u->w, u->h, dx1, dy1, dw, dh)) {
            int k = 0;
            while (k < u->nholes
                   && !(u->hole[k][0] == dx1 && u->hole[k][1] == dy1 && u->hole[k][2] == dw && u->hole[k][3] == dh))
                k++;
            if (k == u->nholes) {
                if (inside(u->x, u->y, u->w, u->h, dx1, dy1, dw, dh) || u->nholes == MAX_HOLES) {
                    up_kill(u);
                    continue;
                }
                u->hole[k][0] = (short)dx1;
                u->hole[k][1] = (short)dy1;
                u->hole[k][2] = (short)dw;
                u->hole[k][3] = (short)dh;
                u->nholes++;
            }
        }
        if (!inside(vx, vy, vw, vh, u->x, u->y, u->w, u->h) || (best && u->w * u->h >= best->w * best->h)) continue;
        int holed = 0;
        for (int k = 0; k < u->nholes; k++)
            if (ov(vx, vy, vw, vh, u->hole[k][0], u->hole[k][1], u->hole[k][2], u->hole[k][3])) holed = 1;
        if (!holed) best = u;
    }
    int np = t->depth == 0 ? 16 : t->depth == 1 ? 256 : 0;
    u64 clut_key = np ? clut_hash(vram, t->cx, t->cy, np) : 0;
    if (best) {
        /* the logical texture is the part of the upload inside this texture page (a level uploads several pages at
           once) */
        int page_words = 256 >> sh;
        int x1 = best->x > t->tx ? best->x : t->tx,
            y1 = best->y > t->ty ? best->y : t->ty;
        int x2 = best->x + best->w < t->tx + page_words ? best->x + best->w : t->tx + page_words,
            y2 = best->y + best->h < t->ty + 256 ? best->y + best->h : t->ty + 256;
        u32 key_hdr[5] = { (u32)t->depth, (u32)(x1 - best->x), (u32)(y1 - best->y), (u32)(x2 - x1), (u32)(y2 - y1) };
        u64 key = fnv(FNV0, &best->hash, 8);
        key = fnv(key, key_hdr, sizeof key_hdr);
        u64 key0 = key;
        if (np) key = fnv(key, &clut_key, 8);
        int ou = (x1 - t->tx) * texels_per_word,
            ovv = y1 - t->ty,
            w = (x2 - x1) * texels_per_word,
            h = y2 - y1;
        if (port_texdump_dir && best->data && best->nkeys < 256) {
            if (dumped_add(key ^ 0x9E3779B97F4A7C15ull)) {   /* (a second set: counts the keys of this upload) */
                best->nkeys++;
                if (best->nkeys == 256)
                    port_printf("[texpack] upload %d,%d %dx%d: 256 page/palette pairs, not dumping more of it\n",
                                best->x, best->y, best->w, best->h);
                dump_up = best;
                dump(key, 0, best->data + (y1 - best->y) * best->w + (x1 - best->x), best->w, 0, w, h, t, vram, x1, y1,
                     x2 - x1, y2 - y1, ou, ovv);
            }
        }
        if (!want_rep) return 0;
        const PortTexRep *r = use(key, ou, ovv, w, h);
        return r ? r : use_any(key0, t, clut_key, np, x1, y1, ou, ovv, w, h);
    }
    if (!port_texdump_dir && !(want_rep && pack_has_sub)) return 0;
    /* the sampled box itself: only inside uploaded VRAM (not the frame buffers or other drawn areas) */
    if (drawing && ov(vx, vy, vw, vh, dx1, dy1, dw, dh)) return 0;
    long covered = 0;
    for (int i = 0; i < n_ups; i++) {
        Up *u = &ups[i];
        if (!u->live || !ov(vx, vy, vw, vh, u->x, u->y, u->w, u->h)) continue;
        int x1 = u->x > vx ? u->x : vx,
            y1 = u->y > vy ? u->y : vy,
            x2 = u->x + u->w < vx + vw ? u->x + u->w : vx + vw,
            y2 = u->y + u->h < vy + vh ? u->y + u->h : vy + vh;
        covered += (long)(x2 - x1) * (y2 - y1);
    }
    if (getenv("TEXPACK_LOG"))
        port_printf("[texpack] f%d box %d,%d %dx%d depth %d covered %ld\n", port_frame, vx, vy, vw, vh, t->depth,
                    covered);
    if (covered < (long)vw * vh) return 0;
    int w = u1 - u0 + 1,
        h = v1 - v0 + 1;
    u64 key = FNV0;
    u32 key_hdr[3] = { (u32)t->depth, (u32)w, (u32)h };
    key = fnv(key, key_hdr, sizeof key_hdr);
    for (int y = 0; y < h; y++) {                    /* the texels of the box (indices for 4/8-bit) */
        const u16 *row = vram + (vy + y) * 1024 + vx;
        if (!sh)
            key = fnv(key, row, (size_t)w * 2);
        else {
            u8 b[256];
            for (int x = 0; x < w; x++) {
                int u = (u0 & (texels_per_word - 1)) + x;
                b[x] = (u8)((row[u >> sh] >> ((u & (texels_per_word - 1)) << (4 - sh))) & (np - 1));
            }
            key = fnv(key, b, (size_t)w);
        }
    }
    if (np) key = fnv(key, &clut_key, 8);
    if (port_texdump_dir) {
        static u16 tmp[256 * 256];
        for (int y = 0; y < vh; y++) memcpy(tmp + y * vw, vram + (vy + y) * 1024 + vx, (size_t)vw * 2);
        dump(key, 1, tmp, vw, u0 & (texels_per_word - 1), w, h, t, vram, vx, vy, vw, vh, u0, v0);
    }
    return want_rep && pack_has_sub ? use(key ^ SUBMARK, u0, v0, w, h) : 0;
}
/* port_texpack_box for a polygon of n vertices: the box of their u/v */
const PortTexRep *port_texpack_prim(const GpuTex *t, const GpuVtx *v, int n) {
    int u0 = 255, u1 = 0, v0 = 255, v1 = 0;
    for (int i = 0; i < n; i++) {
        if (v[i].u < u0) u0 = v[i].u;
        if (v[i].u > u1) u1 = v[i].u;
        if (v[i].v < v0) v0 = v[i].v;
        if (v[i].v > v1) v1 = v[i].v;
    }
    /* a polygon's last row/column (u1, v1) is the edge, not sampled; sprites (below) pass their real box */
    if (u1 > u0) u1--;
    if (v1 > v0) v1--;
    return port_texpack_box(t, u0, v0, u1, v1);
}
void port_texpack_stop(void) {
    if (index_f) {
        fclose(index_f);
        index_f = 0;
    }
    if (port_texdump_dir) port_printf("[texpack] %d textures dumped to %s\n", n_dumped, port_texdump_dir);
}
