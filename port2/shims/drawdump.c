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

typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32; typedef unsigned long long u64;

const char *port_drawdump_path;
static FILE *dd_f;
static u64 *dd_set; static u32 dd_cap, dd_n;

static int dd_add(u64 h) {               /* 1 when h was new */
    if (!h) h = 1;
    if (dd_n * 2 >= dd_cap) {
        u32 oc = dd_cap; u64 *os = dd_set;
        dd_cap = oc ? oc * 2 : 1 << 16; dd_set = calloc(dd_cap, 8);
        dd_n = 0;
        for (u32 i = 0; i < oc; i++) if (os[i]) dd_add(os[i]);
        free(os);
    }
    u32 m = dd_cap - 1, i = (u32)(h ^ (h >> 29)) & m;
    while (dd_set[i]) { if (dd_set[i] == h) return 0; i = (i + 1) & m; }
    dd_set[i] = h; dd_n++;
    return 1;
}
static u64 fnv(u64 h, const void *p, int n) { const u8 *b = p; while (n--) { h ^= *b++; h *= 0x100000001B3ull; } return h; }

void port_drawdump(const u16 *vram, int tx, int ty, int depth, int cx, int cy, int tpage, int clut,
                   int u0, int v0, int w, int h, int sx, int sy, int sw, int sh, int semi, int kind) {
    static u16 tex[256 * 256]; static u16 pal[256];
    if (!port_drawdump_path || w <= 0 || h <= 0 || w > 256 || h > 256) return;
    if (!dd_f) { dd_f = fopen(port_drawdump_path, "wb"); if (!dd_f) { port_drawdump_path = 0; return; } }
    int np = depth == 0 ? 16 : depth == 1 ? 256 : 0;
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++) {
            int u = (u0 + x) & 255, v = (v0 + y) & 255; u16 t;
            if (depth == 0) t = (vram[((ty + v) & 511) * 1024 + ((tx + (u >> 2)) & 1023)] >> ((u & 3) * 4)) & 15;
            else if (depth == 1) t = (vram[((ty + v) & 511) * 1024 + ((tx + (u >> 1)) & 1023)] >> ((u & 1) * 8)) & 255;
            else t = vram[((ty + v) & 511) * 1024 + ((tx + u) & 1023)];
            tex[y * w + x] = t;
        }
    for (int i = 0; i < np; i++) pal[i] = vram[(cy & 511) * 1024 + ((cx + i) & 1023)];
    u32 hdr[6] = { (u32)tx | (u32)ty << 16, (u32)depth | (u32)kind << 8 | (u32)semi << 16, (u32)cx | (u32)cy << 16,
                   (u32)u0 | (u32)v0 << 16, (u32)w | (u32)h << 16, (u32)tpage | (u32)clut << 16 };
    u64 k = fnv(0xCBF29CE484222325ull, hdr, sizeof hdr);
    k = fnv(k, pal, np * 2); k = fnv(k, tex, w * h * 2);
    if (!dd_add(k)) return;
    const u8 *game = port_ram_ptr(0x8009C960u);
    u16 area = *(const u16 *)(game + 0x948), sect = *(const u16 *)(game + 0x94A);
    u32 rec[12] = { 0x31575244u /* "DRW1" */, (u32)area | (u32)sect << 16, (u32)port_frame,
                    hdr[0], hdr[1], hdr[2], hdr[3], hdr[4], hdr[5],
                    (u32)(sx & 0xFFFF) | (u32)sy << 16, (u32)(sw & 0xFFFF) | (u32)sh << 16, (u32)np };
    fwrite(rec, 4, 12, dd_f);
    if (np) fwrite(pal, 2, np, dd_f);
    if (np) { static u8 b[256 * 256]; for (int i = 0; i < w * h; i++) b[i] = (u8)tex[i]; fwrite(b, 1, w * h, dd_f); }
    else fwrite(tex, 2, w * h, dd_f);
    fflush(dd_f);
}
