/* --debug-menu (port only, opt-in; docs/PORT.md "Debug menu"): an in-game debug menu drawn as a port-side overlay.
 *
 * Open / close: Select+L1+R1 (pad or keyboard) or F1 (--window).  While it is open the game gets no buttons.
 * Up/Down pick a line, Left/Right change its value, Cross applies / toggles, Triangle or Start closes.
 *
 * Hooks (all inert unless --debug-menu is given, so the default run is byte-identical):
 *   shims/pad.c     port_pad_vblank -> port_debug_pad     (input: open/close, navigation, fly controls, live cheats)
 *   shims/gpu.c     port_gpu_vblank -> port_debug_draw    (overlay: fills into the displayed frame, before present/dump)
 *   port/fixes.txt  gameMainTask    -> port_debug_tick    (game-loop actions: items, events, warp; the game's own
 *                                                          functions run there, never from the vblank)
 *   port/fixes.txt  pushOutOfWallSegments -> port_debug_noclip_obj (noclip: the player's wall push is skipped)
 *   runtime/sdl.c   F1 -> port_debug_key
 * The overlay is a 5x7 bitmap font drawn with GPU fills (both renderers, --widescreen included); no game graphics. */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "port_runtime.h"

typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32; typedef short s16; typedef int s32;

int port_debug_menu;          /* --debug-menu */
int port_debug_key;           /* F1 pressed (sdl.c) */
int port_debug_noclip;
static int menu_open, cursor, fly, inf_hp, inf_ap, inf_lives, overlay, speed, saved_realtime = -1;
static int item_id = 1, weapon = 1, event_id = 0, w_area, w_sec, w_spawn;
static u16 prev, swallow;   /* swallow: buttons held when the menu closed, kept from the game until released */
static int fps_x100; static unsigned long long fps_t0; static int fps_n0;
static char msg[48]; static int msg_t;
enum { A_NONE, A_ITEM, A_EV_START, A_EV_DONE, A_WARP, A_ALL_EVENTS };
static volatile int action, action_arg;

/* game addresses (notes/data_names_main0.csv, include/tomba/game.h gameConfig) */
#define PLAYER       PORT_M0(0x800A6038u)
#define G_AREA       (*(u16 *)PORT_M0(0x8009C960u))
#define G_SECTION    (*(u16 *)PORT_M0(0x8009C962u))
#define G_AP         (*(u32 *)PORT_M0(0x8009C96Cu))
#define G_HEALTH     (*(u8 *)PORT_M0(0x8009C970u))
#define G_MAXHEALTH  (*(u8 *)PORT_M0(0x8009C971u))
#define G_LIVES      (*(u8 *)PORT_M0(0x8009C980u))
#define G_WEAPON     (*(u8 *)PORT_M0(0x8009D2B2u))
#define G_NEXT_AREA  (*(u16 *)PORT_M0(0x8009CD94u))
#define G_NEXT_SEC   (*(u16 *)PORT_M0(0x8009CD96u))
#define G_NEXT_SPAWN (*(u16 *)PORT_M0(0x8009CDA0u))
#define G_EVENT(i)   (*(u8 *)PORT_M0(0x8009CDA4u + (i)))
#define GV_FREEZE    (*(u8 *)PORT_M0(0x8009C93Eu))   /* GVARS.b3e: updatePlayer returns at once (player kept visible) */

static const struct { int area, n; } rooms[] = {{0, 6}, {1, 5}, {2, 6}, {3, 6}, {4, 20}, {5, 4}, {6, 3}, {8, 4}, {9, 7},
    {10, 9}, {11, 5}, {13, 2}, {14, 8}, {16, 7}, {17, 11}, {18, 3}, {19, 3}};
#define NROOMS (int)(sizeof rooms / sizeof rooms[0])
static int door_room(int a, int s) {   /* sections with an empty load list (port_mods.DOOR_ROOMS): not warpable */
    static const u8 d[][2] = {{11, 0}, {11, 3}, {11, 4}, {16, 0}, {16, 1}, {16, 2}, {17, 0}, {17, 6}, {17, 7}, {17, 8}, {17, 9}, {17, 10}, {18, 0}};
    for (unsigned i = 0; i < sizeof d / sizeof d[0]; i++) if (d[i][0] == a && d[i][1] == s) return 1;
    return 0;
}

int port_debug_noclip_obj(const void *o) { return port_debug_noclip && (const u8 *)o == PLAYER; }
static int wall_frame = -100, wall_h, wall_y, wall_r;
int port_debug_wall(const void *o, int r) {   /* every wall push result (the collision readout: the player's hits) */
    if (r && (const u8 *)o == PLAYER && port_debug_menu) {
        s32 *h = *(s32 **)((u8 *)o + 0x40);
        wall_frame = port_frame; wall_r = r; wall_h = port_is_ram_ptr(h) ? *h >> 16 : 0; wall_y = *(s32 *)((u8 *)o + 0x14) >> 16;
        if (getenv("PORT_DEBUG_WALLLOG")) port_printf("[debug] frame %d: player wall hit %d at %d,%d\n", port_frame, r, wall_h, wall_y);
    }
    return r;
}

static u8 *task_main(void) { return *(u8 **)PORT_SPAD(0x1D4); }
static int in_gameplay(void) {
    u8 *t = task_main(); u16 *w;
    if (!port_is_ram_ptr(t)) return 0;
    w = (u16 *)(t + 0x48);
    return w[0] == 1 && w[1] == 1 && w[2] != 3 && w[2] != 7;
}
static void say(const char *s) { snprintf(msg, sizeof msg, "%s", s); msg_t = 150; port_printf("[debug] %s\n", s); }

/* ---- menu lines ---- */
enum { L_NOCLIP, L_FLY, L_HP, L_AP, L_LIVES, L_MAXHP, L_ITEM, L_WEAPON, L_EVENT, L_ALLEV, L_AREA, L_SEC, L_SPAWN, L_WARP,
       L_OVERLAY, L_SPEED, L_CUT, L_COUNT };
static const char *onoff(int v) { return v ? "ON" : "OFF"; }
static void line_text(int i, char *b, int n) {
    extern int port_cut_content;
    static const char *sp[] = {"NORMAL", "SLOW 1/2", "FAST"};
    switch (i) {
    case L_NOCLIP: snprintf(b, n, "NOCLIP (WALLS)      %s", onoff(port_debug_noclip)); break;
    case L_FLY: snprintf(b, n, "FLY (DPAD L1 R1)    %s", onoff(fly)); break;
    case L_HP: snprintf(b, n, "INFINITE VITALITY   %s", onoff(inf_hp)); break;
    case L_AP: snprintf(b, n, "MAX AP (99999999)   %s", onoff(inf_ap)); break;
    case L_LIVES: snprintf(b, n, "INFINITE LIVES      %s", onoff(inf_lives)); break;
    case L_MAXHP: snprintf(b, n, "MAX VITALITY 16     [X]"); break;
    case L_ITEM: snprintf(b, n, "GIVE ITEM  < %3d >  [X]", item_id); break;
    case L_WEAPON: snprintf(b, n, "EQUIP WEAPON < %d >  [X]", weapon); break;
    case L_EVENT: snprintf(b, n, "EVENT <%3d> %02X X=ST []=DONE O=CLR", event_id, G_EVENT(event_id)); break;
    case L_ALLEV: snprintf(b, n, "COMPLETE ALL EVENTS [X]"); break;
    case L_AREA: snprintf(b, n, "WARP AREA    < %2d >", rooms[w_area].area); break;
    case L_SEC: snprintf(b, n, "WARP SECTION < %2d >%s", w_sec, door_room(rooms[w_area].area, w_sec) ? " (NO)" : ""); break;
    case L_SPAWN: snprintf(b, n, "WARP SPAWN   < %2d >", w_spawn); break;
    case L_WARP: snprintf(b, n, "WARP NOW            [X]"); break;
    case L_OVERLAY: snprintf(b, n, "INFO OVERLAY        %s", onoff(overlay)); break;
    case L_SPEED: snprintf(b, n, "SPEED  < %s >", sp[speed]); break;
    case L_CUT: snprintf(b, n, "CUT CONTENT (NEXT ROOM) %s", onoff(port_cut_content)); break;
    }
}
static int wrap(int v, int n) { return (v % n + n) % n; }
static void adjust(int i, int d) {
    switch (i) {
    case L_ITEM: item_id = wrap(item_id + d, 256); break;
    case L_WEAPON: weapon = wrap(weapon + d, 10); break;
    case L_EVENT: event_id = wrap(event_id + d, 256); break;
    case L_AREA: w_area = wrap(w_area + d, NROOMS); w_sec = 0; break;
    case L_SEC: w_sec = wrap(w_sec + d, rooms[w_area].n); break;
    case L_SPAWN: w_spawn = wrap(w_spawn + d, 16); break;
    case L_SPEED: speed = wrap(speed + d, 3); break;
    default: break;
    }
}
static void activate(int i, u16 press) {
    extern int port_cut_content;
    int x = press & 0x4000, sq = press & 0x8000, ci = press & 0x2000;
    if (i == L_EVENT) {   /* X / Square / Circle: the flag byte only (safe in any room); L1 / R1: the game's
                             setEventStarted / setEventComplete (banner, AP, jingle: events of the current area only,
                             another area's title crashes in acquireSpriteSlot, as --title N:start does) */
        if (x) { G_EVENT(event_id) = 1; say("EVENT FLAG = 1 (STARTED)"); }
        else if (sq) { G_EVENT(event_id) = 0xFF; say("EVENT FLAG = FF (COMPLETED)"); }
        else if (ci) { G_EVENT(event_id) = 0; say("EVENT FLAG CLEARED"); }
        else if (press & 0x0400) { action_arg = event_id; action = A_EV_START; }
        else if (press & 0x0800) { action_arg = event_id; action = A_EV_DONE; }
        return;
    }
    if (!x) return;
    switch (i) {
    case L_NOCLIP: port_debug_noclip = !port_debug_noclip; say(port_debug_noclip ? "NOCLIP ON" : "NOCLIP OFF"); break;
    case L_FLY: fly = !fly; if (!fly) GV_FREEZE = 0; say(fly ? "FLY ON" : "FLY OFF"); break;
    case L_HP: inf_hp = !inf_hp; say(inf_hp ? "INFINITE VITALITY ON" : "INFINITE VITALITY OFF"); break;
    case L_AP: inf_ap = !inf_ap; say(inf_ap ? "MAX AP ON" : "MAX AP OFF"); break;
    case L_LIVES: inf_lives = !inf_lives; break;
    case L_MAXHP: G_MAXHEALTH = 16; G_HEALTH = 16; say("MAX VITALITY 16"); break;
    case L_ITEM: action_arg = item_id; action = A_ITEM; break;
    case L_WEAPON: G_WEAPON = (u8)weapon; say("WEAPON EQUIPPED"); break;
    case L_ALLEV: action = A_ALL_EVENTS; break;
    case L_AREA: case L_SEC: case L_SPAWN: case L_WARP:
        if (door_room(rooms[w_area].area, w_sec)) { say("ROOM HAS NO LOAD LIST"); break; }
        action = A_WARP; break;
    case L_OVERLAY: overlay = !overlay; break;
    case L_SPEED: speed = (speed + 1) % 3; break;
    case L_CUT: port_cut_content = !port_cut_content; break;
    }
}

/* every vblank (pad.c), after the host input is merged: returns the buttons the game sees */
void port_debug_pad(u16 *buttons) {
    u16 b = *buttons, press = b & ~prev;
    int combo = (b & 0x0C01) == 0x0C01 && (prev & 0x0C01) != 0x0C01;
    prev = b;
    swallow &= b;
    {   /* the vblank rate for the overlay */
        unsigned long long t = port_time_us();
        if (!fps_t0) { fps_t0 = t; fps_n0 = port_frame; }
        else if (t - fps_t0 >= 500000ull) { fps_x100 = (int)((unsigned long long)(port_frame - fps_n0) * 100000000ull / (t - fps_t0)); fps_t0 = t; fps_n0 = port_frame; }
    }
    if (msg_t > 0) msg_t--;
    if (combo || port_debug_key) {
        port_debug_key = 0; menu_open = !menu_open; swallow = b;
        port_printf("[debug] frame %d: menu %s\n", port_frame, menu_open ? "open" : "closed");
        *buttons = 0; return;
    }
    /* live cheats (memory only: safe from the vblank, like --poke-every) */
    if (inf_hp) { if (G_MAXHEALTH < 4) G_MAXHEALTH = 4; G_HEALTH = G_MAXHEALTH; *(s16 *)(PLAYER + 0x98) = G_MAXHEALTH; }
    if (inf_ap) G_AP = 99999999u;
    if (inf_lives) G_LIVES = 99;
    /* speed: realtime pacing off = as fast as the host runs; slow = one extra period of sleep per vblank */
    if (speed == 2 && saved_realtime < 0) { saved_realtime = port_realtime; port_realtime = 0; }
    if (speed != 2 && saved_realtime >= 0) { port_realtime = saved_realtime; saved_realtime = -1; }
    if (speed == 1) port_sleep_us(2000000ull / (unsigned)port_hz + 1000);   /* late by more than a period: psyq.c re-anchors its schedule, so each vblank takes ~2 periods */
    if (menu_open) {
        if (press & 0x1008 && !(b & 0x0C00)) { menu_open = 0; swallow = b; *buttons = 0; return; }   /* Triangle / Start */
        if (press & 0x0010) cursor = wrap(cursor - 1, L_COUNT);
        if (press & 0x0040) cursor = wrap(cursor + 1, L_COUNT);
        if (press & 0x0080) adjust(cursor, (b & 0x0100) ? -10 : -1);   /* L2 held: steps of 10 */
        if (press & 0x0020) adjust(cursor, (b & 0x0100) ? 10 : 1);
        if (press & 0xEC00) activate(cursor, press);
        *buttons = 0;
        return;
    }
    *buttons = b &= ~swallow;
    if (fly) {   /* fly: the player's update is suspended (GVARS.b3e) and the d-pad moves it; Cross held = fast */
        u8 *pl = PLAYER; s32 *h = *(s32 **)(pl + 0x40), *d = *(s32 **)(pl + 0x44);
        int v = (b & 0x4000) ? 8 : 2;
        GV_FREEZE = 1;
        if (port_is_ram_ptr(h)) { if (b & 0x0080) *h -= v << 16; if (b & 0x0020) *h += v << 16; }
        if (b & 0x0010) *(s32 *)(pl + 0x14) -= v << 16;
        if (b & 0x0040) *(s32 *)(pl + 0x14) += v << 16;
        if (port_is_ram_ptr(d)) { if (b & 0x0400) *d -= v << 16; if (b & 0x0800) *d += v << 16; }
        *(s16 *)(pl + 0x7C) = *(s16 *)(pl + 0x7E) = *(s16 *)(pl + 0x80) = *(s16 *)(pl + 0x82) = 0;
        *buttons = b & ~0x4CF0;   /* the d-pad, L1/R1 and Cross belong to the flight */
    }
}

/* from the game loop (port/fixes.txt gameMainTask, after ThreadWaitFrames): the game's own functions */
void port_debug_tick(void) {
    extern void addItemToInventory(int, int, int), setEventStarted(int, int, int), setEventComplete(int, int);
    int a = action, arg = action_arg; char b[48];
    if (!port_debug_menu || a == A_NONE) return;
    action = A_NONE;
    if (!in_gameplay()) { say("NOT IN GAMEPLAY"); return; }
    switch (a) {
    case A_ITEM: addItemToInventory(arg, 1, 1); snprintf(b, sizeof b, "ITEM %d GIVEN", arg); say(b); break;
    case A_EV_START: G_EVENT(arg) = 0; setEventStarted(arg, 0, 0); snprintf(b, sizeof b, "EVENT %d STARTED", arg); say(b); break;
    case A_EV_DONE: setEventComplete(arg, 0); snprintf(b, sizeof b, "EVENT %d COMPLETED", arg); say(b); break;
    case A_ALL_EVENTS: for (int i = 0; i < 0x82; i++) G_EVENT(i) = 0xFF; say("ALL EVENTS COMPLETED"); break;
    case A_WARP: {   /* as raceOfferRematch does: next area/section/spawn, then the task's area transition mode 7 */
        u8 *t = task_main();
        if (fly) { fly = 0; GV_FREEZE = 0; }
        G_NEXT_AREA = (u16)rooms[w_area].area; G_NEXT_SEC = (u16)w_sec; G_NEXT_SPAWN = (u16)w_spawn;
        *(s16 *)(t + 0x4C) = 7; *(s16 *)(t + 0x4E) = 0;
        snprintf(b, sizeof b, "WARP TO %d-%d SPAWN %d", rooms[w_area].area, w_sec, w_spawn); say(b);
        menu_open = 0; swallow = 0xFFFF;
        break; }
    }
}

/* ---- overlay ---- */
static const u8 font[64][7] = {   /* ASCII 32..95, 5x7, bit 4 = left column */
    {0,0,0,0,0,0,0}, {4,4,4,4,4,0,4}, {10,10,0,0,0,0,0}, {10,31,10,10,31,10,0}, {4,15,20,14,5,30,4}, {24,25,2,4,8,19,3},
    {12,18,20,8,21,18,13}, {4,4,0,0,0,0,0}, {2,4,8,8,8,4,2}, {8,4,2,2,2,4,8}, {0,4,21,14,21,4,0}, {0,4,4,31,4,4,0},
    {0,0,0,0,12,4,8}, {0,0,0,31,0,0,0}, {0,0,0,0,0,12,12}, {0,1,2,4,8,16,0},
    {14,17,19,21,25,17,14}, {4,12,4,4,4,4,14}, {14,17,1,2,4,8,31}, {31,2,4,2,1,17,14}, {2,6,10,18,31,2,2},
    {31,16,30,1,1,17,14}, {6,8,16,30,17,17,14}, {31,1,2,4,8,8,8}, {14,17,17,14,17,17,14}, {14,17,17,15,1,2,12},
    {0,12,12,0,12,12,0}, {0,12,12,0,12,4,8}, {2,4,8,16,8,4,2}, {0,0,31,0,31,0,0}, {8,4,2,1,2,4,8}, {14,17,1,2,4,0,4},
    {14,17,23,21,23,16,14}, {14,17,17,17,31,17,17}, {30,17,17,30,17,17,30}, {14,17,16,16,16,17,14}, {28,18,17,17,17,18,28},
    {31,16,16,30,16,16,31}, {31,16,16,30,16,16,16}, {14,17,16,23,17,17,15}, {17,17,17,31,17,17,17}, {14,4,4,4,4,4,14},
    {7,2,2,2,2,18,12}, {17,18,20,24,20,18,17}, {16,16,16,16,16,16,31}, {17,27,21,21,17,17,17}, {17,17,25,21,19,17,17},
    {14,17,17,17,17,17,14}, {30,17,17,30,16,16,16}, {14,17,17,17,21,18,13}, {30,17,17,30,20,18,17}, {15,16,16,14,1,1,30},
    {31,4,4,4,4,4,4}, {17,17,17,17,17,17,14}, {17,17,17,17,17,10,4}, {17,17,17,21,21,21,10}, {17,17,10,4,10,17,17},
    {17,17,17,10,4,4,4}, {31,1,2,4,8,16,31}, {14,8,8,8,8,8,14}, {0,16,8,4,2,1,0}, {14,2,2,2,2,2,14}, {4,10,17,0,0,0,0},
    {0,0,0,0,0,0,31}};
extern int port_gpu_overlay_fill(int x, int y, int w, int h, u16 c);   /* shims/gpu.c: display-relative; 0 = no frame */
extern void port_gpu_overlay_end(void);
static void text(int x, int y, const char *s, u16 col) {
    for (; *s; s++, x += 6) {
        int c = *s; if (c >= 'a' && c <= 'z') c -= 32;
        if (c < 32 || c > 95) c = '?';
        for (int r = 0; r < 7; r++) {
            u8 bits = font[c - 32][r];
            for (int k = 0; k < 5;) {
                if (!(bits & (16 >> k))) { k++; continue; }
                int e = k; while (e < 5 && (bits & (16 >> e))) e++;
                port_gpu_overlay_fill(x + k + 1, y + r + 1, e - k, 1, 0x0000);   /* shadow */
                port_gpu_overlay_fill(x + k, y + r, e - k, 1, col);
                k = e;
            }
        }
    }
}
void port_debug_draw(void) {
    char b[64];
    const u16 white = 0x7FFF, yellow = 0x03FF, green = 0x03E0, grey = 0x4210;
    if (!port_gpu_overlay_fill(0, 0, 0, 0, 0)) return;   /* no 15-bit display (24-bit movie) */
    if (overlay) {
        port_gpu_overlay_fill(2, 2, 200, port_frame - wall_frame < 10 ? 38 : 29, 0x0842);
        u8 *pl = PLAYER; s32 *h = *(s32 **)(pl + 0x40), *d = *(s32 **)(pl + 0x44);
        snprintf(b, sizeof b, "F%d %d.%02dFPS A%d-%d", port_frame, fps_x100 / 100, fps_x100 % 100, G_AREA, G_SECTION);
        text(4, 4, b, green);
        snprintf(b, sizeof b, "H%d Y%d D%d HP%d/%d", port_is_ram_ptr(h) ? *h >> 16 : 0, *(s32 *)(pl + 0x14) >> 16,
                 port_is_ram_ptr(d) ? *d >> 16 : 0, G_HEALTH, G_MAXHEALTH);
        text(4, 13, b, green);
        if (port_debug_noclip || fly || inf_hp || inf_ap || speed) {
            snprintf(b, sizeof b, "%s%s%s%s%s", port_debug_noclip ? "NOCLIP " : "", fly ? "FLY " : "", inf_hp ? "INFHP " : "",
                     inf_ap ? "MAXAP " : "", speed == 1 ? "SLOW" : speed == 2 ? "FAST" : "");
            text(4, 22, b, yellow);
        }
        if (port_frame - wall_frame < 10) { snprintf(b, sizeof b, "WALL HIT %d AT %d,%d", wall_r, wall_h, wall_y); text(4, 31, b, 0x001F); }
    }
    if (msg_t > 0) text(4, 220, msg, yellow);
    if (menu_open) {
        int x0 = 20, y0 = 50, lh = 10;
        port_gpu_overlay_fill(x0 - 4, y0 - 14, 288, L_COUNT * lh + 26, 0x0C63);
        text(x0, y0 - 11, "DEBUG MENU  X=OK  TRI=CLOSE  L2=x10", white);
        if (cursor == L_EVENT) text(x0, y0 + L_COUNT * lh + 1, "L1/R1: GAME START/COMPLETE (THIS AREA)", grey);
        for (int i = 0; i < L_COUNT; i++) {
            line_text(i, b, sizeof b);
            text(x0 + 8, y0 + i * lh, b, i == cursor ? yellow : (i == L_CUT ? grey : white));
            if (i == cursor) text(x0, y0 + i * lh, ">", yellow);
        }
    }
    port_gpu_overlay_end();
}
