/* Spanish improved translation (patches/es_mejorada.tombipatch.json): the Select sub-screens' "Elmto." tab and the
 * "Usar elmto" leaf button say "Objetos" / "Usar objeto", like the pause menu entry the patch renames ("Elmto" ->
 * "Objetos").  Both labels are painted into the sub-screen texture page (4 bpp, VRAM x 64 words, y 0: the disc's
 * SYSTEM/A<n>0301.000, n = --lang + 1), so they are rewritten in VRAM right after that page is uploaded, with the
 * original artist's letters only:
 *   - the same disc carries the page of every language, and the Swedish / Danish ones write "Objekt" on the tab and
 *     "Obj" / "Objekt" on the leaf: the tab's O b j e come from Swedish "Objekt", t o from Spanish "Elmto." itself, s
 *     from "Estado"; the leaf's b j from Swedish "Obj", e from Spanish "ordenar", t from "elmto", o from "ordenar" (the "elmto" o touches its t);
 *   - a letter is its letter texels (the tab's dark red, the leaf's white) plus its edge: the texels within SUB_NEAR
 *     of it that differ from the plank / leaf behind the text.  That picture is the same in every language, so it is
 *     recovered texel by texel from the languages with no letter near (clean_group);
 *   - the old text and its edge are replaced by that picture, the new word is laid out with the original spacing
 *     (letters that were neighbours keep their step) on the old line's baseline, centred like the original.
 * Which letters, where: tools/subscreen_glyphs.json, generated with tools/subscreen_glyphs.py into
 * subscreen_glyphs.h -- coordinates only.  Nothing is shipped: every pixel is read from the user's disc (or data/),
 * and a source label whose checksum does not match leaves the page alone.  Only with --lang 5 (Spanish graphics on:
 * not with --no-gfx-es) and a MAIN5.EXE whose pause list holds "Objetos" (the improved translation, applied in
 * memory with --text-patch or written into data/ by the PC launcher). */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "subscreen_glyphs.h"

extern int port_lang;
extern unsigned char *port_cd_read_named(const char *path, unsigned *size);
extern int port_gfx_es;                        /* main.c: --no-gfx-es keeps the disc's sprites */
extern void port_printf(const char *, ...);

#define PX 64                                  /* texture page x (in 16-bit words), y 0 */
#define PAGE_BYTES 32768                       /* 256 x 256 texels, 4 bpp */
#define ES_FILE 6                              /* SYSTEM/A60301.000: Spanish */
static int es_state = -1;                      /* -1 not checked, 0 off, 1 on */

static int es_on(void) {
    if (es_state < 0) {
        static const unsigned char obj[7] = { 0x20, 0x21, 0x13, 0x0F, 0x1B, 0x17, 0x1A };   /* "Objetos" (tools/pause_text.py) */
        /* the MAIN5.EXE the game runs with: a --text-patch in memory, else data/ (the PC launcher writes its translation
           into data/ itself and passes no --text-patch: reading only the in-memory patch left "Elmto." there), else the disc */
        unsigned n = 0; unsigned char *d = port_lang == 5 && port_gfx_es ? port_cd_read_named("EXE/MAIN5.EXE", &n) : 0;
        if (getenv("PORT_SUBSCREEN_ES_LOG")) port_printf("[subscreen_es] MAIN5.EXE: %u bytes\n", n);
        es_state = 0;
        for (unsigned i = 0; d && i + 8 <= n; i++) if (!memcmp(d + i, obj, 7) && (d[i + 7] == 0xFF || !d[i + 7])) { es_state = 1; break; }
        free(d);
    }
    return es_state;
}

/* ---- pages: [1..9] the disc's files, [0] the working copy of the page in VRAM */
static unsigned char *pg[10];
static unsigned char work[PAGE_BYTES];
static int tx(const unsigned char *p, int u, int v) { int b = p[v * 128 + (u >> 1)]; return u & 1 ? b >> 4 : b & 15; }
static void set_tx(unsigned char *p, int u, int v, int t) {
    unsigned char *b = &p[v * 128 + (u >> 1)];
    *b = (unsigned char)(u & 1 ? (*b & 0x0F) | t << 4 : (*b & 0xF0) | t);
}
static unsigned hash_rect(const unsigned char *p, int u0, int v0, int w, int h) {   /* FNV-1a over the texel indices */
    unsigned hs = 2166136261u;
    for (int v = 0; v < h; v++) for (int u = 0; u < w; u++) hs = (hs ^ (unsigned)tx(p, u0 + u, v0 + v)) * 16777619u;
    return hs;
}
static int is_text(int font, int t) { return sub_fonts[font].text_mask >> t & 1; }
static int label_ok(int li) {                   /* the label's file is there and is the one the table was made from */
    const SubLabel *l = &sub_labels[li]; const SubSlot *s = &sub_slots[l->slot];
    return pg[l->file] && hash_rect(pg[l->file], s->u, s->v, s->w, s->h) == l->hash;
}

/* ---- the picture behind the text (tools/subscreen_glyphs.py clean()) */
#define MW 64
#define MH 39
static unsigned char clean_buf[SUB_NGROUPS][MH][MW];
static unsigned char clean_done[SUB_NGROUPS];
static int near_text(const unsigned char *p, const SubSlot *s, int font, int x, int y) {
    for (int yy = y - SUB_NEAR; yy <= y + SUB_NEAR; yy++) for (int xx = x - SUB_NEAR; xx <= x + SUB_NEAR; xx++)
        if (xx >= 0 && yy >= 0 && xx < s->w && yy < s->h && is_text(font, tx(p, s->u + xx, s->v + yy))) return 1;
    return 0;
}
static int group_mode(int group, int x, int y, int strict) {
    int cnt[16] = { 0 }, best = -1;
    for (int n = 1; n <= 9; n++) {
        if (!pg[n]) continue;
        for (unsigned k = 0; k < sizeof sub_slots / sizeof *sub_slots; k++) {
            const SubSlot *s = &sub_slots[k];
            if (s->group != group) continue;
            if (strict ? near_text(pg[n], s, s->font, x, y) : is_text(s->font, tx(pg[n], s->u + x, s->v + y))) continue;
            cnt[tx(pg[n], s->u + x, s->v + y)]++;
        }
    }
    for (int t = 0; t < 16; t++) if (cnt[t] && (best < 0 || cnt[t] > cnt[best])) best = t;
    return best;
}
static unsigned char (*clean_group(int group))[MW] {
    unsigned char (*c)[MW] = clean_buf[group];
    int w = 0, h = 0;
    signed char strict[MH][MW];
    if (clean_done[group]) return c;
    for (unsigned k = 0; k < sizeof sub_slots / sizeof *sub_slots; k++) if (sub_slots[k].group == group) { w = sub_slots[k].w; h = sub_slots[k].h; break; }
    for (int y = 0; y < h; y++) for (int x = 0; x < w; x++) strict[y][x] = (signed char)group_mode(group, x, y, 1);
    for (int y = 0; y < h; y++) for (int x = 0; x < w; x++) {
        int t = strict[y][x];
        for (int d = 1; t < 0 && d < w; d++) {   /* every language has a letter near: the nearest such texel on the row */
            if (x - d >= 0 && strict[y][x - d] >= 0) t = strict[y][x - d];
            else if (x + d < w && strict[y][x + d] >= 0) t = strict[y][x + d];
        }
        if (t < 0) t = group_mode(group, x, y, 0);
        c[y][x] = (unsigned char)(t < 0 ? 0 : t);
    }
    clean_done[group] = 1;
    return c;
}

/* ---- a glyph: its letter texels (8-connected to its seeds inside its rect) and its edge (subscreen_glyphs.py
   letter_texels / glyph_texels) */
typedef struct { unsigned char x, y, t, letter; } Texel;     /* slot-relative */
static int cut_glyph(int gi, const unsigned char *p, Texel *out) {
    const SubGlyph *g = &sub_glyphs[gi];
    const SubLabel *l = &sub_labels[g->label];
    const SubSlot *s = &sub_slots[l->slot];
    const int font = l->font;
    unsigned char (*cl)[MW] = clean_group(s->group);
    static unsigned char let[MH][MW];
    unsigned char stack[MH * MW][2];
    int sp = 0, n = 0;
    const int x0 = g->u - s->u, y0 = g->v - s->v, x1 = x0 + g->w, y1 = y0 + g->h;
    memset(let, 0, sizeof let);
    for (int k = 0; k < g->nseeds; k++) {
        int x = sub_seeds[g->seed + k][0] - s->u, y = sub_seeds[g->seed + k][1] - s->v;
        if (x < x0 || y < y0 || x >= x1 || y >= y1 || let[y][x] || !is_text(font, tx(p, s->u + x, s->v + y))) continue;
        let[y][x] = 1; stack[sp][0] = (unsigned char)x; stack[sp][1] = (unsigned char)y; sp++;
        while (sp) {
            int cx = stack[sp - 1][0], cy = stack[sp - 1][1]; sp--;
            for (int dy = -1; dy <= 1; dy++) for (int dx = -1; dx <= 1; dx++) {
                int xx = cx + dx, yy = cy + dy;
                if (xx < x0 || yy < y0 || xx >= x1 || yy >= y1 || let[yy][xx] || !is_text(font, tx(p, s->u + xx, s->v + yy))) continue;
                let[yy][xx] = 1; stack[sp][0] = (unsigned char)xx; stack[sp][1] = (unsigned char)yy; sp++;
            }
        }
    }
    for (int y = y0 - SUB_NEAR; y < y1 + SUB_NEAR; y++) for (int x = x0 - SUB_NEAR; x < x1 + SUB_NEAR; x++) {
        int t, own = 99, other = 99;
        if (x < 0 || y < 0 || x >= s->w || y >= s->h) continue;
        t = tx(p, s->u + x, s->v + y);
        if (let[y][x]) { out[n].x = (unsigned char)x; out[n].y = (unsigned char)y; out[n].t = (unsigned char)t; out[n].letter = 1; n++; continue; }
        if (is_text(font, t) || t == cl[y][x]) continue;
        for (int yy = y - SUB_NEAR; yy <= y + SUB_NEAR; yy++) for (int xx = x - SUB_NEAR; xx <= x + SUB_NEAR; xx++) {
            int d;
            if (xx < 0 || yy < 0 || xx >= s->w || yy >= s->h || !is_text(font, tx(p, s->u + xx, s->v + yy))) continue;
            d = abs(xx - x) > abs(yy - y) ? abs(xx - x) : abs(yy - y);
            if (let[yy][xx]) { if (d < own) own = d; } else if (d < other) other = d;
        }
        if (own <= SUB_NEAR && own <= other) { out[n].x = (unsigned char)x; out[n].y = (unsigned char)y; out[n].t = (unsigned char)t; out[n].letter = 0; n++; }
    }
    return n;
}

static int find_label(int file, int font, int slot_u, int slot_v) {
    for (unsigned i = 0; i < sizeof sub_labels / sizeof *sub_labels; i++) {
        const SubSlot *s = &sub_slots[sub_labels[i].slot];
        if (sub_labels[i].file == file && sub_labels[i].font == font && s->u == slot_u && s->v == slot_v) return (int)i;
    }
    return -1;
}
static int pick(int font, char ch) {
    for (unsigned i = 0; i < sizeof sub_picks / sizeof *sub_picks; i++) if (sub_picks[i].font == font && sub_picks[i].ch == ch) return sub_picks[i].glyph;
    return -1;
}

/* rewrites line `line` of the Spanish label `li` in work[] as `text`; 0 when a letter or its source is missing */
static int compose(int li, int line, const char *text, int centre_on_slot) {
    static Texel cut[MH * MW];
    const SubLabel *l = &sub_labels[li];
    const SubSlot *s = &sub_slots[l->slot];
    const SubLine *ln = &sub_lines[l->line + line];
    const int font = l->font;
    unsigned char (*cl)[MW] = clean_group(s->group);
    int gl[16], xs[16], n = (int)strlen(text), lo = 255, hi = 0, left;
    if (n > 16) return 0;
    for (int i = 0; i < n; i++) if ((gl[i] = pick(font, text[i])) < 0 || !label_ok(sub_glyphs[gl[i]].label)) return 0;
    /* the old line and its edge: the picture behind it */
    {   static unsigned char old[MH][MW];
        memset(old, 0, sizeof old);
        for (int k = 0; k < ln->nglyphs; k++) {
            const SubGlyph *g = &sub_glyphs[ln->glyph + k];
            int m = cut_glyph(ln->glyph + k, work, cut);
            for (int j = 0; j < m; j++) old[cut[j].y][cut[j].x] = 1;
            if (g->u < lo) lo = g->u;
            if (g->u + g->w > hi) hi = g->u + g->w;
        }
        for (int y = 0; y < s->h; y++) for (int x = 0; x < s->w; x++) if (old[y][x]) set_tx(work, s->u + x, s->v + y, cl[y][x]);
    }
    /* the layout: neighbours in their source word keep their step, the others step by width + the font's pair gap */
    xs[0] = 0;
    for (int i = 1; i < n; i++) {
        const SubGlyph *a = &sub_glyphs[gl[i - 1]];
        int adj = gl[i] == gl[i - 1] + 1 && sub_glyphs[gl[i]].label == a->label && a->advance >= 0;
        xs[i] = xs[i - 1] + (adj ? a->advance : a->w + sub_fonts[font].pair_gap);
    }
    left = ((centre_on_slot ? 2 * s->u + s->w : lo + hi) - (xs[n - 1] + sub_glyphs[gl[n - 1]].w)) / 2;
    /* the edges first, then the letters over them; an edge never covers a letter of another line */
    for (int pass = 0; pass < 2; pass++) for (int i = 0; i < n; i++) {
        const SubGlyph *g = &sub_glyphs[gl[i]];
        const SubSlot *gs = &sub_slots[sub_labels[g->label].slot];
        int m = cut_glyph(gl[i], pg[sub_labels[g->label].file], cut);
        for (int j = 0; j < m; j++) {
            int u = left + xs[i] + (gs->u + cut[j].x - g->u), v = ln->baseline + (gs->v + cut[j].y - g->baseline);
            if (cut[j].letter != pass || u < s->u || v < s->v || u >= s->u + s->w || v >= s->v + s->h) continue;
            if (!pass && is_text(font, tx(work, u, v))) continue;
            set_tx(work, u, v, cut[j].t);
        }
    }
    return 1;
}

/* writes a rect of work[] (u0 and w multiples of 4) back to VRAM */
static void put_rect(int u0, int v0, int w, int h, void (*write)(int, int, int, int, const unsigned short *)) {
    static unsigned short words[MH * 16];
    for (int v = 0; v < h; v++) for (int k = 0; k < w / 4; k++) {
        const unsigned char *b = &work[(v0 + v) * 128 + (u0 >> 1) + k * 2];
        words[v * (w / 4) + k] = (unsigned short)(b[0] | b[1] << 8);
    }
    write(PX + u0 / 4, v0, w / 4, h, words);
}

/* gpu.c, after every host / GP0 upload into VRAM (x, y in words) */
void port_subscreen_es_upload(const unsigned short *vram, int x, int y, int w, int h,
                              void (*write)(int, int, int, int, const unsigned short *)) {
    static int logged, failed, tab_li = -2, leaf_li;
    static unsigned char cached_tab[24][32], cached_leaf[MH][28];   /* the rewritten rects, as page bytes */
    static int cached;
    const SubSlot *ts, *ls;
    unsigned th, lh;
    if (port_lang != 5 || failed) return;
    if (tab_li == -2) {
        tab_li = find_label(ES_FILE, SUB_FONT_TAB, 0x98, 0x38);
        leaf_li = find_label(ES_FILE, SUB_FONT_LEAF, 0x28, 0x40);
        if (tab_li < 0 || leaf_li < 0) { failed = 1; return; }
    }
    ts = &sub_slots[sub_labels[tab_li].slot]; ls = &sub_slots[sub_labels[leaf_li].slot];
    /* the upload must hold both label rects */
    if (x > PX + ls->u / 4 || x + w < PX + (ts->u + ts->w) / 4 || y > ts->v || y + h < ls->v + ls->h) return;
    for (int v = 0; v < 256; v++) for (int k = 0; k < 64; k++) {     /* the page as the file has it */
        unsigned short wd = vram[v * 1024 + PX + k];
        work[v * 128 + k * 2] = (unsigned char)wd; work[v * 128 + k * 2 + 1] = (unsigned char)(wd >> 8);
    }
    th = hash_rect(work, ts->u, ts->v, ts->w, ts->h); lh = hash_rect(work, ls->u, ls->v, ls->w, ls->h);
    if (getenv("PORT_SUBSCREEN_ES_LOG") && logged++ < 20) port_printf("[subscreen_es] upload %d,%d %dx%d: tab 0x%08x leaf 0x%08x\n", x, y, w, h, th, lh);
    if (th != sub_labels[tab_li].hash || lh != sub_labels[leaf_li].hash || !es_on()) return;   /* MAIN5.EXE read once */
    if (!cached) {
        char name[32]; unsigned sz;
        for (int n = 1; n <= 9; n++) {
            snprintf(name, sizeof name, SUB_PAGE_FILE, n);
            pg[n] = port_cd_read_named(name, &sz);
            if (pg[n] && sz != PAGE_BYTES) { free(pg[n]); pg[n] = 0; }
        }
        if (!compose(tab_li, 0, "Objetos", 1) || !compose(leaf_li, 1, "objeto", 0)) {
            port_printf("[subscreen_es] the sub-screen page's letters are not all on this disc: \"Elmto.\" kept\n");
            failed = 1;
        } else {
            for (int v = 0; v < ts->h; v++) memcpy(cached_tab[v], &work[(ts->v + v) * 128 + (ts->u >> 1)], ts->w / 2);
            for (int v = 0; v < ls->h; v++) memcpy(cached_leaf[v], &work[(ls->v + v) * 128 + (ls->u >> 1)], ls->w / 2);
            cached = 1;
        }
        for (int n = 1; n <= 9; n++) { free(pg[n]); pg[n] = 0; }
        if (failed) return;
    }
    /* the next uploads of the same page: the words worked out the first time */
    for (int v = 0; v < ts->h; v++) memcpy(&work[(ts->v + v) * 128 + (ts->u >> 1)], cached_tab[v], ts->w / 2);
    for (int v = 0; v < ls->h; v++) memcpy(&work[(ls->v + v) * 128 + (ls->u >> 1)], cached_leaf[v], ls->w / 2);
    put_rect(ts->u, ts->v, ts->w, ts->h, write);
    put_rect(ls->u, ls->v, ls->w, ls->h, write);
}
