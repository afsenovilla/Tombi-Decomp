/* --widescreen: side strips of the pre-rendered backdrop of AREA02 s5, "Small Strange Room" (docs/PORT.md
 * "Widescreen", "Backdrop strips").
 *
 * That room's background is one 320x224 8 bpp picture (stone wall, brick arch, the carved face door, ivy at both
 * sides, the cobbled floor) drawn as two sprites (256 + 64 columns, pages 128 / 256 x 0, CLUT 128,511) at frame row
 * 16; nothing of it exists past the 4:3 columns, so the wide frame showed the clear colour (grey) beside it.  The
 * strips are made at run time from the picture's own texels in VRAM (no art shipped): the ivy columns at its edges
 * are carried outwards (see build()).  shims/gpu.c calls port_ws_backdrop when it meets the left sprite of the
 * picture while the wide plane is on and fills the strips of the wide copy before the game draws the picture itself,
 * so everything the game draws on top (drums, actors, boxes) stays on top.  Identity without --widescreen and in
 * every other room (the sprite signature and the room are both checked).  Opt-in preview: `PORT_WS_BACKDROP=1` (off by
 * default until the user picks it). */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "port_runtime.h"

typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32;

#define PW 320
#define PH 224

static u16 pic[PH][PW];                 /* the picture as 15-bit colours */
static u16 *strip; static int strip_pad; static u32 strip_sum;
static int enabled = -1;

static int r5(u16 c) { return c & 31; } static int g5(u16 c) { return (c >> 5) & 31; } static int b5(u16 c) { return (c >> 10) & 31; }
static int is_ivy(u16 c) { int r = r5(c), g = g5(c), b = b5(c); return g >= r + 2 && g >= b + 2; }

static int pingpong(int i, int n) { i %= 2 * n; return i < n ? i : 2 * n - 1 - i; }

/* Strip column d (1..pad columns past the edge), row y; left side (the right one is the same, mirrored).
   Base: the picture mirrored at its edge (ping-pong over the BAND columns next to it, short of the arch), so bricks,
   the left roots, the right pillar and the floor run on without a seam.  Ivy: on the wall rows (above FLOOR) the
   ivy of the edge band (IVY columns) is laid over the base again, at another phase and a few rows lower, so the
   strip is covered like the edge and the mirror does not show (only the leaves are copied: green texels). */
#define BAND 56
#define IVY 24
#define FLOOR 176
static u16 at(int side, int y, int m) { if (y < 0) y = 0; if (y >= PH) y = PH - 1; return pic[y][side ? PW - 1 - m : m]; }
static void build(int pad)
{
    free(strip); strip = calloc((size_t)2 * pad * PH, 2); strip_pad = pad;
    if (!strip) return;
    for (int side = 0; side < 2; side++)
        for (int y = 0; y < PH; y++)
            for (int d = 1; d <= pad; d++) {
                u16 c = at(side, y, pingpong(d - 1, BAND));
                if (enabled == 1 && y < FLOOR) {
                    int k = (d - 1) / IVY;                                   /* copy number, for the row shift */
                    int sy = y - 5 * (k % 3) - 4;                           /* (none above the picture's top) */
                    u16 l = sy >= 0 ? at(side, sy, pingpong(d - 1 + IVY / 2, IVY)) : 0;
                    if (sy >= 0 && is_ivy(l)) c = l;
                }
                strip[(size_t)y * 2 * pad + (side ? pad + (d - 1) : pad - d)] = c;
            }
}

/* the picture's texels: 8 bpp page at (tx, ty), CLUT at (cx, cy); vram is 1024x512 words */
static u32 grab(const u16 *vram, int tx, int ty, int cx, int cy)
{
    u32 h = 2166136261u;
    for (int y = 0; y < PH; y++)
        for (int x = 0; x < PW; x++) {
            u16 w = vram[(size_t)((ty + y) & 511) * 1024 + ((tx + x / 2) & 1023)];
            int i = x & 1 ? w >> 8 : w & 255;
            u16 c = vram[(size_t)(cy & 511) * 1024 + ((cx + i) & 1023)];
            pic[y][x] = c; h = (h ^ c) * 16777619u;
        }
    return h;
}

/* sprite (cmd 0x64/0x65) at screen (x, y) of w x h texels from (u, v), page (tx, ty, depth), CLUT (cx, cy):
   1 + the strip buffer when it is the left part of the room's picture (left strip at columns 0..pad-1, right strip
   at pad..2pad-1, PH rows), else 0 */
const u16 *port_ws_backdrop(const u16 *vram, int pad, int x, int y, int w, int h, int u, int v, int tx, int ty,
                            int depth, int cx, int cy)
{
    if (enabled < 0) { const char *e = getenv("PORT_WS_BACKDROP"); enabled = e && *e >= '1' && *e <= '2' ? *e - '0' : 0; }
    if (!enabled || pad <= 0 || x != 0 || y != 16 || w != 256 || h != PH || u || v || tx != 128 || ty || depth != 1
        || cx != 128 || cy != 511) return 0;
    {
        u8 *g = port_ram_ptr(PORT_A_GAME);
        if (*(u16 *)(g + 0) != 2 || *(u16 *)(g + 2) != 5) return 0;
    }
    {
        u32 s = grab(vram, 128, 0, cx, cy);
        if (!strip || strip_pad != pad || s != strip_sum) {
            strip_sum = s; build(pad);
            {   const char *e = getenv("PORT_WS_BACKDROP_DUMP");      /* research: the picture as a PPM */
                FILE *f = e ? fopen(e, "wb") : 0;
                if (f) {
                    fprintf(f, "P6 %d %d 255\n", PW, PH);
                    for (int yy = 0; yy < PH; yy++) for (int xx = 0; xx < PW; xx++) {
                        u16 c = pic[yy][xx]; u8 rgb[3] = { (u8)(r5(c) << 3), (u8)(g5(c) << 3), (u8)(b5(c) << 3) };
                        fwrite(rgb, 1, 3, f);
                    }
                    fclose(f);
                }
            }
        }
    }
    return strip;
}
