/* Widescreen exits (docs/PORT.md "Widescreen exits").  Many side-view rooms have an exit at the 4:3 screen edge (when
   the camera is at its limit): on a wide frame Tomba left the room well inside the picture.  With --widescreen, in a
   room with such exits (tools/colview_exits.json "ws", generated into colview_exits.inc):
   - port_ws_exit_hold: tryStartItemInteraction (port/fixes.txt) ignores the exit call while Tomba is inside the 4:3
     box but not inside the box moved out by the pad, so the exit fires at the same distance from the wide edge;
   - port_ws_side_margin: on the side of such an exit, the camera clamps that keep Tomba 150 / 0x90 units from the
     camera limit or centre let him walk the pad further, and getCollisionPlaneAt repeats the edge column (fixes.txt).
   In side view 1 world unit is about 1 column at Tomba's depth, so the pad in units is the pad in columns. */
#include "port_runtime.h"
#include "ws_exits.h"

const Exit port_exits[] = {
#include "colview_exits.inc"
};
const int port_exit_count = sizeof port_exits / sizeof port_exits[0];

extern int port_ws_cull;
/* the most the exits move: 21:9's pad.  Wider (32:9, pad 267) took AREA00 s5 and AREA09 s0 past what Tomba can reach
   (found by the headless sweep), so there the exits stop 147 columns short of the edge */
#define WS_EXIT_MAX 120
int port_ws_exits = 1;                        /* --ws-exits off: the 4:3 exits (for comparison) */

static int room(int *area, int *sec)
{
    *area = *(unsigned short *)PORT_M0(PORT_A_GAME); *sec = *(unsigned short *)PORT_M0(PORT_A_GAME + 2);
    return *sec < 32;
}

/* Room sides opened up to the wide edge without a moved exit there (decided room by room, from renders: a wide frame
   does not always mean wider limits).  sides: 1 = left, 2 = right */
static const struct { unsigned char area; unsigned int secs; unsigned char sides; } WALK[] = {
    { 5, 1u << 1 | 1u << 3, 2 },     /* AREA05 s1/s3 (one room, two states): the square goes on to the right */
};

/* the pad when Tomba may walk past the 4:3 limit on that side (right: 1 / left: 0; -1: either): a moved exit there, or
   WALK; else 0 */
int port_ws_side_margin(int right)
{
    static int last_area = -1, last_sec = -1, sides = 0;
    int area, sec;
    if (!port_ws_cull || !port_ws_exits || !room(&area, &sec)) return 0;
    if (area != last_area || sec != last_sec) {
        last_area = area; last_sec = sec; sides = 0;
        for (int i = 0; i < port_exit_count; i++) {
            const Exit *x = &port_exits[i];
            if (x->area != area || !(x->secs >> sec & 1)) continue;
            if (x->ws0 > 0 || x->ws1 > 0) sides |= 2;
            if (x->ws0 < 0 || x->ws1 < 0) sides |= 1;
        }
        for (unsigned i = 0; i < sizeof WALK / sizeof WALK[0]; i++)
            if (WALK[i].area == area && (WALK[i].secs >> sec & 1)) sides |= WALK[i].sides;
    }
    if (!(sides & (right < 0 ? 3 : right ? 2 : 1))) return 0;
    return port_ws_cull < WS_EXIT_MAX ? port_ws_cull : WS_EXIT_MAX;
}
int port_ws_player_margin(void) { return port_ws_side_margin(-1); }

static int in(int v, int a, int b) { return v >= a && v <= b; }

/* tryStartItemInteraction(o, mode, exit): 1 = not yet (Tomba is in the 4:3 box of a moved exit, not in the moved one) */
int port_ws_exit_hold(int mode, int exit)
{
    int m = port_ws_player_margin(), area, sec, hold = 0;
    (void)mode;
    if (!m) return 0;
    room(&area, &sec);
    int H = *(short *)PORT_SPAD(0x16A), Y = *(short *)PORT_SPAD(0x16E), D = *(short *)PORT_SPAD(0x172);
    for (int i = 0; i < port_exit_count; i++) {
        const Exit *x = &port_exits[i];
        if (x->area != area || !(x->secs >> sec & 1) || x->exit != exit) continue;
        if (!in(H, x->h0, x->h1) || !in(Y, x->y0, x->y1) || !in(D, x->d0, x->d1)) continue;
        if (!x->ws0 && !x->ws1) return 0;               /* a box the port leaves alone holds Tomba too */
        int h0 = x->h0 == -32768 ? x->h0 : x->h0 + x->ws0 * m, h1 = x->h1 == 32767 ? x->h1 : x->h1 + x->ws1 * m;
        if (!in(H, h0, h1)) hold = 1;
    }
    return hold;
}
