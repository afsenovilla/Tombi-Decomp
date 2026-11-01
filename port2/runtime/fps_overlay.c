/* --show-fps (port only): an on-screen frame counter in the top-left corner of the game picture, drawn at the
   window's resolution after the game frame (runtime/sdl.c for the software renderer, shims/gpu_gl.c for OpenGL /
   GL ES).  It shows the frames actually presented per second and, when they differ, the vblank (game logic) rate:
   "60 FPS" or "30/60 FPS".  Updated twice a second.  The once-a-second log line of shims/psyq.c stays.

   port_fps_overlay(img_h, &w, &h, &changed) counts one presented frame and returns the overlay as RGBA8 (w x h,
   top row first): a semi-transparent dark box with white 3x5 digits, scaled by an integer factor from the height
   of the picture on screen; NULL when --show-fps is off.  *changed is set when the pixels differ from last call. */
#include <stdio.h>
#include <string.h>

extern int port_show_fps, port_frame;
int port_fps_on_screen;   /* --show-fps: the counter on screen (--log-fps: only the log lines) */
extern unsigned long long port_time_us(void);

static const unsigned short glyph[] = {   /* 3x5, bit 14 = top-left */
    075557, 022222, 071747, 071717, 055711, 074717, 074757, 071111, 075757, 075717,   /* 0-9 */
    000000, 011244,                                                                  /* ' ', '/' */
    074744, 075744, 074717,                                                          /* F, P, S */
};
static int gidx(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    switch (c) { case '/': return 11; case 'F': return 12; case 'P': return 13; case 'S': return 14; default: return 10; }
}

static unsigned char buf[400 * 64 * 4];   /* "999/999 FPS" at scale 8 is 360 x 56 */
static int bw, bh;

const unsigned char *port_fps_overlay(int img_h, int *w, int *h, int *changed) {
    static unsigned long long t0; static int n0, presents, last_scale;
    static char text[24] = "-- FPS", shown[24];
    if (!port_fps_on_screen) return 0;
    unsigned long long t = port_time_us();
    presents++;
    if (!t0) { t0 = t; n0 = port_frame; presents = 0; }
    else if (t - t0 >= 500000ull) {
        unsigned long long dt = t - t0;
        int p = (int)((presents * 1000000ull + dt / 2) / dt), v = (int)(((port_frame - n0) * 1000000ull + dt / 2) / dt);
        if (p == v || p - v == 1 || v - p == 1) snprintf(text, sizeof text, "%d FPS", p);
        else snprintf(text, sizeof text, "%d/%d FPS", p, v);
        t0 = t; n0 = port_frame; presents = 0;
    }
    int s = img_h / 240; if (s < 2) s = 2; if (s > 8) s = 8;
    *changed = 0;
    if (s != last_scale || strcmp(text, shown)) {
        int n = (int)strlen(text);
        bw = (n * 4 + 1) * s; bh = 7 * s;   /* 1 px padding around, 1 px between glyphs */
        if ((size_t)bw * bh * 4 > sizeof buf) { s = 2; bw = (n * 4 + 1) * s; bh = 7 * s; }
        for (int i = 0; i < bw * bh; i++) { buf[i * 4] = buf[i * 4 + 1] = buf[i * 4 + 2] = 0; buf[i * 4 + 3] = 170; }
        for (int k = 0; k < n; k++) {
            unsigned g = glyph[gidx(text[k])];
            for (int gy = 0; gy < 5; gy++) for (int gx = 0; gx < 3; gx++) {
                if (!(g >> (14 - gy * 3 - gx) & 1)) continue;
                int x0 = (1 + k * 4 + gx) * s, y0 = (1 + gy) * s;
                for (int y = y0; y < y0 + s; y++) memset(buf + ((size_t)y * bw + x0) * 4, 255, (size_t)s * 4);
            }
        }
        strcpy(shown, text); last_scale = s; *changed = 1;
    }
    *w = bw; *h = bh;
    return buf;
}
