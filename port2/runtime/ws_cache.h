/* ws_cache.c: the widescreen picture strips made once per room (docs/PORT.md "Widescreen strips cache").
 *
 * A generator's finished output (the strip buffers and what later frames read next to them) is kept under a key of
 * everything it depends on: the generator and its room, the strip width (pad), a hash of the picture it was made
 * from, flags (the renderer, the mirror preview, the side), the generators' version (WS_GEN_VERSION in
 * ws_backdrop.c, plus the build stamp of ws_backdrop.c, so any change to the generators' code makes new keys).
 * Two levels: in memory (the last few results, LRU, a few MB) and on disk, ws_cache/ in the folder of the memory
 * cards (--save-dir: the Android files folder, the 3DS SD folder; else next to the disc image or data/), one file per
 * generator, room, width and flags, with a header (format, key, sizes) and a checksum: anything that does not match
 * (another key, a short or corrupt file, a read error) is a miss and the strips are made again (and the file
 * replaced).  Files are written to a temporary name and moved over the old one.
 *
 * --ws-cache off|mem|on (default on; --no-ws-cache = off; PORT_WS_CACHE=0/1/2 outside the wasm2c build). */
#ifndef WS_CACHE_H
#define WS_CACHE_H
#include <stddef.h>

typedef struct {
    const char *tag;          /* the generator / take path ("strip", "tw", "mm", ...; at most 15 characters) */
    int area, sec, pad;       /* its room and the strip width */
    unsigned sum, flags;      /* the hash of its inputs; renderer / mode / side bits */
    unsigned version;         /* the generators' version and build stamp */
} WsCKey;
typedef struct { void *p; size_t n; } WsCPart;          /* one buffer of the result, filled on a hit */

int ws_cache_get(const WsCKey *k, const WsCPart *parts, int np);         /* 1: every part filled (memory or disk) */
void ws_cache_put(const WsCKey *k, const WsCPart *parts, int np);
extern int port_ws_cache;     /* 2 memory + disk (default), 1 memory only, 0 off */
extern int ws_cache_last;     /* the last ws_cache_get: 0 miss, 1 memory, 2 disk */
unsigned ws_cache_hash(unsigned h, const void *p, size_t n);   /* FNV-1a; start with 2166136261u */
#endif
