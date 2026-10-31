/* Second screen panel (port only, opt-in; docs/PORT.md "Second screen"): a live "map + status" page for dual-screen
 * handhelds (AYN Thor...), drawn by the port into its own RGBA buffer from the game's RAM, the game never pauses.
 *
 *   --second-screen     PC: a second SDL window shows it; Android: the buffer is handed to TombiActivity (JNI,
 *                       registered at run time on the activity's class) which shows it on a Presentation
 *   --dump-panel DIR    writes panel_NNNNNN.png every --dump-every frames (headless checks)
 *
 * What it shows (nothing of the game's graphics; the text and the map graph are read from the user's disc at start):
 *   - status: vitality / max, AP, lives, level, the three exp bars, weapon and pants (item names), play time;
 *   - the area / room name of the game's own map screen (MAP_AREA_NAMES[link.c], MAP_ROOM_NAMES[link.c][link.d]
 *     of the room's MAP_LINKS entry, as subScreenMapDrawAreaName / DrawRoomName do);
 *   - the events started and not completed (SCRIPT_FLAGS 1..0xFE) with their names (EVENT_NAME_STRINGS);
 *   - the map: the game's map graph (MAP_NODES, the routes of MAP_AREA_ROUTES of every region the player has
 *     revealed, D_8009D2C4[region]) seen from above, the room's link highlighted and a marker on it as the pause
 *     map puts SUBSCREEN_MAP_OBJECTS[0] (subScreenMapInitMarker).  The 3D terrain model of the pause map is not drawn.
 * The tables live in the sub-screen overlay: SYSTEM/D0SPSUB.BIN for the graph (MAIN0 addresses), SYSTEM/DnSPSUB.BIN
 * (n = --lang) for the names; the language file is linked elsewhere, so each pointer table is found as the run of
 * overlay pointers of the same length nearest to MAIN0's.  The strings are the sub-screen font's glyph ids
 * (latin-1 + 140 below 0x80, + 74 from 0xC0; -3 line break, -1 end: tools/subscreen_text.py).
 * Off (no flag): nothing here runs, the game's output is unchanged. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "port_runtime.h"
#include "panel_font.h"
#ifdef __ANDROID__
#include <jni.h>
#include <pthread.h>
#include <SDL2/SDL_system.h>
#endif

typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32; typedef short s16; typedef int s32;

int port_panel_on;                 /* --second-screen or --dump-panel */
int port_panel_window;             /* --second-screen (the PC window / the Android Presentation) */
const char *port_panel_dump_dir;   /* --dump-panel DIR */

#define PW_MAX 960
#define PH_MAX 960
static int pw = 640, ph = 480, req_w, req_h;
static u32 pix[PW_MAX * PH_MAX];

/* ---------------------------------------------------------------- game RAM (include/gamestate.h, notes/data_names_main0.csv) */
#define G8(a)  (*(u8 *)PORT_M0(a))
#define G16(a) (*(u16 *)PORT_M0(a))
#define G32(a) (*(u32 *)PORT_M0(a))
#define G_AREA      G16(0x8009C960u)
#define G_SECTION   G16(0x8009C962u)
#define G_TIME      G32(0x8009C968u)
#define G_AP        G32(0x8009C96Cu)
#define G_HEALTH    G8(0x8009C970u)
#define G_MAXHEALTH G8(0x8009C971u)
#define G_LEVEL     G8(0x8009C973u)
#define G_LIVES     G8(0x8009C980u)
#define G_EXPLEV(i) G8(0x8009CD98u + (i))
#define G_EXPBAR(i) G8(0x8009CD9Bu + (i))
#define G_EVENT(i)  G8(0x8009CDA4u + (i))
#define G_WEAPON    G8(0x8009D2B2u)
#define G_PANTS     G8(0x8009D2B3u)
#define G_MAPREG(i) G16(0x8009D2C4u + 2 * (i))
#define EVENTS 0x82

/* in a game: the play time (GAME.totalTimePlayed) runs only in gameplay, and is 0 at the title until a game starts
   or a save is loaded */
static int in_gameplay(void) { return G_TIME != 0; }

/* ---------------------------------------------------------------- disc tables */
#define OVL_BASE 0x800E8028u
static const struct { u8 area, n; } rooms[] = {{0, 6}, {1, 5}, {2, 6}, {3, 6}, {4, 20}, {5, 4}, {6, 3}, {8, 4}, {9, 7},
    {10, 9}, {11, 5}, {13, 2}, {14, 8}, {16, 7}, {17, 11}, {18, 3}, {19, 3}};
static int nsections(int a) { for (unsigned i = 0; i < sizeof rooms / sizeof rooms[0]; i++) if (rooms[i].area == a) return rooms[i].n; return 0; }

typedef struct { s16 x, y, z; } Node;
#define NNODES 203
#define NLINKS 154
static Node nodes[NNODES];
static u8 links[NLINKS][6];
static struct { s16 type, a, b; } routes[12][32]; static int nroutes[12];
static s16 roomlink[20][24];
static int loaded;
static void load_tables(void);
static int nb_lo = 9999, nb_hi = -9999, nz_lo = 9999, nz_hi = -9999;

/* names (latin-1, malloc'd; NULL = unknown) */
static char *ev_name[EVENTS], *area_name[12], *room_name[12][24], *a14_area[24], *a14_room[24], *item_name[256];

static u32 rd32(const u8 *d, unsigned o) { return d[o] | d[o + 1] << 8 | d[o + 2] << 16 | (u32)d[o + 3] << 24; }
static s16 rd16(const u8 *d, unsigned o) { return (s16)(d[o] | d[o + 1] << 8); }

typedef struct { const u8 *d; unsigned n; u32 base; } Ovl;
static int in_ovl(const Ovl *o, u32 a) { return a >= o->base && a < o->base + o->n; }
static char *ovl_string(const Ovl *o, u32 a) {
    char b[128]; int k = 0;
    if (!in_ovl(o, a)) return NULL;
    for (unsigned p = a - o->base; p + 2 <= o->n && k < 120; p += 2) {
        int g = rd16(o->d, p), c;
        if (g == -1) break;
        if (g == -3) { if (k && b[k - 1] != ' ') b[k++] = ' '; continue; }
        if (g <= 0) continue;
        c = g - 140 < 0x80 ? g - 140 : g - 74;
        if (c >= 32 && c < 256) b[k++] = (char)c;
    }
    while (k && b[k - 1] == ' ') k--;
    b[k] = 0;
    for (int i = 0; i < k; i++) if (b[i] != ' ') { char *s = malloc(k + 1); memcpy(s, b, k + 1); return s; }
    return NULL;
}
/* the run of overlay pointers containing file offset o (word aligned) */
static int ptr_run(const Ovl *v, unsigned o, unsigned *start) {
    unsigned s = o, e = o;
    if (o + 4 > v->n || !in_ovl(v, rd32(v->d, o))) return 0;
    while (s >= 4 && in_ovl(v, rd32(v->d, s - 4))) s -= 4;
    while (e + 4 <= v->n && in_ovl(v, rd32(v->d, e))) e += 4;
    *start = s;
    return (int)((e - s) / 4);
}
/* MAIN0 address T of a pointer table in D0SPSUB -> the same table's address in the language's file (0: not found) */
static u32 map_table(const Ovl *o0, const Ovl *on, u32 T) {
    unsigned s0; int len = ptr_run(o0, T - o0->base, &s0);
    if (!len) return 0;
    if (on == o0) return T;
    long best = -1, bd = 1L << 30;
    for (unsigned p = 0; p + 4 <= on->n;) {
        if (!in_ovl(on, rd32(on->d, p))) { p += 4; continue; }
        unsigned s; int l = ptr_run(on, p, &s);
        if (l == len) { long dd = labs((long)s - (long)s0); if (dd < bd) { bd = dd; best = s; } }
        p = s + 4 * (unsigned)l;
    }
    return best < 0 ? 0 : on->base + (u32)best + (T - o0->base - s0);
}

void port_panel_init(void) {
    if (loaded) return;
    load_tables();
}
static void load_tables(void) {
    char name[32]; unsigned n0, nn = 0; u8 *d0, *dn = NULL;
    loaded = -1;
    d0 = port_cd_read_named("SYSTEM/D0SPSUB.BIN", &n0);
    if (!d0 || n0 < 0x22200) { port_printf("[panel] SYSTEM/D0SPSUB.BIN not readable (%p, %u bytes): no names, no map\n", (void *)d0, n0); free(d0); return; }
    if (port_lang > 0 && port_lang < 9) {
        snprintf(name, sizeof name, "SYSTEM/D%dSPSUB.BIN", port_lang);
        dn = port_cd_read_named(name, &nn);
    }
    Ovl o0 = { d0, n0, OVL_BASE }, on = o0;
    if (dn && nn > 8) { on.d = dn; on.n = nn; on.base = OVL_BASE + rd32(dn, 4) - rd32(d0, 4); }
#define A0(a) ((a) - OVL_BASE)
    /* the map graph (MAIN0 layout) */
    for (int i = 0; i < NNODES; i++) { nodes[i].x = rd16(d0, A0(0x800FD418u) + 8 * i); nodes[i].y = rd16(d0, A0(0x800FD41Au) + 8 * i); nodes[i].z = rd16(d0, A0(0x800FD41Cu) + 8 * i); }
    memcpy(links, d0 + A0(0x800FDAB4u), sizeof links);
    for (int r = 0; r < 12; r++) {
        u32 e = rd32(d0, A0(0x800FDA70u) + 4 * r);
        nroutes[r] = 0;
        if (!in_ovl(&o0, e)) continue;
        for (unsigned p = e - OVL_BASE; p + 8 <= n0 && rd16(d0, p) != -1 && nroutes[r] < 32; p += 8) {
            int k = nroutes[r]++;
            routes[r][k].type = rd16(d0, p); routes[r][k].a = rd16(d0, p + 2); routes[r][k].b = rd16(d0, p + 4);
        }
    }
    for (int a = 0; a < 20; a++) {
        u32 p = rd32(d0, A0(0x800FE064u) + 4 * a); int ns = nsections(a);
        for (int s = 0; s < 24; s++) roomlink[a][s] = -1;
        if (!in_ovl(&o0, p)) continue;
        for (int s = 0; s < ns && s < 24; s++) { int l = rd16(d0, p - OVL_BASE + 2 * s); roomlink[a][s] = l >= 0 && l < NLINKS ? l : -1; }
    }
    for (int r = 0; r < 12; r++) for (int k = 0; k < nroutes[r]; k++) {
        int ids[2] = { routes[r][k].a, routes[r][k].b };
        for (int j = 0; j < (routes[r][k].type ? 2 : 1); j++) {
            if (ids[j] < 0 || ids[j] >= NNODES) continue;
            Node *q = &nodes[ids[j]];
            if (q->x < nb_lo) nb_lo = q->x; if (q->x > nb_hi) nb_hi = q->x;
            if (q->z < nz_lo) nz_lo = q->z; if (q->z > nz_hi) nz_hi = q->z;
        }
    }
    /* the names (the language's file) */
    u32 t;
#define RD(a) rd32(on.d, (a) - on.base)
    if ((t = map_table(&o0, &on, 0x801093C4u))) for (int e = 0; e < EVENTS; e++) ev_name[e] = ovl_string(&on, RD(t + 4 * e));
    if ((t = map_table(&o0, &on, 0x80108BE4u))) for (int i = 0; i < 12; i++) area_name[i] = ovl_string(&on, RD(t + 4 * i));
    if ((t = map_table(&o0, &on, 0x80108B78u))) for (int i = 0; i < 24; i++) if (in_ovl(&on, RD(t + 4 * i))) a14_area[i] = ovl_string(&on, RD(t + 4 * i));
    if ((t = map_table(&o0, &on, 0x80108DDCu))) for (int i = 0; i < 24; i++) if (in_ovl(&on, RD(t + 4 * i))) a14_room[i] = ovl_string(&on, RD(t + 4 * i));
    if ((t = map_table(&o0, &on, 0x80108F8Cu))) for (int i = 0; i < 12; i++) {
        u32 r = RD(t + 4 * i);
        for (int j = 0; j < 24 && in_ovl(&on, r) && in_ovl(&on, r + 4 * j + 3); j++) {
            u32 sp = RD(r + 4 * j);
            if (!in_ovl(&on, sp)) break;
            room_name[i][j] = ovl_string(&on, sp);
        }
    }
    if ((t = map_table(&o0, &on, 0x8010979Cu))) {
        u32 idx = t - (0x8010979Cu - 0x801096D4u);   /* ITEM_NAME_INDEX (u8[item]) right before the table */
        if (in_ovl(&on, idx) && in_ovl(&on, idx + 199))
            for (int i = 0; i < 200; i++) item_name[i] = ovl_string(&on, RD(t + 4 * on.d[idx - on.base + i]));
    }
    port_printf("[panel] tables read (%s): %d map nodes, event 3 \"%s\", area 0 \"%s\"\n", dn ? name : "SYSTEM/D0SPSUB.BIN", NNODES,
                ev_name[3] ? ev_name[3] : "?", area_name[0] ? area_name[0] : "?");
    free(d0); free(dn);
    loaded = 1;
}

/* ---------------------------------------------------------------- drawing into pix (0xAABBGGRR in memory = R,G,B,A bytes) */
#define RGB(r, g, b) (0xFF000000u | (u32)(b) << 16 | (u32)(g) << 8 | (u32)(r))
static const u32 C_BG = RGB(0xF4, 0xEC, 0xD4), C_CARD = RGB(0xFF, 0xF8, 0xE6), C_GREEN = RGB(0x2F, 0x6B, 0x3A),
    C_DGREEN = RGB(0x1F, 0x4A, 0x28), C_PINK = RGB(0xFF, 0x6A, 0xA8), C_INK = RGB(0x2A, 0x2A, 0x22), C_MUTED = RGB(0x7A, 0x74, 0x60),
    C_LINE = RGB(0xDD, 0xD2, 0xB4), C_CREAM = RGB(0xFF, 0xF8, 0xE6), C_ROUTE = RGB(0x5E, 0x8F, 0x5E), C_RED = RGB(0xE0, 0x4A, 0x3C),
    C_BLUE = RGB(0x3A, 0x7B, 0xD5), C_LGREEN = RGB(0x4C, 0xB0, 0x50);

static inline void blend(int x, int y, u32 c, int a) {   /* a 0..255 */
    if ((unsigned)x >= (unsigned)pw || (unsigned)y >= (unsigned)ph || a <= 0) return;
    u32 *p = &pix[y * pw + x];
    if (a >= 255) { *p = c; return; }
    u32 d = *p;
    u32 r = ((c & 0xFF) * a + (d & 0xFF) * (255 - a)) / 255, g = ((c >> 8 & 0xFF) * a + (d >> 8 & 0xFF) * (255 - a)) / 255,
        b = ((c >> 16 & 0xFF) * a + (d >> 16 & 0xFF) * (255 - a)) / 255;
    *p = 0xFF000000u | b << 16 | g << 8 | r;
}
static void rect(int x, int y, int w, int h, u32 c) {
    for (int j = y < 0 ? 0 : y; j < y + h && j < ph; j++) for (int i = x < 0 ? 0 : x; i < x + w && i < pw; i++) pix[j * pw + i] = c;
}
static void rrect(int x, int y, int w, int h, int r, u32 c) {   /* rounded, anti-aliased corners */
    for (int j = 0; j < h; j++) for (int i = 0; i < w; i++) {
        int cx = i < r ? r - i : i >= w - r ? i - (w - r - 1) : 0, cy = j < r ? r - j : j >= h - r ? j - (h - r - 1) : 0;
        if (cx && cy) {
            float dx = cx - 0.5f, dy = cy - 0.5f, d2 = dx * dx + dy * dy, R = (float)r;
            if (d2 > R * R) { float e = (R + 1) * (R + 1); if (d2 < e) blend(x + i, y + j, c, (int)(255 * (e - d2) / (e - R * R))); continue; }
        }
        blend(x + i, y + j, c, 255);
    }
}
static void disc(float cx, float cy, float r, u32 c) {
    for (int j = (int)(cy - r - 1); j <= (int)(cy + r + 1); j++) for (int i = (int)(cx - r - 1); i <= (int)(cx + r + 1); i++) {
        float dx = i + 0.5f - cx, dy = j + 0.5f - cy, d = dx * dx + dy * dy;
        if (d <= (r - 0.5f) * (r - 0.5f)) blend(i, j, c, 255);
        else if (d < (r + 0.5f) * (r + 0.5f)) { float k = r + 0.5f - __builtin_sqrtf(d); blend(i, j, c, (int)(k * 255)); }
    }
}
static void thick_line(float x0, float y0, float x1, float y1, float w, u32 c) {   /* a capsule, anti-aliased */
    float minx = (x0 < x1 ? x0 : x1) - w, maxx = (x0 > x1 ? x0 : x1) + w, miny = (y0 < y1 ? y0 : y1) - w, maxy = (y0 > y1 ? y0 : y1) + w;
    float dx = x1 - x0, dy = y1 - y0, L = dx * dx + dy * dy, r = w / 2;
    for (int j = (int)miny; j <= (int)maxy; j++) for (int i = (int)minx; i <= (int)maxx; i++) {
        float px = i + 0.5f - x0, py = j + 0.5f - y0, t = L > 0 ? (px * dx + py * dy) / L : 0;
        if (t < 0) t = 0; if (t > 1) t = 1;
        float ex = px - t * dx, ey = py - t * dy, d = __builtin_sqrtf(ex * ex + ey * ey);
        if (d <= r - 0.5f) blend(i, j, c, 255); else if (d < r + 0.5f) blend(i, j, c, (int)((r + 0.5f - d) * 255));
    }
}
static int text_w(const PanelFont *f, const char *s) {
    int w = 0;
    for (; *s; s++) { int c = (u8)*s; if (c < 32) continue; w += f->g[c - 32].adv; }
    return w;
}
static int text(const PanelFont *f, int x, int y, const char *s, u32 col) {   /* y = top of the line; returns the end x */
    for (; *s; s++) {
        int c = (u8)*s; if (c < 32) continue;
        const PanelGlyph *g = &f->g[c - 32];
        const u8 *b = f->bits + g->off;
        for (int j = 0; j < g->h; j++) for (int i = 0; i < g->w; i++) {
            int k = j * g->w + i, v = (b[k >> 1] >> ((k & 1) * 4)) & 15;
            if (v) blend(x + g->ox + i, y + g->oy + j, col, v * 17);
        }
        x += g->adv;
    }
    return x;
}
/* text cut with "..." to fit w */
static void text_fit(const PanelFont *f, int x, int y, int w, const char *s, u32 col) {
    char b[160]; int n = (int)strlen(s);
    if (n > 150) n = 150;
    memcpy(b, s, n); b[n] = 0;
    if (text_w(f, b) <= w) { text(f, x, y, b, col); return; }
    while (n > 0) { b[--n] = 0; while (n > 0 && b[n - 1] == ' ') b[--n] = 0; char t[164]; snprintf(t, sizeof t, "%s...", b); if (text_w(f, t) <= w) { text(f, x, y, t, col); return; } }
}
static void heart(float cx, float cy, float s, u32 c) {
    disc(cx - s * 0.5f, cy - s * 0.15f, s * 0.55f, c);
    disc(cx + s * 0.5f, cy - s * 0.15f, s * 0.55f, c);
    for (int j = 0; j < (int)(s * 1.3f); j++) {
        float hw = s * 1.05f * (1 - (float)j / (s * 1.3f));
        for (int i = (int)(cx - hw); i <= (int)(cx + hw); i++) blend(i, (int)(cy + j), c, 255);
    }
}
static void bar(int x, int y, int w, int h, int v, int max, u32 c) {
    rrect(x, y, w, h, h / 2, C_LINE);
    int f = max > 0 ? w * (v > max ? max : v) / max : 0;
    if (f >= h) rrect(x, y, f, h, h / 2, c); else if (f > 0) disc(x + h / 2.0f, y + h / 2.0f, h / 2.0f, c);
}

/* ---------------------------------------------------------------- the panel */
static int es(void) { return port_lang == 5; }
#define T(en, sp) (es() ? sp : en)

static void card(int x, int y, int w, int h, const char *title) {
    rrect(x + 1, y + 2, w, h, 10, RGB(0xDD, 0xD2, 0xB4));   /* shadow */
    rrect(x, y, w, h, 10, C_CARD);
    if (title) {
        text(&pf_small, x + 12, y + 8, title, C_GREEN);
        rect(x + 12, y + 26, w - 24, 1, C_LINE);
    }
}

static void draw_map(int x, int y, int w, int h, int link) {
    card(x, y, w, h, T("MAP", "MAPA"));
    int mx = x + 14, my = y + 34, mw = w - 28, mh = h - 46;
    if (loaded != 1 || nb_hi <= nb_lo || nz_hi <= nz_lo) { text(&pf_small, mx, my + 4, T("No map data", "Sin datos del mapa"), C_MUTED); return; }
    int cur_reg = link >= 0 ? links[link][2] : -1;
    /* the view: the routes shown (revealed regions and the room's), at least 160 units wide */
    int x0 = 9999, x1 = -9999, z0 = 9999, z1 = -9999;
    for (int r = 0; r < 12; r++) if (G_MAPREG(r) || r == cur_reg) for (int k = 0; k < nroutes[r]; k++) {
        int ids[2] = { routes[r][k].a, routes[r][k].b };
        for (int j = 0; j < (routes[r][k].type ? 2 : 1); j++) if (ids[j] >= 0 && ids[j] < NNODES) {
            Node *q = &nodes[ids[j]];
            if (q->x < x0) x0 = q->x; if (q->x > x1) x1 = q->x; if (q->z < z0) z0 = q->z; if (q->z > z1) z1 = q->z;
        }
    }
    if (x1 < x0) { x0 = nb_lo; x1 = nb_hi; z0 = nz_lo; z1 = nz_hi; }
    if (x1 - x0 < 160) { int c = (x0 + x1) / 2; x0 = c - 80; x1 = c + 80; }
    if (z1 - z0 < 120) { int c = (z0 + z1) / 2; z0 = c - 60; z1 = c + 60; }
    mx += 12; my += 12; mw -= 24; mh -= 24;
    float sx = (float)mw / (x1 - x0), sz = (float)mh / (z1 - z0), s = sx < sz ? sx : sz;
    float ox = mx + (mw - (x1 - x0) * s) / 2, oy = my + (mh - (z1 - z0) * s) / 2;
#define PX(n) (ox + (nodes[n].x - x0) * s)
#define PY(n) (oy + (nodes[n].z - z0) * s)
    for (int pass = 0; pass < 2; pass++) for (int r = 0; r < 12; r++) {
        int shown = G_MAPREG(r) != 0 || r == cur_reg;
        if (!shown) continue;
        for (int k = 0; k < nroutes[r]; k++) {
            int a = routes[r][k].a, b = routes[r][k].b;
            if (a < 0 || a >= NNODES) continue;
            if (routes[r][k].type == 1 && b >= 0 && b < NNODES) {
                if (pass == 0) thick_line(PX(a), PY(a), PX(b), PY(b), 7, C_DGREEN);
                else thick_line(PX(a), PY(a), PX(b), PY(b), 4, C_ROUTE);
            } else if (routes[r][k].type == 0 && pass == 1) { disc(PX(a), PY(a), 4.5f, C_DGREEN); disc(PX(a), PY(a), 2.5f, C_CREAM); }
        }
    }
    if (link >= 0) {
        int a = links[link][0], b = links[link][1];
        if (a < NNODES && b < NNODES) {
            thick_line(PX(a), PY(a), PX(b), PY(b), 6, C_PINK);
            float cx = (PX(a) + PX(b)) / 2, cy = (PY(a) + PY(b)) / 2;
            int pulse = (port_frame / 8) & 3;
            disc(cx, cy, 10.0f + pulse, RGB(0xFF, 0xC4, 0xDC));
            disc(cx, cy, 8, C_PINK);
            disc(cx, cy, 3.5f, C_CREAM);
        }
    }
}

static int fmt_ap(char *b, int n, u32 v) {   /* 1.234.567 / 1,234,567 */
    char t[16]; int k = snprintf(t, sizeof t, "%u", v), o = 0;
    for (int i = 0; i < k && o < n - 2; i++) { if (i && (k - i) % 3 == 0) b[o++] = es() ? '.' : ','; b[o++] = t[i]; }
    b[o] = 0; return o;
}
static const char *weapon_name(void) {
    static const struct { u8 w, item; } t[] = {{0, 27}, {5, 21}, {6, 22}, {7, 23}, {8, 33}, {9, 34}};   /* subScreenDrawItemGrid's checks */
    static char b[16];
    for (unsigned i = 0; i < sizeof t / sizeof t[0]; i++) if (t[i].w == G_WEAPON && item_name[t[i].item]) return item_name[t[i].item];
    snprintf(b, sizeof b, "#%d", G_WEAPON); return b;
}
static const char *pants_name(void) {
    static const u8 t[4] = {32, 29, 24, 28};
    static char b[16];
    if ((G_PANTS & 3) == G_PANTS && item_name[t[G_PANTS]]) return item_name[t[G_PANTS]];
    snprintf(b, sizeof b, "#%d", G_PANTS); return b;
}

static int status_a(int lx, int vx, int ly) {   /* vitality, AP, lives, level; returns the next y */
    char b[64]; int row = 25;
    text(&pf_body, lx, ly, T("Vitality", "Vitalidad"), C_MUTED);
    snprintf(b, sizeof b, "%d/%d", G_HEALTH, G_MAXHEALTH);
    text(&pf_bold, vx - text_w(&pf_bold, b), ly - 1, b, C_INK);
    {
        int mh = G_MAXHEALTH > 16 ? 16 : G_MAXHEALTH, per = 22;
        for (int i = 0; i < mh; i++) heart(lx + 8 + (i % 8) * per, ly + 28 + (i / 8) * 20, 7, i < G_HEALTH ? C_PINK : C_LINE);
        ly += mh > 8 ? 62 : 44;
    }
    text(&pf_body, lx, ly, "AP", C_MUTED); fmt_ap(b, sizeof b, G_AP); text(&pf_bold, vx - text_w(&pf_bold, b), ly - 1, b, C_INK); ly += row;
    text(&pf_body, lx, ly, T("Lives", "Vidas"), C_MUTED); snprintf(b, sizeof b, "%d", G_LIVES); text(&pf_bold, vx - text_w(&pf_bold, b), ly - 1, b, C_INK); ly += row;
    text(&pf_body, lx, ly, T("Level", "Nivel"), C_MUTED); snprintf(b, sizeof b, "%d", G_LEVEL); text(&pf_bold, vx - text_w(&pf_bold, b), ly - 1, b, C_INK); ly += row;
    return ly;
}
static void status_b(int lx, int vx, int ly, int bottom) {   /* the exp bars, weapon, pants */
    char b[16]; u32 col[3] = { C_RED, C_LGREEN, C_BLUE };
    for (int i = 0; i < 3; i++) {   /* red / green / blue: the level and its bar (0..255) */
        snprintf(b, sizeof b, "%d", G_EXPLEV(i));
        text(&pf_small, lx, ly + i * 18, b, col[i]);
        bar(lx + 26, ly + 4 + i * 18, vx - lx - 26, 9, G_EXPBAR(i), 255, col[i]);
    }
    ly += 3 * 18 + 8;
    text(&pf_small, lx, ly, T("WEAPON", "ARMA"), C_MUTED); text_fit(&pf_bold, lx, ly + 16, vx - lx, weapon_name(), C_INK); ly += 44;
    if (ly + 40 <= bottom) { text(&pf_small, lx, ly, T("PANTS", "PANTAL\xD3N"), C_MUTED); text_fit(&pf_bold, lx, ly + 16, vx - lx, pants_name(), C_INK); }
}
static void draw_status(int x, int y, int w, int h) {
    card(x, y, w, h, T("STATUS", "ESTADO"));
    if (w >= 400) {   /* two columns */
        int cw = (w - 42) / 2;
        status_a(x + 14, x + 14 + cw, y + 36);
        status_b(x + 28 + cw, x + w - 14, y + 38, y + h);
    } else {
        int ly = status_a(x + 14, x + w - 14, y + 36) + 2;
        rect(x + 14, ly - 4, w - 28, 1, C_LINE);
        status_b(x + 14, x + w - 14, ly + 4, y + h);
    }
}

static void draw_events(int x, int y, int w, int h) {
    char b[64]; int ids[EVENTS], n = 0, done = 0;
    for (int e = 0; e < EVENTS; e++) { int v = G_EVENT(e); if (v == 0xFF) done++; else if (v) ids[n++] = e; }
    snprintf(b, sizeof b, T("EVENTS IN PROGRESS (%d)", "EVENTOS EN CURSO (%d)"), n);
    card(x, y, w, h, b);
    {   int tw = text_w(&pf_small, b) + 24;
        snprintf(b, sizeof b, T("%d done", "%d completados"), done);
        if (tw + text_w(&pf_small, b) + 12 < w) text(&pf_small, x + w - 12 - text_w(&pf_small, b), y + 8, b, C_MUTED); }
    int ly = y + 34, lh = 23, max = (h - 40) / lh;
    if (!n) { text(&pf_body, x + 14, ly, T("None right now", "Ninguno ahora mismo"), C_MUTED); return; }
    for (int i = 0; i < n && i < max; i++) {
        if (i == max - 1 && n > max) { snprintf(b, sizeof b, T("+ %d more", "+ %d m\xE1s"), n - i); text(&pf_body, x + 30, ly, b, C_MUTED); break; }
        disc(x + 19, ly + 10, 4, C_PINK);
        char t[160];
        if (ev_name[ids[i]]) snprintf(t, sizeof t, "%s", ev_name[ids[i]]); else snprintf(t, sizeof t, T("Event %d", "Evento %d"), ids[i]);
        text_fit(&pf_body, x + 30, ly, w - 44, t, C_INK);
        ly += lh;
    }
}

static void render(void) {
    char b[96];
    if (req_w > 0 && req_h > 0) {   /* the display's aspect (Android: the Presentation's display) */
        if (req_w >= req_h) { pw = 640; ph = 640 * req_h / req_w; if (ph < 400) ph = 400; if (ph > 640) ph = 640; }
        else { pw = 540; ph = 540 * req_h / req_w; if (ph > 900) ph = 900; if (ph < 600) ph = 600; }
    }
    rect(0, 0, pw, ph, C_BG);
    int play = in_gameplay() && G_AREA < 20;
    int area = G_AREA, sec = G_SECTION, link = play && area < 20 && sec < 24 ? roomlink[area][sec] : -1;
    /* header: the map screen's area and room names */
    int hh = 62;
    rect(0, 0, pw, hh, C_GREEN);
    rect(0, hh, pw, 3, C_PINK);
    const char *an = NULL, *rn = NULL;
    if (play) {
        if (area == 14) { an = sec + 14 < 24 ? a14_area[sec + 14] : NULL; rn = sec < 24 ? a14_room[sec] : NULL; }
        else if (link >= 0) { int c = links[link][2], d = links[link][3]; an = c < 12 ? area_name[c] : NULL; rn = c < 12 && d < 24 ? room_name[c][d] : NULL; }
    }
    char an_b[48], rn_b[48];
    if (!play) { an = "Tombi!"; rn = T("Waiting for the game...", "Esperando a la partida..."); }
    if (!an) { snprintf(an_b, sizeof an_b, T("Area %d", "\xC1rea %d"), area); an = an_b; }
    if (!rn) { snprintf(rn_b, sizeof rn_b, T("Section %d", "Secci\xF3n %d"), sec); rn = rn_b; }
    {
        u32 t = G_TIME / 50;
        snprintf(b, sizeof b, "%u:%02u:%02u", t / 3600, t / 60 % 60, t % 60);
        int tw = play ? text_w(&pf_bold, b) : 0;
        if (play) { text(&pf_small, pw - 14 - text_w(&pf_small, T("TIME", "TIEMPO")), 10, T("TIME", "TIEMPO"), RGB(0xBF, 0xD8, 0xB8)); text(&pf_bold, pw - 14 - tw, 30, b, C_CREAM); }
        text_fit(&pf_big, 16, 6, pw - 40 - tw, an, C_CREAM);
        text_fit(&pf_body, 16, 38, pw - 40 - tw, rn, RGB(0xD6, 0xE8, 0xD0));
    }
    if (!play) { draw_map(16, hh + 16, pw - 32, ph - hh - 32, -1); return; }
    int g = 12, top = hh + 3 + g;
    if (pw > ph) {      /* landscape: status left, map and events right */
        int lw = 236, rx = g + lw + g, rw = pw - rx - g, mh = (ph - top - g) * 54 / 100;
        draw_status(g, top, lw, ph - top - g);
        draw_map(rx, top, rw, mh, link);
        draw_events(rx, top + mh + g, rw, ph - top - mh - 2 * g);
    } else {            /* portrait: map, then status and events side by side */
        int sh = 214, mh = (ph - top - sh - 3 * g) * 60 / 100, sy = top + mh + g, ey = sy + sh + g;
        draw_map(g, top, pw - 2 * g, mh, link);
        draw_status(g, sy, pw - 2 * g, sh);
        draw_events(g, ey, pw - 2 * g, ph - ey - g);
    }
}

/* ---------------------------------------------------------------- outputs */
extern int port_gpu_write_png(const char *path, const unsigned char *rgb, int w, int h);   /* shims/gpu.c */
extern int port_dump_every;
extern void port_mkdir(const char *dir);                                                  /* shims/gpu.c */
void port_panel_window_frame(const unsigned char *rgba, int w, int h);                   /* runtime/sdl.c */
#ifdef __ANDROID__
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static u32 out[PW_MAX * PH_MAX]; static int out_w, out_h, out_seq, registered;
static jint JNICALL n_frame(JNIEnv *env, jclass c, jobject buf, jintArray dims, jint seq) {
    (void)c; jint r = 0;
    pthread_mutex_lock(&mtx);
    if (out_seq != seq && out_w) {
        void *p = (*env)->GetDirectBufferAddress(env, buf); jlong cap = (*env)->GetDirectBufferCapacity(env, buf);
        if (p && cap >= (jlong)out_w * out_h * 4) {
            memcpy(p, out, (size_t)out_w * out_h * 4);
            jint d[2] = { out_w, out_h }; (*env)->SetIntArrayRegion(env, dims, 0, 2, d);
            r = out_seq;
        }
    }
    pthread_mutex_unlock(&mtx);
    return r;
}
static void JNICALL n_layout(JNIEnv *env, jclass c, jint w, jint h) { (void)env; (void)c; req_w = w; req_h = h; }
static void android_register(void) {
    JNIEnv *env = (JNIEnv *)SDL_AndroidGetJNIEnv(); jobject act = (jobject)SDL_AndroidGetActivity();
    registered = 1;
    if (!env || !act) return;
    jclass cls = (*env)->GetObjectClass(env, act);
    JNINativeMethod m[] = { { "nativePanelFrame", "(Ljava/nio/ByteBuffer;[II)I", (void *)n_frame }, { "nativePanelLayout", "(II)V", (void *)n_layout } };
    if ((*env)->RegisterNatives(env, cls, m, 2) != 0) { (*env)->ExceptionClear(env); port_printf("[panel] RegisterNatives failed\n"); }
    else port_printf("[panel] JNI ready\n");
    (*env)->DeleteLocalRef(env, cls); (*env)->DeleteLocalRef(env, act);
}
#endif

static int frames;
/* every vblank (shims/gpu.c port_gpu_vblank) while the panel is on */
void port_panel_vblank(void) {
    int dump = port_panel_dump_dir && port_dump_every > 0 && port_frame % port_dump_every == 0;
    if (!dump && (frames++ & 1)) return;   /* ~25 fps is plenty */
    render();
#ifdef __ANDROID__
    if (port_panel_window) {
        if (!registered) android_register();
        pthread_mutex_lock(&mtx);
        memcpy(out, pix, (size_t)pw * ph * 4); out_w = pw; out_h = ph; if (++out_seq == 0) out_seq = 1;
        pthread_mutex_unlock(&mtx);
    }
#else
    if (port_panel_window) port_panel_window_frame((const unsigned char *)pix, pw, ph);
#endif
    if (dump) {
        static u8 rgb[PW_MAX * PH_MAX * 3]; char path[512];
        for (int i = 0; i < pw * ph; i++) { u32 c = pix[i]; rgb[3 * i] = c & 0xFF; rgb[3 * i + 1] = c >> 8 & 0xFF; rgb[3 * i + 2] = c >> 16 & 0xFF; }
        port_mkdir(port_panel_dump_dir);
        snprintf(path, sizeof path, "%s/panel_%06d.png", port_panel_dump_dir, port_frame);
        if (port_gpu_write_png(path, rgb, pw, ph)) port_printf("[panel] cannot write %s\n", path);
    }
}
/* --panel-size WxH (testing the portrait layout on the PC) */
void port_panel_size(int w, int h) { req_w = w; req_h = h; }
