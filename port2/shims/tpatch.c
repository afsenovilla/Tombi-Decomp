/* --text-patch FILE (docs/PORT.md "Standalone (no launcher) mode"): text patches applied in memory, from the disc image
   (or a pristine data/), with no launcher and no Python: what tools/data_patch.py writes into data/ for the PC launcher.
   FILE is a .tpatch (tools/tpatch_build.py: the patched files as COPY runs of the disc's own files, GLYPH ops that build a
   moved/composed glyph bitmap from rows of the disc's glyphs, and LITERAL bytes of our own encoded text only) or the *.tombipatch.json it was compiled from (the .tpatch next to it is used; its recorded sha1 of the
   JSON must match, else the patch is refused as stale).  Each rebuilt file is checked against its sha1 and served for the
   LBA ranges it occupies (its own place, plus the LBA past the end of the disc a grown file moved to, which the patched
   MAINn.EXE table points at) and for reads by name (port_cd_read_named).  Sources and results are verified by sha1: a
   different disc gets no patch at all (logged), never half of one. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "port_runtime.h"

extern unsigned char *port_cd_read_named(const char *path, unsigned *size);

/* ---- sha1 (FIPS 180-1), small and portable */
typedef struct { unsigned h[5]; unsigned long long n; unsigned char b[64]; } Sha1;
static unsigned rol(unsigned x, int k) { return x << k | x >> (32 - k); }
static void sha1_block(Sha1 *s, const unsigned char *p) {
    unsigned w[80], a, b, c, d, e, t; int i;
    for (i = 0; i < 16; i++) w[i] = (unsigned)p[4 * i] << 24 | p[4 * i + 1] << 16 | p[4 * i + 2] << 8 | p[4 * i + 3];
    for (; i < 80; i++) w[i] = rol(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);
    a = s->h[0]; b = s->h[1]; c = s->h[2]; d = s->h[3]; e = s->h[4];
    for (i = 0; i < 80; i++) {
        unsigned f, k;
        if (i < 20) { f = (b & c) | (~b & d); k = 0x5A827999; } else if (i < 40) { f = b ^ c ^ d; k = 0x6ED9EBA1; }
        else if (i < 60) { f = (b & c) | (b & d) | (c & d); k = 0x8F1BBCDC; } else { f = b ^ c ^ d; k = 0xCA62C1D6; }
        t = rol(a, 5) + f + e + k + w[i]; e = d; d = c; c = rol(b, 30); b = a; a = t;
    }
    s->h[0] += a; s->h[1] += b; s->h[2] += c; s->h[3] += d; s->h[4] += e;
}
static void sha1(const unsigned char *p, unsigned n, unsigned char out[20]) {
    Sha1 s = { { 0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0 }, 0, { 0 } };
    unsigned i, r = n;
    while (r >= 64) { sha1_block(&s, p); p += 64; r -= 64; }
    unsigned char last[128] = { 0 };
    memcpy(last, p, r); last[r] = 0x80;
    unsigned tot = r + 1 + 8 <= 64 ? 64 : 128;
    unsigned long long bits = (unsigned long long)n * 8;
    for (i = 0; i < 8; i++) last[tot - 1 - i] = (unsigned char)(bits >> (8 * i));
    sha1_block(&s, last); if (tot == 128) sha1_block(&s, last + 64);
    for (i = 0; i < 20; i++) out[i] = (unsigned char)(s.h[i / 4] >> (24 - 8 * (i % 4)));
}
static void hex(const unsigned char *d, char *o) { static const char *x = "0123456789abcdef"; for (int i = 0; i < 20; i++) { o[2*i] = x[d[i] >> 4]; o[2*i+1] = x[d[i] & 15]; } o[40] = 0; }

/* ---- the installed overlay */
typedef struct { char path[64]; unsigned char *data; unsigned size; int nr; unsigned lba[4], sec[4]; int from; long off; } TpFile;
/* port_tpatch_store (a path; the 3DS's entry point sets it, TOMBI_TPATCH_STORE tries it elsewhere): the rebuilt files are
   kept in that file, not in memory.  A 3DS has no room for the 335 disc files a translation is built from at once, nor
   for the 12 MB it rebuilds: there the sources are read as each file needs them and let go, each rebuilt file is
   appended to the store (data == 0, off its place there), and the next start finds the store made (its head: "TPC1",
   the patch's sha1) and reads from it.  port_tpatch_progress, if set, is told how far a build is. */
const char *port_tpatch_store;
void (*port_tpatch_progress)(int done, int total);
static FILE *tp_store;
#define STORE_HEAD 24
/* the file each installed patch came from (log of the overlap rule below) */
static char tp_names[16][160]; static int tp_npatches;
static TpFile tp_files[512];
static int tp_nfiles, tp_busy;      /* tp_busy: reading the sources (the original files, not the overlay) */
int port_tpatch_count;              /* files served from text patches */

static unsigned char *slurp(const char *path, long *n) {
    FILE *f = fopen(path, "rb"); unsigned char *d;
    if (!f) return 0;
    fseek(f, 0, SEEK_END); *n = ftell(f); fseek(f, 0, SEEK_SET);
    d = malloc(*n > 0 ? (size_t)*n : 1);
    if (d && (long)fread(d, 1, (size_t)*n, f) != *n) { free(d); d = 0; }
    fclose(f); return d;
}
static unsigned u32(const unsigned char *p) { return p[0] | p[1] << 8 | p[2] << 16 | (unsigned)p[3] << 24; }
static unsigned u16(const unsigned char *p) { return p[0] | p[1] << 8; }

/* returns the number of files installed, -1 on error (nothing installed) */
int port_tpatch_load(const char *arg) {
    char path[1200], jsha[48] = ""; long n = 0; unsigned char *b, *json;
    size_t L = strlen(arg);
    const char *suf = ".tombipatch.json";
    snprintf(path, sizeof path, "%s", arg);
    if (L > strlen(suf) && !strcmp(arg + L - strlen(suf), suf)) {      /* the JSON the launcher knows: its compiled twin */
        long jn; unsigned char d[20];
        if ((json = slurp(arg, &jn))) { sha1(json, (unsigned)jn, d); hex(d, jsha); free(json); }
        else { port_printf("[tpatch] cannot read %s\n", arg); return -1; }
        snprintf(path + (L - strlen(suf)), sizeof path - (L - strlen(suf)), ".tpatch");
    }
    if (!(b = slurp(path, &n)) || n < 58 || memcmp(b, "TPCH", 4) || u32(b + 4) != 2) {
        port_printf("[tpatch] %s: %s\n", path, b ? "not a version 2 .tpatch (tools/tpatch_build.py)" : "cannot open (compile the JSON with tools/tpatch_build.py)");
        free(b); return -1;
    }
    unsigned ns = u32(b + 8), ne = u32(b + 12), p = 16 + 40, i, k, skip = 0;
    if (jsha[0] && memcmp(b + 16, jsha, 40)) {
        port_printf("[tpatch] %s was compiled from another version of %s (sha1 %.12s, file %.12s): rebuild it with tools/tpatch_build.py\n", path, arg, (char *)b + 16, jsha);
        free(b); return -1;
    }
    unsigned tl = u16(b + p); p += 2;
    port_printf("[tpatch] %s: \"%.*s\", %u files from %u disc files\n", path, (int)tl, (char *)b + p, ne, ns);
    p += tl;
    unsigned char **src = calloc(ns ? ns : 1, sizeof *src); unsigned *ssz = calloc(ns ? ns : 1, sizeof *ssz);
    unsigned *spos = calloc(ns ? ns : 1, sizeof *spos);   /* (store mode: where each source's entry is, read when a file needs it) */
    int ok = 1, base = tp_nfiles;
    const char *store = port_tpatch_store ? port_tpatch_store : getenv("TOMBI_TPATCH_STORE");
    int made = 0;                       /* the store is there already: nothing is rebuilt */
    unsigned char key[20]; long soff = 0; unsigned loaded = 0;
    if (store && tp_store) store = 0;   /* (one patch's store at a time: a second patch is kept in memory) */
    if (store) {
        unsigned char head[STORE_HEAD];
        sha1(b, (unsigned)n, key);
        tp_store = fopen(store, "rb");
        if (tp_store && fread(head, 1, STORE_HEAD, tp_store) == STORE_HEAD && !memcmp(head, "TPC1", 4) && !memcmp(head + 4, key, 20)) made = 1;
        else {
            if (tp_store) fclose(tp_store);
            tp_store = fopen(store, "wb");
            if (!tp_store) { port_printf("[tpatch] cannot write %s: the patch is kept in memory\n", store); store = 0; }
            else { memset(head, 0, sizeof head); memcpy(head, "TPC0", 4); fwrite(head, 1, STORE_HEAD, tp_store); }
        }
        if (!made && store && port_tpatch_progress) port_tpatch_progress(0, (int)ne);   /* (the screen says so from the start) */
        port_printf("[tpatch] store %s: %s\n", port_tpatch_store ? port_tpatch_store : "(TOMBI_TPATCH_STORE)", made ? "found" : store ? "being made (the first start with this patch)" : "not used");
    }
    tp_busy = 1;
    for (i = 0; i < ns && ok; i++) {
        unsigned pl = u16(b + p); char name[256]; unsigned char d[20];
        spos[i] = p;
        snprintf(name, sizeof name, "%.*s", (int)pl, (char *)b + p + 2); p += 2 + pl;
        unsigned want = u32(b + p); p += 4;
        if (store) { ssz[i] = want; p += 20; continue; }
        src[i] = port_cd_read_named(name, &ssz[i]);
        if (!src[i] || ssz[i] != want) { port_printf("[tpatch] source %s: %s\n", name, src[i] ? "size differs (not the PAL disc SCES-01331?)" : "not on the disc"); ok = 0; }
        else { sha1(src[i], ssz[i], d); if (memcmp(d, b + p, 20)) { port_printf("[tpatch] source %s: sha1 differs (not the PAL disc SCES-01331, or already patched)\n", name); ok = 0; } }
        p += 20;
    }
    tp_busy = 0;
    for (i = 0; i < ne && ok; i++) {
        if (tp_nfiles >= 512) { ok = 0; break; }
        TpFile *f = &tp_files[tp_nfiles];
        unsigned pl = u16(b + p); unsigned char d[20];
        snprintf(f->path, sizeof f->path, "%.*s", (int)pl, (char *)b + p + 2); p += 2 + pl;
        /* the first patch loaded owns its files: a later one that rebuilds any of them is skipped whole (a patch's files
           and its MAINn.EXE tables belong together, half of two patches is never a consistent game).  open_text_patches
           loads the player's own translation file first, so it wins over the improved translation passed by a launcher */
        for (int j = 0; j < base; j++) if (!strcmp(tp_files[j].path, f->path)) {
            port_printf("[tpatch] %s skipped: %s is already provided by %s (loaded first)\n", path, f->path,
                        tp_files[j].from >= 0 && tp_files[j].from < 16 ? tp_names[tp_files[j].from] : "another patch");
            skip = 1; ok = 0; break;
        }
        if (!ok) break;
        f->from = tp_npatches < 16 ? tp_npatches : -1;
        f->size = u32(b + p); p += 4;
        const unsigned char *want = b + p; p += 20;
        f->nr = b[p++];
        for (k = 0; k < (unsigned)f->nr; k++) { if (k < 4) { f->lba[k] = u32(b + p); f->sec[k] = u32(b + p + 4); } p += 8; }
        if (f->nr > 4) f->nr = 4;
        unsigned nops = u32(b + p), o = 0; p += 4;
        f->off = soff; soff += f->size;
        if (made) {   /* in the store: the operations are only stepped over */
            for (k = 0; k < nops; k++) {
                if (b[p] == 0) p += 11;
                else if (b[p] == 2) p += 5 + 8u * u16(b + p + 3);
                else p += 5 + u32(b + p + 1);
            }
            f->data = 0;
            tp_nfiles++;
            continue;
        }
        /* (store mode: the sources this file reads, loaded now and checked as the others are above) */
        #define NEED(s_) do { unsigned s__ = (s_); if (store && s__ < ns && !src[s__]) { \
            unsigned q = spos[s__], pl = u16(b + q); char name[256]; unsigned char dd[20]; unsigned got = 0; \
            snprintf(name, sizeof name, "%.*s", (int)pl, (char *)b + q + 2); \
            tp_busy = 1; src[s__] = port_cd_read_named(name, &got); tp_busy = 0; \
            if (!src[s__] || got != ssz[s__]) { port_printf("[tpatch] source %s: %s\n", name, src[s__] ? "size differs (not the PAL disc SCES-01331?)" : "not on the disc (or no memory for it)"); ok = 0; ssz[s__] = 0; } \
            else { (void)dd; loaded += got; } } } while (0)   /* (no sha1 of a source here: each rebuilt file's own is checked, and hashing 60 MB of sources was most of the minutes a store took to make) */
        f->data = malloc(f->size + 4);
        if (!f->data) { port_printf("[tpatch] no memory for %s (%u bytes)\n", f->path, f->size); ok = 0; }
        for (k = 0; k < nops && ok; k++) {
            if (b[p] == 0) {
                unsigned s = u16(b + p + 1), off = u32(b + p + 3), len = u32(b + p + 7); p += 11;
                NEED(s);
                if (!ok) break;
                if (s >= ns || off + len > ssz[s] || o + len > f->size) ok = 0; else { memcpy(f->data + o, src[s] + off, len); o += len; }
            } else if (b[p] == 2) {     /* GLYPH: w x h 4bpp bitmap, each row a (shifted) row of a disc glyph */
                unsigned w = u16(b + p + 1), h = u16(b + p + 3), len = (w * h + 1) / 2, r, c; p += 5;
                if (o + len > f->size || p + 8u * h > (unsigned)n) { ok = 0; break; }
                memset(f->data + o, 0, len);
                for (r = 0; r < h && ok; r++, p += 8) {
                    unsigned s = u16(b + p), off = u32(b + p + 2), sr = b[p + 6]; int dx = (signed char)b[p + 7];
                    if (sr == 0xFF) continue;
                    NEED(s);
                    if (!ok) break;
                    if (s >= ns || sr >= h || off + (w * h + 1) / 2 > ssz[s]) { ok = 0; break; }
                    for (c = 0; c < w; c++) {
                        int sc = (int)c - dx;
                        if (sc < 0 || sc >= (int)w) continue;
                        unsigned k = sr * w + sc, q = r * w + c, v = src[s][off + k / 2] >> (k & 1 ? 4 : 0) & 15;
                        f->data[o + q / 2] |= (unsigned char)(v << (q & 1 ? 4 : 0));
                    }
                }
                o += len;
            } else {
                unsigned len = u32(b + p + 1); p += 5;
                if (o + len > f->size || p + len > (unsigned)n) ok = 0; else { memcpy(f->data + o, b + p, len); o += len; p += len; }
            }
        }
        if (ok && o == f->size) { sha1(f->data, f->size, d); if (memcmp(d, want, 20)) ok = 0; }
        else ok = 0;
        if (!ok) { port_printf("[tpatch] %s: rebuilt file does not match its sha1 (corrupt .tpatch?)\n", f->path); free(f->data); f->data = 0; break; }
        if (store) {   /* to the store, and out of memory; the sources too once they add up */
            if (fwrite(f->data, 1, f->size, tp_store) != f->size) { port_printf("[tpatch] cannot write %s (the SD card is full?)\n", store); ok = 0; }
            free(f->data); f->data = 0;
            if (loaded > (3u << 20)) { for (unsigned j = 0; j < ns; j++) { free(src[j]); src[j] = 0; } loaded = 0; }
            if (port_tpatch_progress) port_tpatch_progress((int)i + 1, (int)ne);
            if (!ok) break;
        }
        tp_nfiles++;
    }
    #undef NEED
    if (store && !made) {   /* the store is whole: its head says so, and it is read from now on */
        if (ok) {
            unsigned char head[STORE_HEAD]; memcpy(head, "TPC1", 4); memcpy(head + 4, key, 20);
            if (fseek(tp_store, 0, SEEK_SET) || fwrite(head, 1, STORE_HEAD, tp_store) != STORE_HEAD) ok = 0;
        }
        fclose(tp_store);
        tp_store = ok ? fopen(store, "rb") : 0;
        if (!tp_store) ok = 0;
    }
    if (tp_store) { static char sbuf[32768]; setvbuf(tp_store, sbuf, _IOFBF, sizeof sbuf); }
    for (i = 0; i < ns; i++) free(src[i]);
    free(src); free(ssz); free(spos); free(b);
    if (!ok) {
        if ((store || made) && tp_store) { fclose(tp_store); tp_store = 0; }
        while (tp_nfiles > base) free(tp_files[--tp_nfiles].data);
        if (!skip) port_printf("[tpatch] %s NOT applied: the game runs with the disc's own text\n", path);
        return -1;
    }
    if (tp_npatches < 16) snprintf(tp_names[tp_npatches], sizeof tp_names[0], "%s", path);
    tp_npatches++;
    port_tpatch_count = tp_nfiles;
    {   /* TOMBI_TPATCH_DUMP=DIR: write the rebuilt files (tools/tpatch_check.py compares them with the Python pipeline) */
        const char *dd = getenv("TOMBI_TPATCH_DUMP");
        if (dd && *dd) for (int j = base; j < tp_nfiles; j++) {
            char o[1400]; FILE *w;
            snprintf(o, sizeof o, "%s/", dd);
            size_t l = strlen(o);
            snprintf(o + l, sizeof o - l, "%s", tp_files[j].path);
            for (char *c = o + l; *c; c++) if (*c == '/') *c = '_';
            if (tp_files[j].data && (w = fopen(o, "wb"))) { fwrite(tp_files[j].data, 1, tp_files[j].size, w); fclose(w); }
        }
    }
    if (port_verbose) for (int j = base; j < tp_nfiles; j++) port_printf("[tpatch]   %s %u bytes, lba %u\n", tp_files[j].path, tp_files[j].size, tp_files[j].lba[0]);
    return tp_nfiles - base;
}

/* 1 when lba belongs to a patched file: d gets the sector (bytes 2048, or 2340/2352-style raw with the data at +12) */
int port_tpatch_sector(int lba, unsigned char *d, int bytes) {
    if (tp_busy || !tp_nfiles) return 0;
    for (int i = tp_nfiles - 1; i >= 0; i--) {
        TpFile *f = &tp_files[i];
        for (int r = 0; r < f->nr; r++) {
            if ((unsigned)lba < f->lba[r] || (unsigned)lba >= f->lba[r] + f->sec[r]) continue;
            unsigned off = (lba - f->lba[r]) * 2048u;
            unsigned char *u = bytes == 2048 ? d : d + 12;
            if (bytes != 2048) memset(d, 0, bytes);
            unsigned avail = off >= f->size ? 0 : f->size - off > 2048 ? 2048 : f->size - off;
            if (!f->data) {   /* in the store */
                if (!tp_store || fseek(tp_store, STORE_HEAD + f->off + (long)off, SEEK_SET) || fread(u, 1, avail, tp_store) != avail) avail = 0;
            } else
            memcpy(u, f->data + off, avail);
            memset(u + avail, 0, 2048 - avail);
            return 1;
        }
    }
    return 0;
}
/* a malloc'd copy of a patched file read by name (EXE/MAIN5.EXE...), else 0 */
unsigned char *port_tpatch_named(const char *path, unsigned *size) {
    if (tp_busy) return 0;
    for (int i = tp_nfiles - 1; i >= 0; i--) if (!strcmp(tp_files[i].path, path)) {
        unsigned char *d = malloc(tp_files[i].size + 4);
        if (!d) return 0;
        if (!tp_files[i].data) {   /* in the store */
            if (!tp_store || fseek(tp_store, STORE_HEAD + tp_files[i].off, SEEK_SET) || fread(d, 1, tp_files[i].size, tp_store) != tp_files[i].size) { free(d); return 0; }
        } else
        memcpy(d, tp_files[i].data, tp_files[i].size); *size = tp_files[i].size; return d;
    }
    return 0;
}
