/* The widescreen picture strips made once per room: see ws_cache.h and docs/PORT.md "Widescreen strips cache". */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#elif !defined(__3DS__)
#include <fcntl.h>
#endif
#include "ws_cache.h"
#include "port_runtime.h"

int port_ws_cache = -1;                                  /* (-1: not set on the command line: PORT_WS_CACHE, else 2) */
int ws_cache_last;

unsigned ws_cache_hash(unsigned h, const void *p, size_t n)
{
    const unsigned char *b = (const unsigned char *)p;
    for (size_t i = 0; i < n; i++) h = (h ^ b[i]) * 16777619u;
    return h;
}

/* the cache mode: 0 off, 1 memory only, 2 memory + disk (from PORT_WS_CACHE the first time, unless set already) */
static int mode(void)
{
    if (port_ws_cache < 0) {
        const char *env = getenv("PORT_WS_CACHE");
        port_ws_cache = env && *env >= '0' && *env <= '2' ? *env - '0' : 2;
    }
    return port_ws_cache;
}

/* --- the key as stored: fixed fields, then the part sizes */
#define WSC_MAGIC 0x43535754u                            /* "TWSC" */
#define WSC_FORMAT 1u
#define WSC_MAXP 4
typedef struct {
    unsigned magic, format, version, area, sec, pad, sum, flags;
    char tag[16];
    unsigned np, n[WSC_MAXP];
} Head;
/* the stored head of a key and its parts' sizes; 0 when it cannot be stored (too many parts, a long tag) */
static int head_of(const WsCKey *k, const WsCPart *parts, int np, Head *h)
{
    if (np < 1 || np > WSC_MAXP || strlen(k->tag) >= sizeof h->tag) return 0;
    memset(h, 0, sizeof *h);
    h->magic = WSC_MAGIC;
    h->format = WSC_FORMAT;
    h->version = k->version;
    h->area = (unsigned)k->area;
    h->sec = (unsigned)k->sec;
    h->pad = (unsigned)k->pad;
    h->sum = k->sum;
    h->flags = k->flags;
    strcpy(h->tag, k->tag);
    h->np = (unsigned)np;
    for (int i = 0; i < np; i++) {
        if (parts[i].n > 0x7FFFFFFFu) return 0;
        h->n[i] = (unsigned)parts[i].n;
    }
    return 1;
}
static size_t total(const WsCPart *parts, int np)
{
    size_t bytes = 0;
    for (int i = 0; i < np; i++) bytes += parts[i].n;
    return bytes;
}

/* --- memory: the last results, least recently used out first */
#define WSC_NMEM 8
#ifdef __3DS__
#define WSC_MEMMAX ((size_t)2 << 20)
#else
#define WSC_MEMMAX ((size_t)8 << 20)
#endif
static struct { Head h; unsigned char *d; size_t n; unsigned long long used; } mem[WSC_NMEM];
static size_t mem_bytes;
static unsigned long long mem_tick;
static void mem_drop(int i)
{
    free(mem[i].d);
    mem_bytes -= mem[i].n;
    mem[i].d = 0;
    mem[i].n = 0;
}
static void mem_put(const Head *h, const WsCPart *parts, int np)
{
    size_t n = total(parts, np);
    int slot = -1;
    if (n > WSC_MEMMAX / 2) return;
    for (int i = 0; i < WSC_NMEM; i++)
        if (mem[i].d && !memcmp(&mem[i].h, h, sizeof *h)) mem_drop(i);
    for (;;) {                                            /* room for it: a free slot and the byte budget */
        int lru = -1, free_slot = -1;
        for (int i = 0; i < WSC_NMEM; i++) {
            if (!mem[i].d) {
                if (free_slot < 0) free_slot = i;
                continue;
            }
            if (lru < 0 || mem[i].used < mem[lru].used) lru = i;
        }
        if (free_slot >= 0 && mem_bytes + n <= WSC_MEMMAX) {
            slot = free_slot;
            break;
        }
        if (lru < 0) return;
        mem_drop(lru);
    }
    if (!(mem[slot].d = (unsigned char *)malloc(n ? n : 1))) return;
    {
        size_t offset = 0;
        for (int i = 0; i < np; i++) {
            memcpy(mem[slot].d + offset, parts[i].p, parts[i].n);
            offset += parts[i].n;
        }
    }
    mem[slot].h = *h;
    mem[slot].n = n;
    mem[slot].used = ++mem_tick;
    mem_bytes += n;
}
static int mem_get(const Head *h, const WsCPart *parts, int np)
{
    for (int i = 0; i < WSC_NMEM; i++)
        if (mem[i].d && !memcmp(&mem[i].h, h, sizeof *h)) {
            size_t offset = 0;
            for (int j = 0; j < np; j++) {
                memcpy(parts[j].p, mem[i].d + offset, parts[j].n);
                offset += parts[j].n;
            }
            mem[i].used = ++mem_tick;
            return 1;
        }
    return 0;
}

/* --- disk: ws_cache/TAG_AREA_SEC_PAD_FLAGS.bin = the head, the parts, FNV-1a of both */
extern const char *port_save_file(const char *base, char *out, int n);   /* shims/card.c */
extern void port_mkdir(const char *dir);                                 /* shims/gpu.c */
static void path_of(const Head *h, char *out, int n, int mk)
{
    char dir[1024];
    port_save_file("ws_cache", dir, sizeof dir);
    if (mk) port_mkdir(dir);
    snprintf(out, (size_t)n, "%s/%s_%u_%u_%u_%u.bin", dir, h->tag, h->area, h->sec, h->pad, h->flags);
}
/* the new file over the old one (not rename(): the game's BIOS rename stub takes that name in the port's link); the
   3DS has no other name for it here, so there the file is written in place (a torn one fails its checksum) */
static int replace(const char *tmp, const char *dst)
{
#ifdef _WIN32
    return MoveFileExA(tmp, dst, MOVEFILE_REPLACE_EXISTING) ? 0 : -1;
#elif defined(__3DS__)
    (void)tmp;
    (void)dst;
    return -1;
#else
    return renameat(AT_FDCWD, tmp, AT_FDCWD, dst);
#endif
}
static int disk_get(const Head *h, const WsCPart *parts, int np)
{
    char path[1200];
    Head file_head;
    FILE *f;
    size_t n = total(parts, np);
    unsigned char *buf;
    unsigned sum, file_sum;
    int ok = 0;
    path_of(h, path, sizeof path, 0);
    if (!(f = fopen(path, "rb"))) return 0;
    if (fread(&file_head, sizeof file_head, 1, f) == 1 && !memcmp(&file_head, h, sizeof file_head)
        && (buf = (unsigned char *)malloc(n ? n : 1)) != 0) {
        if (fread(buf, 1, n, f) == n && fread(&file_sum, sizeof file_sum, 1, f) == 1 && fgetc(f) == EOF) {
            sum = ws_cache_hash(ws_cache_hash(2166136261u, &file_head, sizeof file_head), buf, n);
            if (sum == file_sum) {
                size_t offset = 0;
                for (int j = 0; j < np; j++) {
                    memcpy(parts[j].p, buf + offset, parts[j].n);
                    offset += parts[j].n;
                }
                ok = 1;
            }
        }
        free(buf);
    }
    fclose(f);
    if (!ok && port_verbose)
        port_printf("[ws] cache: %s does not match (another key, short or corrupt): made again\n", path);
    return ok;
}
static void disk_put(const Head *h, const WsCPart *parts, int np)
{
    static int warned;
    char path[1200], tmp[1300];
    FILE *f;
    unsigned sum = ws_cache_hash(2166136261u, h, sizeof *h);
    int ok;
    path_of(h, path, sizeof path, 1);
#ifdef __3DS__
    snprintf(tmp, sizeof tmp, "%s", path);
#else
    snprintf(tmp, sizeof tmp, "%s.tmp", path);
#endif
    if (!(f = fopen(tmp, "wb"))) {
        if (!warned++) port_printf("[ws] cache: cannot write %s (strips made again next time)\n", tmp);
        return;
    }
    ok = fwrite(h, sizeof *h, 1, f) == 1;
    for (int i = 0; i < np && ok; i++) {
        ok = fwrite(parts[i].p, 1, parts[i].n, f) == parts[i].n;
        sum = ws_cache_hash(sum, parts[i].p, parts[i].n);
    }
    if (ok) ok = fwrite(&sum, sizeof sum, 1, f) == 1;
    if (fclose(f)) ok = 0;
#ifndef __3DS__
    if (ok && replace(tmp, path)) {                       /* (a filesystem without a replacing rename: in place) */
        remove(path);
        ok = !replace(tmp, path);
    }
    if (!ok) remove(tmp);
#else
    if (!ok) remove(path);
#endif
    if (!ok && !warned++) port_printf("[ws] cache: cannot write %s (strips made again next time)\n", path);
}

int ws_cache_get(const WsCKey *k, const WsCPart *parts, int np)
{
    Head h;
    ws_cache_last = 0;
    if (!mode() || !head_of(k, parts, np, &h)) return 0;
    if (mem_get(&h, parts, np)) {
        ws_cache_last = 1;
        return 1;
    }
    if (mode() >= 2 && disk_get(&h, parts, np)) {
        mem_put(&h, parts, np);
        ws_cache_last = 2;
        return 1;
    }
    return 0;
}
void ws_cache_put(const WsCKey *k, const WsCPart *parts, int np)
{
    Head h;
    if (!mode() || !head_of(k, parts, np, &h)) return;
    mem_put(&h, parts, np);
    if (mode() >= 2) disk_put(&h, parts, np);
}
