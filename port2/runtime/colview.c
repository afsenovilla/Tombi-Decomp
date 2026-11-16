/* --show-collision (port only, opt-in; docs/PORT.md "Collision view"): the collision data drawn over the game.
 *
 *  - the WHM boundaries of the depth plane Tomba stands in (docs/FORMATS.md "WHM"), read through the game's own
 *    getCollisionPlaneAt for every path position around him: floors green, ceilings blue, side walls red (pushing
 *    left) / magenta (pushing right), filled blocks yellow (outline), anything else grey;
 *  - the boxes of the live objects (box0..box3 at +0x6C, as boxesOverlapFlipAware reads them) and Tomba's: Tomba white,
 *    layer 2 (enemies, NPCs) red, layer 3 orange, layer 4 (mechanisms, platforms) cyan, layer 5 (items) yellow.
 *
 * The points are taken in the game loop (port_colview_tick, from port_debug_tick: the positions and the camera of the
 * frame that is being shown) and projected like the game projects its sprites (drawObjectSprite: the camera matrix at
 * 0x1F8000C0, then RTPS with the GTE's own screen distance and offset); port_colview_draw draws them in the vblank with
 * the debug overlay's fills.  Nothing runs without the flag. */
#include <string.h>
#include <stdio.h>
#include "port_runtime.h"

typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32; typedef short s16; typedef int s32;
typedef long long s64;

int port_show_collision;

#define CV_VIEW_CAMERA 0xC0     /* scratchpad: MATRIX m[3][3] s16, t[3] s32 at +0x14 (the same in every release) */
#define CV_POOL_N 200
#define CV_POOL_SZ 0xD4

extern char *getCollisionPlaneAt(short x, short y) __attribute__((weak));   /* weak: a version whose port names it otherwise draws only the boxes */
extern int port_ws_whm_is_copy(const void *p);   /* ws_exits.c: a WHM column copied past an opened edge */
extern unsigned long port_gte_cfc2(int r);
extern int port_gpu_overlay_fill(int x, int y, int w, int h, u16 c);
extern void port_gpu_overlay_end(void);

typedef struct { s16 x0, y0, x1, y1; u16 c; } Seg;
/* the room exits (runtime/ws_exits.c: the position checks before each tryStartItemInteraction(o, 1, N) of the area code) */
#include "ws_exits.h"
typedef struct { s16 x, y; u8 exit, cond; } Lab;
static Lab lab[2][64]; static int nlab[2];
#define MAXSEG 4096
static Seg seg[2][MAXSEG]; static int nseg[2], cur;   /* double buffer: the tick fills one while the vblank draws the other */
static volatile int ready = -1;

static s16 cam_m[3][3]; static s32 cam_t[3]; static s32 g_h, g_ofx, g_ofy;

static void cam_load(void) {
    const u8 *c = (const u8 *)PORT_SPAD(CV_VIEW_CAMERA);
    for (int i = 0; i < 9; i++) cam_m[i / 3][i % 3] = *(const s16 *)(c + 2 * i);
    for (int i = 0; i < 3; i++) cam_t[i] = *(const s32 *)(c + 0x14 + 4 * i);
    g_ofx = (s32)port_gte_cfc2(24); g_ofy = (s32)port_gte_cfc2(25); g_h = (s32)(u16)port_gte_cfc2(26);
}
/* world (x, y, z) -> screen, as RTPS; 0 when behind the camera */
static int proj(s32 x, s32 y, s32 z, int *sx, int *sy) {
    s64 vx = (((s64)cam_t[0] << 12) + cam_m[0][0] * (s64)x + cam_m[0][1] * (s64)y + cam_m[0][2] * (s64)z) >> 12;
    s64 vy = (((s64)cam_t[1] << 12) + cam_m[1][0] * (s64)x + cam_m[1][1] * (s64)y + cam_m[1][2] * (s64)z) >> 12;
    s64 vz = (((s64)cam_t[2] << 12) + cam_m[2][0] * (s64)x + cam_m[2][1] * (s64)y + cam_m[2][2] * (s64)z) >> 12;
    if (vz <= 0) return 0;
    *sx = (int)((g_ofx + (s64)g_h * vx * 65536 / vz) >> 16);
    *sy = (int)((g_ofy + (s64)g_h * vy * 65536 / vz) >> 16);
    return 1;
}
/* path position h, height y, depth d of an object whose path pointer is (or is not) its x */
static int proj_hd(int hx, s32 h, s32 y, s32 d, int *sx, int *sy) {
    return hx ? proj(h, y, d, sx, sy) : proj(d, y, h, sx, sy);
}
static void add(Seg *s, int *n, int x0, int y0, int x1, int y1, u16 c) {
    if (*n >= MAXSEG) return;
    if ((x0 < -400 && x1 < -400) || (x0 > 720 && x1 > 720) || (y0 < -64 && y1 < -64) || (y0 > 520 && y1 > 520)) return;
    Seg *q = &s[(*n)++]; q->x0 = (s16)x0; q->y0 = (s16)y0; q->x1 = (s16)x1; q->y1 = (s16)y1; q->c = c;
}
static void add3(Seg *s, int *n, int hx, s32 h0, s32 y0, s32 h1, s32 y1, s32 d, u16 c) {
    int a, b, e, f;
    if (proj_hd(hx, h0, y0, d, &a, &b) && proj_hd(hx, h1, y1, d, &e, &f)) add(s, n, a, b, e, f, c);
}

#define C_FLOOR 0x03E0
#define C_CEIL  0x7DE0
#define C_WALLL 0x001F
#define C_WALLR 0x7C1F
#define C_BLOCK 0x03FF
#define C_OTHER 0x4210

static s32 whole(const void *p) { return *(const s32 *)p >> 16; }

static int in_gameplay(void) {
    u8 *t = *(u8 **)PORT_SPAD(0x1D4);     /* the running task (debugmenu.c task_main) */
    return t && port_is_ram_ptr(t);
}

void port_colview_tick(void) {
    if (!port_show_collision) return;
    int w = cur ^ 1, n = 0; Seg *s = seg[w];
    u8 *pl = PORT_M0(PORT_A_PLAYER);
    s32 *ph = *(s32 **)(pl + 0x40), *pd = *(s32 **)(pl + 0x44);
    if (!in_gameplay() || !port_is_ram_ptr(ph) || !port_is_ram_ptr(pd)) { nseg[w] = 0; ready = w; cur = w; return; }
    cam_load();
    int hx = (u8 *)ph == pl + 0x10;
    s32 H = *ph >> 16, D = *pd >> 16;
    /* the WHM boundaries of Tomba's plane, path column by path column */
    for (s32 x = H - 420; getCollisionPlaneAt && x <= H + 420; x++) {
        const u8 *col = (const u8 *)getCollisionPlaneAt((short)x, (short)D);
        if (!col || (!port_is_ram_ptr((void *)col) && !port_ws_whm_is_copy(col))) continue;   /* (or the copy past an opened edge: ws_exits.c) */
        int cnt = *(const u16 *)col;
        if (cnt <= 0 || cnt > 32) continue;
        for (int i = 0; i < cnt; i++) {
            const u8 *r = col + 2 + 8 * i;
            u16 attr = *(const u16 *)r; s16 by = *(const s16 *)(r + 2), dy = *(const s16 *)(r + 4);
            int st = r[6] & 15, wd = r[6] >> 4, in = x & 7;
            if (dy && (attr & 3) && !(attr & 0x10) && (!wd || in < st || in > st + wd)) continue;   /* a flat floor / ceiling covers the column (probeFloorPlaneAt) */
            int y0 = wd ? by + dy * (in - st) / wd : by, y1 = wd ? by + dy * (in + 1 - st) / wd : by + dy;
            if (attr & 0x10) {            /* filled block: its top edge (the surface) */
                add3(s, &n, hx, x, by, x + 1, by, D, C_BLOCK);
            } else if (attr & 1) add3(s, &n, hx, x, y0, x + 1, y1, D, C_FLOOR);
            else if (attr & 2) add3(s, &n, hx, x, y0, x + 1, y1, D, C_CEIL);
            else if (attr & 4) { if (in == st) add3(s, &n, hx, x, by, x, by + dy, D, C_WALLL); }
            else if (attr & 8) { if (in == st) add3(s, &n, hx, x, by, x, by + dy, D, C_WALLR); }
            else if (wd) add3(s, &n, hx, x, y0, x + 1, y1, D, C_OTHER);
        }
    }
    /* the boxes: the live objects of the pool (layers 2-5), then Tomba */
    for (int k = 0; k <= CV_POOL_N; k++) {
        u8 *o = k < CV_POOL_N ? PORT_M0(PORT_A_OBJECT_POOL + (u32)k * CV_POOL_SZ) : pl;
        if (k < CV_POOL_N && (!o[0] || o[0x1C] < 2 || o[0x1C] > 5)) continue;
        s16 b0 = *(s16 *)(o + 0x6C), b1 = *(s16 *)(o + 0x6E), b2 = *(s16 *)(o + 0x70), b3 = *(s16 *)(o + 0x72);
        s32 *oh = *(s32 **)(o + 0x40), *od = *(s32 **)(o + 0x44);
        if (b1 <= 0 || b3 <= 0 || !port_is_ram_ptr(oh) || !port_is_ram_ptr(od)) continue;
        u16 c = k == CV_POOL_N ? 0x7FFF : o[0x1C] == 2 ? 0x001F : o[0x1C] == 3 ? 0x021F : o[0x1C] == 4 ? 0x7FE0 : 0x03FF;
        int ohx = (u8 *)oh == o + 0x10;
        s32 h = *oh >> 16, y = whole(o + 0x14), d = *od >> 16;
        s32 l = (*(u16 *)(o + 0x2E) & 1) ? b1 - b0 : b0;
        s32 x0 = h - l, x1 = x0 + b1, t = y - b2, bt = t + b3;
        add3(s, &n, ohx, x0, t, x1, t, d, c); add3(s, &n, ohx, x0, bt, x1, bt, d, c);
        add3(s, &n, ohx, x0, t, x0, bt, d, c); add3(s, &n, ohx, x1, t, x1, bt, d, c);
    }
    /* the exits of this room: their finite edges, at Tomba's depth (dim when his depth is outside the exit's) */
    {   int area = *(u16 *)PORT_M0(PORT_A_GAME), sec = *(u16 *)PORT_M0(PORT_A_GAME + 2), nl = 0;
        s32 Y = whole(pl + 0x14);
        for (int e = 0; e < port_exit_count; e++) {
            const Exit *x = &port_exits[e];
            if (x->area != area || sec > 31 || !(x->secs >> sec & 1)) continue;
            int far = D < x->d0 || D > x->d1;
            u16 c = x->cond ? (far ? 0x2129 : 0x4E7F) : (far ? 0x0110 : 0x023F);
            extern int port_ws_player_margin(void);   /* exits the port moves out (tools/colview_exits.py "ws") */
            s32 xh0 = x->h0 + x->ws0 * port_ws_player_margin(), xh1 = x->h1 + x->ws1 * port_ws_player_margin();
            s32 h0 = xh0 < H - 400 ? H - 400 : xh0, h1 = xh1 > H + 400 ? H + 400 : xh1;
            s32 y0 = x->y0 < Y - 320 ? Y - 320 : x->y0, y1 = x->y1 > Y + 240 ? Y + 240 : x->y1;
            if (h0 > h1 || y0 > y1) continue;
            if (xh0 == h0) add3(s, &n, hx, h0, y0, h0, y1, D, c);
            if (xh1 == h1) add3(s, &n, hx, h1 + 1, y0, h1 + 1, y1, D, c);
            if (x->y0 == y0) add3(s, &n, hx, h0, y0, h1 + 1, y0, D, c);
            if (x->y1 == y1) add3(s, &n, hx, h0, y1, h1 + 1, y1, D, c);
            int lx, ly;
            if (nl < 64 && proj_hd(hx, xh0 == h0 ? h0 + 2 : xh1 == h1 ? h1 - 40 : (h0 + h1) / 2, x->y0 == y0 ? y0 + 4 : y1 - 12, D, &lx, &ly)) {
                Lab *q = &lab[w][nl++]; q->x = (s16)lx; q->y = (s16)ly; q->exit = x->exit; q->cond = x->cond;
            }
        }
        nlab[w] = nl;
    }
    nseg[w] = n; ready = w; cur = w;
}

/* every fill is clipped to the displayed picture: shims/gpu.c writes the overlay straight into VRAM, so a fill outside it
   would land in the texture pages */
extern int port_widescreen;
extern int port_gpu_display_h(void);
static int disp_h = 240;
static void put(int x, int y, int w, int h, u16 c) {
    int x0 = -port_widescreen, x1 = 320 + port_widescreen;
    if (x < x0) { w -= x0 - x; x = x0; }
    if (x + w > x1) w = x1 - x;
    if (y < 0) { h += y; y = 0; }
    if (y + h > disp_h) h = disp_h - y;
    if (w > 0 && h > 0) port_gpu_overlay_fill(x, y, w, h, c);
}
static void line(int x0, int y0, int x1, int y1, u16 c) {
    int dx = x1 > x0 ? x1 - x0 : x0 - x1, dy = y1 > y0 ? y1 - y0 : y0 - y1, sx = x0 < x1 ? 1 : -1, sy = y0 < y1 ? 1 : -1;
    if (dy == 0) { put(x0 < x1 ? x0 : x1, y0, dx + 1, 1, c); return; }
    if (dx == 0) { put(x0, y0 < y1 ? y0 : y1, 1, dy + 1, c); return; }
    int e = dx - dy, guard = 2048;
    while (guard--) {
        put(x0, y0, 1, 1, c);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * e;
        if (e2 > -dy) { e -= dy; x0 += sx; }
        if (e2 < dx) { e += dx; y0 += sy; }
    }
}

/* from the vblank (shims/gpu.c port_gpu_vblank) */
void port_colview_draw(void) {
    int r = ready;
    if (!port_show_collision || r < 0) return;
    if (!port_gpu_overlay_fill(0, 0, 0, 0, 0)) return;
    disp_h = port_gpu_display_h();
    for (int i = 0; i < nseg[r]; i++) { Seg *q = &seg[r][i]; line(q->x0, q->y0, q->x1, q->y1, q->c); }
    {   extern void port_debug_text(int x, int y, const char *s, u16 col); char b[16];
        int xl = -port_widescreen, xr = 320 + port_widescreen - 48;
        for (int i = 0; i < nlab[r]; i++) {
            Lab *q = &lab[r][i]; int x = q->x < xl ? xl : q->x > xr ? xr : q->x, y = q->y < 0 ? 0 : q->y > disp_h - 8 ? disp_h - 8 : q->y;
            snprintf(b, sizeof b, q->cond ? "EXIT %d?" : "EXIT %d", q->exit);
            port_debug_text(x, y, b, q->cond ? 0x4E7F : 0x023F);
        }
    }
    port_gpu_overlay_end();
}
