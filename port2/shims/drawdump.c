/* --dump-draws FILE (port only, for tools/gfx_capture.py): logs every distinct textured primitive the game draws.
 *
 * gpu.c calls port_drawdump() for each textured polygon / rectangle with its page, CLUT and the bounding box of
 * its texture coordinates.  A primitive is written once per distinct (page, CLUT, UV box, texel content, palette
 * content): the record holds the room (GAME.currentArea/Section), the frame, the raw tpage/CLUT words, the screen
 * box and the texels (indices + palette for 4/8-bit pages, the words for 15-bit ones), so the graphics editor can
 * compare its own sprite decoding with what the game really put on screen.  Format: see tools/gfx_capture.py. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "port_runtime.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

const char *port_drawdump_path;
static FILE *dump_file;
/* open-addressing hash set of the record keys already written (0 = empty slot) */
static u64 *seen_set;
static u32 seen_cap, seen_count;

static int seen_add(u64 h) {               /* 1 when h was new */
    if (!h) h = 1;
    if (seen_count * 2 >= seen_cap) {
        /* grow (double) and re-insert the old keys */
        u32 old_cap = seen_cap;
        u64 *old_set = seen_set;
        seen_cap = old_cap ? old_cap * 2 : 1 << 16;
        seen_set = calloc(seen_cap, 8);
        seen_count = 0;
        for (u32 i = 0; i < old_cap; i++)
            if (old_set[i]) seen_add(old_set[i]);
        free(old_set);
    }
    u32 slot_mask = seen_cap - 1,
        i = (u32)(h ^ (h >> 29)) & slot_mask;
    while (seen_set[i]) {
        if (seen_set[i] == h) return 0;
        i = (i + 1) & slot_mask;
    }
    seen_set[i] = h;
    seen_count++;
    return 1;
}

/* FNV-1a over n bytes, continuing from h */
static u64 fnv(u64 h, const void *p, int n) {
    const u8 *b = p;
    while (n--) {
        h ^= *b++;
        h *= 0x100000001B3ull;
    }
    return h;
}

void port_drawdump(const u16 *vram, int tx, int ty, int depth, int cx, int cy, int tpage, int clut,
                   int u0, int v0, int w, int h, int sx, int sy, int sw, int sh, int semi, int kind) {
    static u16 tex[256 * 256];
    static u16 pal[256];
    if (!port_drawdump_path || w <= 0 || h <= 0 || w > 256 || h > 256) return;
    if (!dump_file) {
        dump_file = fopen(port_drawdump_path, "wb");
        if (!dump_file) {
            port_drawdump_path = 0;
            return;
        }
    }
    int pal_count = depth == 0 ? 16 : depth == 1 ? 256 : 0;
    /* the texels of the UV box (indices for 4/8-bit pages, the words for 15-bit ones) */
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++) {
            int u = (u0 + x) & 255,
                v = (v0 + y) & 255;
            u16 texel_val;
            if (depth == 0)
                texel_val = (vram[((ty + v) & 511) * 1024 + ((tx + (u >> 2)) & 1023)] >> ((u & 3) * 4)) & 15;
            else if (depth == 1)
                texel_val = (vram[((ty + v) & 511) * 1024 + ((tx + (u >> 1)) & 1023)] >> ((u & 1) * 8)) & 255;
            else
                texel_val = vram[((ty + v) & 511) * 1024 + ((tx + u) & 1023)];
            tex[y * w + x] = texel_val;
        }
    for (int i = 0; i < pal_count; i++) pal[i] = vram[(cy & 511) * 1024 + ((cx + i) & 1023)];
    u32 hdr[6] = { (u32)tx | (u32)ty << 16, (u32)depth | (u32)kind << 8 | (u32)semi << 16, (u32)cx | (u32)cy << 16,
                   (u32)u0 | (u32)v0 << 16, (u32)w | (u32)h << 16, (u32)tpage | (u32)clut << 16 };
    u64 rec_key = fnv(0xCBF29CE484222325ull, hdr, sizeof hdr);
    rec_key = fnv(rec_key, pal, pal_count * 2);
    rec_key = fnv(rec_key, tex, w * h * 2);
    if (!seen_add(rec_key)) return;
    const u8 *game = port_ram_ptr(0x8009C960u);
    u16 area = *(const u16 *)(game + 0x948),
        sect = *(const u16 *)(game + 0x94A);
    u32 rec[12] = { 0x31575244u /* "DRW1" */, (u32)area | (u32)sect << 16, (u32)port_frame,
                    hdr[0], hdr[1], hdr[2], hdr[3], hdr[4], hdr[5],
                    (u32)(sx & 0xFFFF) | (u32)sy << 16, (u32)(sw & 0xFFFF) | (u32)sh << 16, (u32)pal_count };
    fwrite(rec, 4, 12, dump_file);
    if (pal_count) fwrite(pal, 2, pal_count, dump_file);
    if (pal_count) {
        static u8 b[256 * 256];
        for (int i = 0; i < w * h; i++) b[i] = (u8)tex[i];
        fwrite(b, 1, w * h, dump_file);
    } else
        fwrite(tex, 2, w * h, dump_file);
    fflush(dump_file);
}
