/* --log-pop FILE (port only, research; docs/PORT.md "Widescreen", "Pop-in check"): for every object and every
 * background block the scene draws, the screen-X range of the primitives it emitted this frame, one line each, so
 * tools/ws_popcheck.py can find what appears or vanishes inside the wide frame instead of scrolling in from its edge.
 * Hooks (port/fixes.txt): drawObject (wrapped) and the block loop of drawAreaModelList.  Nothing is written, and the
 * hooks only compare a pointer, without the option. */
#include "port_runtime.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

const char *port_poplog_path;
static FILE *pl_f;
static int pl_frame = -1;
extern int port_frame;
extern int port_ws_cull;
extern char *NEXT_PRIM;

unsigned long port_pop_begin(void) { return port_poplog_path ? (unsigned long)(uintptr_t)NEXT_PRIM : 0; }

void port_pop_end(unsigned long b, const void *key, int kind)
{
    if (!port_poplog_path || !b) return;
    if (!pl_f) { pl_f = fopen(port_poplog_path, "w"); if (!pl_f) { port_poplog_path = 0; return; } }
    if (pl_frame != port_frame) {
        pl_frame = port_frame; fflush(pl_f);
        extern unsigned char CAMERA_SCROLL[];           /* minX / maxX at +0x2C / +0x2E */
        fprintf(pl_f, "F %d %d %d %d %d %d\n", port_frame, port_ws_cull, *(volatile int16_t *)PORT_SPAD(0x176),
                *(int16_t *)(CAMERA_SCROLL + 0x2c), *(int16_t *)(CAMERA_SCROLL + 0x2e),
                *(volatile int16_t *)port_ram_ptr(0x800A604Au));
    }
    const uint8_t *p = (const uint8_t *)b, *e = (const uint8_t *)(uintptr_t)NEXT_PRIM;
    int x0 = 32767, x1 = -32768, y0 = 32767, y1 = -32768, n = 0;
    while (p < e) {
        uint32_t tag = *(const uint32_t *)p;
        int len = tag >> 24;
        const uint32_t *w = (const uint32_t *)(p + 4);
        p += 4 + 4 * len;
        if (!len) continue;
        int cmd = w[0] >> 24, nv;
        if (cmd >= 0x60 && cmd < 0x80) {                  /* sprite / tile: xy, [uv], [w h] */
            int sz = (cmd >> 3) & 3, ww = sz == 1 ? 1 : sz == 2 ? 8 : 16, hh = ww;
            int16_t vx = (int16_t)(w[1] & 0xffff), vy = (int16_t)(w[1] >> 16);
            if (!sz) { uint32_t s = w[(cmd & 4) ? 3 : 2]; ww = s & 0xffff; hh = s >> 16; }
            if (vx < x0) x0 = vx;
            if (vx + ww > x1) x1 = vx + ww;
            if (vy < y0) y0 = vy;
            if (vy + hh > y1) y1 = vy + hh;
            n++;
            continue;
        }
        if (cmd < 0x20 || cmd >= 0x40) continue;
        nv = (cmd & 8) ? 4 : 3;
        /* polygon: colour+cmd, then per vertex [colour (gouraud, not the first)] xy [uv] */
        int k = 1;
        for (int v = 0; v < nv; v++) {
            if (v && (cmd & 0x10)) k++;
            if (k >= len) break;
            int16_t vx = (int16_t)(w[k] & 0xffff), vy = (int16_t)(w[k] >> 16);
            vx = (int16_t)(vx << 5) >> 5; vy = (int16_t)(vy << 5) >> 5;
            if (vx < x0) x0 = vx;
            if (vx > x1) x1 = vx;
            if (vy < y0) y0 = vy;
            if (vy > y1) y1 = vy;
            k += 1 + ((cmd & 4) ? 1 : 0);
        }
        n++;
    }
    if (!n) return;
    const uint8_t *o = (const uint8_t *)key;
    if (kind == 2) fprintf(pl_f, "O player 00 00 00 %d %d %d %d %d\n", x0, x1, y0, y1, n);   /* drawPlayer */
    else if (kind == 0) fprintf(pl_f, "O %lx %02x %02x %02x %d %d %d %d %d\n", (unsigned long)(uintptr_t)key, o[0x1c], o[2], o[3],
                           x0, x1, y0, y1, n);
    else fprintf(pl_f, "B %lx %d %d %d %d %d\n", (unsigned long)(uintptr_t)key, x0, x1, y0, y1, n);
}

/* --sweep-x FRAME:STEP:X,X,... (research, with --log-pop): from vblank FRAME on, the player's X is set every vblank,
 * moving STEP units a frame through the listed X positions (=X jumps there), so a room can be scrolled end to end
 * without playing it.  The game still runs: gravity, the camera, the spawners all see an ordinary walk. */
static int sw_frame = -1, sw_step = 4, sw_n, sw_i, sw_x, sw_on, sw_fly, sw_fly_on, sw_y;
/* --sweep-fly DY (with --sweep-x): the player flies at a fixed height, DY units from where the sweep starts (negative
 * = up), as the debug menu's fly does (GVARS.b3e holds updatePlayer, velocities cleared): no gravity, no walls, so
 * the camera scrolls at a steady speed at mid height. */
void port_sweep_fly(int dy) { sw_fly = 1; sw_y = dy; }
static int sw_wp[64]; static char sw_jump[64];
void port_sweep_add(const char *s)
{
    sw_frame = atoi(s); s = strchr(s, ':'); if (!s) return;
    sw_step = atoi(++s); s = strchr(s, ':'); if (!s) return;
    s++;
    while (*s && sw_n < 64) {
        sw_jump[sw_n] = *s == '=';
        if (*s == '=') s++;
        sw_wp[sw_n++] = atoi(s);
        while (*s && *s != ',') s++;
        if (*s == ',') s++;
    }
}
void port_sweep_vblank(void)
{
    if (sw_frame < 0 || port_frame < sw_frame || sw_i >= sw_n) return;
    uint8_t *hp = *(uint8_t **)(PORT_M0(0x800A6038u) + 0x40);
    if (!port_is_ram_ptr(hp)) return;
    if (!sw_on) { sw_on = 1; sw_x = *(int16_t *)(hp + 2); if (sw_fly) sw_y += *(int16_t *)(PORT_M0(PORT_A_PLAYER) + 0x16); }
    if (sw_fly && sw_i < sw_n) {
        uint8_t *pl = PORT_M0(PORT_A_PLAYER);
        *(uint8_t *)PORT_M0(PORT_A_GVARS + 0x0E) = 1; sw_fly_on = 1;
        *(int32_t *)(pl + 0x14) = sw_y << 16;
        *(int16_t *)(pl + 0x7C) = *(int16_t *)(pl + 0x7E) = *(int16_t *)(pl + 0x80) = *(int16_t *)(pl + 0x82) = 0;
    }
    int t = sw_wp[sw_i];
    if (sw_jump[sw_i]) { sw_x = t; sw_i++; }
    else if (sw_x < t) { sw_x += sw_step; if (sw_x >= t) { sw_x = t; sw_i++; } }
    else if (sw_x > t) { sw_x -= sw_step; if (sw_x <= t) { sw_x = t; sw_i++; } }
    else sw_i++;
    *(int16_t *)(hp + 2) = (int16_t)sw_x;
    if (sw_i >= sw_n) {
        port_printf("[port] --sweep-x: done at frame %d\n", port_frame);
        if (sw_fly_on) *(uint8_t *)PORT_M0(PORT_A_GVARS + 0x0E) = 0;
    }
}
