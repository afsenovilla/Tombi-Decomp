/* The 3DS start screen (n3ds/main_3ds.c choose_lang) in the look of the game's own OPTIONS screen (the title screen's
   "Opciones", overlay O0PTSUB0): the checkerboard of green and yellow squares with Tomba and the pig on them, black
   letters of its 8 x 16 font, the chosen line on a green bar.  Everything is read from the user's disc when the
   screen opens (nothing of it is in the repository):
     SYSTEM/A00801.GAM  8 bpp, 384 x 256: the eight squares (80 x 84, slots 80 x 86 apart; the font pages beside them)
     SYSTEM/A00802.000  their 256-colour CLUT
     SYSTEM/A00804.000  4 bpp, 256 x 256: the 8 x 16 font (rows of 32 characters from ' ', then A-grave.., a-grave..)
     SYSTEM/A00803.000  the font's CLUT (row 0)
   With no disc readable the plain screen (n3ds/launcher_ui.c) is drawn instead.  Pictures: RGB, rows from the top. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "port_runtime.h"
#include "panel_assets.h"
#include "../n3ds/launcher_ui.h"

typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32;

static u8 *sq, *font;                 /* the squares' page (8 bpp), the font page (4 bpp) */
static u32 sq_pal[256], font_pal[16]; /* 0xRRGGBB */
static unsigned sq_n, font_n;

static u32 c15(unsigned v) { return (v & 31) << 19 | (v >> 5 & 31) << 11 | (v >> 10 & 31) << 3; }
static int read_pal(const char *name, u32 *pal, int n) {
    unsigned len; u8 *d = pa_read(name, &len);
    if (!d || len < (unsigned)n * 2) { free(d); return 0; }
    for (int i = 0; i < n; i++) pal[i] = c15(d[2 * i] | d[2 * i + 1] << 8);
    free(d); return 1;
}
void port_launcher_game_end(void) { free(sq); free(font); sq = font = NULL; }
/* 1: the pieces were read */
int port_launcher_game_begin(void) {
    port_launcher_game_end();
    sq = pa_read("SYSTEM/A00801.GAM", &sq_n);
    font = pa_read("SYSTEM/A00804.000", &font_n);
    if (!sq || sq_n < 384 * 256 || !font || font_n < 128 * 256 || !read_pal("SYSTEM/A00802.000", sq_pal, 256) ||
        !read_pal("SYSTEM/A00803.000", font_pal, 16)) {
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
/* the squares: green ones where (column + row) is even, yellow ones elsewhere, as on the game's screen */
static void squares(Cv *c, int y0) {
    static const u8 green[4][2] = { {0, 0}, {2, 0}, {0, 1}, {1, 2} }, yellow[4][2] = { {1, 0}, {1, 1}, {2, 1}, {0, 2} };
    for (int j = 0; y0 + j * 84 < c->h; j++) for (int i = 0; i * 80 < c->w; i++) {
        const u8 *t = (i + j) & 1 ? yellow[(i + 3 * j) & 3] : green[(i + 3 * j) & 3];
        for (int y = 0; y < 84; y++) for (int x = 0; x < 80; x++)
            put(c, i * 80 + x, y0 + j * 84 + y, sq_pal[sq[(t[1] * 86 + y) * 384 + t[0] * 80 + x]]);
    }
}
static int glyph_cell(unsigned ch, int *u, int *v) {
    if (ch >= 0x20 && ch < 0x80) { *u = (ch & 31) * 8; *v = (ch - 0x20) / 32 * 16; return 1; }
    if (ch >= 0xC0) { *u = (ch & 31) * 8; *v = ch < 0xE0 ? 48 : 64; return 1; }
    return 0;
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
static int wrap(Cv *c, int x, int y, int w, const char *s, int maxl) {
    int n = 0, per = w / 8;
    while (*s && n < maxl) {
        while (*s == ' ') s++;
        int len = (int)strlen(s), take = len;
        if (len > per) { take = per; while (take > 0 && s[take] != ' ') take--; if (!take) take = per; }
        char ln[64]; if (take > 63) take = 63;
        memcpy(ln, s, take); ln[take] = 0;
        text(c, x, y + 16 * n, ln);
        s += take; n++;
    }
    return n;
}

static const int START_X0 = 40, START_Y0 = 172, START_X1 = 279, START_Y1 = 203;
void port_launcher_game_start_rect(int *x0, int *y0, int *x1, int *y1) { *x0 = START_X0; *y0 = START_Y0; *x1 = START_X1; *y1 = START_Y1; }

void port_launcher_game_draw(unsigned char *top, unsigned char *bot, const LuiState *s) {
    static const char *lab[4] = { "LANGUAGE:", "MOVIES:", "SPEED:", "PICTURE:" }, *name[4] = { "LANGUAGE", "MOVIES", "SPEED", "PICTURE" };
    char lang[64]; upper(lang, s->lang, sizeof lang);
    const char *val[4] = { lang, s->movies ? "PLAY" : "SKIP", s->hz == 50 ? "50 HZ" : "60 HZ",
                           s->picture == 1 ? "WIDE (EXPERIMENTAL)" : s->picture == 2 ? "3D (EXPERIMENTAL)" : "4:3" };
    Cv t = { top, 400, 240 }, b = { bot, 320, 240 };
    /* ---- the top screen: the options, as the game's OPTIONS */
    squares(&t, -6);
    text(&t, 200 - text_w("TOMBI!") / 2, 22, "TOMBI!");
    for (int i = 0; i < 4; i++) {
        const int y = 60 + i * 32;
        if (i == s->row) bar(&t, 16, y - 4, 383, y + 17, 0);
        text(&t, 28, y, lab[i]);
        text(&t, 132, y, val[i]);
    }
    text(&t, 200 - text_w("UP/DOWN: OPTION   LEFT/RIGHT: CHANGE") / 2, 204, "UP/DOWN: OPTION   LEFT/RIGHT: CHANGE");
    /* ---- the bottom screen: what the option does, the disc and the console, Start */
    squares(&b, -6);
    text(&b, 160 - text_w(name[s->row]) / 2, 8, name[s->row]);
    {   const char *d0 = 0, *d1 = 0; int y = 32;
        if (s->row == 0) {
            d0 = "The language of the texts.";
            if (s->lang_es_improved) d1 = s->es_ready ? "La traducci\xF3n mejorada ya est\xE1 preparada."
                                                      : "La traducci\xF3n mejorada se prepara la primera vez: unos minutos.";
        } else if (s->row == 1) { d0 = "Play: the logos, the intro, the movies."; d1 = "Skip: straight to the title screen."; }
        else if (s->row == 2) { d0 = "60 Hz: the speed of the US game."; d1 = "50 Hz: the speed of the PAL disc."; }
        else { d0 = "4:3 as on the PS1, Wide: all of the top screen, 3D: depth with the slider."; d1 = "Wide and 3D are slower."; }
        y += 16 * wrap(&b, 16, y, 288, d0, 3);
        if (d1) wrap(&b, 16, y, 288, d1, 3);
    }
    {   char disc[48]; snprintf(disc, sizeof disc, "%s", s->disc && *s->disc ? s->disc : "-");
        text(&b, 16, 132, s->new3ds ? "New 3DS" : "Old 3DS");
        wrap(&b, 16, 148, 288, disc, 1);
    }
    bar(&b, START_X0, START_Y0, START_X1, START_Y1, s->start_down);
    text(&b, 160 - text_w("START") / 2, 180, "START");
    text(&b, 160 - text_w("A: START   START: EXIT") / 2, 216, "A: START   START: EXIT");
}

#ifndef __3DS__
/* PORT_LAUNCHER_PREVIEW=DIR (a PC, the disc open): the screen for each option, as DIR/launcher_<n>_{top,bottom}.png */
extern int port_gpu_write_png(const char *path, const unsigned char *rgb, int w, int h);   /* shims/gpu.c */
void port_launcher_game_preview(const char *dir) {
    static unsigned char top[400 * 240 * 3], bot[320 * 240 * 3]; char p[512];
    if (!port_launcher_game_begin()) { port_printf("[launcher] the options screen's pieces are not readable\n"); return; }
    for (int r = 0; r < 5; r++) {
        LuiState st = { r & 3, "Espa\xF1ol (traducci\xF3n mejorada)", 1, r != 4, 1, 60, r == 3 ? 2 : 0, "Tombi.chd", 0, r == 4 };
        port_launcher_game_draw(top, bot, &st);
        snprintf(p, sizeof p, "%s/launcher_%d_top.png", dir, r); port_gpu_write_png(p, top, 400, 240);
        snprintf(p, sizeof p, "%s/launcher_%d_bottom.png", dir, r); port_gpu_write_png(p, bot, 320, 240);
    }
    port_launcher_game_end();
}
#endif
