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

/* ---- a glyph for a character: the disc's letter (sub_picks, cut from its page) or one of ours (sub_extra, an ink
   bitmap with the edge / rim the disc's letters carry), with an accent mark for the composed letters; texels are
   relative to (left of the ink, baseline) */
#define GMAX 400
typedef struct { signed char x, y; unsigned char t, letter; } GTexel;
static int extra_of(int font, unsigned ch) {
    for (unsigned i = 0; i < sizeof sub_extra / sizeof *sub_extra; i++) if (sub_extra[i].font == font && sub_extra[i].ch == ch) return (int)i;
    return -1;
}
static int gt_has(const GTexel *g, int n, int x, int y) { for (int i = 0; i < n; i++) if (g[i].x == x && g[i].y == y) return 1; return 0; }
static int gt_add(GTexel *g, int n, int x, int y, int t, int letter) {
    if (n >= GMAX || x < -64 || x > 63 || y < -64 || y > 63) return n;
    g[n].x = (signed char)x; g[n].y = (signed char)y; g[n].t = (unsigned char)t; g[n].letter = (unsigned char)letter; return n + 1;
}
/* the shaded edge of our own tab ink (below and to the right, as the plank's letters are cut); the leaf's letters are
   plain white (the disc's carry a few stray light texels, not a rim), so ours get nothing */
static int gt_edge(int font, GTexel *g, int n) {
    static const signed char tab_d[3][2] = { {1, 1}, {0, 1}, {1, 0} };
    int m = n;
    if (font != SUB_FONT_TAB) return n;
    for (int i = 0; i < n; i++) if (g[i].letter) for (int k = 0; k < 3; k++)
        if (!gt_has(g, m, g[i].x + tab_d[k][0], g[i].y + tab_d[k][1])) m = gt_add(g, m, g[i].x + tab_d[k][0], g[i].y + tab_d[k][1], sub_edge[font], 0);
    return m;
}
static int glyph_extra(int font, int ei, GTexel *g) {
    const SubExtra *e = &sub_extra[ei]; int n = 0;
    for (int y = 0; y < e->h; y++) for (int x = 0; x < e->w; x++) if (sub_extra_rows[e->rows + y] >> x & 1) n = gt_add(g, n, x, y - e->baseline, 1, 1);
    return gt_edge(font, g, n);
}
static int glyph_disc(int gi, GTexel *g) {
    static Texel cut[MH * MW];
    const SubGlyph *sg = &sub_glyphs[gi]; const SubSlot *gs = &sub_slots[sub_labels[sg->label].slot];
    int m = cut_glyph(gi, pg[sub_labels[sg->label].file], cut), n = 0;
    for (int j = 0; j < m; j++) n = gt_add(g, n, gs->u + cut[j].x - sg->u, gs->v + cut[j].y - sg->baseline, cut[j].t, cut[j].letter);
    return n;
}
/* the base letter of an accented one (0 when it is not one) and its mark (U+0301 acute, U+0303 tilde) */
static unsigned accent_base(unsigned ch, unsigned *mark) {
    static const unsigned short tbl[][3] = { {0xE1, 'a', 0x301}, {0xE9, 'e', 0x301}, {0xED, 'i', 0x301}, {0xF3, 'o', 0x301}, {0xFA, 'u', 0x301},
        {0xC1, 'A', 0x301}, {0xC9, 'E', 0x301}, {0xCD, 'I', 0x301}, {0xD3, 'O', 0x301}, {0xDA, 'U', 0x301}, {0xF1, 'n', 0x303}, {0xD1, 'N', 0x303} };
    for (unsigned i = 0; i < sizeof tbl / sizeof *tbl; i++) if (tbl[i][0] == ch) { *mark = tbl[i][2]; return tbl[i][1]; }
    return 0;
}
/* a character's texels: n (0 when no letter is at hand), its ink width in *w; for the disc's letters the pick index in
   *gi (-1 for ours / accented), so neighbours of one source word keep their step */
static int glyph_for(int font, unsigned ch, GTexel *g, int *w, int *gi) {
    unsigned mark = 0, base = accent_base(ch, &mark);
    int n, ei;
    *gi = -1;
    if (base) {
        int mi = extra_of(font, mark), top = 99, x0 = 99, x1 = -99, dummy;
        if (mi < 0 || !(n = glyph_for(font, base, g, w, &dummy))) return 0;
        if (base == 'i' || base == 'j') {          /* the dot gives way to the mark: keep the biggest ink piece only */
            static unsigned char comp[GMAX]; int best = 0, bestn = 0, nc = 0;
            memset(comp, 0, sizeof comp);
            for (int i = 0; i < n; i++) {
                int stack[GMAX], sp = 0, cnt = 0;
                if (!g[i].letter || comp[i]) continue;
                nc++; comp[i] = (unsigned char)nc; stack[sp++] = i;
                while (sp) {
                    int a = stack[--sp]; cnt++;
                    for (int j = 0; j < n; j++) if (g[j].letter && !comp[j] && abs(g[j].x - g[a].x) <= 1 && abs(g[j].y - g[a].y) <= 1) { comp[j] = (unsigned char)nc; stack[sp++] = j; }
                }
                if (cnt > bestn) { bestn = cnt; best = nc; }
            }
            for (int i = 0; i < n; i++) if (g[i].letter && comp[i] != best) g[i].t = 255;
            for (int i = 0; i < n; i++) if (!g[i].letter) {   /* the dot's edge too */
                int near_dot = 0, near_body = 0;
                for (int j = 0; j < n; j++) if (g[j].letter && abs(g[j].x - g[i].x) <= 1 && abs(g[j].y - g[i].y) <= 1) { if (g[j].t == 255) near_dot = 1; else near_body = 1; }
                if (near_dot && !near_body) g[i].t = 255;
            }
            { int m = 0; for (int i = 0; i < n; i++) if (g[i].t != 255) g[m++] = g[i]; n = m; }
        }
        for (int i = 0; i < n; i++) if (g[i].letter) { if (g[i].y < top) top = g[i].y; if (g[i].x < x0) x0 = g[i].x; if (g[i].x > x1) x1 = g[i].x; }
        {   const SubExtra *e = &sub_extra[mi];
            int mx = x0 + (x1 - x0 + 1 - e->w) / 2 + 1, my = top - 1 - e->h, m = n;
            for (int y = 0; y < e->h; y++) for (int x = 0; x < e->w; x++) if (sub_extra_rows[e->rows + y] >> x & 1 && !gt_has(g, m, mx + x, my + y)) m = gt_add(g, m, mx + x, my + y, 1, 1);
            n = gt_edge(font, g, m);
        }
        return n;
    }
    if (ch < 128 && (*gi = pick(font, (char)ch)) >= 0 && label_ok(sub_glyphs[*gi].label)) { *w = sub_glyphs[*gi].w; return glyph_disc(*gi, g); }
    *gi = -1;
    if ((ei = extra_of(font, ch)) < 0) return 0;
    *w = sub_extra[ei].w;
    return glyph_extra(font, ei, g);
}
static unsigned utf8_next(const char **p) {
    const unsigned char *s = (const unsigned char *)*p; unsigned c = *s;
    if (c < 0x80) { *p += 1; return c; }
    if ((c & 0xE0) == 0xC0 && (s[1] & 0xC0) == 0x80) { *p += 2; return (c & 0x1F) << 6 | (s[1] & 0x3F); }
    if ((c & 0xF0) == 0xE0 && (s[1] & 0xC0) == 0x80 && (s[2] & 0xC0) == 0x80) { *p += 3; return (c & 0x0F) << 12 | (s[1] & 0x3F) << 6 | (s[2] & 0x3F); }
    *p += 1; return '?';
}

/* rewrites the Spanish label `li` in work[] as `text` (UTF-8; '|' breaks the line on the leaf); 0 when a letter is
   missing or the label's source is not the one the table was made from */
static int compose(int li, const char *text) {
    static GTexel gt[16][GMAX]; static int gn[16], gw[16], gix[16], xs[16];
    const SubLabel *l = &sub_labels[li]; const SubSlot *s = &sub_slots[l->slot];
    const int font = l->font;
    unsigned char (*cl)[MW] = clean_group(s->group);
    const char *lines[2] = { text, 0 }; int nl = 1, lo = 255, hi = 0;
    /* the old lines and their edges give way to the picture behind them */
    {   static Texel cut[MH * MW]; static unsigned char old[MH][MW];
        memset(old, 0, sizeof old);
        for (int ln = 0; ln < l->nlines; ln++) for (int k = 0; k < sub_lines[l->line + ln].nglyphs; k++) {
            int gi = sub_lines[l->line + ln].glyph + k, m = cut_glyph(gi, work, cut);
            for (int j = 0; j < m; j++) old[cut[j].y][cut[j].x] = 1;
            if (sub_glyphs[gi].u < lo) lo = sub_glyphs[gi].u;
            if (sub_glyphs[gi].u + sub_glyphs[gi].w > hi) hi = sub_glyphs[gi].u + sub_glyphs[gi].w;
        }
        for (int y = 0; y < s->h; y++) for (int x = 0; x < s->w; x++) if (old[y][x]) set_tx(work, s->u + x, s->v + y, cl[y][x]);
    }
    {   const char *bar = strchr(text, '|'); if (bar && font == SUB_FONT_LEAF) { lines[1] = bar + 1; nl = 2; } }
    for (int ln = 0; ln < nl; ln++) {
        const char *p = lines[ln]; int n = 0, left, baseline;
        while (*p && *p != '|' && n < 16) {
            unsigned ch = utf8_next(&p);
            if (ch == ' ') { if (n) { gn[n] = 0; gw[n] = 4; gix[n] = -1; n++; } continue; }
            if (!(gn[n] = glyph_for(font, ch, gt[n], &gw[n], &gix[n]))) {
                port_printf("[subscreen_es] no letter for U+%04X in the %s font: \"%s\" kept as the disc writes it\n", ch, font == SUB_FONT_TAB ? "tab" : "leaf", text);
                return 0;
            }
            n++;
        }
        if (!n) continue;
        xs[0] = 0;
        for (int i = 1; i < n; i++) {   /* neighbours in their source word keep their step, the rest step by width + the pair gap */
            int adj = gix[i] >= 0 && gix[i - 1] >= 0 && gix[i] == gix[i - 1] + 1 && sub_glyphs[gix[i]].label == sub_glyphs[gix[i - 1]].label && sub_glyphs[gix[i - 1]].advance >= 0;
            xs[i] = xs[i - 1] + (adj ? sub_glyphs[gix[i - 1]].advance : gw[i - 1] + sub_fonts[font].pair_gap + (gix[i - 1] < 0 || gix[i] < 0));
        }
        /* the line's baseline: the label's own when it has that many lines, else the font's usual places */
        if (font == SUB_FONT_TAB) baseline = sub_lines[l->line].baseline;
        else if (l->nlines == nl) baseline = sub_lines[l->line + ln].baseline;
        else baseline = s->v + (nl == 1 ? 25 : ln == 0 ? 19 : 31);
        left = (font == SUB_FONT_TAB || l->nlines != nl ? 2 * s->u + s->w : lo + hi) / 2 - (xs[n - 1] + gw[n - 1]) / 2;
        if (left < s->u) left = s->u;
        for (int pass = 0; pass < 2; pass++) for (int i = 0; i < n; i++) for (int j = 0; j < gn[i]; j++) {
            int u = left + xs[i] + gt[i][j].x, v = baseline + gt[i][j].y;
            if (gt[i][j].letter != pass || u < s->u || v < s->v || u >= s->u + s->w || v >= s->v + s->h) continue;
            if (!pass && is_text(font, tx(work, u, v))) continue;
            set_tx(work, u, v, gt[i][j].t);
        }
    }
    return 1;
}

/* ---- the labels: which slots say what (--subscreen-labels; by default the item tab and the use leaf of the improved
   translation's "Objetos") */
#define NLAB 8
static const struct { const char *key; unsigned char font, u, v; } lab_slots[NLAB] = {
    { "item", SUB_FONT_TAB, 0x98, 0x38 }, { "event", SUB_FONT_TAB, 0x98, 0x50 }, { "status", SUB_FONT_TAB, 0x78, 0x68 },
    { "exit", SUB_FONT_TAB, 0xB8, 0x68 }, { "map", SUB_FONT_TAB, 0x78, 0x80 },
    { "use", SUB_FONT_LEAF, 0x28, 0x40 }, { "view", SUB_FONT_LEAF, 0x08, 0x68 }, { "sort", SUB_FONT_LEAF, 0x40, 0x68 } };
static char lab_text[NLAB][48];
static int lab_custom;
int port_subscreen_labels(const char *spec) {          /* main.c: --subscreen-labels item=Objetos,use=Usar|objeto,... */
    const char *p = spec;
    while (*p) {
        const char *eq = strchr(p, '='), *end = strchr(p, ','); int k;
        if (!end) end = p + strlen(p);
        if (!eq || eq > end) return 0;
        for (k = 0; k < NLAB; k++) if ((int)strlen(lab_slots[k].key) == eq - p && !strncmp(lab_slots[k].key, p, eq - p)) break;
        if (k == NLAB || end - eq - 1 >= (int)sizeof lab_text[k]) return 0;
        memcpy(lab_text[k], eq + 1, end - eq - 1); lab_text[k][end - eq - 1] = 0; lab_custom = 1;
        p = *end ? end + 1 : end;
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
    static int logged, failed, init, li[NLAB], used[NLAB], cached;
    static unsigned char cache[NLAB][MH][32];           /* the rewritten rects, as page bytes */
    int u0 = 256, v0 = 256, u1 = 0, v1 = 0, any = 0;
    if (port_lang != 5 || failed) return;
    if (!init) {
        init = 1;
        if (!lab_custom) { strcpy(lab_text[0], "Objetos"); strcpy(lab_text[5], "Usar|objeto"); }
        for (int k = 0; k < NLAB; k++) {
            li[k] = find_label(ES_FILE, lab_slots[k].font, lab_slots[k].u, lab_slots[k].v);
            used[k] = lab_text[k][0] && li[k] >= 0;
            if (lab_text[k][0] && li[k] < 0) port_printf("[subscreen_es] no Spanish label for %s in the table\n", lab_slots[k].key);
        }
    }
    for (int k = 0; k < NLAB; k++) if (used[k]) {
        const SubSlot *s = &sub_slots[sub_labels[li[k]].slot]; any = 1;
        if (s->u < u0) u0 = s->u; if (s->v < v0) v0 = s->v; if (s->u + s->w > u1) u1 = s->u + s->w; if (s->v + s->h > v1) v1 = s->v + s->h;
    }
    if (!any) { failed = 1; return; }
    if (x > PX + u0 / 4 || x + w < PX + u1 / 4 || y > v0 || y + h < v1) return;   /* the upload must hold every label rect */
    for (int v = 0; v < 256; v++) for (int k = 0; k < 64; k++) {     /* the page as the file has it */
        unsigned short wd = vram[v * 1024 + PX + k];
        work[v * 128 + k * 2] = (unsigned char)wd; work[v * 128 + k * 2 + 1] = (unsigned char)(wd >> 8);
    }
    for (int k = 0; k < NLAB; k++) if (used[k]) {
        const SubSlot *s = &sub_slots[sub_labels[li[k]].slot]; unsigned hs = hash_rect(work, s->u, s->v, s->w, s->h);
        if (getenv("PORT_SUBSCREEN_ES_LOG") && logged++ < 40) port_printf("[subscreen_es] upload %d,%d %dx%d: %s 0x%08x\n", x, y, w, h, lab_slots[k].key, hs);
        if (hs != sub_labels[li[k]].hash) return;                     /* not the page the table knows */
    }
    if (!lab_custom && !es_on()) return;                               /* the default pair follows the pause list's "Objetos" */
    if (!cached) {
        char name[32]; unsigned sz; int ok = 1;
        for (int n = 1; n <= 9; n++) {
            snprintf(name, sizeof name, SUB_PAGE_FILE, n);
            pg[n] = port_cd_read_named(name, &sz);
            if (pg[n] && sz != PAGE_BYTES) { free(pg[n]); pg[n] = 0; }
        }
        for (int k = 0; k < NLAB; k++) if (used[k]) {
            const SubSlot *s = &sub_slots[sub_labels[li[k]].slot];
            if (!compose(li[k], lab_text[k])) { ok = 0; break; }
            for (int v = 0; v < s->h; v++) memcpy(cache[k][v], &work[(s->v + v) * 128 + (s->u >> 1)], s->w / 2);
        }
        for (int n = 1; n <= 9; n++) { free(pg[n]); pg[n] = 0; }
        if (!ok) { port_printf("[subscreen_es] the sub-screen labels are left as the disc writes them\n"); failed = 1; return; }
        cached = 1;
    }
    for (int k = 0; k < NLAB; k++) if (used[k]) {                     /* the words worked out the first time */
        const SubSlot *s = &sub_slots[sub_labels[li[k]].slot];
        for (int v = 0; v < s->h; v++) memcpy(&work[(s->v + v) * 128 + (s->u >> 1)], cache[k][v], s->w / 2);
        put_rect(s->u, s->v, s->w, s->h, write);
    }
}
