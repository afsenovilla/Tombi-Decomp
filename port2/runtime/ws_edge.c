/* --widescreen: the ends of a room (docs/PORT.md "Widescreen", "Room ends").
 *
 * A side-scrolling room is one WMD model cut into 80-unit sectors (drawAreaModelList / drawModelUnlit); its artwork
 * stops where the 4:3 camera stops, so on a wide frame the strip past the first / last sector showed the clear
 * colour: the ground, the steps and the tree line just ended in mid-air.
 *
 * This builds, once per room model and end, one extra sector in the model's own format and adds it to the game's draw
 * list while the end sectors are listed (port_ws_area_model_link), so the game's own drawer, GTE and culls draw it.
 * The sector continues the model past its end: every free edge of the silhouette that faces outwards (a ground strip
 * where it starts, the riser of a step, a tree-line card) is extruded along X, keeping the height and depth of the
 * edge, its colours and its texture rows; along X the texture goes on at the polygon's own texel density and wraps
 * inside the tile the room uses for that material (the union of the U ranges its neighbours of the same page, palette
 * and rows cover: 58 texels per 64 units of grass, 64 per leaf card).  No picture is mirrored or stretched and nothing
 * is drawn that the model does not already draw elsewhere: it is the room's own tiling, carried on.
 *
 * What is extruded (so that houses, roofs and one-off props are left alone):
 *   - the edge is on the end's silhouette: nothing of the model lies further out at its depth and height;
 *   - the polygon does not face sideways (|n.x| not dominant);
 *   - it is a surface that goes on: a ground-like polygon (|n.y| >= 0.5) that starts a chain of three or more polygons
 *     joined edge to edge towards the inside, or a polygon whose texture rectangle another polygon of the model uses
 *     too (a tile).
 * Rooms are enabled per (area, section, end) after a look at the result: port_ws_edge 1 = that list, 2 = every room of
 * the block-list areas (--ws-edge all, for checking new rooms), 0 = off.  Off (and without --widescreen) nothing here
 * runs. */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32; typedef short s16; typedef int s32;

extern int port_ws_cull, port_verbose;
extern int port_printf(const char *, ...);

int port_ws_edge = 1;

typedef struct { s16 x, y, z; u8 u, v; } Vx;
typedef struct {
    u16 clut, tpage; u8 cmd, nv, gour, pad;
    u32 col[4];
    Vx v[4];                              /* file order; x in end space (x' = s * x: the end is at min x') */
    s16 x0, x1, y0, y1, z0, z1; u8 u0, u1, v0, v1;
} Poly;

#define MAXP 12000
#define REACH 480                         /* how far in from the end a polygon may start and still be looked at */
#define PIECE 80                          /* longest piece (the GTE drops a polygon whose screen X overflows) */
#define MAXQ 1400                         /* quads per end */

static const u8 RING3[3] = {0, 1, 2}, RING4[4] = {0, 1, 3, 2};

static int parse(const u8 *tbl, Poly *ps, int s)
{
    int n = tbl[0], np = 0;
    const s32 *off = (const s32 *)(tbl + 4);
    if (n < 2) return 0;                                   /* a prop, not a room */
    for (int i = 0; i < n; i++) {
        const u8 *p = tbl + off[i];
        for (int g = 0; g < 4; g++) {
            u32 c; memcpy(&c, p, 4); p += 4;
            int nv = (g & 1) ? 4 : 3, gour = g >= 2;
            if (c > 4000) return -1;
            for (u32 k = 0; k < c; k++) {
                if (np >= MAXP) return -1;
                Poly *q = &ps[np];
                memcpy(&q->clut, p, 2); memcpy(&q->tpage, p + 2, 2); memcpy(&q->col[0], p + 4, 4);
                q->cmd = p[7]; q->nv = nv; q->gour = gour;
                if (((q->cmd & 8) != 0) != (nv == 4) || ((q->cmd & 0x10) != 0) != gour || (q->cmd & 0xE0) != 0x20) return -1;
                p += 8;
                for (int j = 1; j < nv; j++) { if (gour) { memcpy(&q->col[j], p, 4); p += 4; } else q->col[j] = q->col[0]; }
                q->x0 = q->y0 = q->z0 = 32767; q->x1 = q->y1 = q->z1 = -32768; q->u0 = q->v0 = 255; q->u1 = q->v1 = 0;
                for (int j = 0; j < nv; j++) {
                    Vx *v = &q->v[j]; memcpy(v, p, 8); p += 8;
                    if (v->x < -16000 || v->x > 16000) return -1;
                    v->x = (s16)(s * v->x);
                    if (v->x < q->x0) q->x0 = v->x; if (v->x > q->x1) q->x1 = v->x;
                    if (v->y < q->y0) q->y0 = v->y; if (v->y > q->y1) q->y1 = v->y;
                    if (v->z < q->z0) q->z0 = v->z; if (v->z > q->z1) q->z1 = v->z;
                    if (v->u < q->u0) q->u0 = v->u; if (v->u > q->u1) q->u1 = v->u;
                    if (v->v < q->v0) q->v0 = v->v; if (v->v > q->v1) q->v1 = v->v;
                }
                np++;
            }
        }
    }
    return np;
}

static int same_pos(const Vx *a, const Vx *b) { return a->x == b->x && a->y == b->y && a->z == b->z; }
static int has_vertex(const Poly *q, const Vx *a) { for (int j = 0; j < q->nv; j++) if (same_pos(&q->v[j], a)) return 1; return 0; }

/* another polygon uses (at least half of) this one's texture rectangle: a tile of the room */
static int tex_reused(const Poly *ps, int np, const Poly *p)
{
    int area = (p->u1 - p->u0 + 1) * (p->v1 - p->v0 + 1);
    for (int i = 0; i < np; i++) {
        const Poly *q = &ps[i];
        if (q == p || q->tpage != p->tpage || q->clut != p->clut) continue;
        int w = (q->u1 < p->u1 ? q->u1 : p->u1) - (q->u0 > p->u0 ? q->u0 : p->u0) + 1;
        int h = (q->v1 < p->v1 ? q->v1 : p->v1) - (q->v0 > p->v0 ? q->v0 : p->v0) + 1;
        if (w > 0 && h > 0 && 2 * w * h >= area) return 1;
    }
    return 0;
}

/* polygons joined edge to edge from p towards the inside (its two innermost vertices shared), p included; up to 3 */
static int chain(const Poly *ps, int np, const Poly *p)
{
    int len = 1;
    while (len < 3) {
        const Vx *r[2]; int nr = 0;
        for (int j = 0; j < p->nv; j++) if (p->v[j].x >= p->x1 - 1) { if (nr < 2) r[nr] = &p->v[j]; nr++; }
        if (nr != 2) break;
        const Poly *nx = 0;
        for (int i = 0; i < np && !nx; i++) {
            const Poly *q = &ps[i];
            if (q != p && q->x1 > p->x1 && has_vertex(q, r[0]) && has_vertex(q, r[1])) nx = q;
        }
        if (!nx) break;
        p = nx; len++;
    }
    return len;
}

/* the U range of the tile p's texture belongs to: the ranges of the polygons of its page, palette and rows, joined */
static void tile(const Poly *ps, int np, const Poly *p, int *t0, int *t1)
{
    int a = p->u0, b = p->u1, more = 1, vh = p->v1 - p->v0 + 1;
    while (more) {
        more = 0;
        for (int i = 0; i < np; i++) {
            const Poly *q = &ps[i];
            if (q->tpage != p->tpage || q->clut != p->clut) continue;
            int h = (q->v1 < p->v1 ? q->v1 : p->v1) - (q->v0 > p->v0 ? q->v0 : p->v0) + 1;
            if (2 * h < vh || q->u1 < a || q->u0 > b) continue;        /* (shares a texel column: 195..224 + 224..253) */
            if (q->u0 < a) { a = q->u0; more = 1; }
            if (q->u1 > b) { b = q->u1; more = 1; }
        }
    }
    *t0 = a; *t1 = b;
}

typedef struct { u8 flat[40]; } FQ;
typedef struct { u8 g[52]; } GQ;
typedef struct { u8 t[40]; } TR;          /* a triangle packet: flat 32 bytes, shaded 40 */
#define MAXT 300

/* A prop of the room stood again past its end: the polygons of the model inside a box (and on the given texture
   pages: its leaves and wood, not the ground or the fence under it) are copied, moved by (dx, dy, dz).  The first
   room: its big apple tree planted where the path ends on the left, so the way is visibly closed. */
static const struct { u8 area, section, end; s16 x0, x1, y1, z0, z1; u16 tp[2]; s16 dx, dy, dz, narrow; } PROPS[] = {
    /* (narrow: added to dx on a strip under 90 px, 16:9, so the trunk still stands inside the picture) */
    {0, 0, 0, 150, 430, -50, 50, 200, {0x12, 0x0d}, -425, 8, 0, 30},
};
static int cur_area, cur_section;
typedef struct { u8 *blk; int size, nq; } Block;

static void put_v(u8 *d, int s, float x, const Vx *e, float u)
{
    Vx v = *e;
    int xi = (int)(x < 0 ? x - 0.5f : x + 0.5f), ui = (int)(u + 0.5f);
    v.x = (s16)(s * xi); v.u = (u8)(ui < 0 ? 0 : ui > 255 ? 255 : ui);
    memcpy(d, &v, 8);
}

typedef struct { int poly, e, x0, x1, z0, z1, ground; } Cand;
#define MAXC 600

static void build(const u8 *tbl, int s, Block *out, int reach)
{
    Cand *cs = malloc(sizeof(Cand) * MAXC); int nc = 0;
    TR *ft = malloc(sizeof(TR) * MAXT), *gt = malloc(sizeof(TR) * MAXT); int nt3[2] = {0, 0}, nprop = 0;
    Poly *ps = malloc(sizeof(Poly) * MAXP);
    FQ *fq = malloc(sizeof(FQ) * MAXQ); GQ *gq = malloc(sizeof(GQ) * MAXQ);
    int nf = 0, ng = 0, np, nedge = 0;
    static struct { Vx a, b; } done[400]; int ndone = 0;
    out->nq = 0; out->size = 0;
    if (!ps || !fq || !gq || !cs || !ft || !gt || (np = parse(tbl, ps, s)) <= 0) goto end;
    int M = 32767;
    for (int i = 0; i < np; i++) if (ps[i].x0 < M) M = ps[i].x0;
    for (int i = 0; i < np; i++) {
        const Poly *p = &ps[i];
        if (p->x0 > M + REACH) continue;
        const u8 *ring = p->nv == 4 ? RING4 : RING3;
        const Vx *r0 = &p->v[ring[0]], *r1 = &p->v[ring[1]], *r2 = &p->v[ring[2]];
        long long ax = r1->x - r0->x, ay = r1->y - r0->y, az = r1->z - r0->z, bx = r2->x - r0->x, by = r2->y - r0->y, bz = r2->z - r0->z;
        long long nx = ay * bz - az * by, ny = az * bx - ax * bz, nz = ax * by - ay * bx;
        if (nx < 0) nx = -nx; if (ny < 0) ny = -ny; if (nz < 0) nz = -nz;
        if (nx > ny && nx > nz) continue;                              /* faces sideways */
        int ground = 4.0 * (double)ny * (double)ny >= (double)nx * nx + (double)ny * ny + (double)nz * nz;
        int checked = 0, ok = 0;
        for (int e = 0; e < p->nv; e++) {
            const Vx *a = &p->v[ring[e]], *b = &p->v[ring[(e + 1) % p->nv]];
            int dy = b->y - a->y, dz = b->z - a->z, dx = b->x - a->x;
            long long l2 = (long long)dy * dy + (long long)dz * dz;
            if (!l2 || 4 * l2 < (long long)dx * dx) continue;          /* runs along X: nothing to extrude */
            int out_facing = 1;                                        /* the rest of the polygon lies inwards of it */
            for (int k = 0; k < p->nv && out_facing; k++) {
                const Vx *c = &p->v[k];
                if (c == a || c == b) continue;
                double t = ((double)(c->y - a->y) * dy + (double)(c->z - a->z) * dz) / (double)l2;
                if (t < 0) t = 0; if (t > 1) t = 1;
                if (c->x <= a->x + t * dx + 0.5) out_facing = 0;
            }
            if (!out_facing) continue;
            int ex0 = a->x < b->x ? a->x : b->x, ey0 = a->y < b->y ? a->y : b->y, ey1 = a->y > b->y ? a->y : b->y;
            int ez0 = a->z < b->z ? a->z : b->z, ez1 = a->z > b->z ? a->z : b->z, blocked = 0;
            for (int k = 0; k < np && !blocked; k++) {                 /* something further out at this depth and height */
                const Poly *q = &ps[k];
                if (q == p || q->x0 >= ex0 - 1 || q->y1 < ey0 - 4 || q->y0 > ey1 + 4) continue;
                if (ez0 == ez1) blocked = q->z0 == q->z1 ? (q->z0 >= ez0 - 1 && q->z0 <= ez0 + 1) : (q->z0 <= ez0 && q->z1 >= ez0);
                else blocked = q->z0 == q->z1 ? (q->z0 > ez0 && q->z0 < ez1) : ((q->z0 > ez0 ? q->z0 : ez0) < (q->z1 < ez1 ? q->z1 : ez1));
            }
            if (blocked) continue;
            if (!checked) { checked = 1; ok = tex_reused(ps, np, p) || (ground && chain(ps, np, p) >= 3); }
            if (!ok) break;
            if (nc < MAXC) { Cand *c = &cs[nc++]; c->poly = i; c->e = e; c->x0 = ex0; c->x1 = a->x > b->x ? a->x : b->x; c->z0 = ez0; c->z1 = ez1; c->ground = ground && ez1 > ez0; }
        }
    }
    /* 2. where the room ends at each depth: the outermost ground edges (a ledge inside the room has the end's own
          ground further out at its depth).  Cards and risers count only at or past that line: a tree that stands
          inside the 4:3 picture keeps its outline. */
    for (int i = 0; i < nc; i++) {
        if (!cs[i].ground) continue;
        for (int k = 0; k < nc; k++)
            if (k != i && cs[k].ground && cs[k].x0 < cs[i].x0 - 8 && (cs[k].z0 > cs[i].z0 ? cs[k].z0 : cs[i].z0) < (cs[k].z1 < cs[i].z1 ? cs[k].z1 : cs[i].z1)) { cs[i].ground = -1; break; }
    }
    for (int i = 0; i < nc; i++) {
        Cand *c = &cs[i];
        if (c->ground == 1) continue;
        if (c->ground == -1) { c->poly = -1; continue; }
        int lim = -32768, best = 1 << 30, near = -32768;
        for (int k = 0; k < nc; k++) {
            if (cs[k].ground != 1) continue;
            if (cs[k].z0 - 64 <= c->z1 && cs[k].z1 + 64 >= c->z0) { if (cs[k].x1 > lim) lim = cs[k].x1; }
            int d = c->z0 > cs[k].z1 ? c->z0 - cs[k].z1 : cs[k].z0 > c->z1 ? cs[k].z0 - c->z1 : 0;
            if (d < best) { best = d; near = cs[k].x1; }
        }
        if (lim == -32768) lim = near;
        if (lim == -32768 || c->x0 > lim + 16) c->poly = -1;
    }
    /* 3. each edge carried on */
    for (int ci = 0; ci < nc; ci++) {
        if (cs[ci].poly < 0) continue;
        const Poly *p = &ps[cs[ci].poly];
        const u8 *ring = p->nv == 4 ? RING4 : RING3;
        int ground = cs[ci].ground;
        {
            int e = cs[ci].e;
            const Vx *a = &p->v[ring[e]], *b = &p->v[ring[(e + 1) % p->nv]];
            int dup = 0;
            for (int k = 0; k < ndone && !dup; k++)
                dup = (same_pos(&done[k].a, a) && same_pos(&done[k].b, b)) || (same_pos(&done[k].a, b) && same_pos(&done[k].b, a));
            if (dup) continue;
            if (ndone < 400) { done[ndone].a = *a; done[ndone].b = *b; ndone++; }
            /* texel density along X (texels per unit, signed) from the edge's neighbours in the ring */
            const Vx *na = &p->v[ring[(e + p->nv - 1) % p->nv]], *nb = &p->v[ring[(e + 2) % p->nv]];
            float rho = 0; int nr = 0;
            if (na->x - a->x >= 1) { rho += (float)(na->u - a->u) / (na->x - a->x); nr++; }
            if (nb->x - b->x >= 1) { rho += (float)(nb->u - b->u) / (nb->x - b->x); nr++; }
            if (nr) rho /= nr;
            int t0, t1; tile(ps, np, p, &t0, &t1);
            float ar = rho < 0 ? -rho : rho;
            if (ar < 0.01f || (t1 - t0) < 4 || (t1 - t0) / ar < 8) rho = ar = 0;   /* no usable tile: the edge's texel column */
            float xa = a->x, xb = b->x, ua = a->u, ub = b->u, xend = (xa < xb ? xa : xb) - reach;
            int ia = ring[e], ib = ring[(e + 1) % p->nv];
            u32 ca = p->col[p->gour ? ia : 0] & 0xFFFFFF, cb = p->col[p->gour ? ib : 0] & 0xFFFFFF;
            nedge++;
            if (port_verbose > 1) port_printf("[port] ws edge: %s cmd %02x tp %04x cl %04x a (%d,%d,%d u%d v%d) b (%d,%d,%d u%d v%d) poly x %d..%d u %d..%d v %d..%d tile %d..%d rho %.3f ground %d\n", s > 0 ? "L" : "R",
                p->cmd, p->tpage, p->clut, a->x, a->y, a->z, a->u, a->v, b->x, b->y, b->z, b->u, b->v, p->x0, p->x1, p->u0, p->u1, p->v0, p->v1, t0, t1, rho, ground);
            for (int guard = 0; guard < 64; guard++) {
                float xl = xa < xb ? xa : xb, nxt = xl - PIECE, wrap = -1e9f;
                if (xl <= xend + 0.5f) break;
                if (rho != 0) {
                    float wa = rho > 0 ? xa - (ua - t0) / rho : xa - (t1 - ua) / -rho;
                    float wb = rho > 0 ? xb - (ub - t0) / rho : xb - (t1 - ub) / -rho;
                    wrap = wa < wb ? wa : wb;
                    if (wrap > xl - 0.5f) { ua = ub = rho > 0 ? t1 : t0; continue; }   /* at the tile's end already */
                    if (wrap > nxt) nxt = wrap;
                }
                if (nxt < xend) nxt = xend;
                float ua2 = ua - rho * (xa - nxt), ub2 = ub - rho * (xb - nxt);
                if (ua2 < t0) ua2 = t0; if (ua2 > t1) ua2 = t1; if (ub2 < t0) ub2 = t0; if (ub2 > t1) ub2 = t1;
                if (rho == 0) { ua2 = ua; ub2 = ub; }
                if (nf + ng >= MAXQ) goto pack;
                /* the quad on the other side of the edge, same facing: ring (b, a, a', b') = strip order b, a, b', a' */
                u8 *d = p->gour ? gq[ng].g : fq[nf].flat, *vp;
                u32 w0 = cb | ((u32)(p->cmd | 8) << 24);
                memcpy(d, &p->clut, 2); memcpy(d + 2, &p->tpage, 2); memcpy(d + 4, &w0, 4);
                vp = d + 8;
                if (p->gour) { memcpy(d + 8, &ca, 4); memcpy(d + 12, &cb, 4); memcpy(d + 16, &ca, 4); vp = d + 20; ng++; } else nf++;
                put_v(vp, s, xb, b, ub); put_v(vp + 8, s, xa, a, ua); put_v(vp + 16, s, nxt, b, ub2); put_v(vp + 24, s, nxt, a, ua2);
                xa = xb = nxt; ua = ua2; ub = ub2;
                if (rho != 0 && nxt <= wrap + 0.01f) ua = ub = rho > 0 ? t1 : t0;
            }
        }
    }
pack:
    for (unsigned pi = 0; pi < sizeof PROPS / sizeof PROPS[0]; pi++) {
        if (PROPS[pi].area != cur_area || PROPS[pi].section != cur_section || PROPS[pi].end != (s < 0)) continue;
        for (int i = 0; i < np; i++) {
            const Poly *p = &ps[i];
            int rx0 = s > 0 ? p->x0 : -p->x1, rx1 = s > 0 ? p->x1 : -p->x0;
            if (p->tpage != PROPS[pi].tp[0] && p->tpage != PROPS[pi].tp[1]) continue;
            if (rx0 < PROPS[pi].x0 || rx1 > PROPS[pi].x1 || p->y0 > PROPS[pi].y1 || p->z0 < PROPS[pi].z0 || p->z1 > PROPS[pi].z1) continue;
            u8 *d;
            if (p->nv == 4) { if (nf + ng >= MAXQ) break; d = p->gour ? gq[ng++].g : fq[nf++].flat; }
            else { if (nt3[0] + nt3[1] >= MAXT) break; d = p->gour ? gt[nt3[1]++].t : ft[nt3[0]++].t; }
            memcpy(d, &p->clut, 2); memcpy(d + 2, &p->tpage, 2); memcpy(d + 4, &p->col[0], 4); d += 8;
            if (p->gour) for (int j = 1; j < p->nv; j++) { memcpy(d, &p->col[j], 4); d += 4; }
            for (int j = 0; j < p->nv; j++) {
                Vx v = p->v[j];
                v.x = (s16)(s * v.x + PROPS[pi].dx + (port_ws_cull < 90 ? PROPS[pi].narrow : 0)); v.y = (s16)(v.y + PROPS[pi].dy); v.z = (s16)(v.z + PROPS[pi].dz);
                memcpy(d, &v, 8); d += 8;
            }
            nprop++;
        }
    }
    if (nf + ng + nt3[0] + nt3[1]) {
        int size = 4 + nt3[0] * 32 + 4 + nf * 40 + 4 + nt3[1] * 40 + 4 + ng * 52 + 16;
        u8 *b = out->blk = realloc(out->blk, size), *d = b;
        if (b) {
            u32 c;
            c = nt3[0]; memcpy(d, &c, 4); d += 4; for (int i = 0; i < nt3[0]; i++) { memcpy(d, ft[i].t, 32); d += 32; }
            c = nf; memcpy(d, &c, 4); d += 4; memcpy(d, fq, nf * 40); d += nf * 40;
            c = nt3[1]; memcpy(d, &c, 4); d += 4; memcpy(d, gt, nt3[1] * 40); d += nt3[1] * 40;
            c = ng; memcpy(d, &c, 4); d += 4; memcpy(d, gq, ng * 52); d += ng * 52;
            memset(d, 0, 16);
            out->size = size; out->nq = nf + ng + nt3[0] + nt3[1];
        }
    }
    if (port_verbose) port_printf("[port] ws edge: %s end, %d polygons read, %d edges carried on, %d polygons of a prop copied: %d quads, %d triangles\n",
                                  s > 0 ? "left" : "right", np, nedge, nprop, nf + ng, nt3[0] + nt3[1]);
end:
    free(ps); free(fq); free(gq); free(cs); free(ft); free(gt);
}

/* the rooms looked at (area, section, ends: 1 = left, 2 = right) */
static const struct { u8 area, section, ends; } ROOMS[] = {
    {0, 0, 1},          /* Village of All Beginnings, the first room: its left end (the right end is the way on) */
};
static int room_ends(int area, int section)
{
    if (port_ws_edge >= 2) return 3;
    for (unsigned i = 0; i < sizeof ROOMS / sizeof ROOMS[0]; i++) if (ROOMS[i].area == area && ROOMS[i].section == section) return ROOMS[i].ends;
    return 0;
}

static Block blocks[2];
static u32 cur_key; static const u8 *cur_tbl; static int cur_reach;

/* called from port_ws_area_model_link with the game's (merged) block list of this frame: adds the end sectors while
   the first / last sector of the model is listed.  list: [3] = count, u32 pointers from +4, room for 88. */
void port_ws_edge_blocks(char *list, const u8 *tbl, int area, int section)
{
    if (!port_ws_cull || !port_ws_edge || !tbl) return;
    int ends = room_ends(area, section), n = tbl[0];
    if (!ends || n < 2) return;
    const s32 *off = (const s32 *)(tbl + 4);
    u32 key = (u32)n * 2654435761u; int reach = 3 * port_ws_cull + 96;
    for (int i = 0; i < n; i++) key = (key ^ (u32)off[i]) * 16777619u;
    for (int i = 0; i < 64; i++) key = (key ^ tbl[off[0] + i]) * 16777619u;
    if (key != cur_key || tbl != cur_tbl || reach != cur_reach || area != cur_area || section != cur_section) {
        cur_key = key; cur_tbl = tbl; cur_reach = reach; cur_area = area; cur_section = section;
        build(tbl, 1, &blocks[0], reach);
        build(tbl, -1, &blocks[1], reach);
    }
    /* near an end (one of its four outermost sectors listed): those four all drawn (the game's own selection leaves
       the outermost out: the 4:3 camera never showed it), then the end's block */
    int cnt = (u8)list[3], near[2] = {0, 0};
    for (int k = 0; k < cnt; k++) {
        u32 e = ((u32 *)list)[1 + k];
        for (int j = 0; j < 4 && j < n; j++) {
            near[0] |= e == (u32)(unsigned long)tbl + off[j];
            near[1] |= e == (u32)(unsigned long)tbl + off[n - 1 - j];
        }
    }
    for (int e = 0; e < 2; e++) {
        if (!(ends & (1 << e)) || !blocks[e].nq || !near[e]) continue;
        for (int j = 0; j <= 4 && j <= n; j++) {
            u32 w = j == 4 ? (u32)(unsigned long)blocks[e].blk : (u32)(unsigned long)tbl + off[e ? n - 1 - j : j];
            int k;
            if (j == 4 && n < 4) { /* (short models: every sector is an end sector) */ }
            if (j < 4 && j >= n) continue;
            cnt = (u8)list[3];
            for (k = 0; k < cnt; k++) if (((u32 *)list)[1 + k] == w) break;
            if (k < cnt) continue;
            if (cnt >= 88) return;
            ((u32 *)list)[1 + cnt] = w; list[3] = cnt + 1;
        }
    }
}
