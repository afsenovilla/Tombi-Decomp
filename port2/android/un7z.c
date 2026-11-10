/* .7z support of the Android launcher's "Add game" (LauncherActivity.importDisc, DiscArchive): the disc image inside a
   .7z written to the app's cache, where the ISO reader (disccheck.c) has the random access it needs.  Included by
   disccheck.c (its SHA-1, cancel flag and progress); the 7z container and the decoders are Igor Pavlov's LZMA SDK
   (public domain, port2/third_party/lzma-7z, built through lzma7z.c).

   The SDK's own extraction (SzArEx_Extract) decodes a whole solid block into memory: too much for a 500 MB disc
   image on a phone (and impossible in a 32-bit process).  Here a block of one coder (LZMA, LZMA2 or Copy: what 7-Zip
   writes for a disc image) is decoded as a stream, 1 MB at a time, the bytes of each wanted file routed to it as they
   come; a block of several coders (BCJ / BCJ2 / Delta + LZMA: executables) is decoded by the SDK in memory when it is
   small (the loose files of a disc), else refused with the reason.

   nativeUn7z(archive, outDir) -> "ok\n" + lines, or "err:<reason>" ("err:space:<need>:<free>", "err:cancelled"):
     image <file in outDir>\t<path in the archive>     the disc image (largest .bin/.img/.iso/.chd, or the one the .cue names)
     cue <data track the .cue names>                     (only when no image)
     cnf                                                 SYSTEM.CNF written to outDir/SYSTEM.CNF (only when no image)
     exe <BASENAME> <sha1>                               the executables (MAIN0.EXE, SCUS_942.36 ...) (only when no image)
     nested                                              another archive inside
     entries <n>
   nativeUnpackBytes(0) = bytes written so far, (1) = the image's size. */
#include <sys/statvfs.h>
#include "../third_party/lzma-7z/7z.h"
#include "../third_party/lzma-7z/7zCrc.h"
#include "../third_party/lzma-7z/LzmaDec.h"
#include "../third_party/lzma-7z/Lzma2Dec.h"

static volatile long long un_done, un_total;

static void *un_alloc(ISzAllocPtr p, size_t n) { (void)p; return n ? malloc(n) : NULL; }
static void un_free(ISzAllocPtr p, void *a) { (void)p; free(a); }
static const ISzAlloc un_mem = { un_alloc, un_free };

typedef struct { ISeekInStream vt; FILE *f; } UnFile;
static SRes un_read(ISeekInStreamPtr pp, void *buf, size_t *size) {
    const UnFile *p = (const UnFile *)pp;
    size_t n = *size ? fread(buf, 1, *size, p->f) : 0;
    if (n < *size && ferror(p->f)) { *size = n; return SZ_ERROR_READ; }
    *size = n; return SZ_OK;
}
static SRes un_seek(ISeekInStreamPtr pp, Int64 *pos, ESzSeek origin) {
    const UnFile *p = (const UnFile *)pp;
    int w = origin == SZ_SEEK_SET ? SEEK_SET : origin == SZ_SEEK_CUR ? SEEK_CUR : SEEK_END;
    if (fseeko(p->f, (off_t)*pos, w)) return SZ_ERROR_READ;
    *pos = (Int64)ftello(p->f);
    return SZ_OK;
}

/* what to do with the bytes of a file of the archive */
typedef struct { UInt32 file; UInt64 off, size; int kind; FILE *out; Sha1 sha; char name[64]; unsigned char *mem; size_t memlen; } UnWant;
enum { W_IMAGE, W_CUE, W_CNF, W_EXE };

static void un_utf8(const UInt16 *s, char *o, size_t cap) {   /* (names: ASCII / Latin-1 / BMP is enough here) */
    size_t k = 0;
    for (; *s && k + 4 < cap; s++) {
        unsigned c = *s;
        if (c < 0x80) o[k++] = (char)(c == '\\' ? '/' : c);
        else if (c < 0x800) { o[k++] = (char)(0xC0 | c >> 6); o[k++] = (char)(0x80 | (c & 63)); }
        else { o[k++] = (char)(0xE0 | c >> 12); o[k++] = (char)(0x80 | (c >> 6 & 63)); o[k++] = (char)(0x80 | (c & 63)); }
    }
    o[k] = 0;
}
static const char *un_base(const char *p) { const char *b = strrchr(p, '/'); return b ? b + 1 : p; }
static int un_ends(const char *s, const char *ext) { size_t a = strlen(s), b = strlen(ext); return a >= b && !strcasecmp(s + a - b, ext); }

static void un_deliver(UnWant *w, int nw, UInt64 pos, const unsigned char *b, size_t n) {
    for (int i = 0; i < nw; i++) {
        UInt64 a = pos > w[i].off ? pos : w[i].off, z = pos + n < w[i].off + w[i].size ? pos + n : w[i].off + w[i].size;
        if (a >= z) continue;
        const unsigned char *s = b + (a - pos); size_t len = (size_t)(z - a);
        if (w[i].kind == W_IMAGE) { if (w[i].out && fwrite(s, 1, len, w[i].out) != len) { fclose(w[i].out); w[i].out = NULL; } un_done += len; }
        else if (w[i].kind == W_EXE) sha1_update(&w[i].sha, s, (unsigned)len);
        else if (w[i].mem && a - w[i].off + len <= w[i].memlen) memcpy(w[i].mem + (a - w[i].off), s, len);
    }
}

/* one folder (solid block) decoded from its start up to `end` bytes of output, the wanted files' bytes routed */
static const char *un_folder(CSzArEx *db, FILE *f, ILookInStreamPtr look, UInt32 fo, UInt64 end, UnWant *w, int nw, char *res, int rlen) {
    const CSzAr *p = &db->db;
    CSzFolder folder; CSzData sd;
    const Byte *data = p->CodersData + p->FoCodersOffsets[fo];
    sd.Data = data; sd.Size = p->FoCodersOffsets[fo + 1] - p->FoCodersOffsets[fo];
    if (SzGetNextFolderItem(&folder, &sd) != SZ_OK) { snprintf(res, rlen, "err:7z: damaged folder"); return res; }
    UInt64 usize = SzAr_GetFolderUnpackSize(p, fo);
    if (end > usize) end = usize;
    if (folder.NumCoders != 1 || folder.NumPackStreams != 1 ||
        (folder.Coders[0].MethodID != 0x030101 && folder.Coders[0].MethodID != 0x21 && folder.Coders[0].MethodID != 0)) {
        /* executables' filters (BCJ, BCJ2, Delta ...): the SDK decodes the block in memory, when it is small */
        if (usize > (128u << 20)) {
            snprintf(res, rlen, "err:7z: a %u MB block packed with filters (method %x+%u): extract it with 7-Zip",
                     (unsigned)(usize >> 20), (unsigned)folder.Coders[0].MethodID, folder.NumCoders - 1); return res;
        }
        Byte *buf = malloc(usize ? (size_t)usize : 1);
        if (!buf) { snprintf(res, rlen, "err:out of memory"); return res; }
        SRes r = SzAr_DecodeFolder(p, fo, look, db->dataPos, buf, (size_t)usize, &un_mem);
        if (r != SZ_OK) { free(buf); snprintf(res, rlen, r == SZ_ERROR_UNSUPPORTED ? "err:7z: compression method not supported (PPMd, BZip2 or Deflate): extract it with 7-Zip" : "err:7z: damaged data (%d)", r); return res; }
        un_deliver(w, nw, 0, buf, (size_t)end);
        free(buf);
        return NULL;
    }
    const CSzCoderInfo *c = &folder.Coders[0];
    const Byte *props = data + c->PropsOffset;
    UInt64 pack = db->dataPos + p->PackPositions[p->FoStartPackStreamIndex[fo]];
    UInt64 packLeft = p->PackPositions[p->FoStartPackStreamIndex[fo] + 1] - p->PackPositions[p->FoStartPackStreamIndex[fo]];
    if (fseeko(f, (off_t)pack, SEEK_SET)) { snprintf(res, rlen, "err:7z: seek"); return res; }
    enum { IN = 1 << 18, OUT = 1 << 20 };
    unsigned char *in = malloc(IN), *out = malloc(OUT);
    CLzmaDec lz; CLzma2Dec lz2; int kind = c->MethodID == 0x21 ? 2 : c->MethodID ? 1 : 0;
    LzmaDec_CONSTRUCT(&lz); Lzma2Dec_CONSTRUCT(&lz2);
    const char *err = NULL;
    if (!in || !out) { err = "err:out of memory"; goto done; }
    if (kind == 1) { if (LzmaDec_Allocate(&lz, props, c->PropsSize, &un_mem) != SZ_OK) { err = "err:out of memory (LZMA dictionary)"; goto done; } LzmaDec_Init(&lz); }
    if (kind == 2) { if (c->PropsSize < 1 || Lzma2Dec_Allocate(&lz2, props[0], &un_mem) != SZ_OK) { err = "err:out of memory (LZMA2 dictionary)"; goto done; } Lzma2Dec_Init(&lz2); }
    {
        UInt64 pos = 0; size_t inLen = 0, inPos = 0;
        while (pos < end) {
            if (ex_cancel) { err = "err:cancelled"; goto done; }
            if (inPos == inLen && packLeft > 0) {
                size_t want = packLeft < IN ? (size_t)packLeft : IN;
                inLen = fread(in, 1, want, f); inPos = 0;
                if (inLen == 0) { err = "err:7z: the archive ends early (incomplete download?)"; goto done; }
                packLeft -= inLen;
            }
            SizeT outLen = OUT; if (end - pos < outLen) outLen = (SizeT)(end - pos);
            SizeT srcLen = inLen - inPos;
            if (kind == 0) { if (srcLen < outLen) outLen = srcLen; memcpy(out, in + inPos, outLen); srcLen = outLen; }
            else {
                ELzmaStatus st; SRes r;
                if (kind == 1) r = LzmaDec_DecodeToBuf(&lz, out, &outLen, in + inPos, &srcLen, LZMA_FINISH_ANY, &st);
                else r = Lzma2Dec_DecodeToBuf(&lz2, out, &outLen, in + inPos, &srcLen, LZMA_FINISH_ANY, &st);
                if (r != SZ_OK) { err = "err:7z: damaged data"; goto done; }
                if (outLen == 0 && srcLen == 0 && (packLeft == 0 || st == LZMA_STATUS_FINISHED_WITH_MARK)) { err = "err:7z: the data ends early"; goto done; }
            }
            inPos += srcLen;
            if (outLen) { un_deliver(w, nw, pos, out, outLen); pos += outLen; }
        }
    }
done:
    if (kind == 1) LzmaDec_Free(&lz, &un_mem);
    if (kind == 2) Lzma2Dec_Free(&lz2, &un_mem);
    free(in); free(out);
    if (err) { snprintf(res, rlen, "%s", err); return res; }
    return NULL;
}

static const char *un7z(const char *archive, const char *outDir, char *res, int rlen) {
    un_done = 0; un_total = 0; ex_cancel = 0;
    static int crc_ready; if (!crc_ready) { CrcGenerateTable(); crc_ready = 1; }
    FILE *f = fopen(archive, "rb");
    if (!f) { snprintf(res, rlen, "err:cannot open %s", archive); return res; }
    UnFile uf; uf.vt.Read = un_read; uf.vt.Seek = un_seek; uf.f = f;
    CLookToRead2 look; LookToRead2_CreateVTable(&look, 0);
    look.buf = malloc(1 << 16); look.bufSize = 1 << 16; look.realStream = &uf.vt; LookToRead2_INIT(&look);
    CSzArEx db; SzArEx_Init(&db);
    UnWant *w = NULL; int nw = 0;
    UInt16 *nm16 = NULL; size_t nm16cap = 0;
    char out[1200];
    int image = -1, cue = -1, cnf = -1, nested = 0, nimg = 0;
    if (!look.buf) { snprintf(res, rlen, "err:out of memory"); goto end; }
    SRes r = SzArEx_Open(&db, &look.vt, &un_mem, &un_mem);
    if (r != SZ_OK) {
        snprintf(res, rlen, r == SZ_ERROR_NO_ARCHIVE ? "err:7z: not a 7z archive" : r == SZ_ERROR_UNSUPPORTED ? "err:7z: archive format not supported (encrypted?)"
                 : r == SZ_ERROR_INPUT_EOF ? "err:7z: the archive is incomplete" : "err:7z: damaged archive (%d)", r);
        goto end;
    }
    w = calloc(db.NumFiles + 1, sizeof *w);
    if (!w) { snprintf(res, rlen, "err:out of memory"); goto end; }
    /* the files: the image (the .cue's name decides when it can be read first; else the largest), cue, SYSTEM.CNF, exes */
    char **names = calloc(db.NumFiles + 1, sizeof *names);
    if (!names) { snprintf(res, rlen, "err:out of memory"); goto end; }
    int cnfDepth = 99;
    for (UInt32 i = 0; i < db.NumFiles; i++) {
        size_t len = SzArEx_GetFileNameUtf16(&db, i, NULL);
        if (len > nm16cap) { free(nm16); nm16cap = len + 64; nm16 = malloc(nm16cap * sizeof *nm16); if (!nm16) { snprintf(res, rlen, "err:out of memory"); goto names_end; } }
        SzArEx_GetFileNameUtf16(&db, i, nm16);
        char n8[700]; un_utf8(nm16, n8, sizeof n8);
        names[i] = strdup(n8);
        if (SzArEx_IsDir(&db, i) || strstr(n8, "__MACOSX/")) continue;
        UInt64 size = SzArEx_GetFileSize(&db, i);
        const char *b = un_base(n8);
        if ((un_ends(b, ".bin") || un_ends(b, ".img") || un_ends(b, ".iso") || un_ends(b, ".chd")) && size >= (4u << 20)) {
            nimg++;
            if (image < 0 || size > SzArEx_GetFileSize(&db, image)) image = (int)i;
        } else if (un_ends(b, ".cue") && size < 65536 && cue < 0) cue = (int)i;
        else if (!strcasecmp(b, "SYSTEM.CNF") && size < 65536) {
            int d = 0; for (const char *q = n8; *q; q++) d += *q == '/';
            if (d < cnfDepth) { cnf = (int)i; cnfDepth = d; }
        } else if (un_ends(b, ".7z") || un_ends(b, ".zip") || un_ends(b, ".rar") || un_ends(b, ".ecm") || un_ends(b, ".pbp")) nested = 1;
    }
    {   /* pass 1: the small files (.cue; without an image: SYSTEM.CNF and the executables) */
        char cuebuf[65537] = { 0 }, track[300] = "";
        nw = 0;
        if (cue >= 0) { UnWant *x = &w[nw++]; x->file = (UInt32)cue; x->kind = W_CUE; x->size = SzArEx_GetFileSize(&db, cue); x->mem = (unsigned char *)cuebuf; x->memlen = 65536; }
        if (image < 0) {
            if (cnf >= 0) { UnWant *x = &w[nw++]; x->file = (UInt32)cnf; x->kind = W_CNF; x->size = SzArEx_GetFileSize(&db, cnf); x->memlen = 65536; x->mem = calloc(1, 65537); }
            for (UInt32 i = 0; i < db.NumFiles; i++) {
                if (SzArEx_IsDir(&db, i) || !names[i]) continue;
                const char *b = un_base(names[i]);
                size_t bl = strlen(b);
                int ps = bl >= 11 && b[4] == '_' && b[8] == '.' && isalpha((unsigned char)b[0]);   /* SCUS_942.36 */
                if ((!strcasecmp(b, "MAIN0.EXE") || ps) && SzArEx_GetFileSize(&db, i) < (8u << 20)) {
                    UnWant *x = &w[nw++]; x->file = i; x->kind = W_EXE; x->size = SzArEx_GetFileSize(&db, i); sha1_init(&x->sha);
                    snprintf(x->name, sizeof x->name, "%s", b);
                }
            }
        }
        for (int k = 0; k < nw; k++) {
            UInt32 fi = w[k].file, fo = db.FileToFolder[fi];
            w[k].off = fo == (UInt32)-1 ? 0 : db.UnpackPositions[fi] - db.UnpackPositions[db.FolderToFile[fo]];
        }
        /* each folder once, up to its last wanted byte */
        for (int k = 0; k < nw; k++) {
            UInt32 fo = db.FileToFolder[w[k].file];
            if (fo == (UInt32)-1) continue;
            int first = 1; UInt64 end = 0;
            for (int j = 0; j < nw; j++) if (db.FileToFolder[w[j].file] == fo) { if (j < k) first = 0; if (w[j].off + w[j].size > end) end = w[j].off + w[j].size; }
            if (!first) continue;
            UnWant sub[256]; int ns = 0, idx[256];
            for (int j = 0; j < nw && ns < 256; j++) if (db.FileToFolder[w[j].file] == fo) { sub[ns] = w[j]; idx[ns++] = j; }
            if (un_folder(&db, f, &look.vt, fo, end, sub, ns, res, rlen)) goto pass1_fail;
            for (int j = 0; j < ns; j++) w[idx[j]] = sub[j];
        }
        if (cue >= 0) {   /* the data track: the first FILE line */
            for (char *l = cuebuf; l && *l; ) {
                char *nl = strpbrk(l, "\r\n"); if (nl) *nl = 0;
                while (*l == ' ' || *l == '\t') l++;
                if (!strncasecmp(l, "FILE ", 5)) {
                    char *v = l + 5; while (*v == ' ') v++;
                    if (*v == '"') { v++; char *q = strchr(v, '"'); if (q) *q = 0; }
                    else { char *q = strrchr(v, ' '); if (q) *q = 0; }
                    for (char *q = v; *q; q++) if (*q == '\\') *q = '/';
                    snprintf(track, sizeof track, "%s", un_base(v));
                    break;
                }
                l = nl ? nl + 1 : NULL;
            }
            if (track[0] && nimg > 1)
                for (UInt32 i = 0; i < db.NumFiles; i++) if (names[i] && !SzArEx_IsDir(&db, i) && !strcasecmp(un_base(names[i]), track)) image = (int)i;
        }
        int o = snprintf(res, rlen, "ok\nentries %u\n", db.NumFiles);
        if (nested && o < rlen) o += snprintf(res + o, rlen - o, "nested\n");
        if (image < 0) {
            if (track[0] && o < rlen) o += snprintf(res + o, rlen - o, "cue %s\n", track);
            for (int k = 0; k < nw; k++) {
                if (w[k].kind == W_CNF && w[k].mem) {
                    snprintf(out, sizeof out, "%s/SYSTEM.CNF", outDir);
                    FILE *c = fopen(out, "wb");
                    if (c) { fwrite(w[k].mem, 1, (size_t)w[k].size, c); fclose(c); if (o < rlen) o += snprintf(res + o, rlen - o, "cnf\n"); }
                }
                if (w[k].kind == W_EXE && o < rlen) { char hex[41]; sha1_hex(&w[k].sha, hex); o += snprintf(res + o, rlen - o, "exe %s %s\n", w[k].name, hex); }
            }
            goto pass1_ok;
        }
        goto pass2;
pass1_fail:
        for (int k = 0; k < nw; k++) if (w[k].kind == W_CNF) free(w[k].mem);
        goto names_end;
pass1_ok:
        for (int k = 0; k < nw; k++) if (w[k].kind == W_CNF) free(w[k].mem);
        goto names_end;
    }
pass2:
    {   /* pass 2: the image, after the free-space check (it, then its extracted files) */
        UInt64 size = SzArEx_GetFileSize(&db, image);
        struct statvfs vs;
        if (!statvfs(outDir, &vs)) {
            unsigned long long fr = (unsigned long long)vs.f_bavail * vs.f_frsize, need = 2ull * size + (64ull << 20);
            if (fr < need) { snprintf(res, rlen, "err:space:%llu:%llu", need, fr); goto names_end; }
        }
        un_total = (long long)size;
        const char *b = un_base(names[image]);
        char safe[200]; int k = 0;
        for (const char *q = b; *q && k < 190; q++) safe[k++] = (isalnum((unsigned char)*q) || strchr("._()[] !,+-", *q)) ? *q : '_';
        safe[k] = 0;
        if (!k || safe[0] == '.') snprintf(safe, sizeof safe, "image.bin");
        snprintf(out, sizeof out, "%s/%s", outDir, safe);
        UnWant x; memset(&x, 0, sizeof x);
        x.file = (UInt32)image; x.kind = W_IMAGE; x.size = size;
        UInt32 fo = db.FileToFolder[image];
        x.off = db.UnpackPositions[image] - db.UnpackPositions[db.FolderToFile[fo]];
        x.out = fopen(out, "wb");
        if (!x.out) { snprintf(res, rlen, "err:cannot write %s: %s", out, strerror(errno)); goto names_end; }
        const char *e = un_folder(&db, f, &look.vt, fo, x.off + size, &x, 1, res, rlen);
        int bad = !x.out;
        if (x.out && (fflush(x.out) || fsync(fileno(x.out)))) bad = 1;
        if (x.out) fclose(x.out);
        if (e) { unlink(out); goto names_end; }
        struct stat st;
        if (bad || stat(out, &st) || (unsigned long long)st.st_size != size) {
            unlink(out);
            snprintf(res, rlen, "err:writing the image to the cache: %s (storage full?)", strerror(errno)); goto names_end;
        }
        int o = snprintf(res, rlen, "ok\nentries %u\nimage %s\t%s\n", db.NumFiles, safe, names[image]);
        (void)o;
    }
names_end:
    for (UInt32 i = 0; i < db.NumFiles; i++) free(names[i]);
    free(names);
end:
    free(w); free(nm16);
    SzArEx_Free(&db, &un_mem);
    free(look.buf);
    fclose(f);
    return res;
}

JNIEXPORT jstring JNICALL Java_io_github_tombi_port_LauncherActivity_nativeUn7z(JNIEnv *env, jclass cls, jstring jarc, jstring jout) {
    (void)cls;
    const char *a = (*env)->GetStringUTFChars(env, jarc, NULL), *o = (*env)->GetStringUTFChars(env, jout, NULL);
    static char res[4096];
    un7z(a, o, res, sizeof res);
    (*env)->ReleaseStringUTFChars(env, jarc, a); (*env)->ReleaseStringUTFChars(env, jout, o);
    return (*env)->NewStringUTF(env, res);
}
JNIEXPORT jlong JNICALL Java_io_github_tombi_port_LauncherActivity_nativeUnpackBytes(JNIEnv *env, jclass cls, jint which) {
    (void)env; (void)cls;
    return which ? (jlong)un_total : (jlong)un_done;
}
