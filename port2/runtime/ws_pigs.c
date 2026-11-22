/* --widescreen: the Evil Pig battle rooms carried on past the ends of their model (docs/PORT.md "Widescreen",
 * "Evil Pig rooms").
 *
 * The eight rooms of AREA14 (sections 0-6 the seven Evil Pigs, 7 the real one) are one WMD block each, the whole room,
 * drawn by the area list through an unrotated camera that only scrolls (VIEW_CAMERA identity, H 544: screen
 * x = 160 + 544 (x - cx) / (z + 544), y = 128 + 544 (y - cy) / (z + 544), (cx, cy) inside CAMERA_SCROLL minX..maxX /
 * minY..maxY; section 7's camera is pitched and needs nothing).  The model is as wide as the 4:3 camera needs and no
 * wider: with the camera at a limit the wide frame looks past its end and shows the clear colour in pieces between
 * the rocks, or a whole flat strip.  Nothing is culled there (PORT_WSXTRA=300 draws the same pixels; PORT_WSCLEAR
 * paints what no primitive covers): the geometry does not exist.
 *
 * This builds, once per room model / strip width / camera range, one extra block in the model's own format and adds
 * it to the game's draw list, so the game's drawer, GTE, NCLIP, culls and ordering table draw it.  Per side:
 *   - the model's end is its free polygon edges (edges of one drawn polygon only; polygons NCLIP culls do not count)
 *     that run up / down or in depth, at most 48 units inside the 4:3 frame's edge, with the polygon lying inwards
 *     and nothing of the model drawn just outside them (seen from the camera at that limit, at the height of its
 *     range that faces the edge);
 *   - a straight upright line of such edges (3 or more, 150 units) is a wall that was cut there (a pillar's face, a
 *     cave wall): it goes on whatever lies behind it;
 *   - from every edge the surface goes on outwards along x, at the edge's own depth, as columns of facets: a grid
 *     whose first line is the edge itself (same vertices: no crack), the other lines one column further out (the
 *     median reach of the end's polygons, 28-72 units) with every vertex moved a little (a hash of its place), each
 *     cell a quad or two triangles as the polygons of that end are; a column stops where the model has something
 *     of its own;
 *   - an upright rectangle in a plane of constant depth (a block of a pillar, a card of a tiled backdrop) goes on
 *     as itself: columns exactly its width, no vertex moved, its own texture rectangle again (period and phase kept);
 *   - any other facet takes its texture (page, palette, rectangle) from a polygon of that end near it that is about
 *     a column wide (so the texels keep their size), laid the same way round (never mirrored; a triangle's inner
 *     half is the source turned half round), and its colours from the vertices of the end near it (gouraud colours
 *     meet along shared vertices); on the edge itself the colours are the edge polygon's own, so the join is one more
 *     facet line of the same rock.  Polygons whose texture rectangle holds see-through texels (read from VRAM: cloud
 *     layers, fringes) are not used as a facet's look;
 *   - where a run of edges ends and the model has something further in but nothing outside at the heights just past
 *     it, the surface goes on 96 units up / down (a skirt) to close the corner;
 *   - the caves (ROOMS `curtain`) get, behind everything (the model's far depth + 40), a rock wall of the same facets
 *     for whatever gap is left, with the look of the end's far polygon nearest each height;
 *   - nothing is put where a 4:3 frame could ever see it: every new vertex lies outside the 4:3 frustum of the camera
 *     at its limit (x beyond limit -/+ (160 (z + 544) / 544 + 3)), so the 4:3 picture and the 4:3 centre of the wide
 *     one keep the game's own gaps (the caves have black ones by design, the lava cave between its lava curtains).
 * Integers only; every random value is a hash of the position, taken in its own statement, so i386 and wasm2c build
 * the same block and a wider strip starts with the columns of a narrower one.
 *
 * Off by default: a room is extended when it is in ROOMS and approved there, or named by --ws-pigs-rooms, or with
 * --ws-pigs on (every room of the table).  Without --widescreen nothing here runs. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "port_runtime.h"
#include "ws_heap.h"

typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32; typedef short s16; typedef int s32;
typedef long long s64;

extern int port_ws_cull;
extern unsigned long port_gte_cfc2(int);

int port_ws_pigs_on = 1;                 /* 1: the approved rooms (default), 0: off, 3: every room of the table (main.c) */
const char *port_ws_pigs_rooms;          /* --ws-pigs-rooms A:S,A:S,... (decimal; "all" = every room of the table) */

/* the rooms that need it (the debug select's numbers; sections 1, 4 and 7 show nothing missing); approved: on by
   default once the user has seen the room; curtain: a cave, a rock wall behind everything for the gaps left */
static const struct { u8 area, sec, approved, curtain; } ROOMS[] = {
    {14, 0, 1, 1},     /* the grey rock cave with the windmill (the storm pig; evilPigAttackPhoenixMountain) */
    {14, 2, 1, 1},     /* the blue cave with the columns (evilPigAttackTrickVillage) */
    {14, 3, 1, 1},     /* the lava cave (evilPigAttackLavaCaves): only 21:9 and wider look past its ends */
    {14, 5, 1, 0},     /* the flower room between two stone pillars (evilPigAttackDeepJungle) */
    {14, 6, 1, 1},     /* the grey cave with the marble slabs (evilPigAttackHauntedMansion): 21:9 and wider */
};
static int room_curtain;
static int room_enabled(int area, int sec)
{
    for (unsigned i = 0; i < sizeof ROOMS / sizeof ROOMS[0]; i++) {
        if (ROOMS[i].area != area || ROOMS[i].sec != sec) continue;
        const char *l = port_ws_pigs_rooms;
        room_curtain = ROOMS[i].curtain;
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
        return port_ws_pigs_on == 3 || (port_ws_pigs_on && ROOMS[i].approved);
    }
    return 0;
}

typedef struct { s16 x, y, z; u8 u, v; } Vx;
typedef struct { u16 clut, tpage; u8 cmd, nv, gour, back, holes, pad[3]; u32 col[4]; Vx v[4]; } Poly;
/* a free edge of the model's end: a = its upper end (or the nearer one), b the other; c / d = the polygon's vertex
   next to b / a (a triangle: both its third vertex); fwd: a -> b is the polygon's own winding */
typedef struct { s16 poly; u8 ia, ib, ic, id, fwd, reg, wall, virt; s16 ra, rb; s16 my, mz, wd; } Edge;
typedef struct { s16 x, y, z, x0, amp, w; s16 edge; u8 uses, reg; } Rail;
typedef struct { s16 x, y, z; } P3;

#define MAXP 1600
#define MAXE 320                          /* edges per side */
#define MAXN 900                          /* packets per kind */
#define MAXCOL 20
#define H 544
#define INSIDE 48                         /* an end's edge lies at most this far inside the 4:3 frame's edge */

/* the tables on the heap (ws_heap.h): ps and blk while the room is shown (the model is read every frame, the game
   draws blk), the others while the extension is made */
#define BLK (16 + MAXN * (32 + 40 + 40 + 52) + 16)
static Poly (*ps); static int np;
static Edge (*ed); static int ne;
static Rail (*rl); static int nr;
static u8 (*ft3)[32], (*ft4)[40], (*gt3)[40], (*gt4)[52]; static int n3, n4, g3, g4;
static u8 (*blk); static int blk_polys;
static u32 cur_key; static int have_key;
static const WsTab ROOM_TABS[] = { WS_T(ps, MAXP), WS_T(blk, BLK) };
static const WsTab GEN_TABS[] = { WS_T(ed, MAXE), WS_T(rl, 2 * MAXE), WS_T(ft3, MAXN), WS_T(ft4, MAXN), WS_T(gt3, MAXN), WS_T(gt4, MAXN) };
static void pigs_reset(void) { have_key = 0; blk_polys = 0; }   /* (the room changed: blk goes, made again on the way back) */
static int colw;                          /* column width, units */

static const u8 RING3[3] = {0, 1, 2}, RING4[4] = {0, 1, 3, 2};

static u32 hsh(u32 a, u32 b, u32 c)
{
    u32 h = a * 0x9E3779B1u + 0x7F4A7C15u;
    h ^= h >> 15; h = (h ^ b) * 0x85EBCA6Bu;
    h ^= h >> 13; h = (h ^ c) * 0xC2B2AE35u;
    h ^= h >> 16; h *= 0x27D4EB2Fu; h ^= h >> 15;
    return h;
}
static int iabs(int a) { return a < 0 ? -a : a; }
static int isqrt(int v) { int r = 0; if (v <= 0) return 0; for (int b = 1 << 15; b; b >>= 1) if ((r + b) * (r + b) <= v) r += b; return r; }

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

/* does the polygon's texture rectangle hold see-through texels (palette entries of value 0)?  Read from VRAM as it
   is now: the room's own texture.  Such a polygon is a cut-out (a cloud layer, a fringe): fine where the model has
   something behind it, not as the look of a new facet. */
extern const unsigned short *port_gpu_vram(int x, int y, int w, int h);
static void find_holes(void)
{
    const unsigned short *vr = port_gpu_vram(0, 0, 1024, 512);
    for (int i = 0; i < np; i++) {
        Poly *p = &ps[i];
        int u0 = 255, u1 = 0, v0 = 255, v1 = 0, bad = 0, n = 0;
        for (int j = 0; j < p->nv; j++) {
            u0 = p->v[j].u < u0 ? p->v[j].u : u0; u1 = p->v[j].u > u1 ? p->v[j].u : u1;
            v0 = p->v[j].v < v0 ? p->v[j].v : v0; v1 = p->v[j].v > v1 ? p->v[j].v : v1;
        }
        int tx = (p->tpage & 15) * 64, ty = (p->tpage & 16) ? 256 : 0, depth = (p->tpage >> 7) & 3;
        int cx = (p->clut & 63) * 16, cy = (p->clut >> 6) & 511;
        for (int v = v0; v <= v1; v++)
            for (int u = u0; u <= u1; u++) {
                unsigned short t;
                if (depth == 0)      t = vr[cy * 1024 + ((cx + ((vr[(ty + v) * 1024 + ((tx + u / 4) & 1023)] >> ((u & 3) * 4)) & 15)) & 1023)];
                else if (depth == 1) t = vr[cy * 1024 + ((cx + ((vr[(ty + v) * 1024 + ((tx + u / 2) & 1023)] >> ((u & 1) * 8)) & 255)) & 1023)];
                else                 t = vr[(ty + v) * 1024 + ((tx + u) & 1023)];
                bad += t == 0; n++;
            }
        p->holes = (u8)(bad * 64 > n);                                   /* more than a stray texel or two */
    }
}

static int same_pos(const Vx *a, const Vx *b) { return a->x == b->x && a->y == b->y && a->z == b->z; }
static int has_vertex(const Poly *q, const Vx *a) { for (int j = 0; j < q->nv; j++) if (same_pos(&q->v[j], a)) return 1; return 0; }

static int cam_y0, cam_y1;                 /* the camera's height range */
/* which way round a triangle is on the screen, seen from the camera at (cx, cy): the sign NCLIP tests */
static int turn(const s16 *a, const s16 *b, const s16 *c, int cx, int cy)
{
    s32 X[3], Y[3]; const s16 *v[3] = {a, b, c};
    for (int j = 0; j < 3; j++) {
        int d = v[j][2] + H; if (d < 32) d = 32;
        X[j] = (s32)(((s64)(v[j][0] - cx) * H * 16) / d); Y[j] = (s32)(((s64)(v[j][1] - cy) * H * 16) / d);
    }
    s64 cr = (s64)(X[1] - X[0]) * (Y[2] - Y[0]) - (s64)(Y[1] - Y[0]) * (X[2] - X[0]);
    return cr > 0 ? 1 : cr < 0 ? -1 : 0;
}
static int front;                          /* the sign of the polygons the game draws (the others are culled) */
static void facing(int cx)
{
    int n[2] = {0, 0}, cy = (cam_y0 + cam_y1) / 2;
    for (int i = 0; i < np; i++) { int t = turn(&ps[i].v[0].x, &ps[i].v[1].x, &ps[i].v[2].x, cx, cy); ps[i].back = (u8)(t > 0); n[t > 0] += t != 0; }
    front = n[1] > n[0] ? 1 : -1;
    if (front > 0) for (int i = 0; i < np; i++) ps[i].back = !ps[i].back;
}

/* is the point drawn over by the model (any depth), seen from the camera at cx and at the height of its range that
   looks at the point most squarely?  1 too for a point no frame shows (above / below every frame).  screen units * 16 */
static int covered(int x, int y, int z, int cx)
{
    int cy = y < cam_y0 ? cam_y0 : y > cam_y1 ? cam_y1 : y;
    if (z + H < 32) return 1;
    if (iabs(y - cy) * H > 136 * (z + H)) return 1;
    s32 px = (s32)(((s64)(x - cx) * H * 16) / (z + H)), py = (s32)(((s64)(y - cy) * H * 16) / (z + H));
    for (int i = 0; i < np; i++) {
        const Poly *p = &ps[i];
        if ((p->cmd & 2) || p->back) continue;
        s32 X[4], Y[4]; int ok = 1;
        for (int j = 0; j < p->nv; j++) {
            int d = p->v[j].z + H;
            if (d < 32) { ok = 0; break; }
            X[j] = (s32)(((s64)(p->v[j].x - cx) * H * 16) / d); Y[j] = (s32)(((s64)(p->v[j].y - cy) * H * 16) / d);
        }
        if (!ok) continue;
        for (int t = 0; t + 2 < p->nv + 0 && t < 2; t++) {            /* a quad: triangles 0 1 2 and 1 2 3 */
            const int a = t, b = t + 1, c = t + 2;
            if (c >= p->nv) break;
            s64 d1 = (s64)(X[b] - X[a]) * (py - Y[a]) - (s64)(Y[b] - Y[a]) * (px - X[a]);
            s64 d2 = (s64)(X[c] - X[b]) * (py - Y[b]) - (s64)(Y[c] - Y[b]) * (px - X[b]);
            s64 d3 = (s64)(X[a] - X[c]) * (py - Y[c]) - (s64)(Y[a] - Y[c]) * (px - X[c]);
            if ((d1 >= 0 && d2 >= 0 && d3 >= 0) || (d1 <= 0 && d2 <= 0 && d3 <= 0)) return 1;
        }
    }
    return 0;
}

/* side -1 / +1; lim = the camera limit on that side (the world x at the screen's centre); the first x a 4:3 frame can
   never show at depth z, and the wide frame's edge there */
static int x_never(int side, int lim, int z) { return lim + side * (160 * (z + H) / H + 3); }
static int x_wide(int side, int lim, int z) { return lim + side * ((160 + port_ws_cull + 3) * (z + H) / H + 2); }
static int x_start(int side, int lim, int x, int z) { int n = x_never(side, lim, z); return (x - n) * side >= 0 ? x : n; }

static int rail_of(const Vx *v, int side, int lim, int e, int w)
{
    for (int i = 0; i < nr; i++) if (rl[i].x == v->x && rl[i].y == v->y && rl[i].z == v->z) { rl[i].uses++; return i; }
    if (nr >= 2 * MAXE) return 0;
    Rail *r = &rl[nr]; r->x = v->x; r->y = v->y; r->z = v->z; r->amp = 12; r->edge = (s16)e; r->uses = 1; r->w = (s16)w; r->reg = w != 0;
    r->x0 = (s16)x_start(side, lim, v->x, v->z);
    if (w && r->x0 != v->x) {                  /* a tiled wall keeps its phase: whole tiles from the edge */
        int n = ((r->x0 - v->x) * side + w - 1) / w;
        r->x0 = (s16)(v->x + side * n * w);
    }
    return nr++;
}

/* PORT_WS_PIGS_LOG=1 (research): every free edge looked at with what became of it, and the camera each frame */
static int log_on;
#define DBG(why) do { if (dbg) port_printf("[pigs] cand %d %d %d %d %d %d %d %s\n", side, a->x, a->y, a->z, b->x, b->y, b->z, why); } while (0)
static int find_edges(int side, int lim)
{
    facing(lim);
    int dbg = log_on;
    ne = 0; nr = 0;
    for (int i = 0; i < np; i++) {
        const Poly *p = &ps[i];
        if ((p->cmd & 2) || p->back) continue;                            /* translucent layers; culled polygons */
        const u8 *ring = p->nv == 4 ? RING4 : RING3;
        for (int e = 0; e < p->nv && ne < MAXE - 64; e++) {
            int ia = ring[e], ib = ring[(e + 1) % p->nv];
            const Vx *a = &p->v[ia], *b = &p->v[ib];
            int dx = iabs(b->x - a->x), dy = iabs(b->y - a->y), dz = iabs(b->z - a->z);
            if (dy + dz < 8 || dx > 4 * (dy + dz)) continue;              /* runs along x: nothing to carry on */
            /* inside the frame this strip width gives, with the camera at the limit? */
            if ((x_start(side, lim, a->x, a->z) - x_wide(side, lim, a->z)) * side >= 0 &&
                (x_start(side, lim, b->x, b->z) - x_wide(side, lim, b->z)) * side >= 0) continue;
            int sum = 0, cnt = 0, used = 0;
            for (int k = 0; k < np && !used; k++) {
                const Poly *q = &ps[k];
                if (q != p && !(q->cmd & 2) && !q->back && has_vertex(q, a) && has_vertex(q, b)) used = 1;
            }
            if (used) continue;
            for (int k = 0; k < p->nv; k++) if (k != ia && k != ib) { sum += p->v[k].x; cnt++; }
            if ((a->x - x_never(side, lim, a->z)) * side < -INSIDE && (b->x - x_never(side, lim, b->z)) * side < -INSIDE) { DBG("inside"); continue; }
            if ((2 * sum - cnt * (a->x + b->x)) * side >= -4 * cnt) { DBG("outward"); continue; }   /* the polygon must lie inwards */
            Edge *d = &ed[ne];
            int up = a->y < b->y || (a->y == b->y && a->z < b->z);
            d->poly = (s16)i; d->fwd = (u8)up;
            d->ia = (u8)(up ? ia : ib); d->ib = (u8)(up ? ib : ia);
            if (p->nv == 3) { d->ic = d->id = (u8)(3 - ia - ib); }
            else {   /* ring e, e+1, e+2, e+3: e+2 is next to b (ring order), e+3 next to a */
                int i2 = ring[(e + 2) % 4], i3 = ring[(e + 3) % 4];
                d->ic = (u8)(up ? i2 : i3); d->id = (u8)(up ? i3 : i2);
            }
            d->my = (s16)((a->y + b->y) / 2); d->mz = (s16)((a->z + b->z) / 2);
            {   /* the polygon's reach inwards; an upright rectangle in a plane of constant depth (a tile, a block)? */
                int wd = 0;
                for (int k = 0; k < p->nv; k++) {      /* how far the polygon reaches from the edge, in space */
                    if (k == ia || k == ib) continue;
                    const Vx *o = &p->v[k], *n = (k == d->ic) ? &p->v[d->ib] : &p->v[d->ia];
                    if (p->nv == 3) n = iabs(o->y - a->y) + iabs(o->z - a->z) < iabs(o->y - b->y) + iabs(o->z - b->z) ? a : b;
                    int r = isqrt((o->x - n->x) * (o->x - n->x) + (o->y - n->y) * (o->y - n->y) + (o->z - n->z) * (o->z - n->z));
                    wd = r > wd ? r : wd;
                }
                const Vx *c = &p->v[d->ic], *dd = &p->v[d->id], *ea = &p->v[d->ia], *eb = &p->v[d->ib];
                d->reg = p->nv == 4 && ea->x == eb->x && ea->z == eb->z && c->z == ea->z && dd->z == ea->z &&
                         c->x == dd->x && c->y == eb->y && dd->y == ea->y && wd >= 16;
                d->wd = (s16)wd; d->wall = 0; d->virt = 0;
            }
            ne++;
        }
    }
    /* a straight upright line of free edges is a wall that was cut there: it goes on whatever lies behind it */
    for (int i = 0; i < ne; i++) {
        const Poly *p = &ps[ed[i].poly]; const Vx *a = &p->v[ed[i].ia], *b = &p->v[ed[i].ib];
        if (a->x != b->x || a->z != b->z) continue;
        int len = 0, n = 0;
        for (int k = 0; k < ne; k++) {
            const Poly *q = &ps[ed[k].poly]; const Vx *c = &q->v[ed[k].ia], *d = &q->v[ed[k].ib];
            if (c->x == a->x && d->x == a->x && c->z == a->z && d->z == a->z) { len += d->y - c->y; n++; }
        }
        ed[i].wall = n >= 3 && len >= 150;
    }
    {   /* the others only where nothing of the model is drawn just outside them */
        int n = 0;
        for (int i = 0; i < ne; i++) {
            const Poly *p = &ps[ed[i].poly]; const Vx *a = &p->v[ed[i].ia], *b = &p->v[ed[i].ib];
            int mx = (x_start(side, lim, a->x, a->z) + x_start(side, lim, b->x, b->z)) / 2;
            if (!ed[i].wall && covered(mx + side * 10, ed[i].my, ed[i].mz, lim)) { DBG("covered"); continue; }
            DBG(ed[i].wall ? "wall" : "ok");
            ed[n++] = ed[i];
        }
        ne = n;
    }
    for (int i = 1; i < ne; i++) {                                         /* top to bottom, then near to far */
        Edge t = ed[i]; int k = i;
        while (k > 0 && (ed[k - 1].my > t.my || (ed[k - 1].my == t.my && ed[k - 1].mz > t.mz))) { ed[k] = ed[k - 1]; k--; }
        ed[k] = t;
    }
    for (int pass = 0; pass < 2; pass++)
    for (int i = 0; i < ne; i++) {
        if (ed[i].reg != (pass == 0)) continue;
        const Poly *p = &ps[ed[i].poly];
        const Vx *a = &p->v[ed[i].ia], *b = &p->v[ed[i].ib];
        ed[i].ra = (s16)rail_of(a, side, lim, i, ed[i].reg ? ed[i].wd : 0);
        ed[i].rb = (s16)rail_of(b, side, lim, i, ed[i].reg ? ed[i].wd : 0);
        int dy = iabs(b->y - a->y), dz = iabs(b->z - a->z), amp = dy > dz ? dy / 4 : 0;
        if (amp < rl[ed[i].ra].amp) rl[ed[i].ra].amp = (s16)amp;
        if (amp < rl[ed[i].rb].amp) rl[ed[i].rb].amp = (s16)amp;
    }
    int nr0 = nr, ne0 = ne;
    for (int i = 0; i < nr0; i++) if (rl[i].uses < 2) rl[i].amp = 0;      /* the end of a run keeps its height */
    /* the end of a run: where the model has something further in at the heights just past it and nothing outside,
       the surface goes on up / down a little (a skirt), so the corner between the run and the frame's edge closes */
    for (int i = 0; i < ne0 && ne < MAXE && nr < 2 * MAXE; i++) {
        for (int end = 0; end < 2; end++) {
            int r = end ? ed[i].rb : ed[i].ra, dir = end ? 1 : -1;
            if (rl[r].uses != 1 || ed[i].reg) continue;
            const Poly *p = &ps[ed[i].poly]; const Vx *a = &p->v[ed[i].ia], *b = &p->v[ed[i].ib];
            if (iabs(b->y - a->y) < iabs(b->z - a->z)) continue;          /* a floor's edge has no up / down */
            int xn = x_never(side, lim, rl[r].z), y1 = rl[r].y + dir * 96, ym = rl[r].y + dir * 48;
            if (!covered(xn - side * 16, ym, rl[r].z, lim) || covered(rl[r].x0 + side * 10, ym, rl[r].z, lim)) continue;
            if (ne >= MAXE || nr >= 2 * MAXE) break;
            int r2 = nr; Rail *q = &rl[nr++];
            q->x = rl[r].x0; q->y = (s16)y1; q->z = rl[r].z; q->x0 = rl[r].x0; q->amp = 0; q->w = 0; q->reg = 0; q->edge = (s16)i; q->uses = 2;
            Edge *d = &ed[ne++]; *d = ed[i]; d->virt = 1;
            if (end) { d->ra = (s16)r; d->rb = (s16)r2; } else { d->ra = (s16)r2; d->rb = (s16)r; }
            d->my = (s16)ym;
        }
    }
    /* a cave: behind everything, a rock wall for whatever gap is left (never where a 4:3 frame looks): upright runs
       of facets at the model's far depth, each with the look of the end's far polygon nearest its height */
    if (room_curtain && ne0 > 0) {
        int zc = -32768, seg = 56;
        for (int i = 0; i < np; i++) for (int j = 0; j < ps[i].nv; j++) zc = ps[i].v[j].z > zc ? ps[i].v[j].z : zc;
        zc += 40;
        int zf = -32768;
        for (int i = 0; i < ne0; i++) zf = ed[i].mz > zf ? ed[i].mz : zf;
        int y0 = cam_y0 - 140 * (zc + H) / H, y1 = cam_y1 + 140 * (zc + H) / H, prev = -1;
        for (int y = y0; y < y1 && ne < MAXE && nr + 2 < 2 * MAXE; y += seg) {
            int best = -1, bc = 1 << 30;
            for (int i = 0; i < ne0; i++) {
                if (ed[i].reg || ps[ed[i].poly].holes) continue;
                int c = iabs(ed[i].my - (y + seg / 2)) + 2 * (zf - ed[i].mz);
                if (c < bc) { bc = c; best = i; }
            }
            if (best < 0) break;
            int xn = x_never(side, lim, zc);
            for (int k = 0; k < 2; k++) {
                if (k == 0 && prev >= 0) continue;
                Rail *q = &rl[nr];
                q->x = (s16)xn; q->y = (s16)(y + k * seg); q->z = (s16)zc; q->x0 = q->x; q->amp = (s16)(seg / 4); q->w = 0; q->reg = 0;
                q->edge = (s16)best; q->uses = 2;
                if (k == 0) prev = nr;
                nr++;
            }
            Edge *d = &ed[ne++]; *d = ed[best]; d->virt = 2; d->wall = 1;
            d->ra = (s16)prev; d->rb = (s16)(nr - 1); prev = nr - 1;
            d->my = (s16)(y + seg / 2); d->mz = (s16)zc;
        }
    }
    return ne;
}

static P3 rail_at(int r, int j, int side)
{
    P3 o; o.x = rl[r].x0; o.y = rl[r].y; o.z = rl[r].z;
    if (j > 0 && rl[r].reg) o.x = (s16)(rl[r].x0 + side * rl[r].w * j);
    else if (j > 0) {
        u32 h = hsh((u32)(rl[r].y + 4096) * 8192u + (u32)(rl[r].z + 4096), (u32)j, (u32)(side + 2) + (u32)(rl[r].x + 4096) * 4u);
        int jxm = colw / 4;
        int jx = (int)(h & 0xFF) % (2 * jxm + 1) - jxm;
        int jy = rl[r].amp ? (int)((h >> 8) & 0xFF) % (2 * rl[r].amp + 1) - rl[r].amp : 0;
        o.x = (s16)(rl[r].x0 + side * (colw * j + jx)); o.y = (s16)(rl[r].y + jy);
    }
    return o;
}

/* an edge of the end near edge k (at most two up or down, at about the same depth) to take a facet's colours from */
static int near_edge(int k, u32 h)
{
    int i = k + (int)(h % 5u) - 2;
    if (i < 0 || i >= ne || ed[i].virt) return k;
    if (iabs(ed[i].mz - ed[k].mz) > 96 || iabs(ed[i].my - ed[k].my) > 160) return k;
    return i;
}
/* an edge near edge k whose polygon is about a column wide, so its texture rectangle is not stretched: the nearest
   such one up or down from a hashed start, -1 if none */
static int fit(int i) { return i >= 0 && i < ne && 2 * ed[i].wd >= colw && ed[i].wd <= 2 * colw && !ps[ed[i].poly].holes; }
static int tex_edge(int k, u32 h, int want_nv)
{
    int best = -1, start = (int)(h % 5u) - 2;
    for (int d = 0; d <= 10 && best < 0; d++)
        for (int sgn = -1; sgn <= 1 && best < 0; sgn += 2) {
            int i = k + start + sgn * d;
            if (!fit(i) || iabs(ed[i].mz - ed[k].mz) > 160) continue;
            if (want_nv && ps[ed[i].poly].nv != want_nv) continue;
            if (ps[ed[i].poly].tpage != ps[ed[k].poly].tpage || ps[ed[i].poly].clut != ps[ed[k].poly].clut) continue;
            best = i;
        }
    return best;
}
/* the colour of rail r's vertex on line j (gouraud): the edge polygon's own on the edge, further out that of a vertex
   of the end near it */
static u32 rail_col(int r, int j, int k, int is_a)
{
    const Poly *p = &ps[ed[k].poly];
    if (j == 0 || ed[k].reg) return p->col[is_a ? ed[k].ia : ed[k].ib] & 0xFFFFFF;
    u32 h = hsh((u32)(rl[r].y + 4096) * 8192u + (u32)(rl[r].z + 4096), (u32)(j + 32), (u32)(rl[r].x + 4096));
    int e = near_edge(rl[r].edge, h);
    p = &ps[ed[e].poly];
    return p->col[(h >> 9) & 1 ? ed[e].ia : ed[e].ib] & 0xFFFFFF;
}

/* one facet: the texture (page, palette, rectangle) of polygon s, the corners uvi of it; flat: the colour of fc */
static void emit(const Poly *s, const Poly *fc, int nv, const P3 *v, const u8 *uvi, const u32 *col)
{
    int gour = fc->gour;
    u8 *d;
    if (nv == 3) { if (gour) { if (g3 >= MAXN) return; d = gt3[g3++]; } else { if (n3 >= MAXN) return; d = ft3[n3++]; } }
    else         { if (gour) { if (g4 >= MAXN) return; d = gt4[g4++]; } else { if (n4 >= MAXN) return; d = ft4[n4++]; } }
    u32 c0 = gour ? col[0] : (fc->col[0] & 0xFFFFFF);
    u32 w0 = c0 | ((u32)(0x24 | (s->cmd & 1) | (gour ? 0x10 : 0) | (nv == 4 ? 8 : 0)) << 24);
    memcpy(d, &s->clut, 2); memcpy(d + 2, &s->tpage, 2); memcpy(d + 4, &w0, 4); d += 8;
    if (gour) for (int i = 1; i < nv; i++) { memcpy(d, &col[i], 4); d += 4; }
    for (int i = 0; i < nv; i++) {
        Vx o; o.x = v[i].x; o.y = v[i].y; o.z = v[i].z; o.u = s->v[uvi[i]].u; o.v = s->v[uvi[i]].v;
        memcpy(d, &o, 8); d += 8;
    }
}

static void build_side(int side, int lim)
{
    colw = 48;
    if (find_edges(side, lim) < 1) return;
    {   /* the column width: the median reach of the end's polygons */
        static int w[MAXE]; int n = 0;
        for (int i = 0; i < ne; i++) {
            if (ed[i].virt) continue;
            int v = ed[i].wd, k = n++;
            while (k > 0 && w[k - 1] > v) { w[k] = w[k - 1]; k--; }
            w[k] = v;
        }
        colw = n ? w[n / 2] : 48;
        colw = colw < 28 ? 28 : colw > 72 ? 72 : colw;
    }
    int before = n3 + n4 + g3 + g4, cols = 0, walls = 0, skirts = 0;
    for (int k = 0; k < ne; k++) {
        Edge *e = &ed[k];
        const Rail *ra = &rl[e->ra], *rb = &rl[e->rb];
        int na = (x_wide(side, lim, ra->z) - ra->x0) * side, nb = (x_wide(side, lim, rb->z) - rb->x0) * side;
        int need = na > nb ? na : nb, w = e->reg ? e->wd : colw;
        int ncol = need <= 0 ? 0 : (need + (e->reg ? 0 : colw / 4) + w - 1) / w;
        if (ncol > MAXCOL) ncol = MAXCOL;
        walls += e->wall && !e->virt; skirts += e->virt == 1;
        for (int j = 0; j < ncol; j++) {
            P3 a0 = rail_at(e->ra, j, side), b0 = rail_at(e->rb, j, side), a1 = rail_at(e->ra, j + 1, side), b1 = rail_at(e->rb, j + 1, side);
            if (j > 0 && !e->wall && covered((a0.x + b0.x + a1.x + b1.x) / 4, (a0.y + b0.y + a1.y + b1.y) / 4, (a0.z + b0.z) / 2, lim)) break;
            u32 hc = hsh((u32)(e->my + 4096) * 8192u + (u32)(e->mz + 4096), (u32)(j + 64), (u32)(side + 2));
            u32 h2 = hsh(hc, 2, 91);
            const Poly *own = &ps[e->poly];
            /* the texture: the edge's own polygon on a tiled wall; else a polygon of the end about a column wide (the
               edge's own next to the edge if it is one) */
            int k1 = e->reg ? k : ((j == 0 || e->virt) && fit(k)) ? k : tex_edge(e->virt == 2 ? rl[e->ra].edge : k, hc, 0);
            if (k1 < 0) {                      /* none about a column wide: the nearest one that is no cut-out */
                k1 = k;
                if (ps[e->poly].holes) for (int d = 1; d < ne; d++) {
                    if (k - d >= 0 && !ed[k - d].virt && !ps[ed[k - d].poly].holes) { k1 = k - d; break; }
                    if (k + d < ne && !ed[k + d].virt && !ps[ed[k + d].poly].holes) { k1 = k + d; break; }
                }
            }
            const Edge *s1 = &ed[k1]; const Poly *p1 = &ps[s1->poly];
            const Poly *fc = j == 0 || e->reg || e->virt ? own : &ps[ed[near_edge(k, hc)].poly];     /* the kind and the flat colour */
            u32 ca0 = rail_col(e->ra, j, k, 1), cb0 = rail_col(e->rb, j, k, 0), ca1 = rail_col(e->ra, j + 1, k, 1), cb1 = rail_col(e->rb, j + 1, k, 0);
            P3 v[4]; u8 uv[4]; u32 c[4];
            int fwd = e->fwd;
            {   /* the ring b0 a0 a1 (fwd) must turn the way the game draws; a skirt or the wall behind may not */
                int t = turn(&b0.x, &a0.x, &a1.x, lim, (cam_y0 + cam_y1) / 2);
                if (t && (t == front) != (fwd != 0)) fwd = !fwd;
            }
            if (p1->nv == 4) {
                /* ring b0 a0 a1 b1 (edge a -> b is the polygon's winding: its neighbour runs b -> a), strip order */
                if (fwd) { v[0] = b0; v[1] = a0; v[2] = b1; v[3] = a1; uv[0] = s1->ic; uv[1] = s1->id; uv[2] = s1->ib; uv[3] = s1->ia; c[0] = cb0; c[1] = ca0; c[2] = cb1; c[3] = ca1; }
                else        { v[0] = a0; v[1] = b0; v[2] = a1; v[3] = b1; uv[0] = s1->id; uv[1] = s1->ic; uv[2] = s1->ia; uv[3] = s1->ib; c[0] = ca0; c[1] = cb0; c[2] = ca1; c[3] = cb1; }
                emit(p1, fc, 4, v, uv, c);
            } else {
                /* the outer triangle (edge a1 b1, apex b0): the source as it lies */
                if (fwd) { v[0] = b0; v[1] = a1; v[2] = b1; uv[0] = s1->ic; uv[1] = s1->ia; uv[2] = s1->ib; c[0] = cb0; c[1] = ca1; c[2] = cb1; }
                else        { v[0] = b0; v[1] = b1; v[2] = a1; uv[0] = s1->ic; uv[1] = s1->ib; uv[2] = s1->ia; c[0] = cb0; c[1] = cb1; c[2] = ca1; }
                emit(p1, fc, 3, v, uv, c);
                /* the inner triangle (edge a0 b0, apex a1): a source turned half round */
                int k2 = tex_edge(e->virt == 2 ? rl[e->ra].edge : k, h2, 3);
                const Edge *s2 = k2 < 0 ? s1 : &ed[k2]; const Poly *p2 = &ps[s2->poly];
                if (fwd) { v[0] = b0; v[1] = a0; v[2] = a1; uv[0] = s2->ia; uv[1] = s2->ib; uv[2] = s2->ic; c[0] = cb0; c[1] = ca0; c[2] = ca1; }
                else        { v[0] = a0; v[1] = b0; v[2] = a1; uv[0] = s2->ib; uv[1] = s2->ia; uv[2] = s2->ic; c[0] = ca0; c[1] = cb0; c[2] = ca1; }
                emit(p2, fc, 3, v, uv, c);
            }
            cols++;
        }
    }
    if (port_verbose) port_printf("[port] ws pigs: %s end (camera limit %d): %d edges (%d of cut walls, %d skirts), column %d units, %d cells, %d facets\n",
                                  side < 0 ? "left" : "right", lim, ne, walls, skirts, colw, cols, n3 + n4 + g3 + g4 - before);
}

/* called from port_ws_area_model_link with the game's block list of this frame (list[3] = count, u32 pointers from
   +4, room for 88) and the area model's table */
static void blocks(char *list, const u8 *tbl, int area, int section);
void port_ws_pigs_blocks(char *list, const u8 *tbl, int area, int section)
{
    if (ws_heap_failed) return;
    ws_on_room(pigs_reset);
    blocks(list, tbl, area, section);
    ws_free(WS_GEN);
}
static void blocks(char *list, const u8 *tbl, int area, int section)
{
    static const u8 *seen_tbl; static int seen_sec = -1, lo, hi, ylo, yhi;
    { static int once; if (!once) { once = 1; log_on = getenv("PORT_WS_PIGS_LOG") != 0; } }
    if (log_on && tbl) { extern int port_frame; s32 tt[3]; memcpy(tt, (const u8 *)PORT_SPAD(0xC0) + 0x14, 12); port_printf("[pigs] cam %d %d %d\n", port_frame, -(int)tt[0], -(int)tt[1]); }
    if (!port_ws_cull || !tbl || !room_enabled(area, section)) return;
    if (tbl[0] != 1 || (u8)list[3] != 1) return;                           /* these rooms: one block, the whole room */
    s32 off; memcpy(&off, tbl + 4, 4);
    const u8 *b = tbl + off;
    if (((u32 *)list)[1] != (u32)(unsigned long)b) return;
    /* the camera: VIEW_CAMERA (scratchpad 0xC0, MATRIX: m[3][3] s16, t[3] s32 at +0x14) must be unrotated */
    const u8 *c = (const u8 *)PORT_SPAD(0xC0);
    s16 m[9]; s32 t[3]; memcpy(m, c, 18); memcpy(t, c + 0x14, 12);
    for (int i = 0; i < 9; i++) if (m[i] != ((i % 4) ? 0 : 4096)) return;
    if ((int)(u16)port_gte_cfc2(26) != H || (int)((s32)port_gte_cfc2(24) >> 16) != 160 || t[2] != H) return;
    /* the camera's range: CAMERA_SCROLL minX / maxX, and wherever the camera has been in this room */
    const u8 *cs = (const u8 *)port_ram_ptr(0x800A4550u);
    s16 minx, maxx, miny, maxy; memcpy(&minx, cs + 0x2C, 2); memcpy(&maxx, cs + 0x2E, 2); memcpy(&miny, cs + 0x30, 2); memcpy(&maxy, cs + 0x32, 2);
    int cx = -(int)t[0], cy = -(int)t[1];
    if (seen_tbl != tbl || seen_sec != section) { seen_tbl = tbl; seen_sec = section; lo = hi = cx; ylo = yhi = cy; have_key = 0; }
    if (minx <= maxx && maxx - minx < 4000) { if (minx < lo) lo = minx; if (maxx > hi) hi = maxx; }
    if (cx < lo) lo = cx;
    if (cx > hi) hi = cx;
    if (miny <= maxy && maxy - miny < 4000) { if (miny < ylo) ylo = miny; if (maxy > yhi) yhi = maxy; }
    if (cy < ylo) ylo = cy;
    if (cy > yhi) yhi = cy;
    if (!ws_need(WS_ROOM, ROOM_TABS, WS_NT(ROOM_TABS))) return;
    int size = parse(b);
    if (!size) return;
    u32 key = 2166136261u;
    for (int i = 0; i < size; i++) key = (key ^ b[i]) * 16777619u;
    key = hsh(key, (u32)port_ws_cull, (u32)(lo + 8192) * 16384u + (u32)(hi + 8192));
    key = hsh(key, (u32)(ylo + 8192), (u32)(yhi + 8192));
    if (!have_key || key != cur_key) {
        if (!ws_need(WS_GEN, GEN_TABS, WS_NT(GEN_TABS))) return;
        have_key = 1; cur_key = key;
        n3 = n4 = g3 = g4 = 0;
        cam_y0 = ylo; cam_y1 = yhi; find_holes();
        build_side(-1, lo);
        build_side(1, hi);
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
