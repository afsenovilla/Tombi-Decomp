/* The 3DS start screen (n3ds/main_3ds.c choose_lang): the options on the top screen as rounded rows on cream, the
   chosen one pink, under a pink-to-orange band with the title; the bottom screen says what the chosen option does,
   the disc and the console, and has a Start button.  Drawn on the CPU into RGB pictures; DejaVu Sans glyphs
   (launcher_font.c).  Strings are Latin-1. */
#include <string.h>
#include "launcher_ui.h"
#include "launcher_font.h"

typedef struct { unsigned char *p; int w, h; } Cv;
typedef unsigned int Rgb;   /* 0xRRGGBB */
#define RGB(r, g, b) ((Rgb)((r) << 16 | (g) << 8 | (b)))

static void put(Cv *c, int x, int y, Rgb col, int a) {   /* a: 0..255 coverage */
    if (x < 0 || y < 0 || x >= c->w || y >= c->h || a <= 0) return;
    unsigned char *d = c->p + (y * c->w + x) * 3;
    const int r = col >> 16 & 255, g = col >> 8 & 255, b = col & 255;
    if (a >= 255) { d[0] = (unsigned char)r; d[1] = (unsigned char)g; d[2] = (unsigned char)b; return; }
    d[0] = (unsigned char)((r * a + d[0] * (255 - a)) / 255);
    d[1] = (unsigned char)((g * a + d[1] * (255 - a)) / 255);
    d[2] = (unsigned char)((b * a + d[2] * (255 - a)) / 255);
}
static void fill(Cv *c, int x0, int y0, int x1, int y1, Rgb col) {   /* inclusive */
    for (int y = y0; y <= y1; y++) for (int x = x0; x <= x1; x++) put(c, x, y, col, 255);
}
/* a rounded rectangle (inclusive bounds), its corners' edges smoothed by their coverage (4 x 4 samples a pixel) */
static void rrect(Cv *c, int x0, int y0, int x1, int y1, int r, Rgb col) {
    const int w = x1 - x0 + 1, h = y1 - y0 + 1;
    if (r * 2 > w) r = w / 2;
    if (r * 2 > h) r = h / 2;
    for (int y = y0; y <= y1; y++) for (int x = x0; x <= x1; x++) {
        int cx = x < x0 + r ? x0 + r : x > x1 - r ? x1 - r : x, cy = y < y0 + r ? y0 + r : y > y1 - r ? y1 - r : y;
        if (cx == x || cy == y) { if (!(cx != x && cy != y)) { put(c, x, y, col, 255); continue; } }
        int in = 0;   /* (the corner's circle centred on the middle of pixel (cx, cy), radius r) */
        for (int sy = 0; sy < 4; sy++) for (int sx = 0; sx < 4; sx++) {
            float dx = x + (sx + 0.5f) / 4 - (cx + 0.5f), dy = y + (sy + 0.5f) / 4 - (cy + 0.5f);
            if (dx * dx + dy * dy <= (float)r * r) in++;
        }
        put(c, x, y, col, in * 255 / 16);
    }
}
static const LfGlyph *glyph(const LfFont *f, unsigned char ch) { return ch < 32 ? 0 : &f->g[ch - 32]; }
static int text_w(int font, const char *s) {
    const LfFont *f = &lf_fonts[font]; int w = 0;
    for (; *s; s++) { const LfGlyph *g = glyph(f, (unsigned char)*s); if (g) w += g->adv; }
    return w;
}
static void text(Cv *c, int x, int y, int font, const char *s, Rgb col) {   /* (x, y): the pen at the line's top */
    const LfFont *f = &lf_fonts[font];
    for (; *s; s++) {
        const LfGlyph *g = glyph(f, (unsigned char)*s);
        if (!g) continue;
        for (int j = 0; j < g->h; j++) for (int i = 0; i < g->w; i++) {
            const int k = j * g->w + i, a = f->data[g->off + (k >> 1)] >> ((k & 1) * 4) & 15;
            if (a) put(c, x + g->x + i, y + g->y + j, col, a * 17);
        }
        x += g->adv;
    }
}

/* the palette */
#define C_BG     RGB(255, 246, 236)
#define C_CARD   RGB(255, 255, 255)
#define C_EDGE   RGB(240, 214, 200)
#define C_PINK   RGB(255, 150, 170)
#define C_PINKD  RGB(232, 120, 145)
#define C_LABEL  RGB(120, 70, 60)
#define C_TEXT   RGB(90, 60, 50)
#define C_MUTED  RGB(150, 110, 100)
#define C_GREEN  RGB(60, 160, 90)
#define C_WHITE  RGB(255, 255, 255)

static const int START_X0 = 12, START_Y0 = 176, START_X1 = 308, START_Y1 = 222;
void lui_start_rect(int *x0, int *y0, int *x1, int *y1) { *x0 = START_X0; *y0 = START_Y0; *x1 = START_X1; *y1 = START_Y1; }

void lui_draw(unsigned char *top, unsigned char *bot, const LuiState *s) {
    static const char *lab[4] = { "Language", "Movies", "Speed", "Picture" };
    const char *val[4] = { s->lang, s->movies ? "Play" : "Skip", s->hz == 50 ? "50 Hz" : "60 Hz",
                           s->picture == 1 ? "Wide (experimental)" : s->picture == 2 ? "3D (experimental)" : "4:3" };
    Cv t = { top, 400, 240 }, b = { bot, 320, 240 };
    /* ---- the top screen */
    fill(&t, 0, 0, 399, 239, C_BG);
    for (int y = 0; y < 46; y++) fill(&t, 0, y, 399, y, RGB(255 - y * 2 / 5, 120 + y * 6 / 5, 150 - y * 4 / 5));   /* pink to orange */
    text(&t, 16, 8, LF_BOLD24, "Tombi!", C_WHITE);
    text(&t, 112, 18, LF_BOOK12, "3DS port", RGB(255, 235, 240));
    for (int i = 0; i < 4; i++) {
        const int y = 58 + i * 42, sel = i == s->row;
        if (sel) rrect(&t, 16, y, 384, y + 34, 17, C_PINK);
        else { rrect(&t, 16, y, 384, y + 34, 17, C_EDGE); rrect(&t, 17, y + 1, 383, y + 33, 16, C_CARD); }
        text(&t, 32, y + 9, LF_BOLD13, lab[i], sel ? C_WHITE : C_LABEL);
        /* the value at the right; the chosen row's between arrows (Left / Right change it) */
        const int w = text_w(LF_BOLD13, val[i]);
        text(&t, 366 - w - (sel ? 12 : 0), y + 9, LF_BOLD13, val[i], sel ? C_WHITE : C_TEXT);
        if (sel) { text(&t, 366 - w - 26, y + 8, LF_BOLD13, "\xAB", C_WHITE); text(&t, 360, y + 8, LF_BOLD13, "\xBB", C_WHITE); }
    }
    /* ---- the bottom screen: what the option does */
    fill(&b, 0, 0, 319, 239, C_BG);
    text(&b, 12, 8, LF_BOLD15, lab[s->row], C_TEXT);
    const char *ln[6] = { 0 }; Rgb lc[6]; int n = 0;
#define L(str, col) do { ln[n] = (str); lc[n] = (col); n++; } while (0)
    if (s->row == 0) {
        L("The language of the game's texts.", C_MUTED);
        if (s->lang_es_improved && s->es_ready) { L("", C_MUTED); L("La traducci\xF3n mejorada ya est\xE1", C_GREEN); L("preparada.", C_GREEN); }
        else if (s->lang_es_improved) { L("", C_MUTED); L("La traducci\xF3n mejorada se prepara la", C_GREEN); L("primera vez: unos minutos, con una barra.", C_GREEN); }
    } else if (s->row == 1) {
        L("Play: the logos, the intro and the game's", C_MUTED); L("movies.", C_MUTED);
        L("Skip: straight to the title screen, and no", C_MUTED); L("movie in the game either.", C_MUTED);
    } else if (s->row == 2) {
        L("60 Hz: the 3DS's screens and the US", C_MUTED); L("game's speed.", C_MUTED);
        L("50 Hz: the PAL disc's own speed.", C_MUTED);
    } else {
        L("4:3: the picture as on the PS1.", C_MUTED);
        L("Wide: the whole top screen, 400 columns.", C_MUTED);
        L("3D: depth with the 3D slider.", C_MUTED);
        L("Wide and 3D are experimental: slower in", C_MUTED); L("the busiest places.", C_MUTED);
    }
#undef L
    for (int i = 0; i < n; i++) text(&b, 12, 34 + i * 14, LF_BOOK11, ln[i], lc[i]);
    rrect(&b, 12, 118, 308, 160, 14, C_CARD);
    text(&b, 24, 124, LF_BOOK10, "Disc", C_MUTED); text(&b, 80, 124, LF_BOLD10, s->disc, C_TEXT);
    text(&b, 24, 140, LF_BOOK10, "Model", C_MUTED); text(&b, 80, 140, LF_BOLD10, s->new3ds ? "New 3DS" : "Old 3DS", C_TEXT);
    rrect(&b, START_X0, START_Y0, START_X1, START_Y1, 18, s->start_down ? C_PINKD : C_PINK);
    { const char *st = "Start the game"; text(&b, 160 - text_w(LF_BOLD17, st) / 2, 187, LF_BOLD17, st, C_WHITE); }
    { const char *ft = "Up/Down option   Left/Right change   A start   START exit";
      text(&b, 160 - text_w(LF_BOOK9, ft) / 2, 226, LF_BOOK9, ft, C_MUTED); }
}
