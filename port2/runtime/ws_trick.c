/* --widescreen: the left end of Trick Village carried on (docs/PORT.md "Widescreen", "Trick Village's left end").
 *
 * AREA10 s3 (the cursed village) and s7 (the purified one) are one WMD model of 150 blocks drawn through an unrotated
 * scrolling camera (VIEW_CAMERA identity, T = (-cx, -cy, 544), H 544: screen x = 160 + 544 (x - cx) / (z + 544); cx
 * inside CAMERA_SCROLL minX..maxX = 118..1860).  The model's left end is the level's wall with the door in it, and it
 * is as wide as the 4:3 camera needs and no wider: with the camera at its left limit the 4:3 frame's edge is at
 * x -29 at the wall's depth (z -45) and the wall's faces stop at x -28..-50: the pink wall above the ledge (five rows
 * of one quad each, their left edge following the wall's cove profile), the wall of grass blocks under it down the
 * whole height of the room, the water sheets in front of it (z -85) and their surface.  At 16:9 that left 30 columns
 * of sky beside and below them.  Nothing is culled there (PORT_WSXTRA=300 PORT_WSBLK=600 draws the same frame): the
 * geometry does not exist.
 *
 * This builds one extra block in the model's own format (FT4 / GT4 packets) and adds it to the game's draw list, so
 * the game's drawer, GTE, NCLIP, culls and ordering table draw it.  What goes on, read from the model:
 *   - a free edge (an edge of one polygon only) on the left of a polygon whose plane holds the x direction (a face at
 *     one depth, a water surface, a slope that does not turn sideways), both ends outside the 4:3 frustum of the
 *     camera at its left limit and at least one inside the wide one;
 *   - the edge is swept to the left in the polygon's own plane, keeping its heights, depths, vertex colours, command
 *     bits (the water stays see-through), page and palette;
 *   - the texture goes on with the polygon's own mapping (texels per unit along x, from the polygon's edges that run
 *     along x) inside the tile that material uses (the union of the U ranges of the polygons of its page, palette and
 *     rows that share a texel column), and at the tile's end it does what the model does there: the polygons that
 *     follow the edge's polygon inwards in its plane show whether the tile starts again (the water: 192..255 every
 *     109 units; most rows of the grass blocks) or is laid flipped (the other grass rows and the stone at the
 *     bottom: there the model lays every other tile flipped); a polygon alone in its plane (the pink wall) shows nothing: its tile starts again, never flipped, from
 *     the column of the tile's far half that goes best with its first column;
 *   - a cut-out (a texture rectangle with see-through texels: the tree crown card) is left as it ends;
 *   - a piece ends where a vertex reaches the tile's end, so the pieces are the same whatever the strip's width (a
 *     wider strip only has more of them) and rows that share a vertex meet without a crack;
 *   - nothing is put where a 4:3 frame could see it: every new vertex lies outside the 4:3 frustum of the camera at
 *     its limit (x <= limit - ceil(160 (z + 544) / 544)).
 * Integers only, no random values: i386 and wasm2c build the same block.
 *
 * Off by default: a room is extended when it is in ROOMS and approved there, or named by --ws-trick-rooms, or with
 * --ws-trick on (every room of the table).  Without --widescreen nothing here runs. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "port_runtime.h"

typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32; typedef short s16; typedef int s32;

extern int port_ws_cull, port_verbose;
extern unsigned long port_gte_cfc2(int);

int port_ws_trick_on = 1;                /* 1: the approved rooms (default), 0: off, 3: every room of the table (main.c) */
const char *port_ws_trick_rooms;         /* --ws-trick-rooms A:S,A:S,... (decimal; "all" = every room of the table) */

/* the rooms (the debug select's numbers); approved: on by default once the user has seen the room */
static const struct { u8 area, sec, approved; } ROOMS[] = {
    {10, 3, 0},        /* Trick Village, cursed (night, flooded) */
    {10, 7, 0},        /* Trick Village, purified */
};
static int room_enabled(int area, int sec)
{
    for (unsigned i = 0; i < sizeof ROOMS / sizeof ROOMS[0]; i++) {
        if (ROOMS[i].area != area || ROOMS[i].sec != sec) continue;
        const char *l = port_ws_trick_rooms;
        if (l) {
            if (!strcmp(l, "all")) return 1;
            while (*l) {
                int a, s, n = 0;
                if (sscanf(l, "%d:%d%n", &a, &s, &n) == 2 && a == area && s == sec) return 1;
                while (*l && *l != ',') l++;
                if (*l == ',') l++;
            }
            return 0;
        }
        return port_ws_trick_on == 3 || (port_ws_trick_on && ROOMS[i].approved);
    }
    return 0;
}

typedef struct { s16 x, y, z; u8 u, v; } Vx;
typedef struct { u16 clut, tpage; u8 cmd, nv, gour, pad; u32 col[4]; Vx v[4]; s16 x0, x1, y0, y1, z0, z1; u8 u0, u1, v0, v1; } Poly;

#define MAXP 1200                         /* polygons of the end looked at */
#define MAXQ 600                          /* packets per kind */
#define H 544
#define NEAR 400                          /* how far right of the 4:3 frame's left edge (camera at its limit) a polygon is still read */

static Poly ps[MAXP]; static int np;
static u8 ft4[MAXQ][40], gt4[MAXQ][52]; static int n4, g4;
static u8 blk[16 + MAXQ * (40 + 52) + 16];
static u32 cur_key; static int have_key, blk_polys;
static const u8 *seen_tbl; static int seen_sec = -1, lo;

static const u8 RING3[3] = {0, 1, 2}, RING4[4] = {0, 1, 3, 2};
static int iabs(int a) { return a < 0 ? -a : a; }

/* the 4:3 frame's left edge at depth z with the camera at its left limit, rounded outwards; the wide frame's (+ 8) */
static int x43(int z) { return lo - (160 * (z + H) + H - 1) / H; }
static int xwide(int z) { return lo - ((160 + port_ws_cull + 8) * (z + H) + H - 1) / H; }

/* the model's polygons near the left end -> ps[]; 0 if the model is not what this file expects */
static int parse(const u8 *tbl)
{
    int n = tbl[0];
    const s32 *off = (const s32 *)(tbl + 4);
    np = 0;
    for (int i = 0; i < n; i++) {
        const u8 *p = tbl + off[i];
        for (int g = 0; g < 4; g++) {
            u32 c; memcpy(&c, p, 4); p += 4;
            int nv = (g & 1) ? 4 : 3, gour = g >= 2;
            if (c > 4000) return 0;
            for (u32 k = 0; k < c; k++) {
                Poly q;
                memcpy(&q.clut, p, 2); memcpy(&q.tpage, p + 2, 2); memcpy(&q.col[0], p + 4, 4);
                q.cmd = p[7]; q.nv = (u8)nv; q.gour = (u8)gour; q.pad = 0;
                if (((q.cmd & 8) != 0) != (nv == 4) || ((q.cmd & 0x10) != 0) != gour || (q.cmd & 0xE0) != 0x20) return 0;
                p += 8;
                for (int j = 1; j < nv; j++) { if (gour) { memcpy(&q.col[j], p, 4); p += 4; } else q.col[j] = q.col[0]; }
                q.x0 = q.y0 = q.z0 = 32767; q.x1 = q.y1 = q.z1 = -32768; q.u0 = q.v0 = 255; q.u1 = q.v1 = 0;
                int near = 0;
                for (int j = 0; j < nv; j++) {
                    Vx *v = &q.v[j]; memcpy(v, p, 8); p += 8;
                    if (v->x < q.x0) q.x0 = v->x; if (v->x > q.x1) q.x1 = v->x;
                    if (v->y < q.y0) q.y0 = v->y; if (v->y > q.y1) q.y1 = v->y;
                    if (v->z < q.z0) q.z0 = v->z; if (v->z > q.z1) q.z1 = v->z;
                    if (v->u < q.u0) q.u0 = v->u; if (v->u > q.u1) q.u1 = v->u;
                    if (v->v < q.v0) q.v0 = v->v; if (v->v > q.v1) q.v1 = v->v;
                    if (v->z > -H + 64 && v->x <= x43(v->z) + NEAR) near = 1;
                }
                if (!near) continue;
                if (np >= MAXP) return 0;
                ps[np++] = q;
            }
        }
    }
    return np;
}

static int same_pos(const Vx *a, const Vx *b) { return a->x == b->x && a->y == b->y && a->z == b->z; }
static int has_vertex(const Poly *q, const Vx *a) { for (int j = 0; j < q->nv; j++) if (same_pos(&q->v[j], a)) return 1; return 0; }
static int same_stuff(const Poly *p, const Poly *q)          /* same page and palette, at least half of p's rows */
{
    if (q->tpage != p->tpage || q->clut != p->clut) return 0;
    int h = (q->v1 < p->v1 ? q->v1 : p->v1) - (q->v0 > p->v0 ? q->v0 : p->v0) + 1;
    return 2 * h >= p->v1 - p->v0 + 1;
}

/* the U range of the tile p's texture belongs to: the ranges of the polygons of its page, palette and rows, joined
   while they share a texel column (the pink wall's rows 184..232, 188..236, ... 205..253 and the side wall's
   184..219, 219..253: 184..253) */
static void tile(const Poly *p, int *t0, int *t1)
{
    int a = p->u0, b = p->u1, more = 1;
    while (more) {
        more = 0;
        for (int i = 0; i < np; i++) {
            const Poly *q = &ps[i];
            if (!same_stuff(p, q) || q->u1 < a || q->u0 > b) continue;
            if (q->u0 < a) { a = q->u0; more = 1; }
            if (q->u1 > b) { b = q->u1; more = 1; }
        }
    }
    *t0 = a; *t1 = b;
}

/* U at the left / right side of a polygon whose U runs along x (the mean of its vertices in the outer quarter) */
static int side_u(const Poly *q, int right)
{
    int s = 0, n = 0, w = q->x1 - q->x0;
    for (int j = 0; j < q->nv; j++) {
        int d = right ? q->x1 - q->v[j].x : q->v[j].x - q->x0;
        if (4 * d <= w) { s += q->v[j].u; n++; }
    }
    return n ? s / n : 0;
}
static int in_plane(const Poly *p, const Poly *q)
{
    const Vx *o = &p->v[0], *a = &p->v[1], *b = &p->v[2];
    int ay = a->y - o->y, az = a->z - o->z, by = b->y - o->y, bz = b->z - o->z;
    if (ay == 0 && az == 0) { ay = by; az = bz; }             /* (the plane holds x: its trace in y / z is a line) */
    for (int j = 0; j < q->nv; j++) {
        int cy = q->v[j].y - o->y, cz = q->v[j].z - o->z;
        if (iabs(ay * cz - az * cy) > 4 * (iabs(ay) + iabs(az))) return 0;
    }
    return 1;
}

/* what the model does at the end of this tile: 1 = laid flipped, 0 = starts again.  First the polygons that follow p
   inwards in its plane (same stuff, about its rows): the first join that is not just the mapping going on decides.
   -1: the model shows nothing (no polygon follows in the plane). */
static int join_kind(const Poly *p, int t0, int t1)
{
    const Poly *c = p;
    for (int step = 0; step < 4; step++) {
        const Poly *nx = 0;
        for (int i = 0; i < np; i++) {
            const Poly *q = &ps[i];
            if (q == c || !same_stuff(p, q) || q->x1 - q->x0 < 8 || q->u1 - q->u0 < 8 || !in_plane(p, q)) continue;
            if (q->x0 < c->x1 - 8 || q->x0 > c->x1 + 8) continue;
            int oy = (q->y1 < c->y1 ? q->y1 : c->y1) - (q->y0 > c->y0 ? q->y0 : c->y0), oz = (q->z1 < c->z1 ? q->z1 : c->z1) - (q->z0 > c->z0 ? q->z0 : c->z0);
            if (2 * oy < c->y1 - c->y0 || 2 * oz < c->z1 - c->z0) continue;
            if (!nx || q->x1 - q->x0 > nx->x1 - nx->x0) nx = q;
        }
        if (!nx) break;
        int cl = side_u(c, 0), cr = side_u(c, 1), nl = side_u(nx, 0), nr = side_u(nx, 1);
        if (iabs(nl - cr) <= 8) {
            if ((cr > cl) != (nr > nl)) return 1;                        /* turns back: flipped */
        } else if (2 * iabs(nl - cr) >= t1 - t0) return 0;               /* jumps to the tile's other end */
        c = nx;
    }
    return -1;
}

/* a texel of p's page through its palette, as VRAM holds them now (0 = see-through) */
extern const unsigned short *port_gpu_vram(int x, int y, int w, int h);
static int texel(const unsigned short *vr, const Poly *p, int u, int v)
{
    int tx = (p->tpage & 15) * 64, ty = (p->tpage & 16) ? 256 : 0, depth = (p->tpage >> 7) & 3;
    int cx = (p->clut & 63) * 16, cy = (p->clut >> 6) & 511;
    if (depth == 0) return vr[cy * 1024 + ((cx + ((vr[(ty + v) * 1024 + ((tx + u / 4) & 1023)] >> ((u & 3) * 4)) & 15)) & 1023)];
    if (depth == 1) return vr[cy * 1024 + ((cx + ((vr[(ty + v) * 1024 + ((tx + u / 2) & 1023)] >> ((u & 1) * 8)) & 255)) & 1023)];
    return vr[(ty + v) * 1024 + ((tx + u) & 1023)];
}
/* a cut-out (leaves, a fringe): its texture rectangle holds see-through texels */
static int has_holes(const Poly *p)
{
    const unsigned short *vr = port_gpu_vram(0, 0, 1024, 512);
    int bad = 0, n = 0;
    for (int v = p->v0; v <= p->v1; v++) for (int u = p->u0; u <= p->u1; u++) { bad += texel(vr, p, u, v) == 0; n++; }
    return bad * 64 > n;
}
/* where the model shows nothing about the tile's end (a face alone in its plane), the tile starts again, and from
   the column of its far half that goes best with its first column (the least colour difference over the rows that
   page and palette use inside the tile), so the join is the texture's own soft place and not a cut through a mark:
   the pink wall's tile 184..253 has blotches against its right edge, 184..(the column found) has none cut. */
static int wrap_column(const Poly *p, int t0, int t1)
{
    const unsigned short *vr = port_gpu_vram(0, 0, 1024, 512);
    int v0 = 255, v1 = 0, best = t1; long bc = -1;
    for (int i = 0; i < np; i++) {
        const Poly *q = &ps[i];
        if (q->tpage != p->tpage || q->clut != p->clut || q->u0 < t0 || q->u1 > t1) continue;
        if (q->v0 < v0) v0 = q->v0; if (q->v1 > v1) v1 = q->v1;
    }
    if (v1 < v0 || t1 - t0 < 16) return t1;
    for (int c = t1; c >= t0 + (t1 - t0) / 2; c--) {
        long cost = 0;
        for (int v = v0 + 1; v < v1; v++) {
            int d[3] = {0, 0, 0};                                            /* 3 x 3 texels each side of the join */
            for (int k = 0; k < 9; k++) {
                int a = texel(vr, p, t0 + k % 3, v - 1 + k / 3), b = texel(vr, p, c - k % 3, v - 1 + k / 3);
                d[0] += (a & 31) - (b & 31); d[1] += ((a >> 5) & 31) - ((b >> 5) & 31); d[2] += ((a >> 10) & 31) - ((b >> 10) & 31);
            }
            cost += d[0] * d[0] + d[1] * d[1] + d[2] * d[2];                 /* squared: a mark cut through counts more than grain */
        }
        if (port_verbose > 2) port_printf("[port] ws trick: wrap column %d cost %ld\n", c, cost);
        if (bc < 0 || cost < bc) { bc = cost; best = c; }
    }
    return best;
}

static void put_v(u8 *d, const Vx *e, int x, int u256)
{
    Vx v = *e;
    int u = (u256 + 128) >> 8;
    v.x = (s16)x; v.u = (u8)(u < 0 ? 0 : u > 255 ? 255 : u);
    memcpy(d, &v, 8);
}

static void build(void)
{
    int nedge = 0;
    n4 = g4 = 0;
    for (int i = 0; i < np; i++) {
        const Poly *p = &ps[i];
        const u8 *ring = p->nv == 4 ? RING4 : RING3;
        {   /* the plane holds the x direction */
            const Vx *r0 = &p->v[ring[0]], *r1 = &p->v[ring[1]], *r2 = &p->v[ring[2]];
            if ((r1->y - r0->y) * (r2->z - r0->z) - (r1->z - r0->z) * (r2->y - r0->y) != 0) continue;
            if (p->nv == 4) { const Vx *r3 = &p->v[ring[3]]; if ((r1->y - r0->y) * (r3->z - r0->z) - (r1->z - r0->z) * (r3->y - r0->y) != 0) continue; }
        }
        int have_tile = 0, t0 = 0, t1 = 0, t1e = 0, flip = 0, holes = 0;
        for (int e = 0; e < p->nv; e++) {
            int ia = ring[e], ib = ring[(e + 1) % p->nv];
            const Vx *a = &p->v[ia], *b = &p->v[ib];
            const Vx *na = &p->v[ring[(e + p->nv - 1) % p->nv]], *nb = &p->v[ring[(e + 2) % p->nv]];
            int dx = b->x - a->x, dy = b->y - a->y, dz = b->z - a->z;
            if (dx * dx > dy * dy + dz * dz) continue;                       /* runs along x: nothing to sweep */
            if (na->x < a->x || nb->x < b->x || (na->x == a->x && nb->x == b->x)) continue;    /* the polygon lies to its right */
            if (a->x > x43(a->z) + 1 || b->x > x43(b->z) + 1) continue;     /* (part of) it is in the 4:3 picture */
            if (a->x <= xwide(a->z) && b->x <= xwide(b->z)) continue;       /* out of the wide frame too */
            int shared = 0;
            for (int k = 0; k < np && !shared; k++) shared = k != i && has_vertex(&ps[k], a) && has_vertex(&ps[k], b);
            if (shared) continue;
            /* texels per unit along x (8.8), from the polygon's edges that run along x */
            int rho = 0, nr = 0;
            if (na->x - a->x >= 4 && 4 * (iabs(na->y - a->y) + iabs(na->z - a->z)) <= na->x - a->x) { rho += (na->u - a->u) * 256 / (na->x - a->x); nr++; }
            if (nb->x - b->x >= 4 && 4 * (iabs(nb->y - b->y) + iabs(nb->z - b->z)) <= nb->x - b->x) { rho += (nb->u - b->u) * 256 / (nb->x - b->x); nr++; }
            if (!nr) continue;
            rho /= nr;
            if (!have_tile) {
                have_tile = 1; holes = has_holes(p); tile(p, &t0, &t1); flip = join_kind(p, t0, t1);
                if (flip < 0) { flip = 0; t1e = wrap_column(p, t0, t1); if (t1e < a->u || t1e < b->u) t1e = t1; t1 = t1e; }
            }
            if (holes) continue;                                             /* a tree's crown ends where it ends */
            if (iabs(rho) < 8 || t1 - t0 < 4) rho = 0;                       /* one texel column */
            u32 ca = p->col[ia] & 0xFFFFFF, cb = p->col[ib] & 0xFFFFFF;
            int x[2] = {a->x, b->x}, u[2] = {a->u * 256, b->u * 256}, r[2] = {rho, rho}, end[2] = {xwide(a->z), xwide(b->z)};
            const Vx *ev[2] = {a, b};
            for (int k = 0; k < 2; k++) {                                    /* start outside the 4:3 frustum */
                int lim = x43(ev[k]->z);
                if (x[k] > lim) { u[k] -= r[k] * (x[k] - lim); x[k] = lim; }
            }
            nedge++;
            if (port_verbose > 1) port_printf("[port] ws trick: edge (%d,%d,%d u%d) (%d,%d,%d u%d) cmd %02x tp %04x cl %04x tile %d..%d rho %d %s\n",
                a->x, a->y, a->z, a->u, b->x, b->y, b->z, b->u, p->cmd, p->tpage, p->clut, t0, t1, rho, flip ? "flipped" : "again");
            for (int guard = 0; guard < 48 && (x[0] > end[0] || x[1] > end[1]); guard++) {
                int nx[2], nu[2];
                for (int k = 0; k < 2; k++) {
                    if (!r[k]) { nx[k] = x[k] - 64; nu[k] = u[k]; continue; }
                    for (int twice = 0; twice < 2; twice++) {                /* at the tile's end: flipped, or again */
                        int bound = (r[k] > 0 ? t0 : t1) * 256;
                        if (iabs(u[k] - bound) >= 128) break;
                        if (flip) r[k] = -r[k]; else u[k] = (r[k] > 0 ? t1 : t0) * 256;
                    }
                    int bound = (r[k] > 0 ? t0 : t1) * 256;
                    int d = (iabs(u[k] - bound) + iabs(r[k]) / 2) / iabs(r[k]);
                    if (d < 1) d = 1;
                    nx[k] = x[k] - d; nu[k] = bound;
                }
                if (n4 >= MAXQ || g4 >= MAXQ) goto pack;
                /* the quad on the other side of the edge, same facing: ring (b, a, a', b') = strip order b, a, b', a' */
                u8 *d = p->gour ? gt4[g4] : ft4[n4], *vp;
                u32 w0 = cb | ((u32)(p->cmd | 8) << 24);
                memcpy(d, &p->clut, 2); memcpy(d + 2, &p->tpage, 2); memcpy(d + 4, &w0, 4);
                vp = d + 8;
                if (p->gour) { memcpy(d + 8, &ca, 4); memcpy(d + 12, &cb, 4); memcpy(d + 16, &ca, 4); vp = d + 20; g4++; } else n4++;
                put_v(vp, b, x[1], u[1]); put_v(vp + 8, a, x[0], u[0]); put_v(vp + 16, b, nx[1], nu[1]); put_v(vp + 24, a, nx[0], nu[0]);
                for (int k = 0; k < 2; k++) { x[k] = nx[k]; u[k] = nu[k]; }
            }
        }
    }
pack:
    {
        u8 *d = blk; u32 n = 0;
        memcpy(d, &n, 4); d += 4;
        n = (u32)n4; memcpy(d, &n, 4); d += 4; memcpy(d, ft4, (size_t)n4 * 40); d += n4 * 40;
        n = 0; memcpy(d, &n, 4); d += 4;
        n = (u32)g4; memcpy(d, &n, 4); d += 4; memcpy(d, gt4, (size_t)g4 * 52); d += g4 * 52;
        memset(d, 0, 16);
        blk_polys = n4 + g4;
    }
    if (port_verbose) port_printf("[port] ws trick: camera limit %d, %d polygons read, %d edges carried on, %d quads\n", lo, np, nedge, blk_polys);
}

/* called from port_ws_area_model_link with the game's (merged) block list of this frame (list[3] = count, u32
   pointers from +4, room for 88) and the area model's table */
void port_ws_trick_blocks(char *list, const u8 *tbl, int area, int section)
{
    if (!port_ws_cull || !tbl || !room_enabled(area, section)) return;
    int n = tbl[0];
    if (n < 2) return;
    /* the camera: VIEW_CAMERA (scratchpad 0xC0, MATRIX: m[3][3] s16, t[3] s32 at +0x14) must be unrotated */
    const u8 *c = (const u8 *)PORT_SPAD(0xC0);
    s16 m[9]; s32 t[3]; memcpy(m, c, 18); memcpy(t, c + 0x14, 12);
    for (int i = 0; i < 9; i++) if (m[i] != ((i % 4) ? 0 : 4096)) return;
    if ((int)(u16)port_gte_cfc2(26) != H || (int)((s32)port_gte_cfc2(24) >> 16) != 160 || t[2] != H) return;
    /* the camera's left limit: CAMERA_SCROLL minX, and wherever the camera has been in this room */
    const u8 *cs = (const u8 *)port_ram_ptr(0x800A4550u);
    s16 minx, maxx; memcpy(&minx, cs + 0x2C, 2); memcpy(&maxx, cs + 0x2E, 2);
    int cx = -(int)t[0];
    if (seen_tbl != tbl || seen_sec != section) { seen_tbl = tbl; seen_sec = section; lo = cx; have_key = 0; }
    if (minx <= maxx && maxx - minx < 8000 && minx < lo) lo = minx;
    if (cx < lo) lo = cx;
    if (cx > lo + 2 * (160 + port_ws_cull) + 64) return;                 /* the end is out of sight */
    const s32 *off = (const s32 *)(tbl + 4);
    u32 key = (u32)n * 2654435761u;
    for (int i = 0; i < n; i++) key = (key ^ (u32)off[i]) * 16777619u;
    for (int i = 0; i < 4 && i < n; i++) for (int k = 0; k < 256; k++) key = (key ^ tbl[off[i] + k]) * 16777619u;
    key = (key ^ (u32)port_ws_cull) * 16777619u; key = (key ^ (u32)(lo + 16384)) * 16777619u;
    if (!have_key || key != cur_key) {
        have_key = 1; cur_key = key; blk_polys = 0;
        if (parse(tbl)) build();
    }
    if (!blk_polys) return;
    int cnt = (u8)list[3];
    if (cnt >= 88) return;
    ((u32 *)list)[1 + cnt] = (u32)(unsigned long)blk; list[3] = (char)(cnt + 1);
}
