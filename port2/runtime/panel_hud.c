/* Second screen panel, the top screen's HUD and the loading mirror (docs/PORT.md "Second screen"; port only, opt-in).
 *
 *   --hud-bottom   with the panel on (--second-screen / --dump-panel; Android: while the Presentation is showing it),
 *                  the game's HUD (vitality fruit, lives, AP, weapon icon, the exp gauges: drawHud) is not drawn on the
 *                  top screen; the panel's status window shows it.  port/fixes.txt makes drawHudIfShown test
 *                  port_hud_hide; everything else the game draws (banners, dialogue, menus, the pause screens,
 *                  cutscenes) is untouched.  Off (no flag, or no panel): port_hud_hide stays 0, the output is unchanged.
 *   loading mirror until gameplay: before a game runs (intro, title, the load / new game menus), while the game draws
 *                  its loading screen (displayLoadingScreen), at the game over and ending screens, the panel shows the
 *                  game's own NOW LOADING screen instead of the map: the 48 x 48 checkerboard tiles that swap every 15
 *                  frames and the pulsing NOW LOADING logo (drawLoadingSprites / drawNowLoadingSprite), from
 *                  SYSTEM/A00900.000 (4 bpp, 64 x 72 words at VRAM 0, 0, its palettes in the last row) and the sprite
 *                  table at 0x800787F0 of the game's own RAM.  port/fixes.txt records when the game last ran
 *                  displayLoadingScreen and gameStateDispatcher (its Task state1: 0 intro, 1 gameplay, 2 end, 3 over). */
#include <stdlib.h>
#include <string.h>
#include "port_runtime.h"
#include "panel_assets.h"
#include "panel_addrs.h"

typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32;

int port_hud_bottom;                 /* --hud-bottom (launcher: "HUD en la pantalla inferior") */
int port_hud_hide;                   /* read by drawHudIfShown (port/fixes.txt) */
int port_loading_frame = -1000;      /* port_frame of the last displayLoadingScreen (port/fixes.txt) */
int port_game_state = -1, port_game_state_frame = -1000;   /* gameStateDispatcher's TASK_C->state1 and when */
int port_panel_presented = -1000;    /* Android: port_frame of the Presentation's last poll (panel.c n_frame) */
extern int port_panel_window;

/* gameplay: a game runs (play time), the dispatcher is in gameplay and no loading screen is drawn */
int panel_in_play(void) {
    if (*(u32 *)PORT_M0(PV_GAME_TIME) == 0) return 0;
    if (port_frame - port_loading_frame < 4) return 0;
    if (port_frame - port_game_state_frame < 4 && port_game_state != 1) return 0;
    return 1;
}

/* every panel vblank: hide the top HUD while the bottom screen shows the panel */
void panel_hud_update(void) {
    int shown = 1;
#ifdef __ANDROID__
    shown = port_panel_window && port_frame - port_panel_presented < 50;
#endif
    port_hud_hide = port_hud_bottom && shown;
}

/* ---------------------------------------------------------------- NOW LOADING */
static u8 *nl;            /* SYSTEM/A00900.000: 64 x 72 words */
static int nl_state;      /* 0 not read, 1 ok, -1 missing */
static int nl_px(int u, int v) { return u < 0 || v < 0 || u >= 256 || v >= 72 ? 0 : nl[v * 128 + (u >> 1)] >> ((u & 1) * 4) & 15; }
static u32 nl_col(int cx, int cy, int i) {
    if (cx < 0 || cy < 0 || cy >= 72 || cx + i >= 64) return 0;
    u16 c = (u16)(nl[cy * 128 + 2 * (cx + i)] | nl[cy * 128 + 2 * (cx + i) + 1] << 8);
    if (!c) return 0;
    u32 r = (c & 31) * 255 / 31, g = (c >> 5 & 31) * 255 / 31, b = (c >> 10 & 31) * 255 / 31;
    return 0xFF000000u | b << 16 | g << 8 | r;
}
/* entry n of the game's sprite table {u16 u, v, w, h; s16 clutX, clutY} (0x800787F0: 0, 1 tiles; 2 NOW LOADING) */
static void def(int n, int *d) { for (int i = 0; i < 6; i++) d[i] = *(short *)PORT_M0(PV_UI_SPRITES + 12 * n + 2 * i); }
/* the sprite drawn into the rect (x0, y0)-(x1, y1) (a POLY_FT4 for the logo, nearest neighbour), 0x0000 texels skipped */
static void nl_spr(int n, int x0, int y0, int x1, int y1) {
    int d[6]; u32 pal[16];
    def(n, d);
    if (d[2] <= 0 || d[3] <= 0 || x1 <= x0 || y1 <= y0) return;
    for (int i = 0; i < 16; i++) pal[i] = nl_col(d[4], d[5], i);
    /* u = d0 + (x - x0) * d2 / D stepped along the row (quotient and remainder carried: the same integers as the
       division, which the 3DS's ARM11 does in software); v once per row */
    int D = x1 - x0, qs = d[2] / D, rs = d[2] % D;
    for (int y = y0; y < y1; y++) {
        int v = d[1] + (y - y0) * d[3] / (y1 - y0), u = d[0], r = 0;
        for (int x = x0; x < x1; x++) {
            u32 c = pal[nl_px(u, v)];
            if (c) pa_put(x, y, c);
            u += qs; r += rs; if (r >= D) { r -= D; u++; }
        }
    }
}
/* the loading screen on the canvas; 0 when the panel shows the game (or the file is missing) */
int panel_loading_screen(void) {
    if (panel_in_play()) return 0;
    if (!nl_state) {
        unsigned n; nl = pa_read(PA_F_NOW_LOADING, &n);
        nl_state = nl && n >= 64 * 72 * 2 ? 1 : -1;
        if (nl_state < 0) port_printf("[panel] %s not readable: no loading screen on the panel\n", PA_F_NOW_LOADING);
    }
    if (nl_state < 0) return 0;
    int W = pa_cw, H = pa_ch, c = port_frame / 15 & 1, s = port_frame * 12 & 255;
    /* the tiled background has two states (c) and changes with nothing else but the canvas size and the two tiles'
       CLUT colours: each is drawn once into a cache and copied in after that (the 3DS draws the panel on its CPU) */
    static u32 *bg[2], bg_pal[2][2][16]; static int bg_w, bg_h, bg_ok[2];
    u32 pal[2][16]; int dd[6];
    for (int k = 0; k < 2; k++) { def(k, dd); for (int i = 0; i < 16; i++) pal[k][i] = nl_col(dd[4], dd[5], i); }
    if (bg_w != W || bg_h != H) {
        for (int k = 0; k < 2; k++) { free(bg[k]); bg[k] = malloc((size_t)W * H * 4); bg_ok[k] = 0; }
        bg_w = W; bg_h = H;
    }
    if (bg_ok[c] && !memcmp(bg_pal[c], pal, sizeof pal)) memcpy(pa_cv, bg[c], (size_t)W * H * 4);
    else {
        pa_fill(0, 0, W, H, 0xFF000000u);
        for (int j = 0; j * 48 < H; j++) for (int i = 0; i * 48 < W; i++) {
            int k = (i + j) & 1 ? 1 - c : c;
            nl_spr(k, i * 48, j * 48, i * 48 + 48, j * 48 + 48);
        }
        if (bg[c]) { memcpy(bg[c], pa_cv, (size_t)W * H * 4); memcpy(bg_pal[c], pal, sizeof pal); bg_ok[c] = 1; }
    }
    /* drawNowLoadingSprite(0xC0, 0xA0, 0, ..): pad = (u32)NEG_SIN_TABLE[s] >> 11 as a short; the logo keeps the game's
       distance to the bottom right corner of its 320 x 240 screen */
    int d[6]; def(2, d);
    short pad = (short)((u32)(int)*(short *)PORT_M0(PV_NEG_SIN_TABLE + 2 * s) >> 11);
    int x = W - (320 - 0xC0), y = H - (240 - 0xA0);
    nl_spr(2, x - pad, y - pad, x + d[2] + pad, y + d[3] + pad);
    return 1;
}

/* ---------------------------------------------------------------- the HUD's weapon icon (drawHudWeaponIcon) */
/* a pointer held in the game's RAM (native once relocated, or a retail address) */
static u8 *gptr(u32 v) {
    if (v >= 0x80000000u && v < 0x80200000u) return (u8 *)port_ram_ptr(v);
    return port_is_ram_ptr((void *)(unsigned long)v) ? (u8 *)(unsigned long)v : NULL;
}
/* the sprite frame `frame` of the HUD object's bank (HUD_STATE +0x18), its parts {u, v, clut, .., w, h, .., dx, dy, ..}
   from the texture page tp (0x14: 256, 256; 0x15: 320, 256), as drawSpriteFrameParts draws it at (x, y) */
static void hud_frame(int x, int y, u32 pfr, int tp, int clut, int one) {
    u8 *bank = gptr(*(u32 *)PORT_M0(PV_HUD_STATE + 0x18)), *pf = gptr(*(u32 *)PORT_M0(pfr));
    if (!bank || !pf) return;
    int f = *(u16 *)pf, n = *(short *)(bank + 4 * f), off = *(short *)(bank + 4 * f + 2);
    if (one) n = 1;
    if (n <= 0 || n > 16 || off <= 0) return;
    for (int i = 0; i < n; i++) {
        const u8 *s = bank + off + 16 * i;
        int c = clut ? clut : s[2] | s[3] << 8;
        pa_spr(x + (signed char)s[14], y + (signed char)s[15], tp == 0x14 ? 256 : 320, 256, s[0], s[1], s[10], s[11], (c & 63) * 16, c >> 6 & 511, 0);
    }
}
/* the equipped weapon's HUD icon with (x, y) where the HUD puts it (0x48, 0x10 on its screen); 0: no icon */
int panel_weapon_icon(int x, int y) {
    switch (*PORT_M0(PV_GAME_WEAPON)) {
    case 0: hud_frame(x, y, PV_HUD_FRAME_0, 0x14, 0, 0); return 1;
    case 5: hud_frame(x, y, PV_HUD_FRAME_5, 0x15, 0, 0); return 1;
    case 6: hud_frame(x, y, PV_HUD_FRAME_6, 0x15, 0, 0); return 1;
    case 7: hud_frame(x, y, PV_HUD_FRAME_7, 0x15, 0, 0); return 1;
    case 8: hud_frame(x, y, PV_HUD_FRAME_8, 0x14, 0, 0); return 1;
    case 9: hud_frame(x, y, PV_HUD_FRAME_9, 0x14, 0x7E14, 1); return 1;
    }
    return 0;
}
