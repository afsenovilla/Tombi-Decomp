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
extern int port_cut_content;
extern char *WPP_DATA[];          /* scratchpad: WPP_DATA[2] = the top-down player's frame table (uploadSpriteFrameToVram) */
extern char *SPR_DATA[];          /* SPR_DATA[4] = the resident item / HUD sprite table (initItemObject) */
extern void *NEXT_PRIM;
extern char *CURRENT_OT;
extern void lzDecompress(unsigned char *, unsigned char *);
extern void AnimLoadDuration(void *);

const char *port_wings_dir;

#define O_B04(o) (((unsigned char *)(o))[0x04])
#define O_STEP(o) (((unsigned char *)(o))[0x05])
#define O_SUBSTEP(o) (((unsigned char *)(o))[0x07])
#define O_B0F(o) (((unsigned char *)(o))[0x0f])
#define O_ANIM(o) (*(void **)((char *)(o) + 0x24))
#define O_ANIMFRAME(o) (*(unsigned short *)((char *)(o) + 0x2e))
#define O_D3C(o) (*(int *)((char *)(o) + 0x3c))
#define O_B9D(o) (((unsigned char *)(o))[0x9d])

#define WPP_FILE "AREA08/D002.000"
#define WPP_TABLE_OFF 0x3FA28
#define SPR_FILE "AREA08/D003.GAM"
static const int FRAMES[3] = {77, 78, 79};
static const int HAND_Y[3] = {2, 1, 1};              /* top of the raised hand in each frame (x = 22) */
#define HAND_X 22
static const int FEATHER_FRAME[3] = {159, 160, 161}; /* retail item table: static, flap 1, flap 2 */
static const int QUILL[3][2] = {{2, 22}, {2, 14}, {7, 14}};
/* rotor phases: feather frame index (into FEATHER_FRAME) and mirrored or not */
static const int PHASE[8][2] = {{0, 0}, {1, 0}, {2, 0}, {1, 1}, {0, 1}, {1, 1}, {2, 0}, {1, 0}};
#define TICKS_PER_PHASE 2

static unsigned char *wpp_table, *spr_table;   /* the three-frame copies */
static unsigned short script[8 * 4 + 2];        /* 4 entries of 8 bytes + a 4-byte jump back */
static int tried, active, ticks;
static void *saved_wpp, *player;
static int saved_d3c;

static unsigned char *read_file(const char *name, long *len) {
    char p[1024]; FILE *f; unsigned char *b;
    snprintf(p, sizeof p, "%s/%s", port_wings_dir, name);
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

/* builds the three-frame WPP table and SPR table from the demo files; 1 when ready */
static int load(void) {
    unsigned char *wpp, *gam, *spr; long wl, gl; unsigned int n, i, total, size;
    unsigned char *t; unsigned int off[4];
    if (tried) return wpp_table != 0;
    tried = 1;
    if (!port_wings_dir) return 0;
    wpp = read_file(WPP_FILE, &wl);
    if (!wpp) return 0;
    if (wl < WPP_TABLE_OFF + 8) { port_printf("wings: %s is too short\n", WPP_FILE); free(wpp); return 0; }
    t = wpp + WPP_TABLE_OFF;
    n = u32(t);
    if (n != 106) { port_printf("wings: %s +0x%X holds %u frames, expected 106 (not the SLPM-80165 demo?)\n", WPP_FILE, WPP_TABLE_OFF, n); free(wpp); return 0; }
    total = 4 * 4 + 4;
    for (i = 0; i < 3; i++) total += u32(t + 4 + 4 * (FRAMES[i] + 1)) - u32(t + 4 + 4 * FRAMES[i]);
    wpp_table = malloc(total);
    put32(wpp_table, 3);
    off[0] = 4 * 4 + 4;
    for (i = 0; i < 3; i++) {
        unsigned int a = u32(t + 4 + 4 * FRAMES[i]), b = u32(t + 4 + 4 * (FRAMES[i] + 1));
        memcpy(wpp_table + off[i], t + a, b - a);
        off[i + 1] = off[i] + (b - a);
    }
    for (i = 0; i < 4; i++) put32(wpp_table + 4 + 4 * i, off[i]);
    free(wpp);
    gam = read_file(SPR_FILE, &gl);
    if (!gam) { free(wpp_table); wpp_table = 0; return 0; }
    if (gl < 8 || memcmp(gam, "GAM", 3)) { port_printf("wings: %s is not a GAM file\n", SPR_FILE); free(gam); free(wpp_table); wpp_table = 0; return 0; }
    size = u32(gam + 4);
    spr = malloc(size + 64);
    memset(spr, 0, size + 64);
    lzDecompress(gam, spr);
    free(gam);
    /* SPR table: u16 parts, u16 offset per frame; a part is 16 bytes */
    total = 4 * 3;
    for (i = 0; i < 3; i++) total += 16 * (spr[4 * FRAMES[i]] | spr[4 * FRAMES[i] + 1] << 8);
    spr_table = malloc(total);
    {
        unsigned int o = 4 * 3;
        for (i = 0; i < 3; i++) {
            unsigned int cnt = spr[4 * FRAMES[i]] | spr[4 * FRAMES[i] + 1] << 8, src = spr[4 * FRAMES[i] + 2] | spr[4 * FRAMES[i] + 3] << 8;
            spr_table[4 * i] = cnt; spr_table[4 * i + 1] = cnt >> 8; spr_table[4 * i + 2] = o; spr_table[4 * i + 3] = o >> 8;
            memcpy(spr_table + o, spr + src, 16 * cnt);
            o += 16 * cnt;
        }
    }
    free(spr);
    /* the animation script: frames 0 1 2 1, 8 ticks each, then jump back */
    {
        static const unsigned short seq[4] = {0, 1, 2, 1};
        for (i = 0; i < 4; i++) {
            script[4 * i] = seq[i]; script[4 * i + 1] = 0; script[4 * i + 2] = 0;
            script[4 * i + 3] = 8 | (i == 3 ? 0x4000 : 0);
        }
        { unsigned int a = (unsigned int)(unsigned long)script; memcpy(&script[16], &a, 4); }
    }
    port_printf("wings: demo glide frames loaded from %s\n", port_wings_dir);
    return 1;
}

/* topdownPlayerDropExitStep, state 0 (after the retail animation was set): take over when it is the Charity Wings */
void port_wings_begin(void *o) {
    if (!port_cut_content || !port_wings_dir || O_SUBSTEP(o) != 0) return;
    if (!load()) return;
    if (active) return;
    saved_wpp = WPP_DATA[2]; saved_d3c = O_D3C(o);
    WPP_DATA[2] = (char *)wpp_table;
    O_D3C(o) = (int)(unsigned long)spr_table;
    O_ANIM(o) = script;
    AnimLoadDuration(o);
    active = 1; ticks = 0; player = o;
}

static void restore(void *o) {
    /* write back only what nobody else changed in the meantime (a room load replaces both) */
    if (WPP_DATA[2] == (char *)wpp_table) WPP_DATA[2] = saved_wpp;
    if (o && O_D3C(o) == (int)(unsigned long)spr_table) O_D3C(o) = saved_d3c;
    active = 0; player = 0;
}

typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0; unsigned char u0, v0; unsigned short clut;
    short x1, y1; unsigned char u1, v1; unsigned short tpage;
    short x2, y2; unsigned char u2, v2; unsigned short pad2;
    short x3, y3; unsigned char u3, v3; unsigned short pad3;
} PolyFT4;

/* end of topdownDrawRotatedSprite: (x, y) = the projected player position, z = its ordering-table depth (otz) */
void port_wings_draw(void *o, int x, int y, int z) {
    int cur, ph, ff, mirror, w, h, u0, v0, dx, dy, hx, hy, qx, qy, fx, fy, flip, d;
    unsigned char *part, *fe; PolyFT4 *p; unsigned int *ot, v;
    if (!active || o != player) return;
    if (O_B04(o) != 4 || O_STEP(o) != 4 || WPP_DATA[2] != (char *)wpp_table) { restore(o); return; }
    cur = *(unsigned short *)O_ANIM(o);                 /* our frame 0..2 */
    if (cur > 2) cur = 0;
    ph = (ticks / TICKS_PER_PHASE) % 8; ticks++;
    ff = PHASE[ph][0]; mirror = PHASE[ph][1];
    /* the raised hand on screen: torso part of our frame (part 0: dx, dy at +14/+15; w at +10) */
    part = spr_table + (spr_table[4 * cur + 2] | spr_table[4 * cur + 3] << 8);
    dx = (signed char)part[14]; dy = (signed char)part[15]; w = part[10];
    flip = (unsigned)((O_ANIMFRAME(o) & 7) - 3) < 3;      /* topdownDrawRotatedSprite mirrors facings 3, 4, 5 */
    hx = flip ? x - (dx + w) + (w - 1 - HAND_X) : x + dx + HAND_X;
    hy = y + dy + HAND_Y[cur];
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
