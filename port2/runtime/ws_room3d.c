/* --widescreen: the front wall of the 3D side rooms carried on sideways (docs/PORT.md "Widescreen", "3D room walls").
 *
 * The Haunted Mansion's side rooms (AREA04 s10, s11, s15, s19) are one small WMD model each, drawn by the area list
 * (drawAreaModelList / drawModelUnlit) through a fixed, unrotated camera: a faceted stone wall facing the viewer with
 * the room let into it.  The wall is as wide as the 4:3 picture needs and no wider: it ends on both sides in a
 * straight vertical line of free polygon edges a few columns past the 4:3 frame (measured: screen x -29 / 359 in s10
 * and s19, -14 / 361 in s11, -14 / 378 in s15), so a wide frame showed the clear colour beyond it.  Nothing is
 * culled there (the culls are already wide, and a reference with PORT_WSXTRA=300 draws the same primitives): the
 * geometry does not exist.
 *
 * This builds, per frame key (the model's bytes, the strip width and the camera), one extra block in the model's own
 * format and adds it to the game's draw list, so the game's drawer, GTE, culls and ordering table draw it.  Per side:
 *   - the wall's end is the vertical line that carries the most free, front-facing polygon edges (edges used by one
 *     polygon only, both vertices at the same x, the polygon lying inwards of it);
 *   - from every such edge the wall goes on outwards in its own plane (the edge vertices keep their z) as columns of
 *     facets: a grid whose first line is the edge itself (same vertices: no crack), the others 48 units apart with
 *     every vertex moved a little (by a hash of its row height and column), each cell a quad or two triangles;
 *   - the facet next to an edge takes the colour of the polygon it continues (the join is one more facet line of the
 *     wall, not a change of tone); the others take one of the colours of the polygons along that end, so each side
 *     keeps its own light (the model is darker on one side);
 *   - every facet is textured like the model's: its own piece of the wall's stone tile (the UV box the polygons of
 *     that page and palette use), unrotated or turned a quarter, at the model's texel density (9 texels per 16 units;
 *     measured median 0.51-0.57).
 * Nothing is mirrored and nothing is drawn that the wall does not consist of already.  Integers only; every random
 * value is a hash of the position, taken in its own statement, so i386 and wasm2c build the same block and a wider
 * strip starts with the columns of a narrower one.
 *
 * Off by default: a room is extended when it is in ROOMS and approved there, or named by --ws-room3d-rooms, or with
 * --ws-room3d on (every room of the table).  Without --widescreen nothing here runs. */
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "port_runtime.h"

typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32; typedef short s16; typedef int s32;

extern int port_ws_cull;
extern unsigned long port_gte_cfc2(int);

int port_ws_room3d_on = 1;               /* 1: the approved rooms (default), 0: off, 3: every room of the table (main.c) */
const char *port_ws_room3d_rooms;        /* --ws-room3d-rooms A:S,A:S,... (decimal; "all" = every room of the table) */

/* the rooms looked at (the debug select's numbers); approved: on by default once the user has seen the room */
static const struct { u8 area, sec, approved; } ROOMS[] = {
    {4, 10, 1},        /* Haunted Mansion, pink stone room with the pedestal */
    {4, 11, 1},        /* the two rooms with the side doors (one model, one above the other) */
    {4, 15, 1},        /* the fireplace room */
    {4, 19, 1},        /* the teal room (s10's model, tinted) */
};
static int room_enabled(int area, int sec)
{
    for (unsigned i = 0; i < sizeof ROOMS / sizeof ROOMS[0]; i++) {
        if (ROOMS[i].area != area || ROOMS[i].sec != sec) continue;
        const char *l = port_ws_room3d_rooms;
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
        return port_ws_room3d_on == 3 || (port_ws_room3d_on && ROOMS[i].approved);
    }
    return 0;
}

typedef struct { s16 x, y, z; u8 u, v; } Vx;
typedef struct { u16 clut, tpage; u8 cmd, nv, gour, pad; u32 col[4]; Vx v[4]; } Poly;
typedef struct { s16 y0, z0, y1, z1; int poly; } Seg;        /* a free edge of the wall's end, y0 < y1 */

#define MAXP 1024
#define MAXS 48                           /* edges per side */
#define MAXN 640                          /* packets per kind */
#define COLW 48                           /* column width, units */
#define JX 12                             /* a vertex moves up to +-JX along x, +-JY along y */
#define JY 16
#define MAXCOL 12

static Poly ps[MAXP]; static int np;
static u8 ft3[MAXN][32], ft4[MAXN][40], gt3[MAXN][40], gt4[MAXN][52]; static int n3, n4, g3, g4;
static u8 blk[16 + MAXN * (32 + 40 + 40 + 52) + 16]; static int blk_polys;
static u32 cur_key; static int have_key;

static const u8 RING3[3] = {0, 1, 2}, RING4[4] = {0, 1, 3, 2};

static u32 hsh(u32 a, u32 b, u32 c)
{
    u32 h = a * 0x9E3779B1u + 0x7F4A7C15u;
    h ^= h >> 15; h = (h ^ b) * 0x85EBCA6Bu;
    h ^= h >> 13; h = (h ^ c) * 0xC2B2AE35u;
    h ^= h >> 16; h *= 0x27D4EB2Fu; h ^= h >> 15;
    return h;
}

/* the model block -> ps[]; returns its size in bytes, 0 if it is not what this file expects */
static int parse(const u8 *b)
{
    const u8 *p = b;
    np = 0;
    for (int g = 0; g < 4; g++) {
        u32 c; memcpy(&c, p, 4); p += 4;
        int nv = (g & 1) ? 4 : 3, gour = g >= 2;
        if (c > MAXP) return 0;
        for (u32 k = 0; k < c; k++) {
            if (np >= MAXP) return 0;
            Poly *q = &ps[np++];
            memcpy(&q->clut, p, 2); memcpy(&q->tpage, p + 2, 2); memcpy(&q->col[0], p + 4, 4);
            q->cmd = p[7]; q->nv = (u8)nv; q->gour = (u8)gour;
            if (((q->cmd & 8) != 0) != (nv == 4) || ((q->cmd & 0x10) != 0) != gour || (q->cmd & 0xE4) != 0x24) return 0;
            p += 8;
            for (int j = 1; j < nv; j++) { if (gour) { memcpy(&q->col[j], p, 4); p += 4; } else q->col[j] = q->col[0]; }
            for (int j = 0; j < nv; j++) { memcpy(&q->v[j], p, 8); p += 8; if (q->v[j].x < -4000 || q->v[j].x > 4000) return 0; }
        }
    }
    return (int)(p - b);
}

static int same_pos(const Vx *a, const Vx *b) { return a->x == b->x && a->y == b->y && a->z == b->z; }
static int has_vertex(const Poly *q, const Vx *a) { for (int j = 0; j < q->nv; j++) if (same_pos(&q->v[j], a)) return 1; return 0; }

/* the free vertical edges of front-facing polygons whose polygon lies inwards (side -1: to the right of the edge,
   +1: to its left), on the line x that carries the most of them; returns the count, *xs = that line */
static int find_end(int side, Seg *out, int *xs)
{
    static struct { s16 x; Seg s; } c[256]; int nc = 0;
    for (int i = 0; i < np; i++) {
        const Poly *p = &ps[i];
        if (p->cmd & 2) continue;                                         /* the translucent shading layer */
        const u8 *ring = p->nv == 4 ? RING4 : RING3;
        s32 ax = p->v[1].x - p->v[0].x, ay = p->v[1].y - p->v[0].y, az = p->v[1].z - p->v[0].z;
        s32 bx = p->v[2].x - p->v[0].x, by = p->v[2].y - p->v[0].y, bz = p->v[2].z - p->v[0].z;
        s32 nx = ay * bz - az * by, ny = az * bx - ax * bz, nz = ax * by - ay * bx;
        nx = nx < 0 ? -nx : nx; ny = ny < 0 ? -ny : ny; nz = nz < 0 ? -nz : nz;
        if (nz < nx || nz < ny) continue;                                 /* not facing the viewer */
        for (int e = 0; e < p->nv; e++) {
            const Vx *a = &p->v[ring[e]], *b = &p->v[ring[(e + 1) % p->nv]];
            int dy = b->y - a->y, dz = b->z - a->z, ok = 1, used = 0;
            dy = dy < 0 ? -dy : dy; dz = dz < 0 ? -dz : dz;
            if (a->x != b->x || dy < 8 || 2 * dz > dy) continue;
            for (int k = 0; k < p->nv && ok; k++) {
                const Vx *o = &p->v[k];
                if (o == a || o == b) continue;
                if ((o->x - a->x) * side >= 0) ok = 0;                    /* the polygon must lie inwards of the edge */
            }
            if (!ok) continue;
            for (int k = 0; k < np && !used; k++) {
                const Poly *q = &ps[k];
                if (q != p && !(q->cmd & 2) && has_vertex(q, a) && has_vertex(q, b)) used = 1;
            }
            if (used || nc >= 256) continue;
            c[nc].x = a->x;
            if (a->y < b->y) { c[nc].s.y0 = a->y; c[nc].s.z0 = a->z; c[nc].s.y1 = b->y; c[nc].s.z1 = b->z; }
            else             { c[nc].s.y0 = b->y; c[nc].s.z0 = b->z; c[nc].s.y1 = a->y; c[nc].s.z1 = a->z; }
            c[nc].s.poly = i; nc++;
        }
    }
    int best = 0, bx = 0, n = 0;
    for (int i = 0; i < nc; i++) {
        int sum = 0;
        for (int k = 0; k < nc; k++) if (c[k].x == c[i].x) sum += c[k].s.y1 - c[k].s.y0;
        if (sum > best || (sum == best && (c[i].x - bx) * side > 0)) { best = sum; bx = c[i].x; }
    }
    for (int i = 0; i < nc && n < MAXS; i++) if (c[i].x == bx) out[n++] = c[i].s;
    for (int i = 1; i < n; i++) {                                         /* top to bottom */
        Seg t = out[i]; int k = i;
        while (k > 0 && out[k - 1].y0 > t.y0) { out[k] = out[k - 1]; k--; }
        out[k] = t;
    }
    *xs = bx;
    return n;
}

typedef struct { s16 x, y, z; } P3;

/* how far a row line may move: a quarter of the shorter row next to it, 0 at a line only one edge ends on */
static int row_amp(const Seg *sg, int ns, int y)
{
    int up = 0, dn = 0;
    for (int i = 0; i < ns; i++) {
        if (sg[i].y1 == y) up = sg[i].y1 - sg[i].y0;
        if (sg[i].y0 == y) dn = sg[i].y1 - sg[i].y0;
    }
    if (!up || !dn) return 0;
    int a = (up < dn ? up : dn) / 4;
    return a > JY ? JY : a;
}
static P3 grid(int xs, int side, int y, int z, int j, int amp)
{
    P3 r; r.x = (s16)xs; r.y = (s16)y; r.z = (s16)z;
    if (j > 0) {
        u32 h = hsh((u32)(y + 4096), (u32)j, (u32)(side + 2));
        int jx = (int)(h & 0xFF) % (2 * JX + 1) - JX;
        int jy = amp ? (int)((h >> 8) & 0xFF) % (2 * amp + 1) - amp : 0;
        r.x = (s16)(xs + side * (j * COLW + jx)); r.y = (s16)(y + jy);
    }
    return r;
}

/* a polygon of the wall's end near edge k to take a colour from: at most two edges up or down the same unbroken run
   (the light changes along the wall, and a model can hold two rooms, one above the other, in different tints) */
static int near_seg(const Seg *sg, int ns, int k, u32 h)
{
    int lo = k, hi = k;
    while (lo > 0 && sg[lo - 1].y1 == sg[lo].y0) lo--;
    while (hi < ns - 1 && sg[hi + 1].y0 == sg[hi].y1) hi++;
    int i = k + (int)(h % 5u) - 2;
    return i < lo ? lo : i > hi ? hi : i;
}

/* one facet: nv vertices (a quad in strip order), the colour and kind of polygon src, its own piece of the tile */
static void facet(const P3 *v, int nv, const Poly *src, const Poly *tex, const int *tile, int ref, u32 seed)
{
    P3 q[4]; int x0 = 32767, x1 = -32768, y0 = 32767, y1 = -32768;
    for (int i = 0; i < nv; i++) {
        q[i] = v[i];
        x0 = q[i].x < x0 ? q[i].x : x0; x1 = q[i].x > x1 ? q[i].x : x1;
        y0 = q[i].y < y0 ? q[i].y : y0; y1 = q[i].y > y1 ? q[i].y : y1;
    }
    {   /* the winding of the model's wall polygons (NCLIP's sign) */
        s32 cr = (s32)(q[1].x - q[0].x) * (q[2].y - q[0].y) - (s32)(q[1].y - q[0].y) * (q[2].x - q[0].x);
        if ((cr < 0) != (ref < 0)) {
            if (nv == 3) { P3 t = q[1]; q[1] = q[2]; q[2] = t; }
            else { P3 t = q[0]; q[0] = q[1]; q[1] = t; t = q[2]; q[2] = q[3]; q[3] = t; }
        }
    }
    int tu = tile[1] - tile[0], tv = tile[3] - tile[2];
    int sx = (x1 - x0) * 9 / 16, sy = (y1 - y0) * 9 / 16;
    int turn = (int)((seed >> 4) & 1);
    if (!turn && (sx > tu || sy > tv) && sy <= tu && sx <= tv) turn = 1;
    if (turn && (sy > tu || sx > tv)) turn = 0;
    int su = turn ? sy : sx, sv = turn ? sx : sy;
    su = su > tu ? tu : su; sv = sv > tv ? tv : sv;
    u32 hu = hsh(seed, 11, (u32)(x0 + 4096));
    u32 hv = hsh(seed, 23, (u32)(y0 + 4096));
    int ub = tile[0] + (int)(hu % (u32)(tu - su + 1)), vb = tile[2] + (int)(hv % (u32)(tv - sv + 1));
    int gour = src->gour;
    u8 *d;
    if (nv == 3) { if (gour) { if (g3 >= MAXN) return; d = gt3[g3++]; } else { if (n3 >= MAXN) return; d = ft3[n3++]; } }
    else         { if (gour) { if (g4 >= MAXN) return; d = gt4[g4++]; } else { if (n4 >= MAXN) return; d = ft4[n4++]; } }
    u32 col = src->col[0] & 0xFFFFFF;
    u32 w0 = col | ((u32)(0x24 | (src->cmd & 1) | (gour ? 0x10 : 0) | (nv == 4 ? 8 : 0)) << 24);
    memcpy(d, &tex->clut, 2); memcpy(d + 2, &tex->tpage, 2); memcpy(d + 4, &w0, 4); d += 8;
    if (gour) for (int i = 1; i < nv; i++) { memcpy(d, &col, 4); d += 4; }
    for (int i = 0; i < nv; i++) {
        Vx o; int du = ((turn ? q[i].y - y0 : q[i].x - x0) * 9) / 16, dv = ((turn ? q[i].x - x0 : q[i].y - y0) * 9) / 16;
        du = du > su ? su : du; dv = dv > sv ? sv : dv;
        o.x = q[i].x; o.y = q[i].y; o.z = q[i].z; o.u = (u8)(ub + du); o.v = (u8)(vb + dv);
        memcpy(d, &o, 8); d += 8;
    }
}

static void build_side(int side, int tx, int tz, int h, int half)
{
    Seg sg[MAXS]; int xs, ns = find_end(side, sg, &xs);
    if (ns < 2) return;
    int zmax = -32768, tile[4] = {255, 0, 255, 0};
    const Poly *tex = &ps[sg[0].poly];
    for (int i = 0; i < ns; i++) { zmax = sg[i].z0 > zmax ? sg[i].z0 : zmax; zmax = sg[i].z1 > zmax ? sg[i].z1 : zmax; }
    for (int i = 0; i < np; i++) {
        if (ps[i].tpage != tex->tpage || ps[i].clut != tex->clut) continue;
        for (int j = 0; j < ps[i].nv; j++) {
            int u = ps[i].v[j].u, v = ps[i].v[j].v;
            tile[0] = u < tile[0] ? u : tile[0]; tile[1] = u > tile[1] ? u : tile[1];
            tile[2] = v < tile[2] ? v : tile[2]; tile[3] = v > tile[3] ? v : tile[3];
        }
    }
    if (tile[1] - tile[0] < 16 || tile[3] - tile[2] < 16 || zmax + tz < 64) return;
    /* the frame's edge at the wall's depth, from the camera's axis; the wall's end, from the same */
    int need = (half + 2) * (zmax + tz) / h + 2 - (xs + tx) * side;
    if (need <= 0) return;
    int ncol = (need + JX + COLW - 1) / COLW;
    if (ncol > MAXCOL) ncol = MAXCOL;
    int before = n3 + n4 + g3 + g4;
    for (int k = 0; k < ns; k++) {
        const Seg *s = &sg[k]; const Poly *own = &ps[s->poly];
        s32 ref = (s32)(own->v[1].x - own->v[0].x) * (own->v[2].y - own->v[0].y) - (s32)(own->v[1].y - own->v[0].y) * (own->v[2].x - own->v[0].x);
        int a0 = row_amp(sg, ns, s->y0), a1 = row_amp(sg, ns, s->y1);
        for (int j = 0; j < ncol; j++) {
            P3 p00 = grid(xs, side, s->y0, s->z0, j, a0), p10 = grid(xs, side, s->y1, s->z1, j, a1);
            P3 p01 = grid(xs, side, s->y0, s->z0, j + 1, a0), p11 = grid(xs, side, s->y1, s->z1, j + 1, a1);
            u32 hc = hsh((u32)(s->y0 + 4096), (u32)(j + 64), (u32)(side + 2));
            u32 h1 = hsh(hc, 1, 77);
            u32 h2 = hsh(hc, 2, 91);
            const Poly *c1 = j == 0 ? own : &ps[sg[near_seg(sg, ns, k, h1)].poly];     /* the facet on the cell's inner line */
            const Poly *c2 = &ps[sg[near_seg(sg, ns, k, h2)].poly];
            int kind = (int)(hc & 3);
            P3 v[4];
            if (kind < 2) { v[0] = p00; v[1] = p10; v[2] = p01; v[3] = p11; facet(v, 4, c1, tex, tile, ref, hc); }
            else if (kind == 2) {
                v[0] = p00; v[1] = p10; v[2] = p11; facet(v, 3, c1, tex, tile, ref, hc);
                v[0] = p00; v[1] = p11; v[2] = p01; facet(v, 3, c2, tex, tile, ref, h2);
            } else {
                v[0] = p00; v[1] = p10; v[2] = p01; facet(v, 3, c1, tex, tile, ref, hc);
                v[0] = p10; v[1] = p11; v[2] = p01; facet(v, 3, c2, tex, tile, ref, h2);
            }
        }
    }
    if (port_verbose) port_printf("[port] ws room3d: %s end x %d, %d edges, wall z %d, %d units to cover, %d columns, %d facets (tile u %d..%d v %d..%d)\n",
                                  side < 0 ? "left" : "right", xs, ns, zmax, need, ncol, n3 + n4 + g3 + g4 - before, tile[0], tile[1], tile[2], tile[3]);
}

/* called from port_ws_area_model_link with the game's block list of this frame (list[3] = count, u32 pointers from
   +4, room for 88) and the area model's table */
void port_ws_room3d_blocks(char *list, const u8 *tbl, int area, int section)
{
    if (!port_ws_cull || !tbl || !room_enabled(area, section)) return;
    if (tbl[0] != 1 || (u8)list[3] != 1) return;                           /* these rooms: one block, the whole room */
    s32 off; memcpy(&off, tbl + 4, 4);
    const u8 *b = tbl + off;
    if (((u32 *)list)[1] != (u32)(unsigned long)b) return;
    /* the camera: VIEW_CAMERA (scratchpad 0xC0, MATRIX: m[3][3] s16, t[3] s32 at +0x14) must be unrotated */
    const u8 *c = (const u8 *)PORT_SPAD(0xC0);
    s16 m[9]; s32 t[3]; memcpy(m, c, 18); memcpy(t, c + 0x14, 12);
    for (int i = 0; i < 9; i++) if (m[i] != ((i % 4) ? 0 : 4096)) return;
    int h = (int)(u16)port_gte_cfc2(26), ofx = (int)((s32)port_gte_cfc2(24) >> 16);
    if (h < 64 || h > 4096) h = 544;
    if (ofx < 0 || ofx > 320) ofx = 160;
    int size = parse(b);
    if (!size) return;
    u32 key = 2166136261u;
    for (int i = 0; i < size; i++) key = (key ^ b[i]) * 16777619u;
    key = hsh(key, (u32)port_ws_cull, (u32)t[0]); key = hsh(key, (u32)t[2], (u32)(h * 512 + ofx));
    if (!have_key || key != cur_key) {
        have_key = 1; cur_key = key;
        n3 = n4 = g3 = g4 = 0;
        build_side(-1, t[0], t[2], h, ofx + port_ws_cull);
        build_side(1, t[0], t[2], h, 320 - ofx + port_ws_cull);
        u8 *d = blk; u32 n;
        n = (u32)n3; memcpy(d, &n, 4); d += 4; memcpy(d, ft3, (size_t)n3 * 32); d += n3 * 32;
        n = (u32)n4; memcpy(d, &n, 4); d += 4; memcpy(d, ft4, (size_t)n4 * 40); d += n4 * 40;
        n = (u32)g3; memcpy(d, &n, 4); d += 4; memcpy(d, gt3, (size_t)g3 * 40); d += g3 * 40;
        n = (u32)g4; memcpy(d, &n, 4); d += 4; memcpy(d, gt4, (size_t)g4 * 52); d += g4 * 52;
        memset(d, 0, 16);
        blk_polys = n3 + n4 + g3 + g4;
    }
    if (!blk_polys) return;
    ((u32 *)list)[2] = (u32)(unsigned long)blk; list[3] = 2;
}
