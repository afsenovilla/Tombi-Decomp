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
static FILE *pl_file;
static int pl_frame = -1;
extern int port_frame;
extern int port_ws_cull;
extern char *NEXT_PRIM;

/* before the drawing: where the next primitive goes (0 without --log-pop) */
unsigned long port_pop_begin(void) { return port_poplog_path ? (unsigned long)(uintptr_t)NEXT_PRIM : 0; }

/* after it: the bounding box of the primitives written since start (port_pop_begin); kind 0 object, 2 player,
   else background block */
void port_pop_end(unsigned long start, const void *key, int kind)
{
    if (!port_poplog_path || !start) return;
    if (!pl_file) {
        pl_file = fopen(port_poplog_path, "w");
        if (!pl_file) {
            port_poplog_path = 0;
            return;
        }
    }
    if (pl_frame != port_frame) {
        pl_frame = port_frame;
        fflush(pl_file);
        extern unsigned char CAMERA_SCROLL[];           /* minX / maxX at +0x2C / +0x2E */
        fprintf(pl_file, "F %d %d %d %d %d %d\n", port_frame, port_ws_cull, *(volatile int16_t *)PORT_SPAD(0x176),
                *(int16_t *)(CAMERA_SCROLL + 0x2c), *(int16_t *)(CAMERA_SCROLL + 0x2e),
                *(volatile int16_t *)port_ram_ptr(0x800A604Au));
    }
    const uint8_t *p = (const uint8_t *)start,
                  *end = (const uint8_t *)(uintptr_t)NEXT_PRIM;
    int x0 = 32767, x1 = -32768, y0 = 32767, y1 = -32768, n = 0;
    while (p < end) {
        uint32_t tag = *(const uint32_t *)p;
        int len = tag >> 24;
        const uint32_t *words = (const uint32_t *)(p + 4);
        p += 4 + 4 * len;
        if (!len) continue;
        int cmd = words[0] >> 24, vert_count;
        if (cmd >= 0x60 && cmd < 0x80) {                  /* sprite / tile: xy, [uv], [w h] */
            int size_code = (cmd >> 3) & 3,
                width = size_code == 1 ? 1 : size_code == 2 ? 8 : 16,
                height = width;
            int16_t vx = (int16_t)(words[1] & 0xffff),
                    vy = (int16_t)(words[1] >> 16);
            if (!size_code) {
                uint32_t wh = words[(cmd & 4) ? 3 : 2];
                width = wh & 0xffff;
                height = wh >> 16;
            }
            if (vx < x0) x0 = vx;
            if (vx + width > x1) x1 = vx + width;
            if (vy < y0) y0 = vy;
            if (vy + height > y1) y1 = vy + height;
            n++;
            continue;
        }
        if (cmd < 0x20 || cmd >= 0x40) continue;
        vert_count = (cmd & 8) ? 4 : 3;
        /* polygon: colour+cmd, then per vertex [colour (gouraud, not the first)] xy [uv] */
        int k = 1;
        for (int v = 0; v < vert_count; v++) {
            if (v && (cmd & 0x10)) k++;
            if (k >= len) break;
            int16_t vx = (int16_t)(words[k] & 0xffff),
                    vy = (int16_t)(words[k] >> 16);
            vx = (int16_t)(vx << 5) >> 5;                   /* (11-bit signed coordinates) */
            vy = (int16_t)(vy << 5) >> 5;
            if (vx < x0) x0 = vx;
            if (vx > x1) x1 = vx;
            if (vy < y0) y0 = vy;
            if (vy > y1) y1 = vy;
            k += 1 + ((cmd & 4) ? 1 : 0);
        }
        n++;
    }
    if (!n) return;
    const uint8_t *obj = (const uint8_t *)key;
    if (kind == 2) fprintf(pl_file, "O player 00 00 00 %d %d %d %d %d\n", x0, x1, y0, y1, n);   /* drawPlayer */
    else if (kind == 0)
        fprintf(pl_file, "O %lx %02x %02x %02x %d %d %d %d %d\n", (unsigned long)(uintptr_t)key, obj[0x1c], obj[2],
                obj[3], x0, x1, y0, y1, n);
    else fprintf(pl_file, "B %lx %d %d %d %d %d\n", (unsigned long)(uintptr_t)key, x0, x1, y0, y1, n);
}

/* --sweep-x FRAME:STEP:X,X,... (research, with --log-pop): from vblank FRAME on, the player's X is set every vblank,
 * moving STEP units a frame through the listed X positions (=X jumps there, ~N lets the player go for N vblanks), so
 * a room can be scrolled end to end without playing it.  The game still runs: gravity, the camera, the spawners all
 * see an ordinary walk. */
static int sw_frame = -1, sw_step = 4, sw_count, sw_index, sw_x, sw_on, sw_fly, sw_fly_on, sw_y;
/* --sweep-fly DY (with --sweep-x): the player flies at a fixed height, DY units from where the sweep starts (negative
 * = up), as the debug menu's fly does (GVARS.b3e holds updatePlayer, velocities cleared): no gravity, no walls, so
 * the camera scrolls at a steady speed at mid height. */
void port_sweep_fly(int dy)
{
    sw_fly = 1;
    sw_y = dy;
}
static int sw_points[64];                                 /* the listed X positions (or N for a ~N) */
static char sw_jump[64];                                  /* per point: 0 walk, 1 jump (=X), 2 let go (~N) */
void port_sweep_add(const char *s)
{
    sw_frame = atoi(s);
    s = strchr(s, ':');
    if (!s) return;
    sw_step = atoi(++s);
    s = strchr(s, ':');
    if (!s) return;
    s++;
    while (*s && sw_count < 64) {
        /* (~N: let go for N vblanks, e.g. over a screen change) */
        sw_jump[sw_count] = *s == '=' ? 1 : *s == '~' ? 2 : 0;
        if (*s == '=' || *s == '~') s++;
        sw_points[sw_count++] = atoi(s);
        while (*s && *s != ',') s++;
        if (*s == ',') s++;
    }
}
void port_sweep_vblank(void)
{
    if (sw_frame < 0 || port_frame < sw_frame || sw_index >= sw_count) return;
    uint8_t *player_obj = *(uint8_t **)(PORT_M0(0x800A6038u) + 0x40);
    if (!port_is_ram_ptr(player_obj)) return;
    if (sw_jump[sw_index] == 2) {
        if (sw_fly_on) {
            *(int16_t *)(player_obj + 2) = (int16_t)sw_x;
            *(uint8_t *)PORT_M0(PORT_A_GVARS + 0x0E) = 0;
            sw_fly_on = 0;
        }
        sw_fly = 0;                                         /* (on foot from here) */
        if (--sw_points[sw_index] <= 0) {
            sw_index++;
            sw_x = *(int16_t *)(player_obj + 2);
        }
        return;
    }
    if (!sw_on) {
        sw_on = 1;
        sw_x = *(int16_t *)(player_obj + 2);
        if (sw_fly) sw_y += *(int16_t *)(PORT_M0(PORT_A_PLAYER) + 0x16);
    }
    if (sw_fly && sw_index < sw_count) {
        uint8_t *pl = PORT_M0(PORT_A_PLAYER);
        *(uint8_t *)PORT_M0(PORT_A_GVARS + 0x0E) = 1;
        sw_fly_on = 1;
        *(int32_t *)(pl + 0x14) = sw_y << 16;
        *(int16_t *)(pl + 0x7C) = *(int16_t *)(pl + 0x7E) = *(int16_t *)(pl + 0x80) = *(int16_t *)(pl + 0x82) = 0;
    }
    int target = sw_points[sw_index];
    if (sw_jump[sw_index] == 1) {
        sw_x = target;
        sw_index++;
    }
    else if (sw_x < target) {
        sw_x += sw_step;
        if (sw_x >= target) {
            sw_x = target;
            sw_index++;
        }
    }
    else if (sw_x > target) {
        sw_x -= sw_step;
        if (sw_x <= target) {
            sw_x = target;
            sw_index++;
        }
    }
    else sw_index++;
    *(int16_t *)(player_obj + 2) = (int16_t)sw_x;
    if (sw_index >= sw_count) {
        port_printf("[port] --sweep-x: done at frame %d\n", port_frame);
        if (sw_fly_on) *(uint8_t *)PORT_M0(PORT_A_GVARS + 0x0E) = 0;
    }
}
