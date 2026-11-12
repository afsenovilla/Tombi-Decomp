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
#include <ctype.h>
#include "port_runtime.h"
#include "panel_assets.h"
#include "panel_addrs.h"
#ifdef __ANDROID__
#include <jni.h>
#include <pthread.h>
#include <SDL2/SDL_system.h>
#endif

typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32; typedef short s16; typedef int s32;

int port_panel_on;                 /* --second-screen or --dump-panel */
int port_panel_window;             /* --second-screen (the PC window / the Android Presentation) */
const char *port_panel_dump_dir;   /* --dump-panel DIR */

/* the canvas is in game pixels (about 414 x 360), shown scaled by an integer k (nearest neighbour) to the display */
#define CW_MAX 640
#define CH_MAX 640
#define OUT_MAX 1920
static int pw, ph, pk = 1, req_w, req_h;   /* pw x ph = the canvas x pk */
static u32 *pix;                           /* malloc'd: the scaled frame (PC window, PNG dumps) */

/* ---------------------------------------------------------------- game RAM (include/gamestate.h, notes/data_names_main0.csv) */
#define G8(a)  (*(u8 *)PORT_M0(a))
#define G16(a) (*(u16 *)PORT_M0(a))
#define G32(a) (*(u32 *)PORT_M0(a))
#define G_AREA      G16(PV_GAME_AREA)
#define G_SECTION   G16(PV_GAME_SECTION)
#define G_TIME      G32(PV_GAME_TIME)
#define G_AP        G32(PV_GAME_AP)
#define G_HEALTH    G8(PV_GAME_HEALTH)
#define G_MAXHEALTH G8(PV_GAME_MAXHEALTH)
#define G_LEVEL     G8(PV_GAME_LEVEL)
#define G_LIVES     G8(PV_GAME_LIVES)
#define G_EXPLEV(i) G8(PV_GCD_EXPLEV + (i))
#define G_EXPBAR(i) G8(PV_GCD_EXPBAR + (i))
#define G_EVENT(i)  G8(PV_SCRIPT_FLAGS + (i))
#define G_WEAPON    G8(PV_GAME_WEAPON)
#define G_PANTS     G8(PV_GAME_PANTS)
#define G_MAPREG(i) G16(PV_GAME_MAPREG + 2 * (i))
/* event ids: the pause menu's Events list (subScreenDrawEventList) shows SCRIPT_FLAGS[order[i] - 1] for the ids of
   SUBSCREEN_EVENT_ORDER (D0SPSUB 0x801086F0, 256 bytes, 0xFF = none), which go up to 0xBF, past the 130 of the save
   docs: the titles table EVENT_NAME_STRINGS has 195 entries and the event icons 192 */
#define EVENTS 0xC0

/* in a game: the play time (GAME.totalTimePlayed) runs only in gameplay, and is 0 at the title until a game starts
   or a save is loaded */
static int in_gameplay(void) { return G_TIME != 0; }

/* ---------------------------------------------------------------- disc tables */
#define OVL_BASE PORT_OVL_BASE
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
static u8 ev_order[EVENTS]; static int ev_norder;   /* the menu's event ids in its order (from SUBSCREEN_EVENT_ORDER) */
static char *ev_name[EVENTS], *area_name[12], *room_name[12][24], *a14_area[24], *a14_room[24], *item_name[256];
/* 1 when the sub-screen tables come from the improved Spanish translation (its event 16 is "La caja AP", the disc's "La caja
   PA"): applied in memory (--text-patch, translation.tpatch) or written into data/ by the PC launcher, the panel reads
   them through port_cd_read_named like the game; this only switches the names the panel writes itself (panel_givers.h) */
static int es_improved;

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
#if PORT_VERSION_USA
        /* USA: ASCII - 32 up to '?' (0 = space), ASCII - 33 from 'A' (no '@'): English only */
        if (g < 0) continue;
        c = g < 32 ? g + 32 : g + 33;
#else
        if (g <= 0) continue;
        /* Latin-1 + 140 below 0x7F, + 74 from 267 = A-acute (not DEL: "Area Cerdiablo uno", "Arbol del Saber" lost their A); the
           opening marks and the ordinal a sit past the Latin-1 range (tools/subscreen_text.py) */
        c = g == 332 ? 0xA1 : g == 333 ? 0xAA : g == 337 ? 0xBF : g < 267 ? g - 140 : g - 74;
#endif
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
    pa_init();
}
static void load_tables(void) {
    char name[32]; unsigned n0, nn = 0; u8 *d0, *dn = NULL;
    loaded = -1;
    d0 = port_cd_read_named(PA_F_SUB, &n0);
    if (!d0 || n0 < 0x22200) { port_printf("[panel] %s not readable (%p, %u bytes): no names, no map\n", PA_F_SUB, (void *)d0, n0); free(d0); return; }
    if (port_lang > 0 && port_lang < 9) {
        snprintf(name, sizeof name, "SYSTEM/D%dSPSUB.BIN", port_lang);
        dn = port_cd_read_named(name, &nn);
    }
    Ovl o0 = { d0, n0, OVL_BASE }, on = o0;
    if (dn && nn > 8) { on.d = dn; on.n = nn; on.base = OVL_BASE + rd32(dn, 4) - rd32(d0, 4); }
#define A0(a) ((a) - OVL_BASE)
    /* the map graph (MAIN0 layout) */
    for (int i = 0; i < NNODES; i++) { nodes[i].x = rd16(d0, A0(PV_MAP_NODES) + 8 * i); nodes[i].y = rd16(d0, A0(PV_MAP_NODES) + 8 * i + 2); nodes[i].z = rd16(d0, A0(PV_MAP_NODES) + 8 * i + 4); }
    memcpy(links, d0 + A0(PV_MAP_LINKS), sizeof links);
    for (int r = 0; r < 12; r++) {
        u32 e = rd32(d0, A0(PV_MAP_AREA_ROUTES) + 4 * r);
        nroutes[r] = 0;
        if (!in_ovl(&o0, e)) continue;
        for (unsigned p = e - OVL_BASE; p + 8 <= n0 && rd16(d0, p) != -1 && nroutes[r] < 32; p += 8) {
            int k = nroutes[r]++;
            routes[r][k].type = rd16(d0, p); routes[r][k].a = rd16(d0, p + 2); routes[r][k].b = rd16(d0, p + 4);
        }
    }
    for (int a = 0; a < 20; a++) {
        u32 p = rd32(d0, A0(PV_MAP_ROOM_LINK) + 4 * a); int ns = nsections(a);
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
    /* the event ids of the pause menu, in its order */
    ev_norder = 0;
    {   static u8 seen[EVENTS];
        for (int i = 0; i < 256; i++) {
            int v = d0[A0(PV_EVENT_ORDER) + i];
            if (v == 0xFF || v == 0 || v - 1 >= EVENTS || seen[v - 1]) continue;
            seen[v - 1] = 1; ev_order[ev_norder++] = (u8)(v - 1);
        }
    }
    /* the names (the language's file) */
    u32 t;
#define RD(a) rd32(on.d, (a) - on.base)
    if ((t = map_table(&o0, &on, PV_EVENT_NAMES))) for (int e = 0; e < EVENTS; e++) ev_name[e] = ovl_string(&on, RD(t + 4 * e));
    if ((t = map_table(&o0, &on, PV_AREA_NAMES))) for (int i = 0; i < 12; i++) area_name[i] = ovl_string(&on, RD(t + 4 * i));
    if ((t = map_table(&o0, &on, PV_A14_AREA_NAMES))) for (int i = 0; i < 24; i++) if (in_ovl(&on, RD(t + 4 * i))) a14_area[i] = ovl_string(&on, RD(t + 4 * i));
    if ((t = map_table(&o0, &on, PV_A14_ROOM_NAMES))) for (int i = 0; i < 24; i++) if (in_ovl(&on, RD(t + 4 * i))) a14_room[i] = ovl_string(&on, RD(t + 4 * i));
    if ((t = map_table(&o0, &on, PV_ROOM_NAMES))) for (int i = 0; i < 12; i++) {
        u32 r = RD(t + 4 * i);
        for (int j = 0; j < 24 && in_ovl(&on, r) && in_ovl(&on, r + 4 * j + 3); j++) {
            u32 sp = RD(r + 4 * j);
            if (!in_ovl(&on, sp)) break;
            room_name[i][j] = ovl_string(&on, sp);
        }
    }
    if ((t = map_table(&o0, &on, PV_ITEM_NAMES))) {
        u32 idx = t - (PV_ITEM_NAMES - PV_ITEM_NAME_INDEX);   /* ITEM_NAME_INDEX (u8[item]) right before the table */
        if (in_ovl(&on, idx) && in_ovl(&on, idx + 199))
            for (int i = 0; i < 200; i++) item_name[i] = ovl_string(&on, RD(t + 4 * on.d[idx - on.base + i]));
    }
    es_improved = ev_name[16] && !strcmp(ev_name[16], "La caja AP");
    port_printf("[panel] tables read (%s%s): %d map nodes, event 3 \"%s\", area 0 \"%s\"\n", dn ? name : PA_F_SUB,
                es_improved ? ", improved Spanish translation" : "", NNODES, ev_name[3] ? ev_name[3] : "?", area_name[0] ? area_name[0] : "?");
    free(d0); free(dn);
    loaded = 1;
}

/* ---------------------------------------------------------------- drawing (panel_assets.c: canvas in game pixels, scaled x k) */
#define RGB(r, g, b) (0xFF000000u | (u32)(b) << 16 | (u32)(g) << 8 | (u32)(r))
static const u32 C_PINK = RGB(248, 200, 192), C_DARK = RGB(40, 24, 16), C_EDGE = RGB(170, 130, 110), C_RED = RGB(176, 40, 30),
    C_REDE = RGB(200, 150, 130), C_WHITE = RGB(250, 246, 230), C_WOODE = RGB(120, 70, 40), C_GREEN = RGB(30, 110, 40),
    C_MUTED = RGB(130, 90, 80), C_SEA = RGB(120, 176, 232), C_SEA2 = RGB(150, 200, 244), C_SEL = RGB(36, 88, 36),
    C_MUTED8 = RGB(88, 56, 46), C_RED8 = RGB(136, 24, 16), C_ORANGE = RGB(168, 44, 20), C_YELLOW = RGB(255, 224, 96);
/* contrast (WCAG) on the cream window (255, 238, 222): C_DARK 15.1, C_MUTED 5.3 / C_GREEN 5.6 / C_RED 5.9 / C_ORANGE 6.1
   (16 px only), C_MUTED8 9.2 / C_RED8 8.5 (8 px); C_WHITE on C_SEL 7.8; light text over wood, the side map and the AP
   badge carries the game's 1 px C_DARK outline (cream 15.8, yellow 13.1 against it) */

/* the sub-screen's pieces: page (64, 0) = SYSTEM/A10301.000, CLUT rows of D0SPSUB's block at (288, 480) */
#define SUB 64, 0
static void stone(int x, int y, int w, int h) { pa_tile(x, y, w, h, SUB, 152, 0, 80, 56, 288, 480); }
static void window_(int x, int y, int w, int h) { pa_nine(x, y, w, h, SUB, 0, 216, 248, 32, 8, 288, 487, 0); }   /* cream window */
static void frame_(int x, int y, int w, int h) { pa_nine(x, y, w, h, SUB, 0, 216, 248, 32, 8, 288, 487, 1); }    /* its rim only */
static void plank(int x, int y, int w) {   /* blank wood plank, 24 high, with its end grain */
    pa_tile(x + 4, y, w - 8, 24, SUB, 193, 128, 31, 24, 288, 481);
    pa_spr(x, y, SUB, 184, 128, 4, 24, 288, 481, 0);
    pa_spr(x + w - 9, y, SUB, 224, 128, 9, 24, 288, 481, 0);
}
static void ctext(int cx, int y, int h, const char *s, u32 b, u32 e) { pa_text(cx - pa_text_w(h, s) / 2, y, h, s, b, e); }
static void plank_text(int cx, int y, const char *s, int sel) { pa_text_ol(cx - pa_text_w(16, s) / 2, y, 16, s, sel ? C_YELLOW : C_WHITE, C_WOODE, C_DARK); }

/* the HUD's pieces: page (320, 256) = SYSTEM/A00000.GAM, CLUT rows at (368, 480..) */
#define HUD 320, 256
static const u8 fruit_digit_u[10] = { 0, 8, 24, 40, 56, 72, 88, 104, 120, 0 };
/* the number in the fruit (drawHudVitality's frame table): 0..7 in a row at v 120, 8 alone at (120, 128), 9..15 in a row
   at v 136 and 16 alone at (112, 144), 16 wide each (two digits from 10 on); u, v of hp's cell, -1 when out of range */
static int fruit_digit(int hp, int *u, int *v) {
    if (hp < 0 || hp > 16) return -1;
    if (hp <= 7) { *u = fruit_digit_u[hp]; *v = 120; } else if (hp == 8) { *u = 120; *v = 128; } else if (hp == 16) { *u = 112; *v = 144; } else { *u = 16 * (hp - 9); *v = 136; }
    return 0;
}
/* the segments' CLUT row as drawHudVitality picks it: by the vitality (red when low), grey when empty */
static int fruit_clut(int i, int hp) { return i >= hp ? 504 : hp < 3 ? 503 : hp < 5 ? 502 : hp < 7 ? 501 : 500; }
static void fruit(int x, int y, int hp, int max) {   /* the vitality fruit: 4 segments a side, the number in the middle */
    static const u8 seg[4][4] = { {16, 152, 22, 33}, {32, 152, 14, 26}, {48, 152, 14, 13}, {64, 152, 22, 6} };
    if (max > 8) max = 8;
    for (int i = 0; i < max; i++) {
        int k = i & 3, right = i >= 4, sx = right ? 72 - seg[k][2] - 16 : seg[k][2];
        pa_spr(x + sx, y + seg[k][3], HUD, seg[k][0], seg[k][1], 16, 16, 368, fruit_clut(i, hp), right ? PA_FLIPX : 0);
    }
    pa_spr(x + 24, y + 16, HUD, 144, 200, 24, 24, 368, 483, 0);
    { int u, v; if (!fruit_digit(hp, &u, &v)) pa_spr(x + 28, y + 19, HUD, u, v, 16, 16, 368, 489, 0); }
}
static void lives(int x, int y, int stored) {   /* drawHudLives shows GAME.playerLives - 1 (one or two digits) */
    int n = stored - 1;
    pa_spr(x, y, HUD, 128, 56, 32, 24, 368, 481, 0);
    if (n >= 10 && n <= 99) { pa_spr(x + 7, y + 12, HUD, 8 * (n / 10), 64, 8, 8, 368, 481, 0); pa_spr(x + 17, y + 12, HUD, 8 * (n % 10), 64, 8, 8, 368, 481, 0); }
    else if (n >= 0 && n <= 9) pa_spr(x + 12, y + 12, HUD, 8 * n, 64, 8, 8, 368, 481, 0);
}
static void ap_badge(int x, int y) {   /* the HUD's "AP" box (drawn, its letters in the game's 8 px font) */
    pa_fill(x + 1, y, 28, 14, RGB(200, 176, 64)); pa_fill(x, y + 1, 30, 12, RGB(200, 176, 64));
    pa_fill(x + 1, y + 1, 28, 12, RGB(96, 96, 140)); pa_fill(x + 2, y + 2, 26, 4, RGB(130, 130, 176));
    pa_text_ol(x + 15 - pa_text_w(8, "AP") / 2, y + 3, 8, "AP", C_WHITE, RGB(40, 30, 70), C_DARK);
}
/* Tomba's face: the sub-screen sprite has a green box (CLUT 482 indices 2, 3, 5, 6); the map marker drops it */
static void face_f(int x, int y, int f) { pa_cut_mask = 1u << 2 | 1u << 3 | 1u << 5 | 1u << 6; pa_drop_mask = 1u << 3 | 1u << 6; pa_spr(x, y, SUB, 8, 8, 24, 24, 288, 482, PA_CUTBOX | PA_OUTLINE | f); }
static void face(int x, int y) { face_f(x, y, 0); }
/* map markers: drawn at an integer scale sc (1..3: every texel an sc x sc block of canvas pixels, the canvas itself shown
   at an integer scale), never resampled */
static int mk_flag(int sc) { return sc >= 3 ? PA_X3 : sc == 2 ? PA_X2 : 0; }
static int mk_scale(int view_h) { return view_h >= 200 ? 2 : 1; }   /* 2x in the big map views, 1x in the small ones */
static void face_mark(int cx, int by, int sc, int f) { face_f(cx - 12 * sc, by - 22 * sc, mk_flag(sc) | f); }   /* bottom centre at (cx, by) */
static void event_mark(int cx, int by, int e, int sc, int f) {   /* the event's icon, outlined, bottom centre at (cx, by) */
    int w = pa_event_icon_w(e), h = pa_event_icon_h(e);
    if (w <= 0) w = 16; if (h <= 0) h = 16;
    pa_event_icon(cx - w * sc / 2, by - h * sc, e, PA_OUTLINE | mk_flag(sc) | f);
}

static void line(int x0, int y0, int x1, int y1, int t, u32 c) {   /* pixel-art line, t x t brush */
    int dx = x1 > x0 ? x1 - x0 : x0 - x1, dy = y1 > y0 ? y0 - y1 : y1 - y0, sx = x0 < x1 ? 1 : -1, sy = y0 < y1 ? 1 : -1, e = dx + dy;
    for (int n = 0; n < 4096; n++) {
        pa_fill(x0 - t / 2, y0 - t / 2, t, t, c);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * e;
        if (e2 >= dy) { e += dy; x0 += sx; }
        if (e2 <= dx) { e += dx; y0 += sy; }
    }
}
static void dot(int cx, int cy, int r, u32 c) {
    for (int j = -r; j <= r; j++) for (int i = -r; i <= r; i++) if (i * i + j * j <= r * r + r) pa_put(cx + i, cy + j, i * i + j * j >= r * r - r ? RGB(90, 20, 20) : c);
}
static void page_events(int play);
static void page_items(int play);
static const char *map_tab_name(void);   /* panel_side.inc: "Mapa" / "Cenital" by section */

/* ---------------------------------------------------------------- the panel */
static int es(void) { return port_lang == 5; }
#define T(en, sp) (es() ? sp : en)
#if PORT_VERSION_USA
#define PANEL_HERO "Tomba"   /* the English chrome names him as the version does */
#else
#define PANEL_HERO "Tombi"
#endif
enum { PAGE_MAP, PAGE_EVENTS, PAGE_SIDE, PAGE_ITEMS, NPAGES };
static int page;
/* --panel-no-map (an Old 3DS's default: the area map's tiles are more than it has time for): no map tab, and no
   "show on the map" under an event; the status, events and items tabs stay */
int port_panel_no_map;
static const char *page_name(int p) {
    return p == PAGE_MAP ? T("Status", "Estado") : p == PAGE_EVENTS ? T("Events", "Eventos") : p == PAGE_ITEMS ? T("Items", "Objetos") : map_tab_name();
}

/* touch / mouse targets registered while drawing (canvas coordinates) */
enum { HS_PAGE = 0x100, HS_FILTER = 0x200, HS_ROW = 0x300, HS_BACK = 0x400, HS_PREV = 0x500, HS_NEXT = 0x600 };
static struct { short x, y, w, h; int id; } hs[64]; static int nhs;
static void hot(int x, int y, int w, int h, int id) { if (nhs < 64) { hs[nhs].x = (short)x; hs[nhs].y = (short)y; hs[nhs].w = (short)w; hs[nhs].h = (short)h; hs[nhs].id = id; nhs++; } }

/* ---------------------------------------------------------------- the one scroller of every long list (the events
   lists, the items grid, the warp destinations): a vertical offset in canvas pixels that follows the finger pixel for
   pixel, flings on release with inertia (speed measured over the frames drawn, friction per vblank: the same feel at
   the panel's 30 fps or 60 fps), clamped at both ends (no overscroll), a thin indicator.  A touch moves it only once
   it has gone SCR_SLOP pixels (a tap still selects / opens, a long press still pins); a touch on a moving list only
   stops it.  The D-pad moves the selection and scr_show scrolls just enough to keep it visible.  Each list calls
   scr_view while drawing (that registers its viewport for the touches of the next frames). */
#define SCR_SLOP 8
#define SCR_FRICTION .95f   /* speed kept per vblank (1/60 s) */
typedef struct { float off, vel, prev, off0; int drag, ty0, stop, vx, vy, vw, vh, total, frame; } Scroll;
static Scroll *scr_live[4]; static int nscr_live;   /* the lists drawn in the last render */
static Scroll *scr_cur;                             /* the one the finger is on */
static float scr_max(const Scroll *s) { return s->total > s->vh ? (float)(s->total - s->vh) : 0; }
static void scr_clamp(Scroll *s) {
    float mx = scr_max(s);
    if (s->off > mx) { s->off = mx; s->vel = 0; }
    if (s->off < 0) { s->off = 0; s->vel = 0; }
}
static void scr_reset(Scroll *s) { s->off = s->vel = s->prev = 0; s->drag = -1; }
/* while drawing: the viewport and the content's height; steps the fling by the vblanks since the last frame drawn */
static void scr_view(Scroll *s, int vx, int vy, int vw, int vh, int total) {
    int n = port_frame - s->frame;
    if (n < 1 || n > 6) n = s->frame ? 1 : 0;
    s->frame = port_frame; s->vx = vx; s->vy = vy; s->vw = vw; s->vh = vh; s->total = total;
    if (s->drag == 1) { if (n) s->vel = s->vel * .4f + (s->off - s->prev) / n * .6f; }   /* px per vblank, smoothed */
    else if (s->drag < 0 || !s->drag)
        for (int i = 0; i < n && s->vel; i++) { s->off += s->vel; s->vel *= SCR_FRICTION; if (s->vel > -.08f && s->vel < .08f) s->vel = 0; }
    scr_clamp(s);
    s->prev = s->off;
    if (nscr_live < 4) scr_live[nscr_live++] = s;
}
/* the content rows y0..y1 in view (the D-pad's selection) */
static void scr_show(Scroll *s, int y0, int y1) {
    if (y1 - y0 > s->vh || y0 < s->off) s->off = (float)y0;
    else if (y1 > s->off + s->vh) s->off = (float)(y1 - s->vh);
    s->vel = 0; scr_clamp(s); s->prev = s->off;
}
static int scr_y(const Scroll *s) { return (int)(s->off + .5f); }   /* whole canvas pixels for drawing */
static void scr_bar(const Scroll *s, int x) {   /* the thin indicator at x (3 px), only when the content overflows */
    if (s->total <= s->vh) return;
    int bh = s->vh * s->vh / s->total; if (bh < 16) bh = 16;
    float mx = scr_max(s);
    pa_fill(x - 1, s->vy - 1, 5, s->vh + 2, RGB(40, 24, 16));
    pa_fill(x, s->vy, 3, s->vh, RGB(110, 84, 70));
    pa_fill(x, s->vy + (int)((s->vh - bh) * (mx > 0 ? s->off / mx : 0)), 3, bh, RGB(255, 224, 96));
}
static int scr_busy(void) { for (int i = 0; i < nscr_live; i++) if (scr_live[i]->drag == 1 || scr_live[i]->vel) return 1; return 0; }
/* a touch (0 down, 1 up, 2 move) in canvas pixels; 1: it was a drag (or stopped a fling), not a tap */
static int scr_touch(int x, int y, int action) {
    if (action == 0) {
        scr_cur = NULL;
        for (int i = 0; i < nscr_live; i++) {
            Scroll *s = scr_live[i];
            if (x < s->vx || y < s->vy || x >= s->vx + s->vw || y >= s->vy + s->vh) continue;
            scr_cur = s; s->drag = 0; s->ty0 = y; s->off0 = s->off;
            s->stop = s->vel > 1 || s->vel < -1; s->vel = 0;   /* a touch stops a fling (and is not a tap) */
            break;
        }
        return 0;
    }
    Scroll *s = scr_cur;
    if (!s) return 0;
    if (action == 2) {
        if (!s->drag && abs(y - s->ty0) > SCR_SLOP && s->total > s->vh) { s->drag = 1; s->ty0 = y; s->off0 = s->off; s->prev = s->off; }
        if (s->drag == 1) { s->off = s->off0 - (y - s->ty0); scr_clamp(s); return 1; }
        return 0;
    }
    int was = s->drag == 1 || s->stop;
    if (s->drag != 1) s->vel = 0;
    s->drag = -1; s->stop = 0; scr_cur = NULL;
    return was;
}

/* the header (stone, title plank, tabs, the pink page) is the same picture frame after frame: drawn once per title /
   tab names / canvas size and copied afterwards (it was the dearest part of a frame: ~2.5 ms of pa_spr on the PC) */
static void header_draw(const char *title);
static void header(const char *title) {
    static u32 *hc; static char hk[256]; static int hn; static struct { short x, y, w, h; int id; } hhs[64];
    char k[256];
    snprintf(k, sizeof k, "%d %d %d %d %d|%s|%s|%s|%s|%s", pa_cw, pa_ch, page, pa_ok, pa_font_ok, title, page_name(0), page_name(1), page_name(2), page_name(3));
    if (hc && !strcmp(k, hk)) { memcpy(pa_cv, hc, (size_t)pa_cw * pa_ch * 4); memcpy(hs, hhs, sizeof hhs); nhs = hn; return; }
    header_draw(title);
    if (!hc) hc = malloc(CW_MAX * CH_MAX * 4);
    if (!hc) return;
    memcpy(hc, pa_cv, (size_t)pa_cw * pa_ch * 4); memcpy(hhs, hs, sizeof hhs); hn = nhs; strcpy(hk, k);
}
static void header_draw(const char *title) {
    int W = pa_cw;
    stone(0, 0, W, pa_ch);
    plank(0, 0, W);
    pa_spr(2, 0, SUB, 96, 48, 24, 24, 288, 481, 0);
    pa_spr(W - 26, 0, SUB, 96, 72, 24, 24, 288, 481, 0);
    pa_outline = C_DARK;
    pa_text_fit(W / 2 - pa_text_w(16, title) / 2 < 30 ? 30 : W / 2 - pa_text_w(16, title) / 2, 4, 16, W - 60, title, C_WHITE, C_WOODE);
    pa_outline = 0;
    int n = NPAGES - (port_panel_no_map ? 1 : 0), tw = (W - 16 * (n + 1)) / n;
    for (int i = 0, c = 0; i < NPAGES; i++) {
        if (port_panel_no_map && i == PAGE_SIDE) continue;
        int x = 16 + c++ * (tw + 16);
        plank(x, 26, tw); hot(x, 26, tw, 24, HS_PAGE + i);
        plank_text(x + tw / 2, 30, page_name(i), i == page);
    }
    pa_fill(4, 52, W - 8, pa_ch - 56, C_PINK);
}

static int map_mark_link = -1, map_mark_ev = -1;   /* the event browser's marker (a room link) */
static int map_ev_marks;                            /* the Status page: mark the rooms of the started / completed events */
static int ev_link_state(int link);                 /* panel_events.inc: 0 none, 1 an event in progress there, 2 all completed */
static void draw_map(int x, int y, int w, int h, int link) {
    window_(x, y, w, h);
    int mx = x + 7, my = y + 7, mw = w - 14, mh = h - 14;
    for (int j = 0; j < mh; j++) pa_fill(mx, my + j, mw, 1, ((j / 3) & 1) ? C_SEA : C_SEA2);
    if (loaded != 1 || nb_hi <= nb_lo || nz_hi <= nz_lo) { pa_text(mx + 6, my + 6, 16, T("No map data", "Sin datos del mapa"), C_DARK, C_EDGE); return; }
    int cur_reg = link >= 0 ? links[link][2] : -1;
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
    int ix = mx + 12, iy = my + 12, iw = mw - 24, ih = mh - 24;
    float sx = (float)iw / (x1 - x0), sz = (float)ih / (z1 - z0), s = sx < sz ? sx : sz;
    float ox = ix + (iw - (x1 - x0) * s) / 2, oy = iy + (ih - (z1 - z0) * s) / 2;
    int cx0 = pa_clip_x0, cy0 = pa_clip_y0, cx1 = pa_clip_x1, cy1 = pa_clip_y1;
    pa_clip_x0 = mx; pa_clip_y0 = my; pa_clip_x1 = mx + mw; pa_clip_y1 = my + mh;
#define PX(n) ((int)(ox + (nodes[n].x - x0) * s))
#define PY(n) ((int)(oy + (nodes[n].z - z0) * s))
    int sc = mk_scale(mh), lo = 2, li = 2;   /* routes: a white core 2 px wide in a dark edge (4 px) */
    for (int pass = 0; pass < 3; pass++) for (int r = 0; r < 12; r++) {
        if (!(G_MAPREG(r) != 0 || r == cur_reg)) continue;
        for (int k = 0; k < nroutes[r]; k++) {
            int a = routes[r][k].a, b = routes[r][k].b;
            if (a < 0 || a >= NNODES) continue;
            if (routes[r][k].type == 1 && b >= 0 && b < NNODES && pass < 2) line(PX(a), PY(a), PX(b), PY(b), pass ? li : lo + li, pass ? C_WHITE : RGB(40, 60, 90));
            else if (routes[r][k].type == 0 && pass == 2) dot(PX(a), PY(a), 3 + sc, C_RED);
        }
    }
    if (map_ev_marks) for (int l = 0; l < NLINKS; l++) {   /* a dot per room with events: orange in progress, green completed */
        int st = ev_link_state(l), a = links[l][0], b = links[l][1];
        if (!st || a >= NNODES || b >= NNODES || !(G_MAPREG(links[l][2]) || links[l][2] == cur_reg)) continue;
        int cx = (PX(a) + PX(b)) / 2, cy = (PY(a) + PY(b)) / 2, r = 2 + sc;
        pa_fill(cx - r - 1, cy - r - 1, 2 * r + 3, 2 * r + 3, RGB(40, 30, 20));
        pa_fill(cx - r, cy - r, 2 * r + 1, 2 * r + 1, st == 1 ? RGB(255, 150, 30) : RGB(90, 210, 80));
    }
    if (link >= 0) {
        int a = links[link][0], b = links[link][1];
        if (a < NNODES && b < NNODES) {
            line(PX(a), PY(a), PX(b), PY(b), lo + li, RGB(120, 30, 70)); line(PX(a), PY(a), PX(b), PY(b), lo, RGB(255, 96, 160));
            int cx = (PX(a) + PX(b)) / 2, cy = (PY(a) + PY(b)) / 2, bob = ((port_frame / 10) & 1) * sc;
            face_mark(cx, (cy - 2 * sc < my + 23 * sc ? my + 23 * sc : cy - 2 * sc) - bob, sc, 0);   /* kept inside the map */
            pa_fill(cx - sc, cy - sc, 2 * sc + 1, 2 * sc + 1, RGB(120, 30, 70)); pa_fill(cx - sc + 1, cy - sc + 1, 2 * sc - 1, 2 * sc - 1, RGB(255, 96, 160));
        }
    }
    if (map_mark_link >= 0 && map_mark_link < NLINKS) {
        int a = links[map_mark_link][0], b = links[map_mark_link][1];
        if (a < NNODES && b < NNODES) {
            int cx = (PX(a) + PX(b)) / 2, cy = (PY(a) + PY(b)) / 2;
            line(PX(a), PY(a), PX(b), PY(b), lo + li, RGB(110, 80, 10)); line(PX(a), PY(a), PX(b), PY(b), lo, RGB(255, 220, 64));
            event_mark(cx, cy - 3 * sc < my + 18 * sc ? my + 18 * sc : cy - 3 * sc, map_mark_ev, sc, 0);
            for (int i = 0; i < 3 * sc; i++) pa_fill(cx - 3 * sc + i, cy - 3 * sc + i, 6 * sc + 1 - 2 * i, 1, C_RED);
        }
    }
    pa_clip_x0 = cx0; pa_clip_y0 = cy0; pa_clip_x1 = cx1; pa_clip_y1 = cy1;
}

static int fmt_ap(char *b, int n, u32 v) {   /* 1.234.567 / 1,234,567 */
    char t[16]; int k = snprintf(t, sizeof t, "%u", v), o = 0;
    for (int i = 0; i < k && o < n - 2; i++) { if (i && (k - i) % 3 == 0) b[o++] = es() ? '.' : ','; b[o++] = t[i]; }
    b[o] = 0; return o;
}
static int count_events(int *started, int *done) {
    int s = 0, d = 0;
    /* as subScreenDrawEventCounters: every SCRIPT_FLAGS byte but 0x12 */
    for (int e = 0; e < 0x100; e++) { int v = G_EVENT(e); if (e == 0x12) continue; if (v == 0xFF) d++; else if (v) s++; }
    if (started) *started = s; if (done) *done = d;
    return s + d;
}
static const char *ev_title(int e, char *b, int n) {
    if (e >= 0 && e < EVENTS && ev_name[e]) return ev_name[e];
    snprintf(b, n, T("Event %d", "Evento %d"), e); return b;
}
/* the k-th event of the menu's order (all ids when D0SPSUB could not be read) */
static int ev_count(void) { return ev_norder ? ev_norder : EVENTS; }
static int ev_id(int k) { return ev_norder ? ev_order[k] : k; }
static int ev_hidden_now(int e);   /* panel_events.inc: hidden by the player (panel_hidden.txt) */
static int ev_pins(int *ids);      /* panel_events.inc: the pinned events (panel_pins.txt), at most 4 */
static void page_status(void);   /* panel_status.inc: the 2 x 2 dashboard */
static void page_map(int play, int link) {
    (void)link;
    if (play) { page_status(); return; }
    window_(8, 56, pa_cw - 16, pa_ch - 64);
    pa_text(20, 70, 16, T("Waiting for the game...", "Esperando a la partida..."), C_MUTED, C_EDGE);
}

static void room_names(int play, int area, int sec, int link, const char **an, const char **rn) {
    *an = *rn = NULL;
    if (!play) return;
    if (area == 14) { *an = sec + 14 < 24 ? a14_area[sec + 14] : NULL; *rn = sec < 24 ? a14_room[sec] : NULL; }
    else if (link >= 0) { int c = links[link][2], d = links[link][3]; *an = c < 12 ? area_name[c] : NULL; *rn = c < 12 && d < 24 ? room_name[c][d] : NULL; }
}

/* word-wrapped text: lines of at most w pixels broken at spaces (a word longer than w is split), at most maxl lines
   (the last one shrinks to fit only when the text runs past it); centred when center.  Returns the lines drawn
   (wrap_dry: counted, nothing drawn). */
static int wrap_dry;
static int text_wrap(int x, int y, int h, int w, int lh, int maxl, int center, const char *s, unsigned body, unsigned edge) {
    char ln[256]; int n = 0;
    while (*s && n < maxl) {
        while (*s == ' ') s++;
        int len = 0, brk = 0, lw = 0;
        for (int i = 0; s[i] && s[i] != '\n' && i < 255; i++) {
            char c1[2] = { s[i], 0 };
            if ((lw += pa_text_w(h, c1)) > w) break;   /* the glyph widths add up (no kerning) */
            len = i + 1; if (s[i + 1] == ' ' || !s[i + 1] || s[i + 1] == '\n') brk = len;
        }
        if (!len) len = 1;
        int take = brk && s[len] && s[len] != '\n' ? brk : len;
        if (n == maxl - 1 && s[take] && s[take] != '\n') take = (int)strlen(s) < 255 ? (int)strlen(s) : 255;   /* last line: all of the rest */
        memcpy(ln, s, take); ln[take] = 0;
        int tw = pa_text_w(h, ln); if (tw > w) tw = w;
        if (!wrap_dry) pa_text_fit(center ? x + (w - tw) / 2 : x, y + n * lh, h, w, ln, body, edge);
        s += take; if (*s == '\n') s++;
        n++;
    }
    return n;
}
/* the lines text_wrap needs for s in w pixels (at most maxl) */
static int text_lines(int h, int w, int maxl, const char *s) {
    wrap_dry = 1; int n = text_wrap(0, 0, h, w, 0, maxl, 0, s, 0, 0); wrap_dry = 0;
    return n ? n : 1;
}
#include "panel_events.inc"
#include "panel_side.inc"
#include "panel_items.inc"
#include "panel_status.inc"

static void layout(void);
int panel_loading_screen(void);   /* runtime/panel_hud.c: the game's NOW LOADING until gameplay */
static void render(void) {
    layout();
    if (panel_loading_screen()) { nhs = 0; return; }
    int play = in_gameplay() && G_AREA < 20;
    int area = G_AREA, sec = G_SECTION, link = play && area < 20 && sec < 24 ? roomlink[area][sec] : -1;
    const char *an, *rn; char ab[48];
    room_names(play, area, sec, link, &an, &rn);
    if (!play) an = T("Waiting for the game...", "Esperando a la partida...");
    else if (!an) { snprintf(ab, sizeof ab, T("Area %d", "\xC1rea %d"), area); an = ab; }
    pa_clip_x0 = pa_clip_y0 = 0; pa_clip_x1 = pa_cw; pa_clip_y1 = pa_ch; nhs = 0; nscr_live = 0;
    header(page == PAGE_MAP ? an : page_name(page));
    if (page == PAGE_MAP) page_map(play, link);
    else if (page == PAGE_EVENTS) page_events(play);
    else if (page == PAGE_ITEMS) page_items(play);
    else page_side(play);
}

/* canvas size and scale for the display (req_w x req_h; default 828 x 720): k = about display / 400, canvas = display / k */
static void layout(void) {
    int W = req_w > 0 ? req_w : 828, H = req_h > 0 ? req_h : 720;
    int k = (int)((W < H ? W / 360.0 : H / 300.0) + 0.25);
    if (W >= H) { int k2 = (int)(W / 400.0 + 0.25); if (k2 < k) k = k2; }
    if (k < 1) k = 1;
    int cw = W / k, ch = H / k;
    while ((cw > CW_MAX || ch > CH_MAX || cw * k > OUT_MAX || ch * k > OUT_MAX) && k < 16) { k++; cw = W / k; ch = H / k; }
    if (cw > CW_MAX) cw = CW_MAX; if (ch > CH_MAX) ch = CH_MAX;
    if (cw < 320) cw = 320; if (ch < 240) ch = 240;
    if (!pa_cv) pa_cv = malloc(CW_MAX * CH_MAX * 4);
    if (!pix) pix = malloc((size_t)OUT_MAX * OUT_MAX * 4);
    pa_cw = cw; pa_ch = ch; pk = k; pw = cw * k; ph = ch * k;
    if (pw > OUT_MAX || ph > OUT_MAX) { pk = 1; pw = cw; ph = ch; }
}
static void upscale(void) {
    for (int y = 0; y < pa_ch; y++) {
        const u32 *s = pa_cv + y * pa_cw;
        u32 *d = pix + (size_t)y * pk * pw;
        for (int x = 0; x < pa_cw; x++) for (int i = 0; i < pk; i++) d[x * pk + i] = s[x];
        for (int j = 1; j < pk; j++) memcpy(d + (size_t)j * pw, d, (size_t)pw * 4);
    }
}

/* ---------------------------------------------------------------- outputs */
extern int port_gpu_write_png(const char *path, const unsigned char *rgb, int w, int h);   /* shims/gpu.c */
extern int port_dump_every;
extern void port_mkdir(const char *dir);                                                  /* shims/gpu.c */
void port_panel_window_frame(const unsigned char *rgba, int w, int h);                   /* runtime/sdl.c */
static void inq_push(int kind, int a, int x, int y);
/* a pinch (kind 2): f = the fingers' distance now / before, (x, y) its midpoint in canvas pixels (-1: the view's centre) */
static void port_panel_pinch_canvas(float f, int x, int y) { if (f > .2f && f < 5) inq_push(2, (int)(f * 10000 + .5f), x, y); }
#ifdef __ANDROID__
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static u32 out[CW_MAX * CH_MAX]; static int out_w, out_h, out_seq, registered;
static jint JNICALL n_frame(JNIEnv *env, jclass c, jobject buf, jintArray dims, jint seq) {
    (void)c; jint r = 0;
    { extern int port_panel_presented; port_panel_presented = port_frame; }   /* the Presentation is polling (panel_hud.c) */
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
static void JNICALL n_touch(JNIEnv *env, jclass c, jint x, jint y, jint action) { (void)env; (void)c; inq_push(1, action, x, y); }   /* canvas pixels */
static void JNICALL n_pinch(JNIEnv *env, jclass c, jfloat f, jint x, jint y) { (void)env; (void)c; port_panel_pinch_canvas(f, x, y); }   /* two fingers: distance ratio, midpoint (canvas) */
static void JNICALL n_key(JNIEnv *env, jclass c, jint a) { (void)env; (void)c; if (a > 0 && a < 16) inq_push(0, a, 0, 0); }   /* IN_*: a controller button the launcher bound to a panel action */
static void android_register_h(void *u) {
    (void)u;
    JNIEnv *env = (JNIEnv *)SDL_AndroidGetJNIEnv(); jobject act = (jobject)SDL_AndroidGetActivity();
    registered = 1;
    if (!env || !act) return;
    jclass cls = (*env)->GetObjectClass(env, act);
    JNINativeMethod m[] = { { "nativePanelFrame", "(Ljava/nio/ByteBuffer;[II)I", (void *)n_frame }, { "nativePanelLayout", "(II)V", (void *)n_layout },
                              { "nativePanelTouch", "(III)V", (void *)n_touch }, { "nativePanelKey", "(I)V", (void *)n_key },
                            { "nativePanelPinch", "(FII)V", (void *)n_pinch } };
    if ((*env)->RegisterNatives(env, cls, m, 5) != 0) { (*env)->ExceptionClear(env); port_printf("[panel] RegisterNatives failed\n"); }
    else port_printf("[panel] JNI ready\n");
    (*env)->DeleteLocalRef(env, cls); (*env)->DeleteLocalRef(env, act);
}
static void android_register(void) { port_host_call(android_register_h, 0); }   /* JNI: off the task stacks (threads.c) */
#endif

/* input queue: touches / clicks (canvas pixels) and keys, from the Android UI thread or the SDL loop, applied before drawing */
static struct { int kind, a, x, y; } inq[64]; static int inq_n;
#ifdef __ANDROID__
static pthread_mutex_t inq_mtx = PTHREAD_MUTEX_INITIALIZER;
#define INQ_LOCK() pthread_mutex_lock(&inq_mtx)
#define INQ_UNLOCK() pthread_mutex_unlock(&inq_mtx)
#else
#define INQ_LOCK()
#define INQ_UNLOCK()
#endif
static void inq_push(int kind, int a, int x, int y) {
    INQ_LOCK();
    if (inq_n < 64) { inq[inq_n].kind = kind; inq[inq_n].a = a; inq[inq_n].x = x; inq[inq_n].y = y; inq_n++; }
    INQ_UNLOCK();
}
static void inq_run(void) {
    INQ_LOCK();
    for (int i = 0; i < inq_n; i++) { if (inq[i].kind == 2) { if (page == PAGE_SIDE) map_pinch(inq[i].a / 10000.f, inq[i].x, inq[i].y); } else if (inq[i].kind) panel_touch_canvas(inq[i].x, inq[i].y, inq[i].a); else panel_action(inq[i].a); }
    inq_n = 0;
    INQ_UNLOCK();
}
/* PC window: mouse in window pixels (the canvas x pk), keys as IN_* */
/* port_panel_canvas_out: the host shows the canvas itself and scales it (the wasm2c build on Android: TombiActivity's
   Presentation takes canvas pixels, as the 32-bit build's n_frame gives them, and sends touches in canvas pixels) */
int port_panel_canvas_out;
void port_panel_touch(int x, int y, int action) { int k = pk && !port_panel_canvas_out ? pk : 1; inq_push(1, action, x / k, y / k); }
void port_panel_key(int a) { inq_push(0, a, 0, 0); }
void port_panel_pinch(float f, int x, int y) { int k = pk && !port_panel_canvas_out ? pk : 1; port_panel_pinch_canvas(f, x < 0 ? -1 : x / k, y < 0 ? -1 : y / k); }   /* PC: the mouse wheel */
/* --panel-input FRAME:ACTION,... (headless tests): up down left right ok back tabl tabr zoomout zoomin hide showhidden,
   tap@X@Y (canvas pixels), hold@X@Y (a long press), pinch@PERCENT@X@Y (a two-finger pinch: the distance ratio x 100),
   drag@X0@Y0@X1@Y1[@N] (a swipe over N vblanks, 8 by default) */
static const char *panel_script;
static int hold_x, hold_y, hold_at = -1;   /* hold@X@Y: the release, 40 frames later */
void port_panel_input_script(const char *s) { panel_script = s; }
static struct { int f, a, x, y; } sl[80]; static int nsl;   /* touches due at a later vblank (drag@) */
static void script_later(int f, int a, int x, int y) { if (nsl < 80) { sl[nsl].f = f; sl[nsl].a = a; sl[nsl].x = x; sl[nsl].y = y; nsl++; } }
static void script_step(void) {
    for (int i = 0; i < nsl;) if (sl[i].f <= port_frame) { inq_push(1, sl[i].a, sl[i].x, sl[i].y); sl[i] = sl[--nsl]; } else i++;
    static const char *names[] = { "", "up", "down", "left", "right", "ok", "back", "tabl", "tabr", "zoomout", "zoomin", "hide", "showhidden", "pin" };
    for (const char *p = panel_script; p && *p;) {
        int f = atoi(p); const char *c = strchr(p, ':'), *e = strchr(p, ',');
        if (!c) break;
        if (f == port_frame) {
            char a[48]; int n = (int)((e ? e : c + strlen(c)) - c - 1); if (n > 47) n = 47; memcpy(a, c + 1, n); a[n] = 0;
            int x, y;
            int pct, x1, y1;
            if (sscanf(a, "pinch@%d@%d@%d", &pct, &x, &y) == 3) port_panel_pinch_canvas(pct / 100.f, x, y);   /* pinch@PERCENT@X@Y */
            else if (sscanf(a, "drag@%d@%d@%d@%d", &x, &y, &x1, &y1) == 4) {   /* drag@X0@Y0@X1@Y1[@N]: a swipe, a move every vblank for N (8), then up */
                int nf = 8; const char *q = a; for (int k = 0; k < 5 && q; k++) q = strchr(q + 1, '@');
                if (q && atoi(q + 1) > 0) nf = atoi(q + 1) > 60 ? 60 : atoi(q + 1);
                inq_push(1, 0, x, y);
                for (int k = 1; k <= nf; k++) script_later(port_frame + k, 2, x + (x1 - x) * k / nf, y + (y1 - y) * k / nf);
                script_later(port_frame + nf, 1, x1, y1);
            }
            else if (sscanf(a, "tap@%d@%d", &x, &y) == 2) { inq_push(1, 0, x, y); inq_push(1, 1, x, y); }
            else if (sscanf(a, "hold@%d@%d", &x, &y) == 2) { inq_push(1, 0, x, y); hold_x = x; hold_y = y; hold_at = port_frame + 40; }
            else for (int i = 1; i < 14; i++) if (!strcmp(a, names[i])) inq_push(0, i, 0, 0);
        }
        p = e ? e + 1 : NULL;
    }
}

static int frames;
/* --show-fps / --log-fps: the panel's own cost on the game thread (it runs in port_gpu_vblank), a line every 50 renders:
   ms per render (mean / worst), the side views composed and the side map tiles built (panel_side.inc) */
static void panel_time_log(unsigned long long us) {
    static unsigned long long sum, mx; static int n;
    sum += us; if (us > mx) mx = us;
    if (++n < 50) return;
    char cd[160]; side_cache_desc(cd, sizeof cd);
    port_printf("[panel] ms/render: mean %llu.%02llu max %llu.%02llu (%d renders, page %d; side map: %d views composed %llu.%02llu ms mean; cache %s)\n",
                sum / n / 1000, sum / n / 10 % 100, mx / 1000, mx / 10 % 100, n, page, side_raster_n,
                side_raster_n ? side_raster_us / side_raster_n / 1000 : 0, side_raster_n ? side_raster_us / side_raster_n / 10 % 100 : 0, cd);
    sum = mx = 0; n = 0; side_raster_n = 0; side_raster_us = 0;
}
#ifdef __3DS__
/* the worker thread that draws the panel (n3ds/pica_c3d.c has the thread: libctru's headers define u32 too) */
int port_n3ds_worker_start(void (*job)(void));   /* 1: a thread runs job() after each port_n3ds_worker_go() */
void port_n3ds_worker_go(void);
static volatile int pw_state;            /* 0 idle, 1 rendering, 2 a canvas is ready */
/* (the worker's own renders timed: a line every 20, with --log-fps) */
static void pw_job(void) {
    extern unsigned long long port_time_us(void); extern int port_show_fps;
    static unsigned long long sum, mx; static int n;
    unsigned long long t0 = port_time_us();
    render();
    unsigned long long us = port_time_us() - t0;
    pw_state = 2;
    sum += us; if (us > mx) mx = us;
    if (++n >= 20) { if (port_show_fps) port_printf("[panel] worker: %d renders, %llu ms each, the longest %llu (page %d)\n", n, sum / n / 1000, mx / 1000, page); sum = mx = 0; n = 0; }
}
static int n3ds_worker_ok(void) {
    static int st;                        /* 0 not tried, 1 running, -1 none */
    if (!st) st = port_n3ds_worker_start(pw_job) ? 1 : -1;
    return st > 0;
}
#endif
/* every vblank (shims/gpu.c port_gpu_vblank) while the panel is on */
void panel_hud_update(void);      /* runtime/panel_hud.c: --hud-bottom */
void port_panel_vblank(void) {
    panel_hud_update();
    int dump = port_panel_dump_dir && port_dump_every > 0 && port_frame % port_dump_every == 0;
    if (panel_script) script_step();
    if (hold_at == port_frame) { inq_push(1, 1, hold_x, hold_y); hold_at = -1; }
#ifdef __3DS__
    if (page == PAGE_SIDE && pw_state != 1) side_cache_work();   /* (not under a render in the worker thread) */
#else
    if (page == PAGE_SIDE) side_cache_work();   /* the side map's tiles, a slice every vblank (panel_side.inc) */
#endif
#ifdef __3DS__
    /* The 3DS draws the panel on its CPU (~50 ms a render in the emulator, three 60 Hz frames): a worker thread does it,
       on another core where there is one (New 3DS; the Old 3DS's second core with the time the system grants), else in
       the time the main thread sleeps.  The main thread only takes input and starts a render while the worker is
       idle, and hands the finished canvas to the bottom screen: at most a render every fourth vblank. */
    if (!dump && n3ds_worker_ok()) {
        if (pw_state == 2) {
            if (port_panel_window) port_panel_window_frame((const unsigned char *)pa_cv, pa_cw, pa_ch);
            pw_state = 0;
        }
        /* a render when there is something to show: a touch or a button waiting (at the next vblank, not after the
           render in hand and another), a list moving, else a refresh now and then: every fourth vblank, and on an
           Old 3DS (--audio-half tells it) every 25th, half a second: its worker shares a third of a core with the
           sound's mixer, and a panel drawn without pause kept every touch waiting for a render to end */
        { extern int port_sound_half; static int idle;
          /* An Old 3DS: a touch or a button is answered here and now, by the main thread: the worker's render waits
             for the processor (15 to 50 ms measured, 90 in a busy room, for a few ms of work) and its picture for the
             vblank after, and a tap took a tenth of a second or two to show.  (One frame of the game late per tap.) */
          int now = 0;   /* (a finger moving is a move every vblank: those stay the worker's) */
          for (int i = 0; i < inq_n; i++) if (!(inq[i].kind == 1 && inq[i].a == 2)) now = 1;
          if (port_sound_half && now) {
              extern void port_sleep_us(unsigned long long); extern unsigned long long port_time_us(void); extern int port_show_fps;
              static unsigned long long sum, mx, wsum; static int n;
              unsigned long long t0 = port_time_us();
              while (pw_state == 1) port_sleep_us(500);   /* (the canvas is the worker's until its render ends) */
              unsigned long long t1 = port_time_us();
              pw_state = 0; idle = 0;
              inq_run(); render();
              if (port_panel_window) port_panel_window_frame((const unsigned char *)pa_cv, pa_cw, pa_ch);
              unsigned long long us = port_time_us() - t1;
              sum += us; wsum += t1 - t0; if (us > mx) mx = us;
              if (++n >= 10) { if (port_show_fps) port_printf("[panel] touches: %d answered at once, %llu.%llu ms each (the longest %llu), %llu.%llu ms waiting for the worker\n", n, sum / n / 1000, sum / n / 100 % 10, mx / 1000, wsum / n / 1000, wsum / n / 100 % 10); sum = mx = wsum = 0; n = 0; }
              return;
          }
          int due = inq_n > 0 || scr_busy() || ++idle >= (port_sound_half ? 25 : 4);
          if (pw_state == 0 && due) { idle = 0; inq_run(); pw_state = 1; port_n3ds_worker_go(); } }
        return;
    }
    { extern void port_sleep_us(unsigned long long); while (pw_state == 1) port_sleep_us(1000); }   /* (a dump, or no worker: here, as everywhere else) */
    pw_state = 0;
    if (!dump && (frames++ & 3) && !scr_busy()) return;
#else
    if (!dump && (frames++ & 1) && !scr_busy()) return;   /* ~25 fps is plenty, every vblank while a list moves */
#endif
    inq_run();
    { extern int port_show_fps; extern unsigned long long port_time_us(void);
      unsigned long long t0 = port_show_fps ? port_time_us() : 0;
      render();
      if (port_show_fps) panel_time_log(port_time_us() - t0); }
#ifdef __ANDROID__
    if (dump) upscale();
#else
    { extern int port_panel_canvas_out; if (dump || !port_panel_canvas_out) upscale(); }   /* (the 3DS shows the canvas itself) */
#endif
#ifdef __ANDROID__
    if (port_panel_window) {
        if (!registered) android_register();
        pthread_mutex_lock(&mtx);
        memcpy(out, pa_cv, (size_t)pa_cw * pa_ch * 4); out_w = pa_cw; out_h = pa_ch; if (++out_seq == 0) out_seq = 1;
        pthread_mutex_unlock(&mtx);
    }
#else
    if (port_panel_window) {
        if (port_panel_canvas_out) port_panel_window_frame((const unsigned char *)pa_cv, pa_cw, pa_ch);
        else port_panel_window_frame((const unsigned char *)pix, pw, ph);
    }
#endif
    if (dump) {
        static u8 *rgb; char path[512];
        if (!rgb) rgb = malloc((size_t)OUT_MAX * OUT_MAX * 3);
        for (int i = 0; i < pw * ph; i++) { u32 c = pix[i]; rgb[3 * i] = c & 0xFF; rgb[3 * i + 1] = c >> 8 & 0xFF; rgb[3 * i + 2] = c >> 16 & 0xFF; }
        port_mkdir(port_panel_dump_dir);
        snprintf(path, sizeof path, "%s/panel_%06d.png", port_panel_dump_dir, port_frame);
        if (port_gpu_write_png(path, rgb, pw, ph)) port_printf("[panel] cannot write %s\n", path);
    }
}
/* --panel-size WxH (testing the portrait layout on the PC) */
void port_panel_size(int w, int h) { req_w = w; req_h = h; }

/* --log-events: every event start / complete (setEventStarted / setEventComplete / awardEventProgress, wrapped in
   port/fixes.txt) with the frame, the room and Tombi's position: where an event really happens, for the panel's table */
int port_log_events;
void port_event_log(const char *what, int e, int before) {
    if (!port_log_events || e < 0 || e > 255) return;
    port_printf("[event] frame %d: %s %d (0x%02X) %d -> %d at area %d section %d, x %d y %d\n", port_frame, what, e, e, before,
                G_EVENT(e), G_AREA, G_SECTION, (s16)G16(PV_PLAYER_X), (s16)G16(PV_PLAYER_Y));
}
