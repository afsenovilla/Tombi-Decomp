/* --show-collision (port only, opt-in; docs/PORT.md "Collision view"): the collision data drawn over the game.
 *
 *  - the WHM boundaries of the depth plane Tomba stands in (docs/FORMATS.md "WHM"), read through the game's own
 *    getCollisionPlaneAt for every path position around him: floors green, ceilings blue, side walls red (pushing
 *    left) / magenta (pushing right), filled blocks yellow (outline), anything else grey;
 *  - the boxes of the live objects (box0..box3 at +0x6C, as boxesOverlapFlipAware reads them) and Tomba's: Tomba
 *    white, layer 2 (enemies, NPCs) red, layer 3 orange, layer 4 (mechanisms, platforms) cyan, layer 5 (items) yellow.
 *
 * The points are taken in the game loop (port_colview_tick, from port_debug_tick: the positions and the camera of the
 * frame that is being shown) and projected like the game projects its sprites (drawObjectSprite: the camera matrix
 * at 0x1F8000C0, then RTPS with the GTE's own screen distance and offset); port_colview_draw draws them in the vblank
 * with the debug overlay's fills.  Nothing runs without the flag. */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "port_runtime.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef short s16;
typedef int s32;
typedef long long s64;

int port_show_collision;

#define CV_VIEW_CAMERA 0xC0     /* scratchpad: MATRIX m[3][3] s16, t[3] s32 at +0x14 (the same in every release) */
#define CV_POOL_N 200
#define CV_POOL_SZ 0xD4

/* weak: a version whose port names it otherwise draws only the boxes */
extern char *getCollisionPlaneAt(short x, short y) __attribute__((weak));
extern int port_ws_whm_is_copy(const void *p);   /* ws_exits.c: a WHM column copied past an opened edge */
extern unsigned long port_gte_cfc2(int r);
extern int port_gpu_overlay_fill(int x, int y, int w, int h, u16 c);
extern void port_gpu_overlay_end(void);

typedef struct { s16 x0, y0, x1, y1; u16 c; } Seg;       /* a line on the screen and its colour */
/* the room exits (runtime/ws_exits.c: the position checks before each tryStartItemInteraction(o, 1, N) of the area
   code) */
#include "ws_exits.h"
typedef struct { s16 x, y; u8 exit, cond; } Lab;          /* an exit's label: "EXIT n" ("?" with other conditions) */
static Lab labels[2][64];
static int label_count[2];
#define MAXSEG 4096
/* double buffer: the tick fills one while the vblank draws the other */
static Seg seg[2][MAXSEG];
static int seg_count[2], cur;
static volatile int ready = -1;

/* the camera of the frame (the matrix and translation at CV_VIEW_CAMERA) and the GTE's H, OFX, OFY */
static s16 cam_m[3][3];
static s32 cam_t[3];
static s32 g_h, g_ofx, g_ofy;

static void cam_load(void) {
    const u8 *c = (const u8 *)PORT_SPAD(CV_VIEW_CAMERA);
    for (int i = 0; i < 9; i++) cam_m[i / 3][i % 3] = *(const s16 *)(c + 2 * i);
    for (int i = 0; i < 3; i++) cam_t[i] = *(const s32 *)(c + 0x14 + 4 * i);
    g_ofx = (s32)port_gte_cfc2(24);
    g_ofy = (s32)port_gte_cfc2(25);
    g_h = (s32)(u16)port_gte_cfc2(26);
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
/* appends a screen line to s (n lines so far), unless the list is full or the line is far off the screen */
static void add(Seg *s, int *n, int x0, int y0, int x1, int y1, u16 c) {
    if (*n >= MAXSEG) return;
    if ((x0 < -400 && x1 < -400) || (x0 > 720 && x1 > 720) || (y0 < -64 && y1 < -64) || (y0 > 520 && y1 > 520))
        return;
    Seg *out = &s[(*n)++];
    out->x0 = (s16)x0;
    out->y0 = (s16)y0;
    out->x1 = (s16)x1;
    out->y1 = (s16)y1;
    out->c = c;
}
/* a line between two path positions (h0, y0) and (h1, y1) at depth d, projected */
static void add3(Seg *s, int *n, int hx, s32 h0, s32 y0, s32 h1, s32 y1, s32 d, u16 c) {
    int sx0, sy0, sx1, sy1;
    if (proj_hd(hx, h0, y0, d, &sx0, &sy0) && proj_hd(hx, h1, y1, d, &sx1, &sy1)) add(s, n, sx0, sy0, sx1, sy1, c);
}

#define C_FLOOR 0x03E0
#define C_CEIL  0x7DE0
#define C_WALLL 0x001F
#define C_WALLR 0x7C1F
#define C_BLOCK 0x03FF
#define C_OTHER 0x4210

static s32 whole(const void *p) { return *(const s32 *)p >> 16; }

static int in_gameplay(void) {
    u8 *task = *(u8 **)PORT_SPAD(0x1D4);     /* the running task (debugmenu.c task_main) */
    return task && port_is_ram_ptr(task);
}

void port_colview_tick(void) {
    if (!port_show_collision) return;
    int fill = cur ^ 1, n = 0;                 /* the buffer filled this frame */
    Seg *segs = seg[fill];
    u8 *pl = PORT_M0(PORT_A_PLAYER);
    s32 *path_h = *(s32 **)(pl + 0x40),
        *path_d = *(s32 **)(pl + 0x44);
    if (!in_gameplay() || !port_is_ram_ptr(path_h) || !port_is_ram_ptr(path_d)) {
        seg_count[fill] = 0;
        ready = fill;
        cur = fill;
        return;
    }
    cam_load();
    int hx = (u8 *)path_h == pl + 0x10;
    s32 H = *path_h >> 16, D = *path_d >> 16;
    /* the WHM boundaries of Tomba's plane, path column by path column */
    for (s32 x = H - 420; getCollisionPlaneAt && x <= H + 420; x++) {
        const u8 *col = (const u8 *)getCollisionPlaneAt((short)x, (short)D);
        /* (or the copy past an opened edge: ws_exits.c) */
        if (!col || (!port_is_ram_ptr((void *)col) && !port_ws_whm_is_copy(col))) continue;
        int piece_count = *(const u16 *)col;
        if (piece_count <= 0 || piece_count > 32) continue;
        for (int i = 0; i < piece_count; i++) {
            const u8 *piece = col + 2 + 8 * i;
            u16 attr = *(const u16 *)piece;
            s16 base_y = *(const s16 *)(piece + 2),
                dy = *(const s16 *)(piece + 4);
            int start = piece[6] & 15,
                width = piece[6] >> 4,
                cell_x = x & 7;
            /* a flat floor / ceiling covers the column (probeFloorPlaneAt) */
            if (dy && (attr & 3) && !(attr & 0x10) && (!width || cell_x < start || cell_x > start + width)) continue;
            int y0 = width ? base_y + dy * (cell_x - start) / width : base_y,
                y1 = width ? base_y + dy * (cell_x + 1 - start) / width : base_y + dy;
            if (attr & 0x10) {            /* filled block: its top edge (the surface) */
                add3(segs, &n, hx, x, base_y, x + 1, base_y, D, C_BLOCK);
            } else if (attr & 1) add3(segs, &n, hx, x, y0, x + 1, y1, D, C_FLOOR);
            else if (attr & 2) add3(segs, &n, hx, x, y0, x + 1, y1, D, C_CEIL);
            else if (attr & 4) {
                if (cell_x == start) add3(segs, &n, hx, x, base_y, x, base_y + dy, D, C_WALLL);
            }
            else if (attr & 8) {
                if (cell_x == start) add3(segs, &n, hx, x, base_y, x, base_y + dy, D, C_WALLR);
            }
            else if (width) add3(segs, &n, hx, x, y0, x + 1, y1, D, C_OTHER);
        }
    }
    /* the boxes: the live objects of the pool (layers 2-5), then Tomba */
    for (int k = 0; k <= CV_POOL_N; k++) {
        u8 *obj = k < CV_POOL_N ? PORT_M0(PORT_A_OBJECT_POOL + (u32)k * CV_POOL_SZ) : pl;
        if (k < CV_POOL_N && (!obj[0] || obj[0x1C] < 2 || obj[0x1C] > 5)) continue;
        s16 b0 = *(s16 *)(obj + 0x6C),
            b1 = *(s16 *)(obj + 0x6E),
            b2 = *(s16 *)(obj + 0x70),
            b3 = *(s16 *)(obj + 0x72);
        s32 *obj_h = *(s32 **)(obj + 0x40),
            *obj_d = *(s32 **)(obj + 0x44);
        if (b1 <= 0 || b3 <= 0 || !port_is_ram_ptr(obj_h) || !port_is_ram_ptr(obj_d)) continue;
        u16 c = k == CV_POOL_N ? 0x7FFF
              : obj[0x1C] == 2 ? 0x001F : obj[0x1C] == 3 ? 0x021F : obj[0x1C] == 4 ? 0x7FE0 : 0x03FF;
        int obj_hx = (u8 *)obj_h == obj + 0x10;
        s32 h = *obj_h >> 16, y = whole(obj + 0x14), d = *obj_d >> 16;
        s32 left = (*(u16 *)(obj + 0x2E) & 1) ? b1 - b0 : b0;      /* (mirrored when flipped) */
        s32 x0 = h - left, x1 = x0 + b1, top = y - b2, bottom = top + b3;
        add3(segs, &n, obj_hx, x0, top, x1, top, d, c);
        add3(segs, &n, obj_hx, x0, bottom, x1, bottom, d, c);
        add3(segs, &n, obj_hx, x0, top, x0, bottom, d, c);
        add3(segs, &n, obj_hx, x1, top, x1, bottom, d, c);
    }
    /* the exits of this room: their finite edges, at Tomba's depth (dim when his depth is outside the exit's) */
    {
        int area = *(u16 *)PORT_M0(PORT_A_GAME),
            sec = *(u16 *)PORT_M0(PORT_A_GAME + 2),
            count = 0;
        s32 Y = whole(pl + 0x14);
        for (int e = 0; e < port_exit_count; e++) {
            const Exit *x = &port_exits[e];
            if (x->area != area || sec > 31 || !(x->secs >> sec & 1)) continue;
            int far = D < x->d0 || D > x->d1;
            u16 c = x->cond ? (far ? 0x2129 : 0x4E7F) : (far ? 0x0110 : 0x023F);
            extern int port_ws_player_margin(void);   /* exits the port moves out (tools/colview_exits.py "ws") */
            s32 xh0 = x->h0 + x->ws0 * port_ws_player_margin(),
                xh1 = x->h1 + x->ws1 * port_ws_player_margin();
            s32 h0 = xh0 < H - 400 ? H - 400 : xh0,
                h1 = xh1 > H + 400 ? H + 400 : xh1;
            s32 y0 = x->y0 < Y - 320 ? Y - 320 : x->y0,
                y1 = x->y1 > Y + 240 ? Y + 240 : x->y1;
            if (h0 > h1 || y0 > y1) continue;
            if (xh0 == h0) add3(segs, &n, hx, h0, y0, h0, y1, D, c);
            if (xh1 == h1) add3(segs, &n, hx, h1 + 1, y0, h1 + 1, y1, D, c);
            if (x->y0 == y0) add3(segs, &n, hx, h0, y0, h1 + 1, y0, D, c);
            if (x->y1 == y1) add3(segs, &n, hx, h0, y1, h1 + 1, y1, D, c);
            int lx, ly;
            if (count < 64
                && proj_hd(hx, xh0 == h0 ? h0 + 2 : xh1 == h1 ? h1 - 40 : (h0 + h1) / 2,
                           x->y0 == y0 ? y0 + 4 : y1 - 12, D, &lx, &ly)) {
                Lab *label = &labels[fill][count++];
                label->x = (s16)lx;
                label->y = (s16)ly;
                label->exit = x->exit;
                label->cond = x->cond;
            }
        }
        label_count[fill] = count;
    }
    {   /* PORT_CVLOG=1: one line per game frame on stderr, "CVLOG area section H=.. Y=.. D=..", for
           tools/ws_exit_sweep.py */
        static int log_on = -1;
        if (log_on < 0) log_on = getenv("PORT_CVLOG") != NULL;
        if (log_on)
            fprintf(stderr, "CVLOG %d %d H=%d Y=%d D=%d\n", *(u16 *)PORT_M0(PORT_A_GAME),
                    *(u16 *)PORT_M0(PORT_A_GAME + 2), (int)H, (int)whole(pl + 0x14), (int)D);
    }
    seg_count[fill] = n;
    ready = fill;
    cur = fill;
}

/* every fill is clipped to the displayed picture: shims/gpu.c writes the overlay straight into VRAM, so a fill outside
   it would land in the texture pages */
extern int port_widescreen;
extern int port_gpu_display_h(void);
static int disp_h = 240;
static void put(int x, int y, int w, int h, u16 c) {
    int x0 = -port_widescreen, x1 = 320 + port_widescreen;
    if (x < x0) {
        w -= x0 - x;
        x = x0;
    }
    if (x + w > x1) w = x1 - x;
    if (y < 0) {
        h += y;
        y = 0;
    }
    if (y + h > disp_h) h = disp_h - y;
    if (w > 0 && h > 0) port_gpu_overlay_fill(x, y, w, h, c);
}
/* a line of 1x1 fills (Bresenham); horizontal and vertical ones as one fill */
static void line(int x0, int y0, int x1, int y1, u16 c) {
    int dx = x1 > x0 ? x1 - x0 : x0 - x1,
        dy = y1 > y0 ? y1 - y0 : y0 - y1,
        sx = x0 < x1 ? 1 : -1,
        sy = y0 < y1 ? 1 : -1;
    if (dy == 0) {
        put(x0 < x1 ? x0 : x1, y0, dx + 1, 1, c);
        return;
    }
    if (dx == 0) {
        put(x0, y0 < y1 ? y0 : y1, 1, dy + 1, c);
        return;
    }
    int e = dx - dy, guard = 2048;
    while (guard--) {
        put(x0, y0, 1, 1, c);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * e;
        if (e2 > -dy) {
            e -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            e += dx;
            y0 += sy;
        }
    }
}

/* from the vblank (shims/gpu.c port_gpu_vblank) */
void port_colview_draw(void) {
    int shown = ready;                         /* the buffer the last tick finished */
    if (!port_show_collision || shown < 0) return;
    if (!port_gpu_overlay_fill(0, 0, 0, 0, 0)) return;
    disp_h = port_gpu_display_h();
    for (int i = 0; i < seg_count[shown]; i++) {
        Seg *q = &seg[shown][i];
        line(q->x0, q->y0, q->x1, q->y1, q->c);
    }
    {
        extern void port_debug_text(int x, int y, const char *s, u16 col);
        char label_text[16];
        int xl = -port_widescreen, xr = 320 + port_widescreen - 48;
        for (int i = 0; i < label_count[shown]; i++) {
            Lab *q = &labels[shown][i];
            int x = q->x < xl ? xl : q->x > xr ? xr : q->x,
                y = q->y < 0 ? 0 : q->y > disp_h - 8 ? disp_h - 8 : q->y;
            snprintf(label_text, sizeof label_text, q->cond ? "EXIT %d?" : "EXIT %d", q->exit);
            port_debug_text(x, y, label_text, q->cond ? 0x4E7F : 0x023F);
        }
    }
    port_gpu_overlay_end();
}
