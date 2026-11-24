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

/* the GAME area / section; 0 when the section is 32 or more */
static int room(int *area, int *sec)
{
    *area = *(unsigned short *)PORT_M0(PORT_A_GAME);
    *sec = *(unsigned short *)PORT_M0(PORT_A_GAME + 2);
    return *sec < 32;
}

/* Room sides opened up to the wide edge without a moved exit there (decided room by room, from renders: a wide frame
   does not always mean wider limits).  sides: 1 = left, 2 = right;
   d0..d1: the depth planes (lanes) whose WHM is carried on from the camera limit +- 0x80, dropping the 4:3 data's walls
   there; on the other planes the room's own WHM is kept to its end (+- 0xAA), walls included.
   cpu: screen columns per path unit on that lane (1000 = 1.0); on a deeper lane the clamps' margin is scaled so Tomba
   still reaches the wide edge (the clamps count units from the camera limit: 150 + pad units reach the edge at 1.0). */
static const struct { unsigned char area; unsigned int secs; unsigned char sides; short d0, d1, cpu; } WALK[] = {
    /* AREA05 s1/s3 (one room, two states): the square goes on to the right on both lanes; on the back lane (deeper:
       0.835 column per unit, measured on the fountain's wall) the fountain is an obstacle (OBST) */
    { 5, 1u << 1 | 1u << 3, 2, -45, 45, 1000 },
    { 5, 1u << 1 | 1u << 3, 2, 60, 120, 835 },
    /* AREA06 s2, the Mermaid's Singing Rock: the beach goes on (ws_backdrop_mermaid.inc) */
    { 6, 1u << 2, 2, -32768, 32767, 1000 },
};

/* the pad when Tomba may walk past the 4:3 limit on that side (right: 1 / left: 0; -1: either): a moved exit there, or
   WALK; else 0 */
static int sides;          /* this room's opened sides: 1 / 2 = left / right exit moved, 4 / 8 = left / right WALK */
int port_ws_side_margin(int right)
{
    static int last_area = -1, last_sec = -1;
    int area, sec;
    if (!port_ws_cull || !port_ws_exits || !room(&area, &sec)) return 0;
    if (area != last_area || sec != last_sec) {
        last_area = area;
        last_sec = sec;
        sides = 0;
        for (int i = 0; i < port_exit_count; i++) {
            const Exit *ex = &port_exits[i];
            if (ex->area != area || !(ex->secs >> sec & 1)) continue;
            if (ex->ws0 > 0 || ex->ws1 > 0) sides |= 2;
            if (ex->ws0 < 0 || ex->ws1 < 0) sides |= 1;
        }
        for (unsigned i = 0; i < sizeof WALK / sizeof WALK[0]; i++)
            if (WALK[i].area == area && (WALK[i].secs >> sec & 1)) sides |= WALK[i].sides << 2;
    }
    if (!(sides & (right < 0 ? 15 : right ? 10 : 5))) return 0;
    int pad = port_ws_cull < WS_EXIT_MAX ? port_ws_cull : WS_EXIT_MAX;
    if (right >= 0 && !(sides & (right ? 2 : 1))) {          /* a WALK side: the lane Tomba is on, at its scale */
        int lane = *(short *)PORT_SPAD(0x172);
        for (unsigned i = 0; i < sizeof WALK / sizeof WALK[0]; i++)
            if (WALK[i].area == area && (WALK[i].secs >> sec & 1) && (WALK[i].sides & (right ? 2 : 1))
                && lane >= WALK[i].d0 && lane <= WALK[i].d1)
                return WALK[i].cpu < 1000 ? (150 + pad) * 1000 / WALK[i].cpu - 150 : pad;
    }
    return pad;
}
int port_ws_player_margin(void) { return port_ws_side_margin(-1); }

/* getCollisionPlaneAt (fixes.txt): past the opened edge the column of the edge pixel x goes on.  Repeated as it is,
   its sloped pieces and walls came back every 8 units (a sawtooth floor, a wall every column): the copy keeps the
   floors, ceilings and filled blocks, each flat at its height at x, and drops the walls. */
static int port_ws_whm_rep;

/* getCollisionPlaneAt (fixes.txt), before its own range test: x past an opened edge is moved to the edge column, and
   port_ws_whm_col then gives the flattened copy of it.  Beside a moved exit the copy starts at the camera limit
   -/+ 0x80, a column Tomba always walks over on 4:3 before the exit (the WHM ends at -/+ 0xAA and may already stop
   or slope there).  On a WALK side (no exit) the same on the lanes WALK lists; on the others it starts only where
   the room's WHM ends, -/+ 0xAA, so a wall inside the 4:3 data stays (AREA05 s1: the fountain on the back lane, a
   wall at H 321, was walkable when every lane was carried on from +0x80). */
/* Obstacles in the WHM carried on past an opened edge: from h0 to h1 (path units) on the depth planes d0..d1 the
   floor and the filled block's top are raised by `up` units, so Tomba has to jump over (and may stand on top: the
   user's choice, 2026-11-17).  AREA05 s1/s3: the fountain (pump and trough) on the back lane, H about 311..354 (that
   lane is deeper: 0.84 column per unit); on 4:3 the room's wall at H 324, in the middle of it, stopped Tomba. */
static const struct { unsigned char area; unsigned int secs; short d0, d1, h0, h1, up; } OBST[] = {
    { 5, 1u << 1 | 1u << 3, 60, 120, 306, 346, 32 },
};
/* Lanes whose opened left side is not the edge column repeated but a plain ledge: from x < x0 a filled block with its
   floor at h.  AREA02 s1 (the Dwarf Village; the blossom village runs as 2:1 too), the back lane: its left end is
   the hatch down to the Underground Prison, a pit (floor 0) at x < 40 that Tomba reaches over the hut's roof, where
   exit 1 fires (H < 30, Y >= -77; kept at its 4:3 place, colview_exits.json).  Carried on from the column at the
   camera limit + 0x80 (x 32..39: the pit and the roof's left tip, a floor at -184) Tomba walked on air at the roof's
   height out to the wide edge.  Now the pit keeps its place and the ground the fence stands on (-88, as right of
   the hut) goes on left of it (the user's call, 2026-11-17). */
static const struct { unsigned char area; unsigned int secs; short d0, d1, x0, h; } LEDGE[] = {
    { 2, 1u << 1, 45, 32767, 8, -88 },
};
static int whm_ledge = -1;                /* the LEDGE row port_ws_whm_col makes the column from */
static int whm_xo, whm_d;                 /* the x and depth getCollisionPlaneAt was asked for (port_ws_whm_col) */

static int walk_lane(int d)               /* a WALK side of this room whose lane d is in: carried on from +- 0x80 */
{
    int area, sec;
    room(&area, &sec);
    for (unsigned i = 0; i < sizeof WALK / sizeof WALK[0]; i++)
        if (WALK[i].area == area && (WALK[i].secs >> sec & 1) && d >= WALK[i].d0 && d <= WALK[i].d1)
            return 1;
    return 0;
}
short port_ws_whm_x(short x, short d, short minx, short maxx)
{
    port_ws_whm_rep = 0;
    whm_ledge = -1;
    whm_xo = x;
    whm_d = d;
    if (!port_ws_side_margin(-1)) return x;
    {
        int area, sec;
        room(&area, &sec);
        for (unsigned i = 0; i < sizeof LEDGE / sizeof LEDGE[0]; i++)
            if (LEDGE[i].area == area && (LEDGE[i].secs >> sec & 1) && d >= LEDGE[i].d0 && d <= LEDGE[i].d1) {
                if (x >= LEDGE[i].x0) return x;             /* the room's own data, the pit included */
                port_ws_whm_rep = 1;
                whm_ledge = (int)i;
                return LEDGE[i].x0;
            }
    }
    int on_walk_lane = (sides & 12) && walk_lane(d);
    int lo = sides & 1 || (sides & 4 && on_walk_lane) ? minx - 0x80 : sides & 4 ? minx - 0xAA : -0x8000;
    int hi = sides & 2 || (sides & 8 && on_walk_lane) ? maxx + 0x80 : sides & 8 ? maxx + 0xAA : 0x7FFF;
    if (lo < 0 && *(unsigned short *)PORT_M0(PORT_A_GAME) != 0xe && (sides & 5)) lo = 0;   /* (the game's x < 0 test) */
    if ((sides & 5) && x < lo) {
        port_ws_whm_rep = 1;
        return (short)lo;
    }
    if ((sides & 10) && x > hi) {
        port_ws_whm_rep = 1;
        return (short)hi;
    }
    return x;
}

/* the flattened columns port_ws_whm_col hands out, 16 in turn: a piece count, then up to 32 pieces of 4 halfwords */
static unsigned short ring[16][1 + 32 * 4];
static int ring_next;
int port_ws_whm_is_copy(const void *p)
{
    return (const char *)p >= (const char *)ring && (const char *)p < (const char *)(ring + 16);
}
char *port_ws_whm_col(char *col, int x)
{
    const unsigned short *column = (const unsigned short *)col;
    int piece_count = column[0], n = 0;
    if (!port_ws_whm_rep || piece_count <= 0 || piece_count > 32) return col;
    port_ws_whm_rep = 0;
    unsigned short *copy = ring[ring_next++ & 15];
    if (whm_ledge >= 0) {                                  /* a filled block from 1 up to h, its top a floor */
        short h = LEDGE[whm_ledge].h;
        whm_ledge = -1;
        copy[0] = 2;
        copy[1] = 0x11;
        copy[2] = 1;
        copy[3] = (unsigned short)h;
        copy[4] = 0x80;
        copy[5] = 1;
        copy[6] = (unsigned short)h;
        copy[7] = 0;
        copy[8] = 0x80;
        return (char *)copy;
    }
    for (int i = 0; i < piece_count; i++) {
        const unsigned short *piece = column + 1 + 4 * i;
        unsigned short flags = piece[0];
        short h = (short)piece[1], k = (short)piece[2];
        if (!(flags & 0x10) && !(flags & 3)) continue;     /* a wall (or other boundary): not carried on */
        if (!(flags & 0x10) && k) {                        /* a slope: its height at x (probeFloorPlaneAt's formula) */
            int lo = piece[3] & 15,
                nn = piece[3] >> 4 & 15,
                m = x & 7,
                mc = m < lo ? lo : m > lo + nn ? lo + nn : m;
            if (!nn) continue;
            if (mc == lo + nn) mc--;                       /* (the formula wraps at lo + nn) */
            h = (short)(h + k * ((x - m + mc - lo) % nn) / nn);
            k = 0;
        }
        unsigned short *out = copy + 1 + 4 * n++;
        out[0] = flags;
        out[1] = (unsigned short)h;
        out[2] = (unsigned short)k;
        out[3] = piece[3];
    }
    copy[0] = (unsigned short)n;
    {
        int area, sec;
        room(&area, &sec);
        for (unsigned i = 0; i < sizeof OBST / sizeof OBST[0]; i++) {
            if (OBST[i].area != area || !(OBST[i].secs >> sec & 1) || whm_d < OBST[i].d0 || whm_d > OBST[i].d1
                || whm_xo < OBST[i].h0 || whm_xo > OBST[i].h1) continue;
            for (int k = 0; k < n; k++) {             /* up is negative Y: a filled block's top is base + dY */
                unsigned short *out = copy + 1 + 4 * k;
                if (out[0] & 0x10) out[2] = (unsigned short)((short)out[2] - OBST[i].up);
                else if (out[0] & 1) out[1] = (unsigned short)((short)out[1] - OBST[i].up);
            }
        }
    }
    return (char *)copy;
}

static int in(int v, int a, int b) { return v >= a && v <= b; }

/* tryStartItemInteraction(o, mode, exit): 1 = not yet (Tomba is in the 4:3 box of a moved exit, not in the moved
   one) */
int port_ws_exit_hold(int mode, int exit)
{
    int pad = port_ws_player_margin(), area, sec, hold = 0;
    (void)mode;
    if (!pad) return 0;
    room(&area, &sec);
    int H = *(short *)PORT_SPAD(0x16A),
        Y = *(short *)PORT_SPAD(0x16E),
        D = *(short *)PORT_SPAD(0x172);
    for (int i = 0; i < port_exit_count; i++) {
        const Exit *ex = &port_exits[i];
        if (ex->area != area || !(ex->secs >> sec & 1) || ex->exit != exit) continue;
        if (!in(H, ex->h0, ex->h1) || !in(Y, ex->y0, ex->y1) || !in(D, ex->d0, ex->d1)) continue;
        if (!ex->ws0 && !ex->ws1) return 0;               /* a box the port leaves alone holds Tomba too */
        int h0 = ex->h0 == -32768 ? ex->h0 : ex->h0 + ex->ws0 * pad,
            h1 = ex->h1 == 32767 ? ex->h1 : ex->h1 + ex->ws1 * pad;
        if (!in(H, h0, h1)) hold = 1;
    }
    return hold;
}
