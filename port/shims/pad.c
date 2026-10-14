/* libpad replacement.  The game's pad driver (PadInitDirect, the _pad* SIO state machine, ...) talks to the
 * controller port registers; port/tools/gen_port.py leaves those files out (GAME_SKIP) and this file provides
 * the Psy-Q API on top of host input.  Each vblank fills the receive buffers the game registered with
 * PadInitDirect exactly like the driver does: [0] = 0 (ok), [1] = 0x41 (digital pad), [2..3] = buttons,
 * active low.  The game reads them in readPadPort / updatePadInput. */
#include "port_runtime.h"

typedef unsigned char u8; typedef unsigned short u16;
static u8 *buf[2];
static int started;
u16 port_pad_buttons[2];       /* active high, PSX bit order: 0 Select,3 Start,4 Up,5 Right,6 Down,7 Left,
                                  8 L2,9 R2,10 L1,11 R1,12 Triangle,13 Circle,14 Cross,15 Square */
int port_autostart;            /* headless test input: tap Start (and Cross) every autostart frames */

void port_pad_vblank(void) {
    if (port_autostart && port_frame % port_autostart < 4)
        port_pad_buttons[0] = (port_frame / port_autostart) & 1 ? 0x4000 : 0x0008;
    else if (port_autostart)
        port_pad_buttons[0] = 0;
    if (!started) return;
    for (int i = 0; i < 2; i++) {
        if (!buf[i]) continue;
        if (i == 1) { buf[i][0] = 0xFF; continue; }         /* port 2: nothing connected */
        u16 b = (u16)~port_pad_buttons[i];
        buf[i][0] = 0; buf[i][1] = 0x41; buf[i][2] = b & 0xFF; buf[i][3] = b >> 8;
    }
}

void PadInitDirect(u8 *a, u8 *b) { buf[0] = a; buf[1] = b; if (a) a[0] = 0xFF; if (b) b[0] = 0xFF; }
void PadStartCom(void) { started = 1; }
void PadStopCom(void) { started = 0; }
int PadChkVsync(void) { return 0; }
int PadGetState(int port) { return port == 0 ? 6 /* PadStateStable */ : 0 /* PadStateDiscon */; }
int PadInfoMode(int port, int term, int offs) {
    if (term == 1) return 4;      /* InfoModeCurID: standard digital pad */
    return 0;                      /* no analog modes, no actuators */
}
int PadInfoAct(int port, int act, int term) { return 0; }
int PadInfoComb(int port, int a, int b) { return 0; }
int PadSetAct(int port, u8 *data, int len) { return 1; }
int PadSetActAlign(int port, u8 *data) { return 1; }
int PadSetMainMode(int port, int offs, int lock) { return 1; }
int PadChkMtap(void) { return 0; }
