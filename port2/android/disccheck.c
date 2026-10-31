/* libtombidisc.so: the Android launcher's disc check (LauncherActivity.main0Sha1), in C so that every image format the
   game reads (.bin raw 2352, .iso cooked 2048, .chd through shims/cd_chd.c + libchdr) is verified by the same code
   path: sector 16 (PVD) -> root directory -> EXE -> MAIN0.EXE -> SHA-1.  Small on purpose: no game code, no SDL. */
#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "port_chd.h"

static volatile int progress;            /* 0..1000 while hashing (nativeCheckProgress) */
static FILE *img; static int is_chd, sec_size, sec_off;

static int read_sector(int lba, unsigned char *out) {
    if (is_chd) { unsigned char raw[2352]; if (port_chd_read_raw(lba, raw)) return -1; memcpy(out, raw + 24, 2048); return 0; }
    if (fseeko(img, (off_t)lba * sec_size + sec_off, SEEK_SET)) return -1;
    return fread(out, 1, 2048, img) == 2048 ? 0 : -1;
}
static unsigned le32(const unsigned char *b) { return b[0] | b[1] << 8 | b[2] << 16 | (unsigned)b[3] << 24; }
static int find(unsigned lba, unsigned size, const char *name, unsigned *elba, unsigned *esize) {
    size_t n = strlen(name);
    for (unsigned s = 0; s < (size + 2047) / 2048; s++) {
        unsigned char b[2048];
        if (read_sector((int)(lba + s), b)) return 0;
        for (int o = 0; o < 2048 && b[o]; o += b[o]) {
            int nl = b[o + 32], k = nl;
            const char *nm = (const char *)b + o + 33;
            for (int i = 0; i < nl; i++) if (nm[i] == ';') { k = i; break; }
            if ((size_t)k == n && !strncasecmp(nm, name, n)) { *elba = le32(b + o + 2); *esize = le32(b + o + 10); return 1; }
        }
    }
    return 0;
}

/* ---- SHA-1 (FIPS 180-1) */
typedef struct { unsigned h[5]; unsigned long long len; unsigned char buf[64]; int n; } Sha1;
#define ROL(x, c) (((x) << (c)) | ((x) >> (32 - (c))))
static void sha1_block(Sha1 *s, const unsigned char *p) {
    unsigned w[80], a = s->h[0], b = s->h[1], c = s->h[2], d = s->h[3], e = s->h[4];
    for (int i = 0; i < 16; i++) w[i] = (unsigned)p[4 * i] << 24 | p[4 * i + 1] << 16 | p[4 * i + 2] << 8 | p[4 * i + 3];
    for (int i = 16; i < 80; i++) w[i] = ROL(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);
    for (int i = 0; i < 80; i++) {
        unsigned f = i < 20 ? (b & c) | (~b & d) : i < 40 ? b ^ c ^ d : i < 60 ? (b & c) | (b & d) | (c & d) : b ^ c ^ d;
        unsigned k = i < 20 ? 0x5A827999 : i < 40 ? 0x6ED9EBA1 : i < 60 ? 0x8F1BBCDC : 0xCA62C1D6;
        unsigned t = ROL(a, 5) + f + e + k + w[i]; e = d; d = c; c = ROL(b, 30); b = a; a = t;
    }
    s->h[0] += a; s->h[1] += b; s->h[2] += c; s->h[3] += d; s->h[4] += e;
}
static void sha1_init(Sha1 *s) { static const unsigned h[5] = { 0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0 }; memcpy(s->h, h, sizeof h); s->len = 0; s->n = 0; }
static void sha1_update(Sha1 *s, const unsigned char *p, unsigned n) {
    s->len += n;
    while (n--) { s->buf[s->n++] = *p++; if (s->n == 64) { sha1_block(s, s->buf); s->n = 0; } }
}
static void sha1_hex(Sha1 *s, char *out) {
    unsigned long long bits = s->len * 8; unsigned char pad = 0x80, z = 0, l[8];
    sha1_update(s, &pad, 1);
    while (s->n != 56) sha1_update(s, &z, 1);
    for (int i = 0; i < 8; i++) l[i] = (unsigned char)(bits >> (56 - 8 * i));
    sha1_update(s, l, 8);
    for (int i = 0; i < 5; i++) sprintf(out + 8 * i, "%08x", s->h[i]);
}

/* "sha1:<40 hex>", or "err:<reason>" ("nomain0" when the image has no EXE/MAIN0.EXE) */
static const char *check(const char *path, char *res, int rlen) {
    size_t pl = strlen(path);
    is_chd = pl > 4 && !strcasecmp(path + pl - 4, ".chd");
    progress = 0;
    if (is_chd) {
        char err[200];
        if (port_chd_open(path, NULL, err, sizeof err)) { snprintf(res, rlen, "err:%s", err); return res; }
    } else {
        img = fopen(path, "rb");
        if (!img) { snprintf(res, rlen, "err:cannot open %s", path); return res; }
        unsigned char head[16] = { 0 };
        static const unsigned char sync[12] = { 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0 };
        fseeko(img, 0, SEEK_END); off_t size = ftello(img);
        fseeko(img, 0, SEEK_SET);
        int raw = fread(head, 1, 16, img) == 16 && !memcmp(head, sync, 12);
        if (!raw && size % 2352 == 0 && size % 2048 != 0) raw = 1;
        sec_size = raw ? 2352 : 2048; sec_off = 0;
        if (raw) { unsigned char m = 2; fseeko(img, 16L * 2352 + 15, SEEK_SET); if (fread(&m, 1, 1, img) != 1) m = 2; sec_off = m == 1 ? 16 : 24; }
    }
    unsigned char pvd[2048]; unsigned dl, ds, fl, fs;
    if (read_sector(16, pvd) || memcmp(pvd + 1, "CD001", 5)) snprintf(res, rlen, "err:sector 16 is not an ISO9660 volume descriptor");
    else if (!find(le32(pvd + 158), le32(pvd + 166), "EXE", &dl, &ds) || !find(dl, ds, "MAIN0.EXE", &fl, &fs)) snprintf(res, rlen, "err:nomain0");
    else {
        Sha1 s; sha1_init(&s);
        unsigned left = fs, done = 0; int ok = 1;
        for (unsigned l = fl; left > 0; l++) {
            unsigned char b[2048]; unsigned n = left < 2048 ? left : 2048;
            if (read_sector((int)l, b)) { ok = 0; break; }
            sha1_update(&s, b, n); left -= n; done += n;
            progress = (int)(1000ull * done / (fs ? fs : 1));
        }
        if (ok) { char hex[41]; sha1_hex(&s, hex); snprintf(res, rlen, "sha1:%s", hex); }
        else snprintf(res, rlen, "err:read error in MAIN0.EXE");
    }
    if (is_chd) port_chd_close(); else if (img) { fclose(img); img = 0; }
    progress = 1000;
    return res;
}

JNIEXPORT jstring JNICALL Java_io_github_tombi_port_LauncherActivity_nativeMain0Sha1(JNIEnv *env, jclass cls, jstring jpath) {
    (void)cls;
    const char *path = (*env)->GetStringUTFChars(env, jpath, NULL);
    char res[300];
    check(path, res, sizeof res);
    (*env)->ReleaseStringUTFChars(env, jpath, path);
    return (*env)->NewStringUTF(env, res);
}
JNIEXPORT jint JNICALL Java_io_github_tombi_port_LauncherActivity_nativeCheckProgress(JNIEnv *env, jclass cls) {
    (void)env; (void)cls; return progress;
}
