/* The 3DS start screen (n3ds/main_3ds.c choose_lang) in the look of the game's own OPTIONS screen (the title screen's
   "Opciones", overlay O0PTSUB0): the checkerboard of green and yellow squares with Tomba and the pig on them, black
   letters of its 8 x 16 font, the chosen line on a green bar.  Everything is read from the user's disc when the
   screen opens (nothing of it is in the repository):
     SYSTEM/A00801.GAM  8 bpp, 384 x 256: the eight squares (80 x 84, slots 80 x 86 apart; the font pages beside them)
     SYSTEM/A00802.000  their 256-colour CLUT
     SYSTEM/A00804.000  4 bpp, 256 x 256: the 8 x 16 font (rows of 32 characters from ' ', then A-grave.., a-grave..)
     SYSTEM/A00803.000  the font's CLUT (row 0)
     SYSTEM/A00202.000  8 bpp, 256 x 256: the title screen's TOMBI! logo in two pieces (the main one at the top, the
                        right end of the "!" and the TM at x 0-63, y 112-223, 240 to the right and 112 up), CLUT A00203
   The top screen has the green squares, the bottom one the yellow ones; the texts that stay put sit in the cream boxes
   of the game's messages (the LOAD screen's "No saved games": a blue rim, a dark line inside it, rounded corners).
   With no disc readable the plain screen (n3ds/launcher_ui.c) is drawn instead.  Pictures: RGB, rows from the top. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "port_runtime.h"
#include "panel_assets.h"
#include "../n3ds/launcher_ui.h"

typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32;

static u8 *sq, *font, *logo;          /* the squares' page (8 bpp), the font page (4 bpp), the logo's (8 bpp) */
static u32 sq_pal[256], font_pal[16], logo_pal[256]; /* 0xRRGGBB */
static unsigned sq_n, font_n, logo_n;

static u32 c15(unsigned v) { return (v & 31) << 19 | (v >> 5 & 31) << 11 | (v >> 10 & 31) << 3; }
static int read_pal(const char *name, u32 *pal, int n) {
    unsigned len; u8 *d = pa_read(name, &len);
    if (!d || len < (unsigned)n * 2) { free(d); return 0; }
    for (int i = 0; i < n; i++) pal[i] = c15(d[2 * i] | d[2 * i + 1] << 8);
    free(d); return 1;
}
static u8 *bg_top, *bg_bot; static int bg_ok;   /* the screens' parts that never change (port_launcher_game_draw) */
void port_launcher_game_end(void) { free(sq); free(font); free(logo); free(bg_top); free(bg_bot); sq = font = logo = bg_top = bg_bot = NULL; bg_ok = 0; }
/* 1: the pieces were read */
int port_launcher_game_begin(void) {
    port_launcher_game_end();
    sq = pa_read("SYSTEM/A00801.GAM", &sq_n);
    font = pa_read("SYSTEM/A00804.000", &font_n);
    logo = pa_read("SYSTEM/A00202.000", &logo_n);
    if (!logo || logo_n < 256 * 256 || !read_pal("SYSTEM/A00203.000", logo_pal, 256)) { free(logo); logo = NULL; }   /* (then the title in letters) */
    if (!sq || sq_n < 384 * 256 || !font || font_n < 128 * 256 ||
        !read_pal("SYSTEM/A00802.000", sq_pal, 256) || !read_pal("SYSTEM/A00803.000", font_pal, 16)) {
        port_launcher_game_end(); return 0;
    }
    return 1;
}

typedef struct { u8 *p; int w, h; } Cv;
static void put(Cv *c, int x, int y, u32 col) {
    if (x < 0 || y < 0 || x >= c->w || y >= c->h) return;
    u8 *d = c->p + (y * c->w + x) * 3; d[0] = (u8)(col >> 16); d[1] = (u8)(col >> 8); d[2] = (u8)col;
}
static void mix(Cv *c, int x, int y, u32 col) {   /* half the colour, half what is there */
    if (x < 0 || y < 0 || x >= c->w || y >= c->h) return;
    u8 *d = c->p + (y * c->w + x) * 3;
    d[0] = (u8)((d[0] + (col >> 16 & 255)) / 2); d[1] = (u8)((d[1] + (col >> 8 & 255)) / 2); d[2] = (u8)((d[2] + (col & 255)) / 2);
}
/* the squares of one colour (0 green, 1 yellow), the four of it in turn so that no two alike touch */
static void squares(Cv *c, int y0, int yellow) {
    static const u8 tile[2][4][2] = { { {0, 0}, {2, 0}, {0, 1}, {1, 2} }, { {1, 0}, {1, 1}, {2, 1}, {0, 2} } };
    for (int j = 0; y0 + j * 84 < c->h; j++) for (int i = 0; i * 80 < c->w; i++) {
        const u8 *t = tile[yellow][(i + 2 * j) & 3];
        for (int y = 0; y < 84; y++) for (int x = 0; x < 80; x++)
            put(c, i * 80 + x, y0 + j * 84 + y, sq_pal[sq[(t[1] * 86 + y) * 384 + t[0] * 80 + x]]);
    }
}
/* a message box (inclusive bounds): cream, a dark line inside a blue rim, the corners rounded (radius 5, their edge
   pixels blended by coverage) */
static void blend(Cv *c, int x, int y, u32 col, int a) {
    if (x < 0 || y < 0 || x >= c->w || y >= c->h || a <= 0) return;
    u8 *d = c->p + (y * c->w + x) * 3;
    d[0] = (u8)(((col >> 16 & 255) * a + d[0] * (255 - a)) / 255); d[1] = (u8)(((col >> 8 & 255) * a + d[1] * (255 - a)) / 255);
    d[2] = (u8)(((col & 255) * a + d[2] * (255 - a)) / 255);
}
static void box(Cv *c, int x0, int y0, int x1, int y1) {
    static const u32 ring[3] = { 0x0000FF, 0x393939, 0xFFEFDE };   /* rim, line, inside */
    const float r = 5.5f;
    for (int y = y0; y <= y1; y++) for (int x = x0; x <= x1; x++) {
        const int dx = x - x0 < x1 - x ? x - x0 : x1 - x, dy = y - y0 < y1 - y ? y - y0 : y1 - y;
        if (dx >= 7 || dy >= 7) {   /* (away from the corners: the rings by the distance to the nearest side) */
            const int d = dx < dy ? dx : dy;
            put(c, x, y, ring[d > 2 ? 2 : d]);
            continue;
        }
        int a[3] = { 0, 0, 0 };   /* coverage (of 16) of the rounded rect shrunk by 0, 1, 2 px */
        for (int k = 0; k < 3; k++) for (int sy = 0; sy < 4; sy++) for (int sx = 0; sx < 4; sx++) {
            float px = x + (sx + 0.5f) / 4, py = y + (sy + 0.5f) / 4, l = x0 + k, t = y0 + k, rr = x1 + 1 - k, bb = y1 + 1 - k, q = r - k;
            float cx = px < l + q ? l + q : px > rr - q ? rr - q : px, cy = py < t + q ? t + q : py > bb - q ? bb - q : py;
            if (px >= l && px <= rr && py >= t && py <= bb && (px - cx) * (px - cx) + (py - cy) * (py - cy) <= q * q) a[k]++;
        }
        blend(c, x, y, ring[0], a[0] * 255 / 16);
        blend(c, x, y, ring[1], a[1] * 255 / 16);
        blend(c, x, y, ring[2], a[2] * 255 / 16);
    }
}
static int glyph_cell(unsigned ch, int *u, int *v) {
    if (ch >= 0x20 && ch < 0x80) { *u = (ch & 31) * 8; *v = (ch - 0x20) / 32 * 16; return 1; }
    if (ch >= 0xC0) { *u = (ch & 31) * 8; *v = ch < 0xE0 ? 48 : 64; return 1; }
    return 0;
}
/* the logo's texel at (x, y) of the whole logo (295 x 111): its index, 0 transparent */
static int logo_at(int x, int y) {
    if (x < 0 || y < 0 || y > 110) return 0;
    int k = x < 256 ? logo[y * 256 + x] : 0;
    if (!k && x >= 240 && x < 304) k = logo[(y + 112) * 256 + x - 240];
    return k;
}
/* the logo at half its size (each pixel the mean of four, its edge blended by how many of them are opaque), its top
   left at (x0, y0); 148 x 56 */
static void blend(Cv *c, int x, int y, u32 col, int a);
static void logo_half(Cv *c, int x0, int y0) {
    for (int y = 0; y < 56; y++) for (int x = 0; x < 148; x++) {
        int r = 0, g = 0, b = 0, n = 0;
        for (int j = 0; j < 2; j++) for (int i = 0; i < 2; i++) {
            const int k = logo_at(2 * x + i, 2 * y + j);
            if (!k) continue;
            const u32 v = logo_pal[k]; r += v >> 16 & 255; g += v >> 8 & 255; b += v & 255; n++;
        }
        if (n) blend(c, x0 + x, y0 + y, (u32)(r / n) << 16 | (u32)(g / n) << 8 | (u32)(b / n), n * 255 / 4);
    }
}
static void text(Cv *c, int x, int y, const char *s) {
    for (; *s; s++, x += 8) {
        int u, v;
        if (!glyph_cell((u8)*s, &u, &v)) continue;
        for (int j = 0; j < 16; j++) for (int i = 0; i < 8; i++) {
            const int px = u + i, k = (font[(v + j) * 128 + px / 2] >> ((px & 1) * 4)) & 15;
            if (k) put(c, x + i, y + j, font_pal[k]);
        }
    }
}
static int text_w(const char *s) { return 8 * (int)strlen(s); }
static void upper(char *d, const char *s, int cap) {
    int n = 0;
    for (; *s && n < cap - 1; s++) {
        u8 ch = (u8)*s;
        if ((ch >= 'a' && ch <= 'z') || (ch >= 0xE0 && ch != 0xF7 && ch != 0xFF)) ch -= 0x20;
        d[n++] = (char)ch;
    }
    d[n] = 0;
}
/* the game's cursor: a green bar, its 2 px rim solid and its inside half see-through */
static void bar(Cv *c, int x0, int y0, int x1, int y1, int down) {
    const u32 rim = down ? 0x2C9C24 : 0x6BEF39, in = down ? 0x18A034 : 0x31E752;
    for (int y = y0; y <= y1; y++) for (int x = x0; x <= x1; x++) {
        if (y < y0 + 2 || y > y1 - 2 || x < x0 + 2 || x > x1 - 2) put(c, x, y, rim); else mix(c, x, y, in);
    }
}
/* lines of at most w pixels, broken at spaces (a word longer than a line is broken where it has to be) */
static int wrap_dry;
static int wrap(Cv *c, int x, int y, int w, const char *s, int maxl) {
    int n = 0, per = w / 8;
    while (*s && n < maxl) {
        while (*s == ' ') s++;
        int len = (int)strlen(s), take = len;
        if (len > per) { take = per; while (take > 0 && s[take] != ' ') take--; if (!take) take = per; }
        char ln[64]; if (take > 63) take = 63;
        memcpy(ln, s, take); ln[take] = 0;
        if (!wrap_dry) text(c, x, y + 16 * n, ln);
        s += take; n++;
    }
    return n;
}

/* the touch button of the bottom screen: Start (the only one; i > 0: none) */
static const int START_X0 = 40, START_Y0 = 172, START_X1 = 279, START_Y1 = 201;
int port_launcher_game_button(int i, int *x0, int *y0, int *x1, int *y1) {
    if (i) return 0;
    *x0 = START_X0; *y0 = START_Y0; *x1 = START_X1; *y1 = START_Y1;
    return 1;
}
/* a dark arrow (0 up, 1 down, 2 left, 3 right) centred on (cx, cy), its edge pixels blended by coverage */
static void arrow(Cv *c, int cx, int cy, int dir) {
    const float L = 6.0f, W = 4.5f;   /* half its width across, half its length along */
    for (int y = cy - 10; y <= cy + 10; y++) for (int x = cx - 10; x <= cx + 10; x++) {
        int in = 0;
        for (int sy = 0; sy < 4; sy++) for (int sx = 0; sx < 4; sx++) {
            float dx = x + (sx + 0.5f) / 4 - cx, dy = y + (sy + 0.5f) / 4 - cy, a, w;   /* a: along (tip at -W), w: across */
            if (dir == 0) { a = dy; w = dx; } else if (dir == 1) { a = -dy; w = dx; } else if (dir == 2) { a = dx; w = dy; } else { a = -dx; w = dy; }
            if (a >= -W && a <= W && (w < 0 ? -w : w) <= L * (a + W) / (2 * W)) in++;
        }
        blend(c, x, y, 0x080008, in * 255 / 16);
    }
}
/* the console's A button as on the AYN Thor with black buttons: a matt black round key, a touch lighter at its upper
   left and darker at its rim, and a thin light grey A (two legs and a bar); drawn n x n (16: the capitals' height)
   centred on (cx, cy), every pixel from 8 x 8 samples */
static float seg_d2(float px, float py, float ax, float ay, float bx, float by) {
    const float vx = bx - ax, vy = by - ay, wx = px - ax, wy = py - ay, l = vx * vx + vy * vy;
    float t = l > 0 ? (wx * vx + wy * vy) / l : 0; t = t < 0 ? 0 : t > 1 ? 1 : t;
    const float dx = wx - t * vx, dy = wy - t * vy;
    return dx * dx + dy * dy;
}
/* the console's START key: a black key with slightly rounded corners and a thin grey rim, START in thin light grey strokes drawn
   here (not the game font: the A key's look), w x 16 from (x0, y0); each pixel from 8 x 8 samples */
static int exit_style;
static float seg_d2(float px, float py, float ax, float ay, float bx, float by);
static void startkey_draw(Cv *c, int x0, int y0, int w) {
    /* the letters on a 4 x 7 grid (y down), as polylines; NaN-free: a pair (-1, -1) breaks a line */
    static const float S_[] = { 4, 1, 3, 0, 1, 0, 0, 1, 0, 2.4f, 1, 3.4f, 3, 3.6f, 4, 4.6f, 4, 6, 3, 7, 1, 7, 0, 6 };
    static const float T_[] = { 0, 0, 4, 0, -1, -1, 2, 0, 2, 7 };
    static const float A_[] = { 0, 7, 2, 0, 4, 7, -1, -1, 0.8f, 4.6f, 3.2f, 4.6f };
    static const float R_[] = { 0, 7, 0, 0, 3, 0, 4, 1, 4, 2.6f, 3, 3.6f, 0, 3.6f, -1, -1, 2, 3.6f, 4, 7 };
    static const struct { const float *p; int n; } L[5] = { {S_, 12}, {T_, 5}, {A_, 6}, {R_, 10}, {T_, 5} };
    const float sc = 1.15f, adv = 6.6f, hw = 0.62f, tw = 4 * adv + 4 * sc, tx = x0 + (w - tw) / 2, ty = y0 + (16 - 7 * sc) / 2;
    for (int y = 0; y < 16; y++) for (int x = 0; x < w; x++) {
        int ink = 0;
        for (int sy = 0; sy < 8; sy++) for (int sx = 0; sx < 8; sx++) {
            const float px = x0 + x + (sx + 0.5f) / 8, py = y0 + y + (sy + 0.5f) / 8;
            int hit = 0;
            for (int l = 0; l < 5 && !hit; l++) {
                const float ox = tx + l * adv;
                for (int k = 0; k + 1 < L[l].n && !hit; k++) {
                    const float *a = L[l].p + 2 * k, *b = a + 2;
                    if (a[0] < 0 || b[0] < 0) continue;
                    hit = seg_d2(px, py, ox + a[0] * sc, ty + a[1] * sc, ox + b[0] * sc, ty + b[1] * sc) <= hw * hw;
                }
            }
            ink += hit;
        }
        /* the corners rounded (radius 3.5): coverage of the key and of its inside (1 px in) from 4 x 4 samples */
        int out = 0, in = 0;
        for (int sy = 0; sy < 4; sy++) for (int sx = 0; sx < 4; sx++) {
            const float px = x + (sx + 0.5f) / 4, py = y + (sy + 0.5f) / 4;
            for (int k = 0; k < 2; k++) {
                const float r = 3.5f - k, l = (float)k, t = (float)k, rr = (float)w - k, bb = 16.0f - k;
                const float cx = px < l + r ? l + r : px > rr - r ? rr - r : px, cy = py < t + r ? t + r : py > bb - r ? bb - r : py;
                const int hit = px >= l && px <= rr && py >= t && py <= bb && (px - cx) * (px - cx) + (py - cy) * (py - cy) <= r * r;
                if (k) in += hit; else out += hit;
            }
        }
        if (!out) continue;
        const int body = (int)(0x2C - 0x12 * y / 15);   /* (a touch lighter at the top, as the A key) */
        const int face = (body * (64 - ink) + 0xDA * ink) / 64;
        const int v = (0x6E * (16 - in) + face * in) / 16;   /* (the rim where the inside does not reach) */
        blend(c, x0 + x, y0 + y, (u32)v * 0x010101, out * 255 / 16);
    }
}
/* the keys never change: each is drawn once over black and over white, its colour and coverage recovered from the
   two (blend is linear), and later copies blend that (the drawing itself takes 64 samples a pixel: too slow on a 3DS) */
typedef struct { int w, h, ok; u8 rgb[64 * 16 * 3], a[64 * 16]; } Spr;
static void spr_make(Spr *sp, int w, int h, void (*draw)(Cv *, int, int, int), int arg) {
    static u8 k0[64 * 16 * 3], k1[64 * 16 * 3];
    Cv c0 = { k0, w, h }, c1 = { k1, w, h };
    memset(k0, 0, sizeof k0); memset(k1, 255, sizeof k1);
    draw(&c0, 0, 0, arg); draw(&c1, 0, 0, arg);
    for (int i = 0; i < w * h; i++) {
        const int al = 255 - (k1[3 * i] - k0[3 * i]);   /* (black: a * col; white: a * col + (1 - a) * 255) */
        sp->a[i] = (u8)(al < 0 ? 0 : al > 255 ? 255 : al);
        for (int k = 0; k < 3; k++) { const int v = al > 0 ? k0[3 * i + k] * 255 / al : 0; sp->rgb[3 * i + k] = (u8)(v > 255 ? 255 : v); }
    }
    sp->w = w; sp->h = h; sp->ok = 1;
}
static void spr_put(Cv *c, const Spr *sp, int x0, int y0) {
    for (int y = 0; y < sp->h; y++) for (int x = 0; x < sp->w; x++) {
        const int i = y * sp->w + x;
        if (sp->a[i]) blend(c, x0 + x, y0 + y, (u32)sp->rgb[3 * i] << 16 | (u32)sp->rgb[3 * i + 1] << 8 | sp->rgb[3 * i + 2], sp->a[i]);
    }
}
static void abutton_draw(Cv *c, int cx, int cy, int n) {
    const float R = n / 2.0f, s = n / 16.0f, hw = 0.72f * s;   /* radius, scale, half the stroke */
    for (int y = 0; y < n; y++) for (int x = 0; x < n; x++) {
        int disc = 0, rim = 0, ink = 0; float shade = 0;
        for (int sy = 0; sy < 8; sy++) for (int sx = 0; sx < 8; sx++) {
            const float px = x + (sx + 0.5f) / 8 - R, py = y + (sy + 0.5f) / 8 - R, d2 = px * px + py * py;
            if (d2 > R * R) continue;
            disc++; rim += d2 > (R - 1.1f * s) * (R - 1.1f * s);
            shade += (px + py) / (2 * R);   /* -1 upper left .. 1 lower right */
            /* the A: apex (0, -4.3), feet (+-3.5, 4.3), bar at y 1.6 (in 16 px units) */
            const float qx = px / s, qy = py / s, h2 = (hw / s) * (hw / s);
            if (seg_d2(qx, qy, 0, -4.3f, -3.5f, 4.3f) <= h2 || seg_d2(qx, qy, 0, -4.3f, 3.5f, 4.3f) <= h2 ||
                (qy >= 1.6f - hw / s && qy <= 1.6f + hw / s && qx > -2.6f && qx < 2.6f)) ink++;
        }
        if (!disc) continue;
        const float t = shade / disc;   /* the body: 0x3A at the upper left to 0x1A at the lower right */
        int v = (int)(0x2A - 0x10 * t);
        v = (v * (disc - rim) + 0x12 * rim) / disc;   /* the rim darker */
        u32 col = (u32)v * 0x010101;
        if (ink) {   /* the letter over it */
            const int a = ink * 255 / disc;
            const int r = (0xD8 * a + v * (255 - a)) / 255;
            col = (u32)r * 0x010101;
        }
        blend(c, cx - n / 2 + x, cy - n / 2 + y, col, disc * 255 / 64);
    }
}
static void abutton_at(Cv *c, int x0, int y0, int n) { abutton_draw(c, x0 + n / 2, y0 + n / 2, n); }
static void abutton(Cv *c, int cx, int cy, int n) {
    static Spr sp; if (!sp.ok || sp.w != n) spr_make(&sp, n, n, abutton_at, n);
    spr_put(c, &sp, cx - n / 2, cy - n / 2);
}
static void startkey(Cv *c, int x0, int y0, int w) {
    static Spr sp; if (!sp.ok || sp.w != w) spr_make(&sp, w, 16, startkey_draw, w);
    spr_put(c, &sp, x0, y0);
}

void port_launcher_game_draw(unsigned char *top, unsigned char *bot, const LuiState *s) {
    static const char *lab[4] = { "LANGUAGE:", "MOVIES:", "SPEED:", "PICTURE:" }, *name[4] = { "LANGUAGE", "MOVIES", "SPEED", "PICTURE" };
    char lang[64]; upper(lang, s->lang, sizeof lang);
    const char *val[4] = { lang, s->movies ? "PLAY" : "SKIP", s->hz == 50 ? "50 HZ" : "60 HZ",
                           s->picture == 1 ? "WIDE (EXPERIMENTAL)" : s->picture == 2 ? "3D (EXPERIMENTAL)" : "4:3" };
    Cv t = { top, 400, 240 }, b = { bot, 320, 240 };
    /* ---- the top screen: the options, as the game's OPTIONS, on the green squares.  What never changes (the squares,
       the logo, the keys' hint; the bottom screen's squares) is drawn once and copied afterwards: drawing it all again
       took the 3DS a good part of a second */
    if (!bg_top) bg_top = malloc(400 * 240 * 3);
    if (!bg_bot) bg_bot = malloc(320 * 240 * 3);
    if (!bg_top || !bg_bot) return;
    if (!bg_ok) {
        Cv bt = { bg_top, 400, 240 }, bb = { bg_bot, 320, 240 };
        {   Cv t = bt;
            squares(&t, -6, 0);
            if (logo) logo_half(&t, 200 - 74, 4);
            else { box(&t, 200 - text_w("TOMBI!") / 2 - 14, 10, 200 + text_w("TOMBI!") / 2 + 13, 37); text(&t, 200 - text_w("TOMBI!") / 2, 16, "TOMBI!"); }
            {   /* what the keys do: the arrows drawn, the words in the game's letters */
                static const char *w[3] = { "OPTION", "CHANGE", "START" };
                const int gap = 24, wa = 2 * 14 + 4, wt = wa + text_w(w[0]) + gap + wa + text_w(w[1]) + gap + 20 + text_w(w[2]);
                int x = 200 - wt / 2;
                box(&t, x - 14, 204, x + wt + 13, 231);
                arrow(&t, x + 6, 217, 0); arrow(&t, x + 20, 217, 1); x += wa; text(&t, x, 210, w[0]); x += text_w(w[0]) + gap;
                arrow(&t, x + 6, 217, 2); arrow(&t, x + 20, 217, 3); x += wa; text(&t, x, 210, w[1]); x += text_w(w[1]) + gap;
                abutton(&t, x + 8, 217, 16); x += 20; text(&t, x, 210, w[2]);
            }
        }
        squares(&bb, -6, 1);
        bg_ok = 1;
    }
    memcpy(top, bg_top, 400 * 240 * 3);
    for (int i = 0; i < 4; i++) {
        const int y = 72 + i * 34;
        if (i == s->row) bar(&t, 16, y - 4, 383, y + 17, 0);
        text(&t, 28, y, lab[i]);
        text(&t, 132, y, val[i]);
    }
    /* ---- the bottom screen, on the yellow squares: what the option does, the disc and the console, Start */
    memcpy(bot, bg_bot, 320 * 240 * 3);
    box(&b, 160 - text_w(name[s->row]) / 2 - 14, 4, 160 + text_w(name[s->row]) / 2 + 13, 29);
    text(&b, 160 - text_w(name[s->row]) / 2, 9, name[s->row]);
    {   const char *d0 = 0, *d1 = 0;
        if (s->row == 0) {
            d0 = "The language of the texts.";
            if (s->lang_es_improved) d1 = s->es_ready ? "La traducci\xF3n mejorada ya est\xE1 preparada."
                                                      : "La traducci\xF3n mejorada se prepara la primera vez: unos minutos.";
        } else if (s->row == 1) { d0 = "Play: the logos, the intro, the movies."; d1 = "Skip: straight to the title screen."; }
        else if (s->row == 2) { d0 = "60 Hz: the speed of the US game."; d1 = "50 Hz: the speed of the PAL disc."; }
        else { d0 = "4:3 as on the PS1, Wide: all of the top screen, 3D: depth with the slider."; d1 = "Wide and 3D are slower."; }
        char disc[96]; snprintf(disc, sizeof disc, "%s", s->disc && *s->disc ? s->disc : "-");
        wrap_dry = 1;
        int n0 = wrap(&b, 0, 0, 288, d0, 3), n1 = d1 ? wrap(&b, 0, 0, 288, d1, 3) : 0, nd = wrap(&b, 0, 0, 288, disc, 3);
        wrap_dry = 0;
        /* (the text box from y 34 down; the console and disc box under it, both above Start) */
        int y = 32, h = 16 * (n0 + n1) + 8;
        box(&b, 8, y, 311, y + h - 1);
        wrap(&b, 16, y + 4, 288, d0, 3);
        if (d1) wrap(&b, 16, y + 4 + 16 * n0, 288, d1, 3);
        /* (the console, then the disc image's name on lines of its own) */
        y = START_Y0 - 6 - (16 * (1 + nd) + 8);
        box(&b, 8, y, 311, y + 16 * (1 + nd) + 7);
        text(&b, 16, y + 4, s->new3ds ? "New 3DS" : "Old 3DS");
        wrap(&b, 16, y + 20, 288, disc, 3);
    }
    bar(&b, START_X0, START_Y0, START_X1, START_Y1, s->start_down);
    text(&b, 160 - text_w("START GAME") / 2, START_Y0 + 7, "START GAME");
    if (exit_style == 0) {   /* the console's START key, then what it does */
        const int w = 42 + 8 + text_w("EXIT"), x = 160 - w / 2;
        box(&b, x - 14, 208, x + w + 13, 235);
        startkey(&b, x, 214, 42);
        text(&b, x + 50, 214, "EXIT");
    } else {
        box(&b, 160 - text_w("START BUTTON: EXIT") / 2 - 14, 208, 160 + text_w("START BUTTON: EXIT") / 2 + 13, 235);
        text(&b, 160 - text_w("START BUTTON: EXIT") / 2, 214, "START BUTTON: EXIT");
    }
}

/* ---- the OPTIONS screen's music while the start screen is up: the game's own (loadSoundSet 8 + startBgmTrack 0:
   SOUND/SND_SYS.WVD, one bank, and SOUND/S_SYS00.SEQ at volume 100), played by the port's SPU and sequencer
   (shims/sound.c); the host's audio device pulls it (port_sound_pull).  A WVD: u32 offset of the bodies' table,
   u32 offset of the headers' table; each table a u32 of its own size (the offset of its first item) and the items'
   offsets after it, the bodies' one the end of the last body too (src/cdLoadTask.c case 5). */
void SpuSetTransferStartAddr(unsigned a); unsigned SpuRead(unsigned char *p, unsigned n);
short SsVabFakeHead(unsigned char *h, short vabid, unsigned sbaddr); void SsVabClose(short id);
short SsSeqOpen(unsigned char *d, short vab); void SsSeqPlay(short id, char mode, short loops);
void SsSeqStop(short id); void SsSeqClose(short id); void SsSeqSetVol(short id, short l, short r); void SsSetMVol(short l, short r);
short SsUtKeyOnV(short voice, short vabid, short prog, short tone, short note, short fine, short voll, short volr);
char SsSetReservedVoice(char n);
static unsigned rd32le(const u8 *d) { return d[0] | d[1] << 8 | d[2] << 16 | (unsigned)d[3] << 24; }
static u8 *mus_wvd, *mus_seq;
static short mus_vab = -1, mus_id = -1, sfx_vab = -1;
/* the disc's language menu's own sounds (its boot program, SYSTEM.CNF BOOT: SCES_013.31 / .30): the second VAB inside
   it (the first is this music again); a VAB's body (fsize - its header's size) lies just before its header there.  Its cursor sound, when the flag changes:
   sound 0 of its table at 0x800AC328 (prog, tone, note, volume = 0, 0, 8, 100), played by its playSfx (0x80012108)
   as SsUtKeyOnV(23, that VAB, 0, 0, 8, 0, 100, 100); the sequencer keeps to voices 0-22 meanwhile */
static void sfx_load(void) {
    unsigned n = 0, m = 0; char path[64] = "";
    u8 *cnf = pa_read("SYSTEM.CNF", &n), *exe = NULL;
    if (cnf) {
        char t[256]; const unsigned k0 = n < sizeof t - 1 ? n : (unsigned)sizeof t - 1;
        memcpy(t, cnf, k0); t[k0] = 0;
        const char *b = strstr(t, "cdrom:");
        if (b) {
            b += 6; while (*b == '\\' || *b == '/') b++;
            int k = 0;
            while (b[k] && b[k] != ';' && b[k] != '\r' && b[k] != '\n' && k < 60) { path[k] = b[k] == '\\' ? '/' : b[k]; k++; }
            path[k] = 0;
        }
        free(cnf);
    }
    if (path[0]) exe = pa_read(path, &m);
    int seen = 0;
    for (unsigned o = 0; exe && o + 0x20 < m; o += 4) {
        if (memcmp(exe + o, "pBAV", 4) || ++seen != 2) continue;
        const unsigned ps = exe[o + 0x12] | exe[o + 0x13] << 8, fsize = rd32le(exe + o + 0xC), hs = 0x20 + 0x800 + ps * 0x200 + 0x200;
        if (ps > 128 || fsize <= hs || fsize - hs > o || o + hs > m) break;
        SpuSetTransferStartAddr(0x10000);   /* (past the music's bank) */
        SpuRead(exe + o - (fsize - hs), fsize - hs);
        sfx_vab = SsVabFakeHead(exe + o, -1, 0x10000);
        break;
    }
    free(exe);
    if (sfx_vab >= 0) SsSetReservedVoice(23);
    else port_printf("[launcher] the language menu's sounds are not readable (%s)\n", path[0] ? path : "no SYSTEM.CNF");
}
void port_launcher_sfx_move(void) {   /* (the caller holds the audio device's lock) */
    if (sfx_vab >= 0) SsUtKeyOnV(23, sfx_vab, 0, 0, 8, 0, 100, 100);
}
void port_launcher_music_stop(void) {
    if (mus_id >= 0) { SsSeqStop(mus_id); SsSeqClose(mus_id); }
    if (mus_vab >= 0) SsVabClose(mus_vab);
    if (sfx_vab >= 0) { SsVabClose(sfx_vab); SsSetReservedVoice(24); }
    mus_id = mus_vab = sfx_vab = -1;
    free(mus_wvd); free(mus_seq); mus_wvd = mus_seq = NULL;
}
int port_launcher_music_start(void) {   /* 1: playing */
    unsigned n = 0, m = 0;
    port_launcher_music_stop();
    mus_wvd = pa_read("SOUND/SND_SYS.WVD", &n);
    mus_seq = pa_read("SOUND/S_SYS00.SEQ", &m);
    if (mus_wvd && mus_seq && n >= 16 && m >= 16) {
        const unsigned bo = rd32le(mus_wvd), ho = rd32le(mus_wvd + 4);
        if (bo + 8 <= n && ho + 8 <= n) {
            const unsigned body = bo + rd32le(mus_wvd + bo), end = bo + rd32le(mus_wvd + bo + 4), head = ho + rd32le(mus_wvd + ho);
            if (body < end && end <= n && head + 0x20 <= n) {
                SpuSetTransferStartAddr(0x1010);   /* (the first bank's address in the game) */
                SpuRead(mus_wvd + body, end - body);
                mus_vab = SsVabFakeHead(mus_wvd + head, -1, 0x1010);
                if (mus_vab >= 0) mus_id = SsSeqOpen(mus_seq, mus_vab);
            }
        }
    }
    if (mus_id < 0) { port_printf("[launcher] the options music is not readable\n"); port_launcher_music_stop(); return 0; }
    sfx_load();
    SsSetMVol(100, 100);
    SsSeqSetVol(mus_id, 100, 100);
    SsSeqPlay(mus_id, 1, 1);
    return 1;
}

#ifndef __3DS__
/* PORT_LAUNCHER_PREVIEW=DIR (a PC, the disc open): the screen for each option, as DIR/launcher_<n>_{top,bottom}.png */
extern int port_gpu_write_png(const char *path, const unsigned char *rgb, int w, int h);   /* shims/gpu.c */
void port_launcher_game_preview(const char *dir) {
    static unsigned char top[400 * 240 * 3], bot[320 * 240 * 3]; char p[512];
    if (!port_launcher_game_begin()) { port_printf("[launcher] the options screen's pieces are not readable\n"); return; }
    { const char *v = getenv("PORT_LAUNCHER_EXIT"); if (v) exit_style = atoi(v); }
    if (getenv("PORT_LAUNCHER_BENCH")) {   /* (the time of one drawing of both screens) */
        extern unsigned long long port_time_us(void);
        LuiState st = { 0, "Espa\xF1ol (traducci\xF3n mejorada)", 1, 1, 1, 60, 0, "Tombi.chd", 0, 0, 0 };
        unsigned long long t0 = port_time_us();
        for (int i = 0; i < 20; i++) { st.row = i & 3; port_launcher_game_draw(top, bot, &st); }
        port_printf("[launcher] bench: %llu us a drawing\n", (port_time_us() - t0) / 20);
    }
    for (int r = 0; r < 5; r++) {
        LuiState st = { r & 3, "Espa\xF1ol (traducci\xF3n mejorada)", 1, r != 4, 1, 60, r == 3 ? 2 : 0, r == 1 ? "Tombi! (Europe) (En,Fr,De,Es,It,Nl,Sv,Da).chd" : "Tombi.chd", 0, r == 4, r == 4 ? 1 : 0 };
        port_launcher_game_draw(top, bot, &st);
        snprintf(p, sizeof p, "%s/launcher_%d_top.png", dir, r); port_gpu_write_png(p, top, 400, 240);
        snprintf(p, sizeof p, "%s/launcher_%d_bottom.png", dir, r); port_gpu_write_png(p, bot, 320, 240);
    }
    {   /* PORT_LAUNCHER_VIDEO=DIR: a scripted walk through the screen (the 3DS's keys, as n3ds/main_3ds.c handles
           them), each state's screens as DIR/v_<n>_{top,bottom}.png, DIR/v.txt their start times, DIR/v.wav the music
           with the cursor sound at every change (the PC's video: tools assemble it) */
        const char *vd = getenv("PORT_LAUNCHER_VIDEO");
        extern int port_sound_pull(short *dst, int frames);
        if (vd && port_launcher_music_start()) {
            static const char *name[10] = { "English", "Fran\xE7" "ais", "Deutsch", "Italiano", "Nederlands", "Espa\xF1ol (traducci\xF3n mejorada)",
                                            "Espa\xF1ol (original)", "Portugu\xEAs", "Svenska", "Dansk" };
            /* (seconds, key: d down, u up, r right, l left, s Start touched, . the end) */
            static const struct { float t; char k; } ev[] = {
                {2.0f, 'r'}, {2.8f, 'r'}, {3.6f, 'l'}, {4.4f, 'l'}, {5.4f, 'd'}, {6.4f, 'r'}, {7.2f, 'r'}, {8.2f, 'd'}, {9.0f, 'r'},
                {9.8f, 'r'}, {10.8f, 'd'}, {11.6f, 'r'}, {12.4f, 'r'}, {13.2f, 'r'}, {14.2f, 'u'}, {14.8f, 'u'}, {15.4f, 'u'},
                {16.6f, 's'}, {17.4f, '.'} };
            int row = 0, lang = 5, movies = 1, hz = 60, wide = 0, n = 0; float t = 0; unsigned total = 0;
            char p2[512]; snprintf(p2, sizeof p2, "%s/v.wav", vd);
            FILE *wf = fopen(p2, "wb"); snprintf(p2, sizeof p2, "%s/v.txt", vd); FILE *tf = fopen(p2, "w");
            u8 h[44] = { 0 }; if (wf) fwrite(h, 1, 44, wf);
            static short buf[2 * 4410];
            for (unsigned e = 0; e <= sizeof ev / sizeof ev[0] && wf && tf; e++) {
                LuiState st = { row, name[lang], lang == 5, 1, movies, hz, wide, "Tombi! (Europe).chd", 0, e > 0 && ev[e - 1].k == 's', 0 };
                st.pressed = st.start_down;
                port_launcher_game_draw(top, bot, &st);
                snprintf(p, sizeof p, "%s/v_%03d_top.png", vd, n); port_gpu_write_png(p, top, 400, 240);
                snprintf(p, sizeof p, "%s/v_%03d_bottom.png", vd, n); port_gpu_write_png(p, bot, 320, 240);
                fprintf(tf, "%d %.3f\n", n, t); n++;
                if (e == sizeof ev / sizeof ev[0]) break;
                const float until = ev[e].t;
                while (t + 0.0001f < until) { port_sound_pull(buf, 441); fwrite(buf, 4, 441, wf); total += 441 * 4; t += 0.01f; }
                const char k = ev[e].k; const int was = row * 4096 + lang * 64 + movies * 16 + (hz == 50) * 8 + wide;
                if (k == 'd' && row < 3) row++;
                if (k == 'u' && row > 0) row--;
                if (k == 'r' || k == 'l') {
                    const int d = k == 'r' ? 1 : -1;
                    if (row == 0) lang = (lang + d + 10) % 10; else if (row == 1) movies = !movies; else if (row == 2) hz = hz == 60 ? 50 : 60; else wide = (wide + d + 3) % 3;
                }
                if (k == '.') break;
                if (row * 4096 + lang * 64 + movies * 16 + (hz == 50) * 8 + wide != was) port_launcher_sfx_move();
            }
            for (int i = 0; i < 100 && wf; i++) { port_sound_pull(buf, 441); fwrite(buf, 4, 441, wf); total += 441 * 4; }
            if (wf) {
                const unsigned v[] = { 36 + total, 16, 1 | 2u << 16, 44100, 44100 * 4, 4 | 16u << 16 };
                fseek(wf, 0, SEEK_SET); fwrite("RIFF", 1, 4, wf); fwrite(&v[0], 4, 1, wf); fwrite("WAVEfmt ", 1, 8, wf);
                fwrite(&v[1], 4, 5, wf); fwrite("data", 1, 4, wf); fwrite(&total, 4, 1, wf); fclose(wf);
            }
            if (tf) fclose(tf);
            port_launcher_music_stop();
        }
    }
    port_launcher_game_end();
    {   /* PORT_LAUNCHER_MUSIC_WAV=FILE: 20 s of the music, as the device would pull it */
        const char *w = getenv("PORT_LAUNCHER_MUSIC_WAV");
        extern int port_sound_pull(short *dst, int frames);
        FILE *f = w ? fopen(w, "wb") : NULL;
        if (f && port_launcher_music_start()) {
            static short buf[2 * 1024]; unsigned total = 0; u8 h[44] = "RIFF\0\0\0\0WAVEfmt ";
            fwrite(h, 1, 44, f);
            for (int i = 0; i < 20 * 44100 / 1024; i++) {
                if (i % 86 == 43) port_launcher_sfx_move();   /* (the cursor sound once a second) */
                port_sound_pull(buf, 1024); fwrite(buf, 4, 1024, f); total += 4096;
            }
            port_launcher_music_stop();
            const unsigned v[] = { 36 + total, 16, 1 | 2u << 16, 44100, 44100 * 4, 4 | 16u << 16 };
            fseek(f, 4, SEEK_SET); fwrite(&v[0], 4, 1, f); fseek(f, 16, SEEK_SET); fwrite(&v[1], 4, 5, f);
            fwrite("data", 1, 4, f); fwrite(&total, 4, 1, f);
        }
        if (f) fclose(f);
    }
}
#endif
