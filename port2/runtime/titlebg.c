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
extern int port_ntsc;
#if defined(__has_include)
#if __has_include("port_version.h")
#include "port_version.h"         /* the game version (gen/version, port/tools/portver.py) */
#endif
#endif
#ifndef PORT_VERSION_PAL
#define PORT_VERSION_PAL 1
#endif
/* the NTSC picture draws the landscape 8 rows up (port/fixes.txt titleLogoDraw), the USA title has no band either */
#define TB_Y0 (port_ntsc || !PORT_VERSION_PAL ? 0 : 8)
#define TB_H 240
static const double TB_FW = 0.5624, TB_FX = (1 - 0.5624) / 2, TB_FY = 0.0, TB_FH = 1.0;

static unsigned char *img;
static int img_w, img_h, tried;
static unsigned short *cache;
static int cache_w, cache_h, cache_pad;
static void *mark;
static int mark_frame;

/* --early-title DIR (port only, with --cut-content): the title-screen landscape of the PrePre Vol. 8 build (PCPX-96084,
   docs/PREPRE8.md, docs/CUT_CONTENT.md) instead of the retail one.  DIR is the user's own copy of that disc's TOMBA
   files (`tools/prepre8.py extract DISC OUT`: OUT, OUT/TOMBA or OUT/TOMBA/SYSTEM); nothing of it is shipped.
   SYS005.GAM is the GAM (LZ) packed 8 bpp page (768 x 256 px) whose x 0..319, y 0..239 is the landscape,
   SYS006.000 its 256-colour palette.  The retail landscape (SYSTEM A00200) is the same size and depth at VRAM (0,0)
   with its palette at (128,511) (measured in VRAM), so the early picture is written over it (and its palette over
   that CLUT row) while the title is up; the retail logo, menu and text are drawn over it unchanged.  Called every
   title frame from titleLogoDraw (via port_title_bg_mark); re-applied when the title comes back (the game reloads
   A00200). */
const char *port_early_title_dir;
static unsigned char *et_page;
static unsigned short et_pal[256];
static int et_tried, et_frame = -100;
static unsigned char *et_read(const char *dir, const char *sub, const char *name, long *len) {
    char path[1024];
    FILE *f;
    unsigned char *file_buf;
    snprintf(path, sizeof path, "%s%s%s", dir, sub, name);
    if (!(f = fopen(path, "rb"))) return 0;
    fseek(f, 0, SEEK_END);
    *len = ftell(f);
    fseek(f, 0, SEEK_SET);
    file_buf = malloc(*len > 0 ? *len : 1);
    if (file_buf && fread(file_buf, 1, *len, f) != (size_t)*len) {
        free(file_buf);
        file_buf = 0;
    }
    fclose(f);
    return file_buf;
}
static int et_load(void) {
    static const char *subs[] = {"/", "/SYSTEM/", "/TOMBA/SYSTEM/", "/fs/TOMBA/SYSTEM/"};
    unsigned char *gam = 0, *pal = 0;
    long gam_len = 0, pal_len = 0;
    unsigned i;
    if (et_tried) return et_page != 0;
    et_tried = 1;
    for (i = 0; i < 4 && !gam; i++)
        if ((gam = et_read(port_early_title_dir, subs[i], "SYS005.GAM", &gam_len)))
            pal = et_read(port_early_title_dir, subs[i], "SYS006.000", &pal_len);
    if (gam && pal && gam_len > 8 && pal_len >= 512 && !memcmp(gam, "GAM", 4)) {
        unsigned size = gam[4] | gam[5] << 8 | gam[6] << 16 | (unsigned)gam[7] << 24,
                 out_pos = 0,
                 flags = 0,
                 left = 0;
        long in_pos = 8;
        if (size == 768 * 256 && (et_page = malloc(size))) {
            /* GAM: 16-bit flag words, 1 = (offset, length) copy, 0 = literal */
            while (out_pos < size && in_pos < gam_len) {
                if (!left) {
                    if (in_pos + 2 > gam_len) break;
                    flags = gam[in_pos] | gam[in_pos + 1] << 8;
                    in_pos += 2;
                    left = 16;
                }
                left--;
                if (flags & 1) {
                    unsigned off, copy_len;
                    if (in_pos + 2 > gam_len) break;
                    off = gam[in_pos];
                    copy_len = gam[in_pos + 1];
                    in_pos += 2;
                    if (!off || off > out_pos) break;
                    while (copy_len-- && out_pos < size) {
                        et_page[out_pos] = et_page[out_pos - off];
                        out_pos++;
                    }
                } else et_page[out_pos++] = gam[in_pos++];
                flags >>= 1;
            }
            if (out_pos != size) {
                free(et_page);
                et_page = 0;
            }
        }
        for (i = 0; i < 256; i++) et_pal[i] = pal[2 * i] | pal[2 * i + 1] << 8;
    }
    free(gam);
    free(pal);
    port_printf(et_page ? "[port] --early-title: PrePre Vol. 8 title landscape loaded from %s\n"
                        : "[port] --early-title %s: SYS005.GAM / SYS006.000 of the PrePre Vol. 8 disc "
                          "not found or unreadable\n",
                port_early_title_dir);
    return et_page != 0;
}
static void et_apply(void) {
    typedef struct {
        short x, y, w, h;
    } R;
    extern int LoadImage(void *, unsigned *);
    static unsigned short px[160 * 2 * 240 / 2];
    R r = {0, 0, 160, 240}, c = {128, 511, 256, 1};
    int y;
    for (y = 0; y < 240; y++) memcpy((unsigned char *)px + y * 320, et_page + y * 768, 320);
    LoadImage(&r, (unsigned *)px);
    LoadImage(&c, (unsigned *)et_pal);
}
static void port_early_title_frame(void) {
    if (!port_early_title_dir || !et_load()) return;      /* its own option, not tied to --cut-content */
    if (port_frame - et_frame > 2) et_apply();       /* first title frame (again after the game reloaded A00200) */
    et_frame = port_frame;
}
void port_title_bg_mark(void *node) {
    port_early_title_frame();
    if (port_title_bg_path) {
        mark = node;
        mark_frame = port_frame;
    }
}
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
        img = stbi_load(port_title_bg_path, &img_w, &img_h, &n, 3);
        if (!img)
            port_printf("[port2] --title-bg %s: cannot read the image (%s)\n", port_title_bg_path,
                        stbi_failure_reason());
    }
    return img != 0;
}
/* the wide buffer's picture (w = 320 + 2 pad columns, h rows, 15-bit PS1 colour), built once per size */
const unsigned short *port_title_bg_pixels(int w, int h, int pad) {
    if (!port_title_bg_path || !load()) return 0;
    if (cache && cache_w == w && cache_h == h && cache_pad == pad) return cache;
    free(cache);
    cache = calloc((size_t)w * h, 2);
    cache_w = w;
    cache_h = h;
    cache_pad = pad;
    if (!cache) return 0;
    /* buffer pixel (x, y) -> image: the game rectangle x pad..pad+319, rows TB_Y0..TB_Y0+TB_H-1 maps onto
       (TB_FX..TB_FX+TB_FW) x (TB_FY..TB_FY+TB_FH) of the image; box-filtered (the image is several times larger) */
    double sx = TB_FW * img_w / 320.0,
           sy = TB_FH * img_h / TB_H;
    double ox = TB_FX * img_w - pad * sx,
           oy = TB_FY * img_h - TB_Y0 * sy;
    for (int y = 0; y < h; y++) {
        int y0 = (int)(oy + y * sy),
            y1 = (int)(oy + (y + 1) * sy);
        if (y1 <= y0) y1 = y0 + 1;
        if (y0 < 0 || y1 > img_h) continue;                        /* above / below the picture: black */
        for (int x = 0; x < w; x++) {
            int x0 = (int)(ox + x * sx),
                x1 = (int)(ox + (x + 1) * sx);
            if (x1 <= x0) x1 = x0 + 1;
            if (x0 < 0 || x1 > img_w) continue;   /* wider than the picture (32:9): black, as the 4:3 bars */
            unsigned r = 0, g = 0, b = 0, n = 0;
            /* box filter: the average of the image pixels under this buffer pixel */
            for (int j = y0; j < y1; j++) for (int i = x0; i < x1; i++) {
                const unsigned char *p = img + ((size_t)j * img_w + i) * 3;
                r += p[0];
                g += p[1];
                b += p[2];
                n++;
            }
            r /= n;
            g /= n;
            b /= n;
            cache[(size_t)y * w + x] = (unsigned short)((r >> 3) | (g >> 3) << 5 | (b >> 3) << 10);
        }
    }
    return cache;
}
