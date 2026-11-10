/* libtombidisc.so: the Android launcher's disc check (LauncherActivity.main0Sha1), in C so that every image format the
   game reads (.bin raw 2352, .iso cooked 2048, .chd through shims/cd_chd.c + libchdr) is verified by the same code
   path: sector 16 (PVD) -> root directory -> EXE -> MAIN0.EXE -> SHA-1.  Small on purpose: no game code, no SDL. */
#include <jni.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <strings.h>
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

static off_t img_size;
/* opens the image (sets is_chd / sec_size / sec_off / img_size); 0 = ok, else res holds "err:..." */
static int open_img(const char *path, char *res, int rlen) {
    size_t pl = strlen(path);
    is_chd = pl > 4 && !strcasecmp(path + pl - 4, ".chd");
    img_size = 0;
    if (is_chd) {
        char err[200];
        int ns = 0;
        if (port_chd_open(path, &ns, err, sizeof err)) { snprintf(res, rlen, "err:%s", err); return -1; }
        img_size = (off_t)ns * 2352;
    } else {
        img = fopen(path, "rb");
        if (!img) { snprintf(res, rlen, "err:cannot open %s", path); return -1; }
        {   /* a .chd under another name (/proc/self/fd/N of a picked file): its magic */
            char mg[8] = { 0 };
            if (fread(mg, 1, 8, img) == 8 && !memcmp(mg, "MComprHD", 8)) {
                fclose(img); img = 0; is_chd = 1;
                char err[200]; int ns = 0;
                if (port_chd_open(path, &ns, err, sizeof err)) { snprintf(res, rlen, "err:%s", err); return -1; }
                img_size = (off_t)ns * 2352;
                return 0;
            }
            fseeko(img, 0, SEEK_SET);
        }
        unsigned char head[16] = { 0 };
        static const unsigned char sync[12] = { 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0 };
        fseeko(img, 0, SEEK_END); off_t size = ftello(img);
        fseeko(img, 0, SEEK_SET);
        int raw = fread(head, 1, 16, img) == 16 && !memcmp(head, sync, 12);
        if (!raw && size % 2352 == 0 && size % 2048 != 0) raw = 1;
        sec_size = raw ? 2352 : 2048; sec_off = 0;
        if (raw) { unsigned char m = 2; fseeko(img, 16L * 2352 + 15, SEEK_SET); if (fread(&m, 1, 1, img) != 1) m = 2; sec_off = m == 1 ? 16 : 24; }
        img_size = size;
    }
    return 0;
}
static void close_img(void) { if (is_chd) port_chd_close(); else if (img) { fclose(img); img = 0; } }

/* "sha1:<40 hex>", or "err:<reason>" ("nomain0" when the image has no EXE/MAIN0.EXE) */
static const char *check(const char *path, char *res, int rlen) {
    progress = 0;
    if (open_img(path, res, rlen)) return res;
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
    close_img();
    progress = 1000;
    return res;
}

/* ---- the whole image: the volume size against the file size (a truncated copy), then every sector of every file
   of the ISO9660 tree read (a .chd decompressed), hashed in tree order.  "ok:<files> files, <sectors> sectors read,
   volume <n> sectors, image <bytes> bytes, content sha1 <hex>" or "err:<reason>" */
static unsigned vol_sectors, img_sectors, sectors_done, files_done; static Sha1 content; static char walk_err[200];
static int walk(unsigned lba, unsigned size, int depth) {
    if (depth > 10) return 0;
    for (unsigned s = 0; s < (size + 2047) / 2048; s++) {
        unsigned char b[2048];
        if (read_sector((int)(lba + s), b)) { snprintf(walk_err, sizeof walk_err, "directory sector %u unreadable", lba + s); return -1; }
        for (int o = 0; o < 2048 && b[o]; o += b[o]) {
            if (o + 33 > 2048 || o + 33 + b[o + 32] > 2048) break;
            int nl = b[o + 32]; const unsigned char *nm = b + o + 33;
            if (nl == 1 && (nm[0] == 0 || nm[0] == 1)) continue;           /* . and .. */
            unsigned el = le32(b + o + 2), es = le32(b + o + 10), ns = (es + 2047) / 2048;
            char name[64]; int k = nl < 63 ? nl : 63; memcpy(name, nm, k); name[k] = 0;
            if (img_sectors && el + ns > img_sectors) {
                snprintf(walk_err, sizeof walk_err, "%s ends at sector %u, past the end of the image (%u sectors): truncated copy", name, el + ns, img_sectors);
                return -1;
            }
            if (b[o + 25] & 2) { if (walk(el, es, depth + 1)) return -1; continue; }
            for (unsigned i = 0; i < ns; i++) {
                unsigned char d[2048];
                if (read_sector((int)(el + i), d)) { snprintf(walk_err, sizeof walk_err, "%s: sector %u unreadable", name, el + i); return -1; }
                sha1_update(&content, d, i + 1 < ns ? 2048 : es - 2048 * i);
                sectors_done++;
                progress = vol_sectors ? (int)(1000ull * (sectors_done < vol_sectors ? sectors_done : vol_sectors) / vol_sectors) : 0;
            }
            files_done++;
        }
    }
    return 0;
}
static const char *verify(const char *path, char *res, int rlen) {
    progress = 0;
    if (open_img(path, res, rlen)) return res;
    unsigned char pvd[2048];
    if (read_sector(16, pvd) || memcmp(pvd + 1, "CD001", 5)) { snprintf(res, rlen, "err:sector 16 is not an ISO9660 volume descriptor"); close_img(); return res; }
    vol_sectors = le32(pvd + 80);
    img_sectors = is_chd ? 0 : (unsigned)(img_size / sec_size);   /* (a .chd decompresses or fails: walk() reads it all) */
    if (!is_chd && img_sectors < vol_sectors)
        snprintf(res, rlen, "err:truncated image: %u of the volume's %u sectors (%lld bytes, %u-byte sectors)", img_sectors, vol_sectors, (long long)img_size, sec_size);
    else {
        sectors_done = files_done = 0; walk_err[0] = 0; sha1_init(&content);
        if (walk(le32(pvd + 158), le32(pvd + 166), 0)) snprintf(res, rlen, "err:%s", walk_err);
        else { char hex[41]; sha1_hex(&content, hex);
               snprintf(res, rlen, "ok:%u files, %u sectors read, volume %u sectors, image %lld bytes (%u-byte sectors%s), content sha1 %s",
                        files_done, sectors_done, vol_sectors, (long long)img_size, is_chd ? 2352 : sec_size, is_chd ? ", chd" : "", hex); }
    }
    close_img();
    progress = 1000;
    return res;
}
JNIEXPORT jstring JNICALL Java_io_github_tombi_port_LauncherActivity_nativeVerifyImage(JNIEnv *env, jclass cls, jstring jpath) {
    (void)cls;
    const char *path = (*env)->GetStringUTFChars(env, jpath, NULL);
    char res[400];
    verify(path, res, sizeof res);
    (*env)->ReleaseStringUTFChars(env, jpath, path);
    return (*env)->NewStringUTF(env, res);
}

/* ---- which game the image is (the game library, LauncherActivity GameVersion): the boot executable SYSTEM.CNF names
   (`BOOT = cdrom:\SCUS_942.36;1`) and its SHA-1, as tools/extract_data.py boot_exe.  On the PAL discs the boot exe
   (SCES_013.31 / SCES_013.30) only loads EXE/MAIN0.EXE, the game: the game executable is MAIN0 there.  An open image. */
typedef struct { char serial[16], boot[64], boot_sha1[41], main0_sha1[41]; } DiscId;
static int sha1_extent(unsigned lba, unsigned size, char *hex) {
    Sha1 s; sha1_init(&s);
    unsigned left = size, done = 0;
    for (unsigned l = lba; left > 0; l++) {
        unsigned char b[2048]; unsigned n = left < 2048 ? left : 2048;
        if (read_sector((int)l, b)) return -1;
        sha1_update(&s, b, n); left -= n; done += n;
        progress = (int)(1000ull * done / (size ? size : 1));
    }
    sha1_hex(&s, hex);
    return 0;
}
/* path "EXE/MAIN0.EXE" (either slash) from the root directory: its extent */
static int find_path(const unsigned char *pvd, const char *path, unsigned *elba, unsigned *esize) {
    unsigned lba = le32(pvd + 158), size = le32(pvd + 166);
    char part[64];
    while (*path) {
        int k = 0;
        while (*path && *path != '/' && *path != '\\' && k < 63) part[k++] = *path++;
        part[k] = 0;
        while (*path == '/' || *path == '\\') path++;
        if (!k) continue;
        if (!find(lba, size, part, &lba, &size)) return 0;
    }
    *elba = lba; *esize = size;
    return 1;
}
/* 0 = identified (some fields may be empty: no SYSTEM.CNF, no MAIN0), else res holds "err:..." */
static int identify(DiscId *id, char *res, int rlen) {
    memset(id, 0, sizeof *id);
    unsigned char pvd[2048], cnf[4096]; unsigned cl, cs, bl, bs, ml, ms;
    if (read_sector(16, pvd) || memcmp(pvd + 1, "CD001", 5)) { snprintf(res, rlen, "err:sector 16 is not an ISO9660 volume descriptor"); return -1; }
    int have_main0 = find_path(pvd, "EXE/MAIN0.EXE", &ml, &ms);
    if (have_main0 && sha1_extent(ml, ms, id->main0_sha1)) { snprintf(res, rlen, "err:read error in MAIN0.EXE"); return -1; }
    if (find_path(pvd, "SYSTEM.CNF", &cl, &cs)) {
        unsigned n = cs < sizeof cnf - 1 ? cs : sizeof cnf - 1;
        for (unsigned s = 0; s * 2048 < n; s++) if (read_sector((int)(cl + s), cnf + s * 2048)) { snprintf(res, rlen, "err:SYSTEM.CNF unreadable"); return -1; }
        cnf[n] = 0;
        for (char *line = (char *)cnf; line && *line; ) {
            char *nl = strpbrk(line, "\r\n"); if (nl) *nl = 0;
            while (*line == ' ' || *line == '\t') line++;
            if (!strncasecmp(line, "BOOT", 4)) {
                char *v = strchr(line, '=');
                if (v) {
                    v++; while (*v == ' ' || *v == '\t') v++;
                    char *c = strchr(v, ':'); if (c) v = c + 1;
                    while (*v == '\\' || *v == '/') v++;
                    int k = 0;
                    while (v[k] && v[k] != ';' && v[k] != ' ' && v[k] != '\t' && k < (int)sizeof id->boot - 1) { id->boot[k] = v[k] == '\\' ? '/' : v[k]; k++; }
                    id->boot[k] = 0;
                }
                break;
            }
            line = nl ? nl + 1 : NULL;
        }
    }
    if (id->boot[0]) {   /* SCUS_942.36 -> SCUS-94236 */
        const char *b = strrchr(id->boot, '/'); b = b ? b + 1 : id->boot;
        char u[16]; int k = 0;
        for (const char *p = b; *p && k < 15; p++) if (*p != '.') u[k++] = (char)(*p >= 'a' && *p <= 'z' ? *p - 32 : *p);
        u[k] = 0;
        if (k == 10 && u[4] == '_') snprintf(id->serial, sizeof id->serial, "%.4s-%s", u, u + 5);
        if (have_main0 && !strncasecmp(b, "SCES_", 5)) { snprintf(id->boot, sizeof id->boot, "EXE/MAIN0.EXE"); memcpy(id->boot_sha1, id->main0_sha1, 41); }
        else if (find_path(pvd, id->boot, &bl, &bs) && sha1_extent(bl, bs, id->boot_sha1)) { snprintf(res, rlen, "err:read error in %s", id->boot); return -1; }
    } else if (have_main0) { snprintf(id->boot, sizeof id->boot, "EXE/MAIN0.EXE"); memcpy(id->boot_sha1, id->main0_sha1, 41); }
    return 0;
}
/* "id:serial=<SCES-01331>;boot=<EXE/MAIN0.EXE>;sha1=<boot sha1>;main0=<MAIN0 sha1>" (empty fields: not found), or
   "err:<reason>"; "err:noboot" when the image names no game executable at all */
static const char *identify_image(const char *path, char *res, int rlen) {
    progress = 0;
    if (open_img(path, res, rlen)) return res;
    DiscId id;
    if (!identify(&id, res, rlen)) {
        if (!id.boot_sha1[0]) snprintf(res, rlen, "err:noboot");
        else snprintf(res, rlen, "id:serial=%s;boot=%s;sha1=%s;main0=%s", id.serial, id.boot, id.boot_sha1, id.main0_sha1);
    }
    close_img();
    progress = 1000;
    return res;
}
JNIEXPORT jstring JNICALL Java_io_github_tombi_port_LauncherActivity_nativeIdentify(JNIEnv *env, jclass cls, jstring jpath) {
    (void)cls;
    const char *path = (*env)->GetStringUTFChars(env, jpath, NULL);
    char res[400];
    identify_image(path, res, sizeof res);
    (*env)->ReleaseStringUTFChars(env, jpath, path);
    return (*env)->NewStringUTF(env, res);
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

/* ---- extraction into a data/ folder for the port's --data mode (the same layout and manifest as
   tools/extract_data.py, docs/PORT.md "Running without the disc image: data/"): every ISO9660 file as 2048-byte user
   data, the files holding Mode 2 Form 2 sectors (the MOVIE .STR files, XA) as raw 2352-byte sectors, the sectors no file
   covers as _disc/<lba>.raw; each file written as <path>.part, size checked, renamed; manifest.txt last. */
typedef struct { unsigned lba, n, stored, bytes; char path[160]; } Ent;
static Ent *ents; static int nents, cents;
static volatile int ex_cancel, ex_done, ex_total;
static char ex_err[300];

static int raw_sector(unsigned lba, unsigned char *o) {
    static const unsigned char sync[12] = { 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0 };
    if (is_chd) return port_chd_read_raw((int)lba, o) ? -1 : 0;
    if (sec_size == 2352) {
        if (fseeko(img, (off_t)lba * 2352, SEEK_SET)) return -1;
        return fread(o, 1, 2352, img) == 2352 ? 0 : -1;
    }
    /* a cooked image: a synthesised Mode 2 Form 1 frame (zero EDC/ECC), as tools/extract_data.py Image.raw_sector */
    unsigned a = lba + 150;
    #define BCD(x) (unsigned char)(((x) / 10) << 4 | (x) % 10)
    memcpy(o, sync, 12); o[12] = BCD(a / 4500); o[13] = BCD(a / 75 % 60); o[14] = BCD(a % 75); o[15] = 2;
    static const unsigned char sub[8] = { 0, 0, 8, 0, 0, 0, 8, 0 };
    memcpy(o + 16, sub, 8);
    memset(o + 24 + 2048, 0, 280);
    return read_sector((int)lba, o + 24);
}
static int has_form2(unsigned lba, unsigned n) {
    if (!is_chd && sec_size == 2048) return 0;
    unsigned char r[2352];
    for (unsigned i = 0; i < n; i++) { if (raw_sector(lba + i, r)) return -1; if (r[18] & 0x20) return 1; }
    return 0;
}
static int add_ent(unsigned lba, unsigned n, unsigned stored, unsigned bytes, const char *path) {
    if (nents == cents) { int c = cents ? cents * 2 : 1024; Ent *e = realloc(ents, (size_t)c * sizeof *e); if (!e) return -1; ents = e; cents = c; }
    Ent *e = &ents[nents++]; e->lba = lba; e->n = n; e->stored = stored; e->bytes = bytes;
    snprintf(e->path, sizeof e->path, "%s", path);
    return 0;
}
static int list_files(unsigned lba, unsigned size, const char *pre, int depth) {
    if (depth > 10) return 0;
    for (unsigned s = 0; s < (size + 2047) / 2048; s++) {
        unsigned char b[2048];
        if (read_sector((int)(lba + s), b)) { snprintf(ex_err, sizeof ex_err, "directory %s: sector %u unreadable", pre, lba + s); return -1; }
        for (int o = 0; o < 2048 && b[o]; o += b[o]) {
            if (o + 33 > 2048 || o + 33 + b[o + 32] > 2048) break;
            int nl = b[o + 32]; const unsigned char *nm = b + o + 33;
            if (nl == 1 && (nm[0] == 0 || nm[0] == 1)) continue;
            char name[64]; int k = 0;
            for (int i = 0; i < nl && i < 63 && nm[i] != ';'; i++) name[k++] = (char)nm[i];
            name[k] = 0;
            unsigned el = le32(b + o + 2), es = le32(b + o + 10);
            char path[160]; snprintf(path, sizeof path, "%s%s", pre, name);
            if (b[o + 25] & 2) { char sub[164]; snprintf(sub, sizeof sub, "%s/", path); if (list_files(el, es, sub, depth + 1)) return -1; continue; }
            unsigned n = (es + 2047) / 2048;
            size_t pl = strlen(path);
            int raw = (pl > 4 && (!strcasecmp(path + pl - 4, ".STR"))) || (pl > 3 && !strcasecmp(path + pl - 3, ".XA"));
            if (!raw) { int f2 = has_form2(el, n); if (f2 < 0) { snprintf(ex_err, sizeof ex_err, "%s: sector unreadable", path); return -1; } raw = f2; }
            if (add_ent(el, n, raw ? 2352 : 2048, raw ? n * 2352 : es, path)) { snprintf(ex_err, sizeof ex_err, "out of memory"); return -1; }
        }
    }
    return 0;
}
static int ent_cmp(const void *a, const void *b) { const Ent *x = a, *y = b; return x->lba < y->lba ? -1 : x->lba > y->lba; }
static int mkdirs(char *path) {     /* the directories of path (a file path) */
    for (char *p = path + 1; *p; p++) if (*p == '/') { *p = 0; if (mkdir(path, 0700) && errno != EEXIST) { *p = '/'; return -1; } *p = '/'; }
    return 0;
}
static const char *extract(const char *image, const char *out, char *res, int rlen) {
    progress = 0; ex_cancel = 0; ex_done = 0; ex_total = 0; ex_err[0] = 0; nents = 0;
    if (open_img(image, res, rlen)) return res;
    unsigned char pvd[2048], *buf = NULL; char main0[41] = ""; Sha1 content; sha1_init(&content);
    DiscId id;
    unsigned total_sectors = 0, done_sectors = 0, sectors;
    if (read_sector(16, pvd) || memcmp(pvd + 1, "CD001", 5)) { snprintf(res, rlen, "err:sector 16 is not an ISO9660 volume descriptor"); goto out; }
    vol_sectors = le32(pvd + 80);
    sectors = (unsigned)(img_size / (is_chd ? 2352 : sec_size));
    if (sectors < vol_sectors) { snprintf(res, rlen, "err:truncated image: %u of the volume's %u sectors (%lld bytes)", sectors, vol_sectors, (long long)img_size); goto out; }
    if (list_files(le32(pvd + 158), le32(pvd + 166), "", 0)) { snprintf(res, rlen, "err:%s", ex_err); goto out; }
    if (identify(&id, res, rlen)) goto out;   /* the game version for the manifest (port2/shims/psyq.c checks boot_sha1) */
    progress = 0;
    qsort(ents, (size_t)nents, sizeof *ents, ent_cmp);
    {   /* the sectors no file covers */
        unsigned prev = 0; int nf = nents;
        for (int i = 0; i < nf; i++) {
            if (ents[i].lba > prev) { char p[32]; snprintf(p, sizeof p, "_disc/%06u.raw", prev); if (add_ent(prev, ents[i].lba - prev, 2352, (ents[i].lba - prev) * 2352, p)) goto oom; }
            if (ents[i].lba + ents[i].n > prev) prev = ents[i].lba + ents[i].n;
        }
        if (prev < sectors) { char p[32]; snprintf(p, sizeof p, "_disc/%06u.raw", prev); if (add_ent(prev, sectors - prev, 2352, (sectors - prev) * 2352, p)) goto oom; }
        qsort(ents, (size_t)nents, sizeof *ents, ent_cmp);
    }
    for (int i = 0; i < nents; i++) total_sectors += ents[i].n;
    ex_total = nents;
    buf = malloc(64 * 2352);
    if (!buf) goto oom;
    mkdir(out, 0700);
    for (int i = 0; i < nents; i++) {
        Ent *e = &ents[i];
        char dst[700], part[710];
        snprintf(dst, sizeof dst, "%s/%s", out, e->path); snprintf(part, sizeof part, "%s.part", dst);
        if (mkdirs(dst)) { snprintf(res, rlen, "err:mkdir for %s: %s", e->path, strerror(errno)); goto out; }
        FILE *o = fopen(part, "wb");
        if (!o) { snprintf(res, rlen, "err:cannot write %s: %s", e->path, strerror(errno)); goto out; }
        unsigned left = e->bytes; int bad = 0;
        Sha1 m0; int is_main0 = !strcasecmp(e->path, "EXE/MAIN0.EXE"); if (is_main0) sha1_init(&m0);
        for (unsigned s0 = 0; s0 < e->n && !bad; s0 += 64) {
            unsigned cnt = e->n - s0 < 64 ? e->n - s0 : 64, len = 0;
            for (unsigned k = 0; k < cnt; k++) {
                if (e->stored == 2352 ? raw_sector(e->lba + s0 + k, buf + len) : read_sector((int)(e->lba + s0 + k), buf + len)) {
                    snprintf(res, rlen, "err:%s: sector %u unreadable (damaged or incomplete image)", e->path, e->lba + s0 + k); bad = 1; break;
                }
                len += e->stored;
            }
            if (bad) break;
            if (len > left) len = left;
            if (fwrite(buf, 1, len, o) != len) { snprintf(res, rlen, "err:writing %s: %s (storage full?)", e->path, strerror(errno)); bad = 1; break; }
            if (strncmp(e->path, "_disc/", 6)) sha1_update(&content, buf, len);
            if (is_main0) sha1_update(&m0, buf, len);
            left -= len; done_sectors += cnt;
            progress = (int)(1000ull * done_sectors / (total_sectors ? total_sectors : 1));
            if (ex_cancel) { snprintf(res, rlen, "err:cancelled"); bad = 1; break; }
        }
        if (fflush(o) || fsync(fileno(o))) { if (!bad) snprintf(res, rlen, "err:writing %s: %s", e->path, strerror(errno)); bad = 1; }
        fclose(o);
        struct stat st;
        if (!bad && (stat(part, &st) || (unsigned)st.st_size != e->bytes)) { snprintf(res, rlen, "err:%s: %lld bytes written, %u expected", e->path, (long long)st.st_size, e->bytes); bad = 1; }
        if (!bad && rename(part, dst)) { snprintf(res, rlen, "err:rename %s: %s", e->path, strerror(errno)); bad = 1; }
        if (bad) { unlink(part); goto out; }
        if (is_main0) sha1_hex(&m0, main0);
        ex_done = i + 1;
    }
    {   /* the manifest last: its presence means a complete extraction */
        char mp[600], mt[610];
        snprintf(mp, sizeof mp, "%s/manifest.txt", out); snprintf(mt, sizeof mt, "%s.part", mp);
        FILE *m = fopen(mt, "w");
        if (!m) { snprintf(res, rlen, "err:cannot write the manifest: %s", strerror(errno)); goto out; }
        const char *base = strrchr(image, '/');
        fprintf(m, "tombi-data 1\n# one-time extraction of the player's own disc for the native port (Android launcher, port2/android/disccheck.c)\n");
        fprintf(m, "sectors %u\nmain0_sha1 %s\nsource %s\n", sectors, main0, base ? base + 1 : image);
        if (id.boot_sha1[0])   /* the game version (as tools/extract_data.py): serial, game executable, its sha1 */
            fprintf(m, "serial %s\nboot %s\nboot_sha1 %s\n", id.serial[0] ? id.serial : "?", id.boot, id.boot_sha1);
        fprintf(m, "# file <lba> <sectors> <stored sector bytes> <file bytes> <path>\n");
        for (int i = 0; i < nents; i++) fprintf(m, "file %u %u %u %u %s\n", ents[i].lba, ents[i].n, ents[i].stored, ents[i].bytes, ents[i].path);
        if (fflush(m) || fsync(fileno(m))) { fclose(m); snprintf(res, rlen, "err:writing the manifest: %s", strerror(errno)); goto out; }
        fclose(m);
        if (rename(mt, mp)) { snprintf(res, rlen, "err:rename the manifest: %s", strerror(errno)); goto out; }
    }
    { char hex[41]; sha1_hex(&content, hex);
      snprintf(res, rlen, "ok:%d files, %u sectors (%u-byte source sectors%s), main0 %s, %s %s %s, content sha1 %s", nents, sectors, is_chd ? 2352 : sec_size, is_chd ? ", chd" : "",
               main0[0] ? main0 : "-", id.serial[0] ? id.serial : "?", id.boot[0] ? id.boot : "boot", id.boot_sha1[0] ? id.boot_sha1 : "-", hex); }
    goto out;
oom:
    snprintf(res, rlen, "err:out of memory");
out:
    free(buf); free(ents); ents = NULL; nents = cents = 0;
    close_img();
    progress = 1000;
    return res;
}
JNIEXPORT jstring JNICALL Java_io_github_tombi_port_LauncherActivity_nativeExtract(JNIEnv *env, jclass cls, jstring jimg, jstring jout) {
    (void)cls;
    const char *im = (*env)->GetStringUTFChars(env, jimg, NULL), *out = (*env)->GetStringUTFChars(env, jout, NULL);
    char res[500];
    extract(im, out, res, sizeof res);
    (*env)->ReleaseStringUTFChars(env, jimg, im); (*env)->ReleaseStringUTFChars(env, jout, out);
    return (*env)->NewStringUTF(env, res);
}
/* files done << 16 | files total, while extracting */
JNIEXPORT jint JNICALL Java_io_github_tombi_port_LauncherActivity_nativeExtractCount(JNIEnv *env, jclass cls) {
    (void)env; (void)cls; return (jint)((ex_done & 0xFFFF) << 16 | (ex_total & 0xFFFF));
}
JNIEXPORT void JNICALL Java_io_github_tombi_port_LauncherActivity_nativeCancel(JNIEnv *env, jclass cls) { (void)env; (void)cls; ex_cancel = 1; }

/* .7z: the disc image inside an archive (LauncherActivity.importDisc) */
#include "un7z.c"
