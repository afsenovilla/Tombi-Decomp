/* --title-bg FILE (port only, widescreen): a user-supplied wide version of the title-screen background (PNG / JPG),
   drawn into the wide plane behind the title screen so the side strips are not black.  The image is not shipped
   (it is game art); with no file nothing here runs and the frames are unchanged.

   Gating: titleLogoDraw (O0PTSUB0, called only while loopTitleScreen shows the title: its state 3) marks the
   ordering-table node it links the background / logo sprites into (port/fixes.txt); DrawOTag (shims/gpu.c) fills the
   wide copy from this image when the list it is about to draw passes through that node.

   Alignment (measured against a frame of the original background, 320x240 at buffer rows 8..247, the game's own black
   8-line tile above and the cleared rows below): the game's 4:3 background is the part of the wide image whose
   scale and origin are TB_* below, as fractions of the image size, so any resolution of the same picture lines up.
   Found by a brute-force search (scale, aspect, offset; least absolute difference over the parts the logo and text
   leave visible) on a 2000x848 picture: its full height is the game's 240 rows, its pixels have the PS1 pixel's
   aspect, and the game's 320 columns are its centred 56.24 %. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#define STBI_ONLY_JPEG
#define STBI_ASSERT(x) ((void)0)
#include "stb_image.h"

extern int port_frame;
extern int port_printf(const char *, ...);
const char *port_title_bg_path;

/* the game's background rectangle (rows 8..247 of the 320-wide buffer) in image units: its left / top edge and
   its width / height as fractions of the image width / height */
#define TB_Y0 8
#define TB_H 240
static const double TB_FW = 0.5624, TB_FX = (1 - 0.5624) / 2, TB_FY = 0.0, TB_FH = 1.0;

static unsigned char *img; static int iw, ih, tried;
static unsigned short *cache; static int cw, ch, cpad;
static void *mark; static int mark_frame;

void port_title_bg_mark(void *node) { if (port_title_bg_path) { mark = node; mark_frame = port_frame; } }
/* the node marked by the title screen within the last couple of frames (0 = none); consumed by the caller */
void *port_title_bg_take(void) {
    void *m = mark;
    if (m && port_frame - mark_frame > 2) m = mark = 0;
    return m;
}
void port_title_bg_done(void) { mark = 0; }

static int load(void) {
    if (!tried) {
        int n;
        tried = 1;
        img = stbi_load(port_title_bg_path, &iw, &ih, &n, 3);
        if (!img) port_printf("[port2] --title-bg %s: cannot read the image (%s)\n", port_title_bg_path, stbi_failure_reason());
    }
    return img != 0;
}
/* the wide buffer's picture (w = 320 + 2 pad columns, h rows, 15-bit PS1 colour), built once per size */
const unsigned short *port_title_bg_pixels(int w, int h, int pad) {
    if (!port_title_bg_path || !load()) return 0;
    if (cache && cw == w && ch == h && cpad == pad) return cache;
    free(cache); cache = calloc((size_t)w * h, 2); cw = w; ch = h; cpad = pad;
    if (!cache) return 0;
    /* buffer pixel (x, y) -> image: the game rectangle x pad..pad+319, rows TB_Y0..TB_Y0+TB_H-1 maps onto
       (TB_FX..TB_FX+TB_FW) x (TB_FY..TB_FY+TB_FH) of the image; box-filtered (the image is several times larger) */
    double sx = TB_FW * iw / 320.0, sy = TB_FH * ih / TB_H;
    double ox = TB_FX * iw - pad * sx, oy = TB_FY * ih - TB_Y0 * sy;
    for (int y = 0; y < h; y++) {
        int y0 = (int)(oy + y * sy), y1 = (int)(oy + (y + 1) * sy);
        if (y1 <= y0) y1 = y0 + 1;
        if (y0 < 0 || y1 > ih) continue;                        /* above / below the picture: black */
        for (int x = 0; x < w; x++) {
            int x0 = (int)(ox + x * sx), x1 = (int)(ox + (x + 1) * sx);
            if (x1 <= x0) x1 = x0 + 1;
            if (x0 < 0 || x1 > iw) continue;                   /* wider than the picture (32:9): black, as the 4:3 bars */
            unsigned r = 0, g = 0, b = 0, n = 0;
            for (int j = y0; j < y1; j++) for (int i = x0; i < x1; i++) {
                const unsigned char *p = img + ((size_t)j * iw + i) * 3;
                r += p[0]; g += p[1]; b += p[2]; n++;
            }
            r /= n; g /= n; b /= n;
            cache[(size_t)y * w + x] = (unsigned short)((r >> 3) | (g >> 3) << 5 | (b >> 3) << 10);
        }
    }
    return cache;
}
