/* --wings-glide DIR (port only, with --cut-content): the cut top-down Charity Wings glide of the Japanese demo
   (SLPM-80165, docs/CUT_CONTENT.md "Lost: the top-down Charity Wings glide", tools/wings_glide.py).

   In a top-down area, using the Charity Wings runs topdownPlayerDropExitStep (I0NFO2): Tomba rises for 20 ticks, the
   screen fades and the room is left.  Retail animates that with the generic pose; the demo's overhead Tomba set keeps
   three frames of the real thing (77, 78, 79: right arm raised, holding the feather) that no script of the demo plays.
   While that step runs, this file swaps the player's sprite tables for a three-frame copy of the demo's (frame data and
   SPR parts read from the user's own extraction of the demo disc: DIR/AREA08/D002.000 and DIR/AREA08/D003.GAM, nothing
   shipped) and draws the Charity Wings item sprite (retail's resident item table, frames 159..161) turning like a rotor
   in the raised hand.  Positions come from the pixels (docs/CUT_CONTENT.md): the hand's top is at (22,2) / (22,1) /
   (22,1) of the three frames, the quill end of the feather frames at (2,22), (2,14), (7,14).  The spin order and
   timing are a reconstruction: flat, angled, edge-on, angled mirrored, flat mirrored, angled mirrored, edge-on, angled;
   two game ticks per phase.

   Hooks (port/fixes.txt): port_wings_begin(o) at the start of topdownPlayerDropExitStep (substep 0 = the wings),
   port_wings_draw(o, x, y, z) at the end of topdownDrawRotatedSprite (I0NFO2).  Everything is restored when the step ends or the
   tables are reloaded by a room change (then nothing is written back). */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int port_printf(const char *, ...);
extern char *WPP_DATA[];          /* scratchpad: WPP_DATA[2] = the top-down player's frame table (uploadSpriteFrameToVram) */
extern char *SPR_DATA[];          /* SPR_DATA[4] = the resident item / HUD sprite table (initItemObject) */
extern void *NEXT_PRIM;
extern char *CURRENT_OT;
extern void lzDecompress(unsigned char *, unsigned char *);
extern void AnimLoadDuration(void *);

const char *port_wings_dir;        /* --wings-glide: the extracted JP demo (Tomba frames 77-79) */
const char *port_wings_pre9_dir;   /* --wings-pre9: the extracted PrePre Vol. 9 (the feather page) */

#define O_B04(o) (((unsigned char *)(o))[0x04])
#define O_STEP(o) (((unsigned char *)(o))[0x05])
#define O_SUBSTEP(o) (((unsigned char *)(o))[0x07])
#define O_B0F(o) (((unsigned char *)(o))[0x0f])
#define O_ANIM(o) (*(void **)((char *)(o) + 0x24))
#define O_ANIMFRAME(o) (*(unsigned short *)((char *)(o) + 0x2e))
#define O_D3C(o) (*(int *)((char *)(o) + 0x3c))
#define O_B9D(o) (((unsigned char *)(o))[0x9d])

/* the two frame sets: the JP demo's overhead Tomba (frames 77-79, WPP slot 2, the top-down engines) and PrePre 9's
   side-view Tomba (frames 282-284 of its orphan script 0x800124A8: kneeling with the fist raised overhead, WPP slot 0,
   the side-view player) */
typedef struct {
    const char *name, *wpp_file, *spr_file; const char **dir;
    unsigned int table_off, expect_n, spr_off; int frames[3], slot, ticks;
    int hand_x[3], hand_y[3];                      /* top of the raised hand per frame (sprite px, measured) */
    unsigned char wpp[3][1024], parts[3][16 * 8]; unsigned int wpp_len[3], part_n[3];
    int tried, loaded;
} Set;
static Set SETS[2] = {
    {"JP demo top-down", "AREA08/D002.000", "AREA08/D003.GAM", &port_wings_dir, 0x3FA28, 106, 0, {77, 78, 79}, 2, 8, {22, 22, 22}, {2, 1, 1}},
    {"PrePre 9 side", "TOMBA/AREA01/A007.000", "TOMBA/AREA01/A008.GAM", &port_wings_pre9_dir, 0, 379, 0x14A0, {282, 283, 284}, 0, 10, {20, 21, 13}, {7, 7, 7}},
};
#define PRE9_FEATHER_PAGE "TOMBA/A00002.GAM"    /* 4 bpp, 256 px wide, 256 rows: the resident item page of PrePre 9 */
#define JP_FEATHER_PAGE "SYSTEM/A00000.GAM"    /* the same page in the JP demo, 512 px wide; must hold the same feather */
/* feather frames on that page (page px): x, y, w, h; retail keeps them at the same place of tpage 0x14 (VRAM 256,256) */
static const int FEATHER_RECT[3][4] = {{224, 104, 24, 24}, {200, 80, 24, 16}, {216, 64, 16, 16}};
static unsigned char feather_px[3][24 * 24 / 2];
static int feather_ok, feather_tried;
static const int FEATHER_FRAME[3] = {159, 160, 161}; /* retail item table: static, flap 1, flap 2 */
static const int QUILL[3][2] = {{2, 22}, {2, 14}, {7, 14}};
/* rotor phases: feather frame index (into FEATHER_FRAME) and mirrored or not */
static const int PHASE[8][2] = {{0, 0}, {1, 0}, {2, 0}, {1, 1}, {0, 1}, {1, 1}, {2, 0}, {1, 0}};
#define TICKS_PER_PHASE 2

/* the swapped tables are the area's own retail tables (every object drawn from the same WPP slot keeps its frames:
   Baron, the player's other indexes) with the three frames appended after the last entry; the script uses those.
   Static: they sit in the executable's data (below 16 MB) like every table the game reads through its packets. */
#define WPP_MAX (256 * 1024)
#define SPR_MAX (64 * 1024)
static unsigned char wpp_table_buf[WPP_MAX], spr_table_buf[SPR_MAX];
static unsigned char *wpp_table, *spr_table;
static int base_frame;                          /* the retail table's count = our first frame's index */
static unsigned short script[8 * 4 + 2];        /* 4 entries of 8 bytes + a 4-byte jump back */
static int active, ticks;
static Set *cur;                                /* the set in use while active */
static void *saved_wpp, *player;
static int saved_d3c;
extern unsigned char *PLAYER_EXT;

static unsigned char *read_file(const char *dir, const char *name, long *len) {
    char p[1024]; FILE *f; unsigned char *b;
    snprintf(p, sizeof p, "%s/%s", dir, name);
    f = fopen(p, "rb");
    if (!f) { port_printf("wings: cannot open %s\n", p); return 0; }
    fseek(f, 0, SEEK_END); *len = ftell(f); fseek(f, 0, SEEK_SET);
    b = malloc(*len + 16);
    if (!b || fread(b, 1, *len, f) != (size_t)*len) { fclose(f); free(b); port_printf("wings: cannot read %s\n", p); return 0; }
    fclose(f);
    return b;
}

static unsigned int u32(const unsigned char *p) { return p[0] | p[1] << 8 | p[2] << 16 | p[3] << 24; }
static void put32(unsigned char *p, unsigned int v) { p[0] = v; p[1] = v >> 8; p[2] = v >> 16; p[3] = v >> 24; }

/* unpacks a GAM file; 0 if not a GAM (or, with want, not that size) */
static unsigned char *read_gam(const char *dir, const char *name, unsigned int want, unsigned int *out_size) {
    unsigned char *gam, *pg; long gl; unsigned int size;
    gam = read_file(dir, name, &gl);
    if (!gam) return 0;
    if (gl < 8 || memcmp(gam, "GAM", 3)) { port_printf("wings: %s/%s is not a GAM file\n", dir, name); free(gam); return 0; }
    size = u32(gam + 4);
    if (want && size != want) { port_printf("wings: %s/%s unpacks to %u bytes, expected %u\n", dir, name, size, want); free(gam); return 0; }
    if (size > (64u << 20)) { free(gam); return 0; }
    pg = malloc(size + 1024);          /* a copy run can overshoot the size by up to 255 bytes */
    memset(pg, 0, size + 1024);
    lzDecompress(gam, pg);
    free(gam);
    if (out_size) *out_size = size;
    return pg;
}

/* the three feather frames, cut from the PrePre 9 page and checked against the JP demo's copy; 1 when ready */
static int load_feather(void) {
    unsigned char *p9, *jp; int i, y, ok = 1;
    if (feather_tried) return feather_ok;
    feather_tried = 1;
    if (!port_wings_dir || !port_wings_pre9_dir) { port_printf("wings: both --wings-glide JPDEMO_DIR and --wings-pre9 PRE9_DIR are needed\n"); return 0; }
    p9 = read_gam(port_wings_pre9_dir, PRE9_FEATHER_PAGE, 128 * 256, 0);
    if (!p9) return 0;
    jp = read_gam(port_wings_dir, JP_FEATHER_PAGE, 256 * 256, 0);
    if (!jp) { free(p9); return 0; }
    for (i = 0; i < 3; i++) {
        int x0 = FEATHER_RECT[i][0], y0 = FEATHER_RECT[i][1], w = FEATHER_RECT[i][2] / 2, h = FEATHER_RECT[i][3];
        for (y = 0; y < h; y++) {
            memcpy(feather_px[i] + y * w, p9 + (y0 + y) * 128 + x0 / 2, w);
            if (memcmp(feather_px[i] + y * w, jp + (y0 + y) * 256 + x0 / 2, w)) ok = 0;
        }
    }
    free(p9); free(jp);
    if (!ok) port_printf("wings: the feather of %s differs from the JP demo's: not the PrePre 9 / SLPM-80165 pages?\n", PRE9_FEATHER_PAGE);
    feather_ok = ok;
    return ok;
}

/* reads a set's three frames (WPP bytes and SPR parts) once; 1 when ready */
static int load_set(Set *st) {
    unsigned char *wpp, *spr; long wl; unsigned int n, i, size;
    unsigned char *t;
    if (st->tried) return st->loaded;
    st->tried = 1;
    if (!load_feather()) return 0;
    wpp = read_file(*st->dir, st->wpp_file, &wl);
    if (!wpp) return 0;
    if (wl < (long)st->table_off + 8) { port_printf("wings: %s is too short\n", st->wpp_file); free(wpp); return 0; }
    t = wpp + st->table_off;
    n = u32(t);
    if (n != st->expect_n) { port_printf("wings: %s +0x%X holds %u frames, expected %u (not the expected disc?)\n", st->wpp_file, st->table_off, n, st->expect_n); free(wpp); return 0; }
    for (i = 0; i < 3; i++) {
        unsigned int a = u32(t + 4 + 4 * st->frames[i]), b = u32(t + 4 + 4 * (st->frames[i] + 1));
        if (b < a || b - a > sizeof st->wpp[0] || b > (unsigned)(wl - st->table_off)) { port_printf("wings: frame %d of %s is bigger than expected\n", st->frames[i], st->wpp_file); free(wpp); return 0; }
        memcpy(st->wpp[i], t + a, b - a); st->wpp_len[i] = b - a;
    }
    free(wpp);
    spr = read_gam(*st->dir, st->spr_file, 0, &size);
    if (!spr) return 0;
    if (size < st->spr_off + 4 * (st->expect_n)) { port_printf("wings: %s is too short\n", st->spr_file); free(spr); return 0; }
    /* SPR table: u16 parts, u16 offset per frame; a part is 16 bytes */
    for (i = 0; i < 3; i++) {
        int k = st->frames[i]; unsigned char *tb = spr + st->spr_off;
        unsigned int cnt = tb[4 * k] | tb[4 * k + 1] << 8, src = tb[4 * k + 2] | tb[4 * k + 3] << 8;
        if (cnt == 0 || cnt > 8 || st->spr_off + src + 16 * cnt > size) { port_printf("wings: the SPR parts of frame %d of %s are not as expected\n", k, st->spr_file); free(spr); return 0; }
        memcpy(st->parts[i], tb + src, 16 * cnt); st->part_n[i] = cnt;
    }
    free(spr);
    st->loaded = 1;
    port_printf("wings: %s frames loaded from %s (feather from %s)\n", st->name, *st->dir, port_wings_pre9_dir);
    return 1;
}

/* the area's retail tables (the set's WPP slot and the player's part table) copied with the three frames appended;
   the script then plays base_frame .. base_frame+2; 0 when they do not fit */
static int compose(void *o, Set *st) {
    unsigned char *rw = (unsigned char *)WPP_DATA[st->slot], *rs = (unsigned char *)(unsigned long)O_D3C(o);
    unsigned int n, i, total, send = 0, o2;
    if (!rw || !rs) return 0;
    n = u32(rw);
    if (n == 0 || n > 1024) return 0;
    total = u32(rw + 4 + 4 * n);                /* the retail table's end offset = its size */
    if (total < 4 + 4 * (n + 1) || total + 12 + st->wpp_len[0] + st->wpp_len[1] + st->wpp_len[2] > WPP_MAX) return 0;
    /* WPP: header with n + 3 entries, every retail frame moved by 12 bytes (the three new header words), ours after */
    put32(wpp_table_buf, n + 3);
    for (i = 0; i <= n; i++) put32(wpp_table_buf + 4 + 4 * i, u32(rw + 4 + 4 * i) + 12);
    memcpy(wpp_table_buf + 4 + 4 * (n + 1) + 12, rw + 4 + 4 * (n + 1), total - (4 + 4 * (n + 1)));
    o2 = total + 12;
    for (i = 0; i < 3; i++) {
        put32(wpp_table_buf + 4 + 4 * (n + i), o2);
        memcpy(wpp_table_buf + o2, st->wpp[i], st->wpp_len[i]);
        o2 += st->wpp_len[i];
    }
    put32(wpp_table_buf + 4 + 4 * (n + 3), o2);
    /* SPR: n entries of (u16 parts, u16 offset); the parts after the last entry's parts */
    for (i = 0; i < n; i++) {
        unsigned int cnt = rs[4 * i] | rs[4 * i + 1] << 8, off = rs[4 * i + 2] | rs[4 * i + 3] << 8;
        if (off + 16 * cnt > send) send = off + 16 * cnt;
    }
    if (send < 4 * n || send + 12 + 16 * (st->part_n[0] + st->part_n[1] + st->part_n[2]) > SPR_MAX || send + 12 > 0xFFFF) return 0;
    for (i = 0; i < n; i++) {   /* every offset moved by 12 (the three new entries) */
        unsigned int cnt = rs[4 * i] | rs[4 * i + 1] << 8, off = (rs[4 * i + 2] | rs[4 * i + 3] << 8) + 12;
        spr_table_buf[4 * i] = cnt; spr_table_buf[4 * i + 1] = cnt >> 8; spr_table_buf[4 * i + 2] = off; spr_table_buf[4 * i + 3] = off >> 8;
    }
    memcpy(spr_table_buf + 4 * n + 12, rs + 4 * n, send - 4 * n);
    o2 = send + 12;
    for (i = 0; i < 3; i++) {
        spr_table_buf[4 * (n + i)] = st->part_n[i]; spr_table_buf[4 * (n + i) + 1] = 0;
        spr_table_buf[4 * (n + i) + 2] = o2; spr_table_buf[4 * (n + i) + 3] = o2 >> 8;
        memcpy(spr_table_buf + o2, st->parts[i], 16 * st->part_n[i]);
        o2 += 16 * st->part_n[i];
    }
    base_frame = n;
    wpp_table = wpp_table_buf; spr_table = spr_table_buf;
    /* the animation script: our frames 0 1 2 1, the set's ticks each, then jump back */
    {
        static const unsigned short seq[4] = {0, 1, 2, 1};
        for (i = 0; i < 4; i++) {
            script[4 * i] = n + seq[i]; script[4 * i + 1] = 0; script[4 * i + 2] = 0;
            script[4 * i + 3] = st->ticks | (i == 3 ? 0x4000 : 0);
        }
        { unsigned int a = (unsigned int)(unsigned long)script; memcpy(&script[16], &a, 4); }
    }
    return 1;
}

static int want_b04, want_step;     /* the player step the glide belongs to: tables restored once it moves on */

static void restore(void *o);
static void start(void *o, Set *st) {
    if (!port_wings_dir || !port_wings_pre9_dir) return;     /* its own option, not tied to --cut-content */
    if (!load_set(st)) return;
    if (active) {
        /* a glide whose step ended without a draw of ours (the player not drawn, a room load): let it go */
        if (o == player && O_B04(o) == want_b04 && O_STEP(o) == want_step) return;
        port_printf("wings: a stale %s glide (b04 %d step %d) released\n", cur ? cur->name : "?", want_b04, want_step);
        restore(player);
    }
    port_printf("wings: %s glide at b04 %d step %d state %d sub %d\n", st->name, O_B04(o), O_STEP(o),
                ((unsigned char *)o)[6], O_SUBSTEP(o));
    {   /* the feather texels of the PrePre 9 page over retail's (tpage 0x14 = VRAM 256,256; same place, 4 bpp) */
        typedef struct { short x, y, w, h; } R;
        extern int LoadImage(void *, unsigned *);
        int i;
        for (i = 0; i < 3; i++) {
            R r = {256 + FEATHER_RECT[i][0] / 4, 256 + FEATHER_RECT[i][1], FEATHER_RECT[i][2] / 4, FEATHER_RECT[i][3]};
            LoadImage(&r, (unsigned *)feather_px[i]);
        }
    }
    if (!compose(o, st)) { port_printf("wings: the area's tables could not be extended (%s)\n", st->name); return; }
    cur = st;
    saved_wpp = WPP_DATA[st->slot]; saved_d3c = O_D3C(o);
    WPP_DATA[st->slot] = (char *)wpp_table;
    O_D3C(o) = (int)(unsigned long)spr_table;
    O_ANIM(o) = script;
    O_ANIMFRAME(o) = base_frame;       /* the index the next upload reads: one of ours */
    AnimLoadDuration(o);
    active = 1; ticks = 0; player = o; want_b04 = O_B04(o); want_step = O_STEP(o);
}

/* topdownPlayerDropExitStep, state 0 (after the retail animation was set): take over when it is the Charity Wings */
void port_wings_begin(void *o) { if (O_SUBSTEP(o) == 0) start(o, &SETS[0]); }
/* topdownPlayerLandStep, state 0: the arrival in the next overhead area (the drop from the sky after the flight) gets
   the same glide until the feet touch the ground */
void port_wings_land(void *o) { start(o, &SETS[0]); }
/* the side view: playerEventStepRiseFadeOut state 0 (substep 0 = the wings), playerEventStepDropIn state 0 */
void port_wings_side_begin(void *o) {
    if (O_SUBSTEP(o) == 0) start(o, &SETS[1]);
    else port_printf("wings: the rise is not the Charity Wings (sub %d)\n", O_SUBSTEP(o));
}
void port_wings_side_land(void *o) { start(o, &SETS[1]); }
/* playerEventStepRiseFadeOut: while our glide runs, the retail air spin (playerAirSpinToFall) must not set the anim */
int port_wings_side_active(void *o) { return active && o == player && cur == &SETS[1]; }

static void restore(void *o) {
    /* write back only what nobody else changed in the meantime (a room load replaces both) */
    if (cur && WPP_DATA[cur->slot] == (char *)wpp_table) WPP_DATA[cur->slot] = saved_wpp;
    if (o && O_D3C(o) == (int)(unsigned long)spr_table) O_D3C(o) = saved_d3c;
    active = 0; player = 0; cur = 0;
}

typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0; unsigned char u0, v0; unsigned short clut;
    short x1, y1; unsigned char u1, v1; unsigned short tpage;
    short x2, y2; unsigned char u2, v2; unsigned short pad2;
    short x3, y3; unsigned char u3, v3; unsigned short pad3;
} PolyFT4;

/* end of topdownDrawRotatedSprite / drawPlayerSpriteDefault: (x, y) = the projected player position, z = its
   ordering-table depth (otz) */
void port_wings_draw(void *o, int x, int y, int z) {
    int c, ph, ff, mirror, w, h, u0, v0, dx, dy, hx, hy, qx, qy, fx, fy, flip, d;
    unsigned char *part, *fe; PolyFT4 *p; unsigned int *ot, v;
    if (!active || o != player || !cur) return;
    if (O_B04(o) != want_b04 || O_STEP(o) != want_step || WPP_DATA[cur->slot] != (char *)wpp_table) { restore(o); return; }
    c = *(unsigned short *)O_ANIM(o) - base_frame;      /* the script's frame -> 0..2 */
    if (c < 0 || c > 2) c = 0;
    ph = (ticks / TICKS_PER_PHASE) % 8; ticks++;
    ff = PHASE[ph][0]; mirror = PHASE[ph][1];
    /* the raised hand on screen: part 0 of our frame (dx, dy at +14/+15; w at +10) */
    part = spr_table + (spr_table[4 * (base_frame + c) + 2] | spr_table[4 * (base_frame + c) + 3] << 8);
    dx = (signed char)part[14]; dy = (signed char)part[15]; w = part[10];
    if (cur->slot == 2) flip = (unsigned)((O_ANIMFRAME(o) & 7) - 3) < 3;      /* topdownDrawRotatedSprite mirrors facings 3, 4, 5 */
    else flip = ((O_B9D(o) ? PLAYER_EXT[7] : O_ANIMFRAME(o)) & 1) != 0;       /* drawPlayerSpriteDefault: the facing bit */
    hx = flip ? x - (dx + w) + (w - 1 - cur->hand_x[c]) : x + dx + cur->hand_x[c];
    hy = y + dy + cur->hand_y[c];
    /* the feather frame of the retail item table */
    if (!SPR_DATA[4]) return;
    fe = (unsigned char *)SPR_DATA[4];
    fe = fe + (fe[4 * FEATHER_FRAME[ff] + 2] | fe[4 * FEATHER_FRAME[ff] + 3] << 8);
    u0 = fe[0]; v0 = fe[1]; w = fe[10]; h = fe[11];
    qx = QUILL[ff][0]; qy = QUILL[ff][1];
    if (mirror ^ flip) qx = w - 1 - qx;
    fx = hx - qx; fy = hy - 1 - qy;
    p = (PolyFT4 *)NEXT_PRIM;
    memset(p, 0, sizeof *p);
    p->code = 0x2d; p->r0 = p->g0 = p->b0 = 0x80;
    p->tpage = 0x14; p->clut = 0x7AD6;
    if (mirror ^ flip) { p->u0 = u0 + w; p->u1 = u0; p->u2 = u0 + w; p->u3 = u0; }
    else { p->u0 = u0; p->u1 = u0 + w; p->u2 = u0; p->u3 = u0 + w; }
    p->v0 = p->v1 = v0; p->v2 = p->v3 = v0 + h;
    p->x0 = fx; p->y0 = fy; p->x1 = fx + w; p->y1 = fy; p->x2 = fx; p->y2 = fy + h; p->x3 = fx + w; p->y3 = fy + h;
    /* one ordering-table slot nearer than the player, so the feather is drawn over the hand (the game's otadd) */
    d = (((signed char)O_B0F(o) - 1) + z) << 2;
    if (d < 0) d = 0;
    ot = (unsigned int *)(CURRENT_OT + 0x10 + d);
    if ((unsigned)((char *)ot - CURRENT_OT) >= 0xca0) return;
    v = *ot;
    *ot = (unsigned int)(unsigned long)p;
    p->tag = v | 0x9000000;
    NEXT_PRIM = (char *)NEXT_PRIM + 0x28;
}
