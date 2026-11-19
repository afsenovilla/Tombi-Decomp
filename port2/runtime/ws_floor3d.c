/* --widescreen: the floor and the side walls of a 3D fixed-camera room carried on sideways (docs/PORT.md
 * "Widescreen", "3D room floors").
 *
 * AREA18 s1 (the Y crossing) is one small WMD model drawn by the area list through a fixed, unrotated camera: a grass
 * floor with a dirt path, two stone walls running back to a tower, and a slope of grass towards the viewer.  The
 * model is as wide as the 4:3 picture needs and no wider: the walls end 20 columns past the 4:3 frame and the floor's
 * rows end in straight free edges just past it, so a wide frame showed the sky beside the walls and under the floor.
 * Nothing is culled there (a reference with PORT_WSXTRA=300 draws the same frame): the geometry does not exist.
 *
 * This builds, per frame key (the model's bytes, the strip width and the camera), one extra block in the model's own
 * format and adds it to the game's draw list, so the game's drawer, GTE, culls and ordering table draw it.  Per side:
 *   - the wall's end is the vertical line (one x and z) that carries the most free polygon edges of polygons running
 *     back and inwards from it.  Every such polygon goes on towards the viewer along its own two edges (so the wall
 *     keeps its direction, the slope of its top and its rows), with its own texture continued the way the model
 *     continues it from one wall segment to the next (the texture starts again), and the colours of its end.  It goes
 *     on to the near end of the floor polygon it ends on and no further (the path passes in front of it);
 *   - a floor row is a free edge of a floor polygon that runs in depth (one x), the polygon inwards of it, the whole
 *     edge outside the 4:3 picture.  The row goes on outwards as cells with the edge's own heights and colours at
 *     each depth, textured by the edge polygon's own mapping moved by one period: the run of polygons the model maps
 *     with that same mapping (the dirt path: four cells in a row; the grass: one, repeated, as the model repeats it);
 *   - the rows the wall now stands on are filled up to the wall's line only.
 * Nothing is mirrored, no object is invented, and only the model's own texture pages, palettes, UVs and colours are
 * used.  Everything new lies outwards of an edge that is outside the 4:3 picture, so the 4:3 picture cannot change.
 * Integers only, no random values: i386 and wasm2c build the same block.
 *
 * Off by default: a room is extended when it is in ROOMS and approved there, or named by --ws-room3d-rooms, or with
 * --ws-room3d on (the flags of runtime/ws_room3d.c).  Without --widescreen nothing here runs. */
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "port_runtime.h"
#include "lazy_bss.h"

typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32; typedef short s16; typedef int s32;
typedef long long s64;

extern int port_ws_cull;
extern unsigned long port_gte_cfc2(int);
extern int port_ws_room3d_on;             /* runtime/ws_room3d.c: 1 the approved rooms, 0 off, 3 every room */
extern const char *port_ws_room3d_rooms;  /* --ws-room3d-rooms A:S,A:S,... or all */

/* the rooms looked at (the debug select's numbers); approved: on by default once the user has seen the room */
static const struct { u8 area, sec, approved; } ROOMS[] = {
    {18, 1, 1},        /* the Y crossing */
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
typedef struct { s32 x, y, z, u, v; u32 col; } Out;

#define MAXP 1024
#define MAXN 512                          /* packets per kind */
#define MAXCELL 24                        /* cells per floor row */
#define MAXROW 32

static Poly ps[MAXP]; static int np;
static u8 gt3[MAXN][40], gt4[MAXN][52]; static int g3, g4;
static u8 blk[16 + MAXN * (40 + 52) + 16]; static int blk_polys;
static u32 cur_key; static int have_key;
static int cam_x, cam_y, cam_z, cam_h;    /* the camera's translation and projection distance */

static const u8 RING3[3] = {0, 1, 2}, RING4[4] = {0, 1, 3, 2};

static int iabs(int a) { return a < 0 ? -a : a; }
static s32 rdiv(s64 n, s64 d)             /* n / d rounded to nearest, d != 0 */
{
    if (d < 0) { n = -n; d = -d; }
    return (s32)(n >= 0 ? (n + d / 2) / d : -((-n + d / 2) / d));
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
/* nothing goes on past the edge a-b: no other polygon with both vertices reaches outwards of it (side) or, with
   front set, nearer the viewer (a layer lying on the same polygons, like the shade along a wall's foot, does not) */
static int edge_free(const Poly *p, const Vx *a, const Vx *b, int side, int front)
{
    for (int k = 0; k < np; k++) {
        const Poly *q = &ps[k];
        if (q == p || (q->cmd & 2) || !has_vertex(q, a) || !has_vertex(q, b)) continue;
        for (int j = 0; j < q->nv; j++)
            if ((q->v[j].x - a->x) * side > 0 || (front && q->v[j].z < a->z)) return 0;
    }
    return 1;
}

/* the sign of the polygon on screen (what NCLIP tests), from camera space */
static int facing(const s32 *x, const s32 *y, const s32 *z)
{
    s64 ax = x[0] + cam_x, ay = y[0] + cam_y, az = z[0] + cam_z;
    s64 bx = x[1] + cam_x, by = y[1] + cam_y, bz = z[1] + cam_z;
    s64 cx = x[2] + cam_x, cy = y[2] + cam_y, cz = z[2] + cam_z;
    s64 d = ax * (by * cz - bz * cy) - ay * (bx * cz - bz * cx) + az * (bx * cy - by * cx);
    return d < 0 ? -1 : d > 0;
}
static int facing_poly(const Poly *p)
{
    s32 x[3], y[3], z[3];
    for (int i = 0; i < 3; i++) { x[i] = p->v[i].x; y[i] = p->v[i].y; z[i] = p->v[i].z; }
    return facing(x, y, z);
}

/* one gouraud polygon (a quad in strip order) of the kind of src, wound like ref (a sign of facing()) */
static void emit(Out *q, int nv, const Poly *src, int ref)
{
    s32 x[3], y[3], z[3];
    for (int i = 0; i < 3; i++) { x[i] = q[i].x; y[i] = q[i].y; z[i] = q[i].z; }
    int f = facing(x, y, z);
    if (f && ref && f != ref) {
        if (nv == 3) { Out t = q[1]; q[1] = q[2]; q[2] = t; }
        else { Out t = q[0]; q[0] = q[1]; q[1] = t; t = q[2]; q[2] = q[3]; q[3] = t; }
    }
    u8 *d;
    if (nv == 3) { if (g3 >= MAXN) return; d = gt3[g3++]; } else { if (g4 >= MAXN) return; d = gt4[g4++]; }
    u32 w0 = (q[0].col & 0xFFFFFF) | ((u32)(0x34 | (src->cmd & 3) | (nv == 4 ? 8 : 0)) << 24);
    memcpy(d, &src->clut, 2); memcpy(d + 2, &src->tpage, 2); memcpy(d + 4, &w0, 4); d += 8;
    for (int i = 1; i < nv; i++) { u32 c = q[i].col & 0xFFFFFF; memcpy(d, &c, 4); d += 4; }
    for (int i = 0; i < nv; i++) {
        Vx o; o.x = (s16)q[i].x; o.y = (s16)q[i].y; o.z = (s16)q[i].z; o.u = (u8)q[i].u; o.v = (u8)q[i].v;
        memcpy(d, &o, 8); d += 8;
    }
}

/* ---- the wall ---------------------------------------------------------------------------------------------------- */

typedef struct { int poly, a, b, a2, b2; } WEdge;       /* vertex indices: the free edge a-b, a2 / b2 behind a / b */

/* the free vertical edges at one (x, z) of quads that run back (larger z) and inwards from it, on the line carrying
   the most of them; returns the count */
static int find_wall_end(int side, WEdge *out, int max, int *xw, int *zw)
{
    static struct { WEdge e; s16 x, z; int len; } c[128]; int nc = 0;
    for (int i = 0; i < np; i++) {
        const Poly *p = &ps[i];
        if ((p->cmd & 2) || p->nv != 4) continue;
        for (int e = 0; e < 4 && nc < 128; e++) {
            int ia = RING4[e], ib = RING4[(e + 1) & 3], ib2 = RING4[(e + 2) & 3], ia2 = RING4[(e + 3) & 3];
            const Vx *a = &p->v[ia], *b = &p->v[ib], *a2 = &p->v[ia2], *b2 = &p->v[ib2];
            if (a->x != b->x || a->z != b->z || iabs(a->y - b->y) < 8) continue;
            if ((a->x + cam_x) * side <= 0) continue;                                  /* this side of the picture */
            if (a2->z <= a->z || b2->z <= b->z || a2->z != b2->z) continue;            /* the polygon runs back */
            if ((a2->x - a->x) * side >= 0 || (b2->x - b->x) * side >= 0) continue;    /* and inwards */
            if (!edge_free(p, a, b, side, 1)) continue;
            c[nc].e.poly = i; c[nc].e.a = ia; c[nc].e.b = ib; c[nc].e.a2 = ia2; c[nc].e.b2 = ib2;
            c[nc].x = a->x; c[nc].z = a->z; c[nc].len = iabs(a->y - b->y); nc++;
        }
    }
    int best = 0, bi = -1, n = 0;
    for (int i = 0; i < nc; i++) {
        int sum = 0;
        for (int k = 0; k < nc; k++) if (c[k].x == c[i].x && c[k].z == c[i].z) sum += c[k].len;
        if (sum > best) { best = sum; bi = i; }
    }
    if (bi < 0 || best < 64) return 0;
    for (int i = 0; i < nc && n < max; i++) if (c[i].x == c[bi].x && c[i].z == c[bi].z) out[n++] = c[i].e;
    *xw = c[bi].x; *zw = c[bi].z;
    return n;
}

/* a wall polygon carried on from its end edge to depth zend along its own two edges */
static void extend_wall(const WEdge *w, int zend)
{
    const Poly *p = &ps[w->poly];
    const Vx *e[2] = {&p->v[w->a], &p->v[w->b]}, *f[2] = {&p->v[w->a2], &p->v[w->b2]};
    int den = f[0]->z - e[0]->z, todo = e[0]->z - zend, done = 0;
    int ref = facing_poly(p);
    while (todo > 0 && den > 0) {
        int step = todo > den ? den : todo;           /* at most one length of the polygon: its texture once */
        Out q[4];
        for (int k = 0; k < 2; k++) {
            int dx = e[k]->x - f[k]->x, dy = e[k]->y - f[k]->y, du = e[k]->u - f[k]->u, dv = e[k]->v - f[k]->v;
            Out *o0 = &q[k], *o1 = &q[2 + k];         /* o0: the inner line of this piece, o1: its outer line */
            o0->x = e[k]->x + rdiv((s64)dx * done, den); o0->y = e[k]->y + rdiv((s64)dy * done, den); o0->z = e[k]->z - done;
            o1->x = e[k]->x + rdiv((s64)dx * (done + step), den); o1->y = e[k]->y + rdiv((s64)dy * (done + step), den);
            o1->z = e[k]->z - done - step;
            o0->u = f[k]->u; o0->v = f[k]->v;         /* the texture starts again, as at the model's own joints */
            o1->u = f[k]->u + rdiv((s64)du * step, den); o1->v = f[k]->v + rdiv((s64)dv * step, den);
            o0->col = o1->col = p->col[k ? w->b : w->a];
        }
        emit(q, 4, p, ref);
        done += step; todo -= step;
    }
}

/* ---- the floor --------------------------------------------------------------------------------------------------- */

typedef struct {
    int poly, a, b, c;                    /* the free edge a-b (a nearer the viewer), c the vertex inwards of a */
    int xe, z0, z1, w, period;            /* the edge's x, its depths z0 < z1, the cell width, the texture's period */
    int ub[4];                            /* the UV box of the polygons sharing the mapping */
    Vx cv;                                /* the mapping's third point: c, or the furthest vertex sharing it */
} Row;

/* the edge polygon's own texture mapping at t units inwards of the edge, depth z */
static void row_uv(const Row *r, int t, int z, s32 *u, s32 *v)
{
    const Poly *p = &ps[r->poly];
    const Vx *a = &p->v[r->a], *b = &p->v[r->b], *c = &r->cv;
    s64 dx2 = iabs(c->x - r->xe), dz1 = b->z - a->z, dz2 = c->z - a->z, den = dz1 * dx2;
    s64 k = (s64)(z - a->z) * dx2 - (s64)t * dz2;
    *u = a->u + rdiv(k * (b->u - a->u) + (s64)t * (c->u - a->u) * dz1, den);
    *v = a->v + rdiv(k * (b->v - a->v) + (s64)t * (c->v - a->v) * dz1, den);
}
static void row_clamp(const Row *r, Out *o)
{
    o->u = o->u < r->ub[0] ? r->ub[0] : o->u > r->ub[1] ? r->ub[1] : o->u;
    o->v = o->v < r->ub[2] ? r->ub[2] : o->v > r->ub[3] ? r->ub[3] : o->v;
}

/* the free edges of floor polygons that run in depth at one x, the polygon inwards, wholly outside the 4:3 picture */
static int find_rows(int side, Row *out, int max, int half43)
{
    int n = 0;
    for (int i = 0; i < np && n < max; i++) {
        const Poly *p = &ps[i];
        if (p->cmd & 2) continue;
        const u8 *ring = p->nv == 4 ? RING4 : RING3;
        s32 ax = p->v[1].x - p->v[0].x, ay = p->v[1].y - p->v[0].y, az = p->v[1].z - p->v[0].z;
        s32 bx = p->v[2].x - p->v[0].x, by = p->v[2].y - p->v[0].y, bz = p->v[2].z - p->v[0].z;
        s32 nx = iabs(ay * bz - az * by), ny = iabs(az * bx - ax * bz), nz = iabs(ax * by - ay * bx);
        if (ny < 2 * nx || ny < 2 * nz) continue;                         /* not a floor */
        for (int e = 0; e < p->nv && n < max; e++) {
            int ia = ring[e], ib = ring[(e + 1) % p->nv], ic = ring[(e + p->nv - 1) % p->nv];
            if (p->v[ia].z > p->v[ib].z) { int t = ia; ia = ib; ib = t; ic = ring[(e + 2) % p->nv]; }
            const Vx *a = &p->v[ia], *b = &p->v[ib], *c = &p->v[ic];
            int ok = 1;
            if (a->x != b->x || b->z - a->z < 8) continue;
            if ((a->x + cam_x) * side <= 0) continue;
            for (int k = 0; k < p->nv; k++) if (k != ia && k != ib && (p->v[k].x - a->x) * side >= 0) ok = 0;
            if (!ok || !edge_free(p, a, b, side, 0)) continue;
            /* both ends outside the 4:3 picture (so is everything outwards of the edge) */
            if ((s64)(a->x + cam_x) * side * cam_h < (s64)half43 * (a->z + cam_z)) continue;
            if ((s64)(b->x + cam_x) * side * cam_h < (s64)half43 * (b->z + cam_z)) continue;
            if (a->z + cam_z < 64) continue;
            Row *r = &out[n];
            r->poly = i; r->a = ia; r->b = ib; r->c = ic; r->xe = a->x; r->z0 = a->z; r->z1 = b->z;
            r->w = iabs(c->x - a->x);
            if (r->w < 8) continue;
            /* the period: how far inwards the model goes on with this very mapping */
            r->period = r->w; r->cv = *c;
            for (int pass = 0; pass < 3; pass++) {
                Vx fv = r->cv;
                r->ub[0] = r->ub[2] = 255; r->ub[1] = r->ub[3] = 0;
                for (int k = 0; k < np; k++) {
                    const Poly *q = &ps[k]; int same = q->clut == p->clut && q->tpage == p->tpage, far = 0;
                    for (int j = 0; j < q->nv && same; j++) {
                        const Vx *o = &q->v[j]; s32 u, v; int t = (r->xe - o->x) * side;
                        if (t < 0 || o->z < r->z0 || o->z > r->z1) { same = 0; break; }
                        row_uv(r, t, o->z, &u, &v);
                        if (iabs(u - o->u) > 2 + t / 16 || iabs(v - o->v) > 2 + t / 16) same = 0;
                        far = t > far ? t : far;
                    }
                    if (!same) continue;
                    r->period = far > r->period ? far : r->period;
                    for (int j = 0; j < q->nv; j++) if ((r->xe - q->v[j].x) * side > (r->xe - fv.x) * side) fv = q->v[j];
                    for (int j = 0; j < q->nv; j++) {
                        r->ub[0] = q->v[j].u < r->ub[0] ? q->v[j].u : r->ub[0]; r->ub[1] = q->v[j].u > r->ub[1] ? q->v[j].u : r->ub[1];
                        r->ub[2] = q->v[j].v < r->ub[2] ? q->v[j].v : r->ub[2]; r->ub[3] = q->v[j].v > r->ub[3] ? q->v[j].v : r->ub[3];
                    }
                }
                r->cv = fv;                   /* measured over the whole run: the model's UVs are rounded per vertex */
            }
            n++;
        }
    }
    return n;
}

/* a cell of row r: from d0 to d1 units outwards of the edge at depth z0 (e0 / e1 at z1), d < the period */
static void cell(const Row *r, int side, int d0, int d1, int e0, int e1, int shift)
{
    const Poly *p = &ps[r->poly];
    const Vx *a = &p->v[r->a], *b = &p->v[r->b];
    Out q[4]; int d[4] = {d0, e0, d1, e1};
    for (int i = 0; i < 4; i++) {
        const Vx *s = (i & 1) ? b : a;
        q[i].x = r->xe + side * d[i]; q[i].y = s->y; q[i].z = s->z; q[i].col = p->col[(i & 1) ? r->b : r->a];
        row_uv(r, shift - d[i], s->z, &q[i].u, &q[i].v);
        row_clamp(r, &q[i]);
    }
    int ref = facing_poly(p);
    if (d0 == d1) { Out t[3] = {q[0], q[1], q[3]}; emit(t, 3, p, ref); }
    else if (e0 == e1) { Out t[3] = {q[0], q[1], q[2]}; emit(t, 3, p, ref); }
    else emit(q, 4, p, ref);
}

static void build_side(int side, int half)
{
    WEdge we[24]; Row rows[MAXROW];
    int xw = 0, zw = 0, zend = 0, have_wall = 0;
    int nw = find_wall_end(side, we, 24, &xw, &zw);
    int nr = find_rows(side, rows, MAXROW, side < 0 ? -cam_x : 320 + cam_x);
    int before = g3 + g4;
    /* the wall goes on to the near end of the floor polygon it ends on */
    const Vx *wb = 0, *wb2 = 0;
    if (nw) {
        for (int i = 0; i < nr; i++) if (rows[i].xe == xw && rows[i].z1 == zw) { zend = rows[i].z0; have_wall = 1; }
        for (int i = 0; i < nw; i++) {                                    /* its lowest edge: the wall's line on the floor */
            const Poly *p = &ps[we[i].poly];
            for (int k = 0; k < 2; k++) {
                const Vx *v = &p->v[k ? we[i].b : we[i].a], *v2 = &p->v[k ? we[i].b2 : we[i].a2];
                if (!wb || v->y > wb->y) { wb = v; wb2 = v2; }
            }
        }
    }
    for (int i = 0; i < nr; i++) {
        const Row *r = &rows[i];
        if (have_wall && r->xe == xw && r->z0 >= zend && r->z1 <= zw) {
            /* under the wall: up to the wall's line, x = xw + (xw - x behind) * (zw - z) / (z behind - zw) */
            int den = wb2->z - wb->z;
            int d0 = iabs(rdiv((s64)(wb->x - wb2->x) * (zw - r->z0), den)), d1 = iabs(rdiv((s64)(wb->x - wb2->x) * (zw - r->z1), den));
            int m = d0 > d1 ? d0 : d1;
            if (m > 0) cell(r, side, 0, d0, 0, d1, r->period * ((m - 1) / r->period + 1));
            continue;
        }
        if (have_wall && r->xe == xw && r->z1 > zend && r->z0 < zw) continue;      /* a row the wall's end cuts: not handled */
        /* the frame's edge at the row's far depth, from the camera's axis; the edge, from the same */
        int need = (half + 2) * (r->z1 + cam_z) / cam_h + 2 - (r->xe + cam_x) * side;
        int nc = need <= 0 ? 0 : (need + r->w - 1) / r->w;
        if (nc > MAXCELL) nc = MAXCELL;
        for (int j = 0; j < nc; j++) {
            int shift = r->period * ((j * r->w) / r->period + 1);         /* the cell's place in the period before */
            cell(r, side, j * r->w, (j + 1) * r->w, j * r->w, (j + 1) * r->w, shift);
        }
    }
    if (have_wall) for (int i = 0; i < nw; i++) extend_wall(&we[i], zend);
    if (port_verbose) {
        port_printf("[port] ws floor3d: %s: wall end x %d z %d, %d edges, on to z %d (%s); %d floor rows; %d polygons\n",
                    side < 0 ? "left" : "right", xw, zw, nw, zend, have_wall ? "floor found" : "no wall", nr, g3 + g4 - before);
        for (int i = 0; i < nr; i++)
            port_printf("[port] ws floor3d:   row x %d z %d..%d, cell %d, period %d, clut %04x, u %d..%d v %d..%d\n", rows[i].xe, rows[i].z0, rows[i].z1,
                        rows[i].w, rows[i].period, ps[rows[i].poly].clut, rows[i].ub[0], rows[i].ub[1], rows[i].ub[2], rows[i].ub[3]);
    }
}

/* called from port_ws_area_model_link with the game's block list of this frame (list[3] = count, u32 pointers from
   +4, room for 88) and the area model's table */
void port_ws_floor3d_blocks(char *list, const u8 *tbl, int area, int section)
{
    if (!PORT_LAZY_OK()) return;            /* (the 3DS: its tables are not in the program, runtime/lazy_bss.h) */
    if (!port_ws_cull || !tbl || !room_enabled(area, section)) return;
    if (tbl[0] != 1 || (u8)list[3] != 1) return;                           /* one block, the whole room */
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
    if (t[0] != -ofx) return;                                              /* the camera's axis through the picture's middle column */
    int size = parse(b);
    if (!size) return;
    u32 key = 2166136261u;
    for (int i = 0; i < size; i++) key = (key ^ b[i]) * 16777619u;
    key = (key ^ (u32)port_ws_cull) * 16777619u; key = (key ^ (u32)t[0]) * 16777619u; key = (key ^ (u32)t[1]) * 16777619u;
    key = (key ^ (u32)t[2]) * 16777619u; key = (key ^ (u32)(h * 512 + ofx)) * 16777619u;
    if (!have_key || key != cur_key) {
        have_key = 1; cur_key = key;
        g3 = g4 = 0;
        cam_x = t[0]; cam_y = t[1]; cam_z = t[2]; cam_h = h;
        build_side(-1, ofx + port_ws_cull);
        build_side(1, 320 - ofx + port_ws_cull);
        u8 *d = blk; u32 n = 0;
        memcpy(d, &n, 4); d += 4;
        memcpy(d, &n, 4); d += 4;
        n = (u32)g3; memcpy(d, &n, 4); d += 4; memcpy(d, gt3, (size_t)g3 * 40); d += g3 * 40;
        n = (u32)g4; memcpy(d, &n, 4); d += 4; memcpy(d, gt4, (size_t)g4 * 52); d += g4 * 52;
        memset(d, 0, 16);
        blk_polys = g3 + g4;
    }
    if (!blk_polys) return;
    ((u32 *)list)[2] = (u32)(unsigned long)blk; list[3] = 2;
}
