/* libpad replacement.  The game's pad driver (PadInitDirect, the _pad* SIO state machine, ...) talks to the
 * controller port registers; port/tools/gen_port.py leaves those files out (GAME_SKIP) and this file provides
 * the Psy-Q API on top of host input.  Each vblank fills the receive buffers the game registered with
 * PadInitDirect exactly like the driver does: [0] = 0 (ok), [1] = 0x41 (digital pad), [2..3] = buttons,
 * active low.  The game reads them in readPadPort / updatePadInput.  Carried over from port/shims/pad.c.
 *
 * --dualshock (launcher: "Mando DualShock (analógico)"): the pad reports itself as a DualShock in analog mode (id 0x73)
 * with the left stick in bytes 6 / 7, as the real driver fills them.  Tombi! supports it natively: readPadPort keeps the
 * stick for type 7, playerDecodePadInput turns it into three walking speeds (dead zone 0x50..0xAF, then 0x30 / 0x10
 * steps), setVelXFromPadLR / playerUpdateSpeedFromPad scale the speed by it, and menus see the d-pad bits the driver
 * derives at the stick's ends.  As on the real pad in analog mode the game ignores the d-pad bits where it reads the
 * stick (playerDecodePadInput rebuilds them from the axes), so a held d-pad direction is sent as the stick at that end.
 * The game's vibration (startPadVibration / updatePadVibration: PadSetAct with the small motor on/off in byte 0 and the
 * big motor's level in byte 1, when the player is hurt, grabbed, bitten...) goes to the host controller's rumble
 * (port_window_rumble: SDL_GameControllerRumble on the PC, the controller's or the phone's vibrator on Android). */
#include "port_runtime.h"

typedef unsigned char u8; typedef unsigned short u16;
static u8 *buf[2];
static int started;
u16 port_pad_buttons[2];       /* active high, PSX bit order: 0 Select,3 Start,4 Up,5 Right,6 Down,7 Left,
                                  8 L2,9 R2,10 L1,11 R1,12 Triangle,13 Circle,14 Cross,15 Square */
int port_autostart;            /* headless test input: tap Start (and Cross) every autostart frames */
int port_dualshock;            /* --dualshock: analog pad (0x73) with the host's left stick, and vibration */
int port_pad_lx = 0x80, port_pad_ly = 0x80;   /* the host's left stick, 0..255 (0x80 centre; set by runtime/sdl.c) */
static const u8 *act_data;     /* PadSetAct: the game's actuator bytes (read every vblank, as the driver does) */
static int act_sent = -1;
void port_window_rumble(int small, int big);

/* --pad-script "FRAME:BUTTONS[:LEN[xCOUNT/PERIOD]],...": hold BUTTONS (names joined by '+': start select up down
   left right cross circle square triangle l1 r1 l2 r2, or a hex mask) from vblank FRAME for LEN vblanks (default
   4); with xCOUNT/PERIOD the press repeats COUNT times, every PERIOD vblanks. */
#define MAX_SCRIPT 64
static struct { int frame, len, count, period; u16 mask; } script[MAX_SCRIPT];
static int n_script;
void port_pad_script_reset(void) { n_script = 0; port_autostart = 0; }   /* before the command line is parsed again (--load) */
static const struct { const char *n; u16 m; } pad_names[] = {
    {"select", 0x0001}, {"start", 0x0008}, {"up", 0x0010}, {"right", 0x0020}, {"down", 0x0040}, {"left", 0x0080},
    {"l2", 0x0100}, {"r2", 0x0200}, {"l1", 0x0400}, {"r1", 0x0800}, {"triangle", 0x1000}, {"circle", 0x2000},
    {"cross", 0x4000}, {"square", 0x8000}, {0, 0}};
static int num(const char **p, int base) {
    int v = 0;
    for (;;) {
        char c = **p; int d;
        if (c >= '0' && c <= '9') d = c - '0';
        else if (base == 16 && (c | 32) >= 'a' && (c | 32) <= 'f') d = (c | 32) - 'a' + 10;
        else return v;
        v = v * base + d; (*p)++;
    }
}
void port_pad_script(const char *s) {
    while (*s && n_script < MAX_SCRIPT) {
        int f = num(&s, 10), len = 4; u16 m = 0;
        if (*s == ':') s++;
        while (*s && *s != ':' && *s != ',') {
            if (s[0] == '0' && s[1] == 'x') { s += 2; m |= num(&s, 16); }
            else {
                int k, hit = 0;
                for (k = 0; pad_names[k].n; k++) {
                    const char *a = pad_names[k].n, *b = s;
                    while (*a && *a == (*b | 32)) { a++; b++; }
                    if (!*a && (*b == '+' || *b == ':' || *b == ',' || !*b)) { m |= pad_names[k].m; s = b; hit = 1; break; }
                }
                if (!hit) port_fatal("--pad-script: unknown button at \"%s\"", s);
            }
            if (*s == '+') s++;
        }
        int count = 1, period = 0;
        if (*s == ':') { s++; len = num(&s, 10); }
        if (*s == 'x') { s++; count = num(&s, 10); if (*s == '/') { s++; period = num(&s, 10); } }
        if (*s && *s != ',') port_fatal("--pad-script: bad entry at \"%s\"", s);
        if (*s == ',') s++;
        script[n_script].frame = f; script[n_script].len = len; script[n_script].mask = m;
        script[n_script].count = count; script[n_script].period = period > 0 ? period : (len > 0 ? len : 1); n_script++;
    }
}

void port_pad_vblank(void) {
    if (n_script) {
        u16 m = 0;
        for (int i = 0; i < n_script; i++) {
            int d = port_frame - script[i].frame;
            if (d >= 0 && d / script[i].period < script[i].count && d % script[i].period < script[i].len) m |= script[i].mask;
        }
        port_pad_buttons[0] = m;
    }
    {   /* --window: the keyboard / game controller state (merged with a script, if any) */
        u16 w;
        if (port_window_poll_pad(&w)) { if (n_script) port_pad_buttons[0] |= w; else port_pad_buttons[0] = w; }
    }
    if (port_autostart && port_frame % port_autostart < 4)
        port_pad_buttons[0] = (port_frame / port_autostart) & 1 ? 0x4000 : 0x0008;
    else if (port_autostart)
        port_pad_buttons[0] = 0;
    { extern int port_debug_menu; extern void port_debug_pad(u16 *); if (port_debug_menu) port_debug_pad(&port_pad_buttons[0]); }
    if (!started) return;
    for (int i = 0; i < 2; i++) {
        if (!buf[i]) continue;
        if (i == 1) { buf[i][0] = 0xFF; continue; }         /* port 2: nothing connected */
        u16 b = (u16)~port_pad_buttons[i];
        buf[i][0] = 0; buf[i][1] = 0x41; buf[i][2] = b & 0xFF; buf[i][3] = b >> 8;
        if (port_dualshock) {
            int lx = port_pad_lx, ly = port_pad_ly, m = port_pad_buttons[i];
            if (m & 0x80) lx = 0x00; else if (m & 0x20) lx = 0xFF;     /* the d-pad: the stick at that end */
            if (m & 0x10) ly = 0x00; else if (m & 0x40) ly = 0xFF;
            buf[i][1] = 0x73; buf[i][4] = 0x80; buf[i][5] = 0x80; buf[i][6] = (u8)lx; buf[i][7] = (u8)ly;
        }
    }
    if (port_dualshock && act_data) {
        int v = (act_data[0] ? 0x100 : 0) | act_data[1];
        if (v != act_sent) { act_sent = v; port_window_rumble(act_data[0] != 0, act_data[1]); }
    }
}

/* The JP game (SLPS-01144) drives the pads with the older libetc (PadInit / PadRead over the BIOS InitPAD buffers), not
   libpad: its vblank handler reads PadRead(1) (port/fixes.txt, @version jp).  PadRead's word is pad 1 in the low half,
   pad 2 in the high half, each the two BIOS bytes big-endian and inverted (pressed = 1): select 0x0100, start 0x0800,
   up/right/down/left 0x1000..0x8000, L2 R2 L1 R1 0x01..0x08, triangle circle cross square 0x10..0x80. */
int port_pad_read_libetc(void) {
    u16 b = port_pad_buttons[0];
    return ((b & 0xFF) << 8) | (b >> 8);
}
void PadInitDirect(u8 *a, u8 *b) {
    if (port_verbose) port_printf("[port] PadInitDirect(%p, %p) frame %d\n", a, b, port_frame);
    buf[0] = a; buf[1] = b; if (a) a[0] = 0xFF; if (b) b[0] = 0xFF;
}
void PadStartCom(void) { if (port_verbose) port_printf("[port] PadStartCom frame %d\n", port_frame); started = 1; }
void PadStopCom(void) { if (port_verbose) port_printf("[port] PadStopCom frame %d\n", port_frame); started = 0; }
int PadChkVsync(void) { return 0; }
int PadGetState(int port) { return port == 0 ? 6 /* PadStateStable */ : 0 /* PadStateDiscon */; }
int PadInfoMode(int port, int term, int offs) {
    if (port_verbose > 1 && port == 0 && buf[0])
        port_printf("[pad] frame %d: game reads %04x\n", port_frame, (~(buf[0][2] | buf[0][3] << 8)) & 0xFFFF);
    if (port_dualshock && port == 0) {
        if (term == 1) return 7;   /* InfoModeCurID: analog (DualShock) */
        if (term == 2) return 0x73; /* InfoModeCurExID: a DualShock (setAnalogPadMode asks it) */
        return 0;
    }
    if (term == 1) return 4;      /* InfoModeCurID: standard digital pad */
    return 0;                      /* no analog modes, no actuators */
}
int PadInfoAct(int port, int act, int term) { return 0; }
int PadInfoComb(int port, int a, int b) { return 0; }
int PadSetAct(int port, u8 *data, int len) { if (port == 0 && len >= 2) act_data = data; return 1; }
int PadSetActAlign(int port, u8 *data) { return 1; }
int PadSetMainMode(int port, int offs, int lock) { return 1; }
int PadChkMtap(void) { return 0; }
