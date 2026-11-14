/* --widescreen: side strips of the pre-rendered backdrop of AREA02 s5, "Small Strange Room" (docs/PORT.md
 * "Widescreen", "Room picture strips").
 *
 * That room's background is one 320x224 8 bpp picture (stone wall, brick arch, the carved face door, ivy at both
 * sides, the cobbled floor) drawn as two sprites (256 + 64 columns, pages 128 / 256 x 0, CLUT 128,511) at frame row
 * 16; nothing of it exists past the 4:3 columns, so the wide frame showed the clear colour (grey) beside it.
 * shims/gpu.c calls port_ws_backdrop when it meets the left sprite of the picture while the wide plane is on and fills
 * the strips of the wide copy before the game draws the picture itself, so everything the game draws on top (drums,
 * actors, boxes) stays on top.  Identity without --widescreen and in every other room (the sprite signature and the
 * room are both checked).  Opt-in preview, off by default until the user picks it: `--ws-backdrop on` (the redraw)
 * or `--ws-backdrop mirror` (the first preview: the picture mirrored at its edge); also PORT_WS_BACKDROP=1 / 2 (not
 * in the wasm2c build, which has no environment).
 *
 * The redraw (build_redraw) is drawn by code, the way shims/gfx_es.c makes its Spanish graphics: nothing of the
 * picture is copied or mirrored and no pixels are shipped.  The tables below hold coordinates only (the brick bond,
 * the floor line, where to sample); every colour comes from the user's picture in VRAM at run time:
 *   - colour ramps (dark to light, 32 quantile bins) sampled from the picture's brick faces, mortar, floor, ivy
 *     leaves and the left tree roots, and the per-course tone of the bricks next to each edge;
 *   - the wall continued course by course with the picture's bond (18-row courses, 27-column bricks, half bond),
 *     every brick with its own tone, bevel (lit top-left, shadowed bottom-right), grain (value noise), stains and
 *     the odd crack, laid out by a hash of its course / index (no copy of the picture's bricks);
 *   - the floor continued with streaky value noise over the floor ramp and the row tones of the picture's floor;
 *   - on the left, the tree trunk behind the edge ivy and a fan of new roots (tapered curves, shaded across);
 *   - ivy grown as stems: a fringe hanging from the top, curtains (long stems that sway and branch), climbers from
 *     the roots, and a mass next to each edge as deep as the picture's own ivy on that row so the join closes; every
 *     leaf drawn one by one (a tapered ellipse at one of 16 angles, lit top-left, rim shadow), back leaves darker
 *     first, front leaves lighter last, and the wall shadowed under the leaves;
 *   - the three columns next to the picture blended with its edge column; every pixel quantised to the nearest colour
 *     the picture itself uses (the room's 8 bpp CLUT).
 * Deterministic: integers only (no floating point, no libm), fixed seeds and position hashes, and never two random
 * draws in one argument list (gcc on i386 evaluates arguments right to left, clang / wasm2c left to right); a structure depends on
 * where it is, not on the strip width, so a 21:9 / 32:9 strip starts with the same columns as the 16:9 one and goes
 * on with new content (no repeat).  The same code gives the same strips on i386, x86-64 and the wasm2c build.
 *
 * Standalone check / iteration (WS_BACKDROP_MAIN): PORT_WS_BACKDROP_DUMP=FILE writes the picture (raw, 320x224
 * little-endian 15-bit colours) during a run; then
 *   cc -O2 -DWS_BACKDROP_MAIN port2/runtime/ws_backdrop.c -o /tmp/wsb && /tmp/wsb FILE PAD OUT.ppm
 * writes the picture with both strips. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef WS_BACKDROP_MAIN
#include "port_runtime.h"
#endif

typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32;

#define PW 320
#define PH 224

static u16 pic[PH][PW];                 /* the picture as 15-bit colours */
static u16 *strip; static int strip_pad; static u32 strip_sum;
int port_ws_backdrop_on;                /* --ws-backdrop on = 1 (redraw), mirror = 2 (main.c) */
static int enabled = -1;

static int r5(u16 c) { return c & 31; } static int g5(u16 c) { return (c >> 5) & 31; } static int b5(u16 c) { return (c >> 10) & 31; }
static int is_ivy(u16 c) { int r = r5(c), g = g5(c), b = b5(c); return g >= r + 2 && g >= b + 2; }

/* ================================================================== mode 2: the first preview (mirror) */
static int pingpong(int i, int n) { i %= 2 * n; return i < n ? i : 2 * n - 1 - i; }
#define BAND 56
#define IVY 24
#define MFLOOR 176
static u16 at(int side, int y, int m) { if (y < 0) y = 0; if (y >= PH) y = PH - 1; return pic[y][side ? PW - 1 - m : m]; }
static void build_mirror(int pad)
{
    for (int side = 0; side < 2; side++)
        for (int y = 0; y < PH; y++)
            for (int d = 1; d <= pad; d++) {
                u16 c = at(side, y, pingpong(d - 1, BAND));
                if (y < MFLOOR) {
                    int k = (d - 1) / IVY, sy = y - 5 * (k % 3) - 4;
                    u16 l = sy >= 0 ? at(side, sy, pingpong(d - 1 + IVY / 2, IVY)) : 0;
                    if (sy >= 0 && is_ivy(l)) c = l;
                }
                strip[(size_t)y * 2 * pad + (side ? pad + (d - 1) : pad - d)] = c;
            }
}

/* ================================================================== mode 1: the redraw */
typedef struct { int r, g, b; } Rgb;
#define NRAMP 32
typedef struct { Rgb bin[NRAMP]; int ok; } Ramp;

/* --- integer helpers (all arithmetic below is on ints; no floating point anywhere) */
static int clampi(int v, int a, int b) { return v < a ? a : v > b ? b : v; }
static int fdiv(int a, int b) { int q = a / b; if ((a % b != 0) && ((a < 0) != (b < 0))) q--; return q; }  /* floor */
static int fmodi(int a, int b) { return a - fdiv(a, b) * b; }
static u32 hash3(int a, int b, int c)
{
    u32 h = 0x9E3779B9u ^ (u32)a * 0x85EBCA6Bu; h ^= h >> 15; h += (u32)b * 0xC2B2AE35u; h ^= h >> 13;
    h *= 0x27D4EB2Fu; h += (u32)c * 0x165667B1u; h ^= h >> 16; h *= 0x85EBCA6Bu; h ^= h >> 13; return h;
}
typedef struct { u32 s; } Rng;
static u32 rnd(Rng *r) { u32 x = r->s; x ^= x << 13; x ^= x >> 17; x ^= x << 5; return r->s = x ? x : 0x6D2B79F5u; }
static int rndi(Rng *r, int n) { return n > 0 ? (int)((rnd(r) >> 8) % (u32)n) : 0; }         /* 0..n-1 */
static int rndr(Rng *r, int a, int b) { return a + rndi(r, b - a + 1); }                        /* a..b */
static Rng rng_at(int a, int b, int c) { Rng r; r.s = hash3(a, b, c) | 1; rnd(&r); rnd(&r); return r; }

/* value noise 0..255 at (x, y) in pixels, cells sx x sy pixels, smoothstep blend */
static int noise(int x, int y, int sx, int sy, int seed)
{
    int fx = fdiv((x + 4096) * 256, sx), fy = fdiv((y + 4096) * 256, sy);
    int ix = fx >> 8, iy = fy >> 8, tx = fx & 255, ty = fy & 255;
    int a = hash3(ix, iy, seed) & 255, b = hash3(ix + 1, iy, seed) & 255;
    int c = hash3(ix, iy + 1, seed) & 255, d = hash3(ix + 1, iy + 1, seed) & 255;
    tx = tx * tx * (768 - 2 * tx) / 65536; ty = ty * ty * (768 - 2 * ty) / 65536;
    { int top = a * 256 + (b - a) * tx, bot = c * 256 + (d - c) * tx; return (top * 256 + (bot - top) * ty) >> 16; }
}

static int lum(Rgb c) { return (c.r * 3 + c.g * 6 + c.b) / 10; }
static Rgb rgb15(u16 c) { Rgb o = { r5(c) << 3, g5(c) << 3, b5(c) << 3 }; return o; }
static Rgb mix(Rgb a, Rgb b, int t) { Rgb o = { a.r + (b.r - a.r) * t / 256, a.g + (b.g - a.g) * t / 256, a.b + (b.b - a.b) * t / 256 }; return o; }
static Rgb scale(Rgb a, int k) { Rgb o = { a.r * k / 256, a.g * k / 256, a.b * k / 256 }; return o; }

/* --- ramps: the samples sorted by (luminance, colour) - a total order, so any qsort gives the same bins */
static u32 samp[PW * PH]; static int nsamp;
static int samp_cmp(const void *a, const void *b) { u32 x = *(const u32 *)a, y = *(const u32 *)b; return x < y ? -1 : x > y; }
static void samp_add(u16 c) { if (nsamp < PW * PH) samp[nsamp++] = (u32)lum(rgb15(c)) << 16 | (c & 0x7FFF); }
static void ramp_make(Ramp *rp)
{
    memset(rp, 0, sizeof *rp);
    if (nsamp < NRAMP) { nsamp = 0; return; }
    qsort(samp, (size_t)nsamp, sizeof samp[0], samp_cmp);
    for (int i = 0; i < NRAMP; i++) {
        int a = (int)((long long)nsamp * i / NRAMP), b = (int)((long long)nsamp * (i + 1) / NRAMP), n = b - a;
        Rgb s = { 0, 0, 0 };
        for (int j = a; j < b; j++) { Rgb c = rgb15((u16)samp[j]); s.r += c.r; s.g += c.g; s.b += c.b; }
        rp->bin[i].r = s.r / n; rp->bin[i].g = s.g / n; rp->bin[i].b = s.b / n;
    }
    rp->ok = 1; nsamp = 0;
}
/* t 0..255 (dark..light), clamped */
static Rgb ramp(const Ramp *rp, int t)
{
    int p = clampi(t, 0, 255) * (NRAMP - 1), i = p / 255, f = (p % 255) * 256 / 255;
    return i >= NRAMP - 1 ? rp->bin[NRAMP - 1] : mix(rp->bin[i], rp->bin[i + 1], f);
}

/* --- the picture's geometry (coordinates only).  Strip coordinates: X = 1..pad columns past the edge (outwards),
   picture column = -X (left) or 319 + X (right); the wall planes next to both edges are the back wall. */
#define COURSE 18           /* course height (mortar rows 13 + 18k) */
#define COURSE0 13
#define MORTAR 2
#define BRICK 27            /* brick length incl. the joint */
#define FLOOR 176           /* first floor row at the wall foot */
static const int JOINT_EVEN[2] = { 5, 4 }, JOINT_ODD[2] = { 19, 18 };   /* X of the first joint, even / odd courses */
static int pcol(int side, int X) { return side ? PW - 1 + X : -X; }      /* X <= 0: inside the picture */
static u16 picx(int side, int X, int y) { int x = clampi(pcol(side, X), 0, PW - 1); return pic[clampi(y, 0, PH - 1)][x]; }

static Ramp R_wall, R_floor, R_ivy, R_root;
static Rgb C_mortar, C_stem, C_gap;
static int course_tone[2][14];          /* mean tone (ramp index 0..255) of the bricks next to each edge, per course */
static int floor_tone[2][PH];           /* the floor's row tone next to each edge */
static int edge_ivy[2][PH];             /* 0..256: share of ivy texels in the 6 columns at the edge (+-2 rows) */
static int edge_depth[2][PH];           /* how far (columns) the picture's ivy reaches in from the edge on that row */
static int edge_tone[2][PH];            /* the tone (on R_ivy) of the picture's ivy at the edge on that row */

static int green(u16 c) { Rgb k = rgb15(c); return k.g >= k.r + 16 && k.g >= k.b + 16; }
static int greenish(u16 c) { Rgb k = rgb15(c); return k.g >= k.r + 8 && k.g >= k.b + 8; }
static int brownish(u16 c) { Rgb k = rgb15(c); return k.r >= k.b + 16 && k.g < k.r + 8; }
static int wall_px(u16 c) { Rgb k = rgb15(c); int l = lum(k); return !greenish(c) && l >= 40 && l < 150 && k.r >= k.b; }

/* tone (0..255 on R_wall) of colour c: the ramp bin nearest in luminance */
static int tone_of(const Ramp *rp, Rgb c)
{
    int l = lum(c), best = 0, bd = 1 << 30;
    for (int t = 0; t < 256; t += 4) { int d = lum(ramp(rp, t)) - l; d = d < 0 ? -d : d; if (d < bd) { bd = d; best = t; } }
    return best;
}

static void analyse(void)
{
    /* brick faces: the wall beside the arch on both sides (no ivy, no mortar, no roots) */
    for (int y = 0; y < FLOOR; y++) {
        int t = fmodi(y - COURSE0, COURSE);
        if (t < MORTAR + 1 || t > COURSE - 2) continue;
        for (int x = 24; x < 60; x++) if (y < 122 && wall_px(pic[y][x])) samp_add(pic[y][x]);
        for (int x = 262; x < 292; x++) if (y > 40 && wall_px(pic[y][x])) samp_add(pic[y][x]);
    }
    ramp_make(&R_wall);
    /* mortar: the darkest texels of the left wall's mortar rows */
    for (int k = 0; k < 6; k++) for (int x = 26; x < 58; x++) { u16 c = pic[COURSE0 + COURSE * (k + 1)][x]; if (!greenish(c)) samp_add(c); }
    { Ramp m; ramp_make(&m); C_mortar = m.ok ? m.bin[NRAMP / 4] : (Rgb){ 40, 32, 30 }; }
    /* floor */
    for (int y = 186; y < PH; y++) { for (int x = 30; x < 90; x++) samp_add(pic[y][x]); for (int x = 250; x < 300; x++) samp_add(pic[y][x]); }
    ramp_make(&R_floor);
    /* ivy leaves: the green texels at both edges */
    for (int y = 0; y < 200; y++) for (int x = 0; x < 40; x++) { if (green(pic[y][x])) samp_add(pic[y][x]); if (green(pic[y][PW - 1 - x])) samp_add(pic[y][PW - 1 - x]); }
    ramp_make(&R_ivy);
    /* roots (left): the brown texels of the root fan */
    for (int y = 126; y < 182; y++) for (int x = 8; x < 64; x++) if (brownish(pic[y][x])) samp_add(pic[y][x]);
    ramp_make(&R_root);
    if (!R_root.ok) R_root = R_wall;
    /* stems: dark non-green texels among the left ivy; gaps between leaves: the darkest greens */
    for (int y = 0; y < 120; y++) for (int x = 0; x < 30; x++) { u16 c = pic[y][x]; Rgb k = rgb15(c); if (!greenish(c) && lum(k) < 60 && k.r >= k.b) samp_add(c); }
    { Ramp m; ramp_make(&m); C_stem = m.ok ? m.bin[NRAMP / 2] : (Rgb){ 56, 40, 32 }; }
    C_gap = R_ivy.ok ? R_ivy.bin[0] : (Rgb){ 16, 24, 8 };
    /* per-course tone of the visible bricks next to each edge */
    for (int side = 0; side < 2; side++)
        for (int k = 0; k < 14; k++) {
            int top = COURSE0 + COURSE * (k - 1) + MORTAR + 1, n = 0, s = 0;
            for (int y = top; y < top + COURSE - MORTAR - 2 && y < FLOOR; y++) {
                if (y < 0) continue;
                for (int x = 0; x < 56; x++) {
                    int px = side ? 300 - x : 20 + x; u16 c = pic[y][px];
                    if (px < 0 || px >= PW || (side == 0 && px > 58) || (side == 1 && px < 262)) continue;
                    if (wall_px(c) && !brownish(c)) { s += lum(rgb15(c)); n++; }
                }
            }
            course_tone[side][k] = n > 30 ? s / n : -1;
        }
    for (int side = 0; side < 2; side++) {
        int last = -1;
        for (int k = 0; k < 14; k++) if (course_tone[side][k] >= 0) last = course_tone[side][k];
        for (int k = 13; k >= 0; k--) { if (course_tone[side][k] >= 0) last = course_tone[side][k]; else course_tone[side][k] = last; }
        for (int k = 0; k < 14; k++) {
            Rgb c = { course_tone[side][k], course_tone[side][k], course_tone[side][k] };
            course_tone[side][k] = R_wall.ok ? tone_of(&R_wall, c) : 128;
        }
    }
    /* floor row tones next to each edge, and the edge ivy profile */
    for (int side = 0; side < 2; side++)
        for (int y = 0; y < PH; y++) {
            int s = 0, n = 0, g = 0, gn = 0, depth = 0;
            for (int x = 6; x < 60; x++) { u16 c = picx(side, -x, y); if (!greenish(c)) { s += lum(rgb15(c)); n++; } }
            floor_tone[side][y] = n ? s / n : 80;
            for (int dy = -2; dy <= 2; dy++) for (int x = 0; x < 6; x++) { gn++; if (greenish(picx(side, -x, y + dy)) || lum(rgb15(picx(side, -x, y + dy))) < 28) g++; }
            edge_ivy[side][y] = g * 256 / gn;
            for (int x = 0, miss = 0; x < 80 && miss < 6; x++) { if (greenish(picx(side, -x, y))) { depth = x + 1; miss = 0; } else miss++; }
            edge_depth[side][y] = depth;
            { int ls = 0, ln = 0;
              for (int dy = -3; dy <= 3; dy++) for (int x = 0; x < 8; x++) { u16 c = picx(side, -x, y + dy); if (greenish(c)) { ls += lum(rgb15(c)); ln++; } }
              { Rgb c = { ln ? ls / ln : 60, ln ? ls / ln : 60, ln ? ls / ln : 60 }; edge_tone[side][y] = ln ? tone_of(&R_ivy, c) : -1; } }
        }
    for (int side = 0; side < 2; side++) for (int y = 0; y < PH; y++) {
        Rgb c = { floor_tone[side][y], floor_tone[side][y], floor_tone[side][y] };
        if (y >= FLOOR) floor_tone[side][y] = R_floor.ok ? tone_of(&R_floor, c) : 128;
    }
}

/* --- the canvas of one strip: X = 1..pad, y = 0..PH-1 */
static Rgb *cv; static u8 *occ; static int cw;         /* occ: leaf coverage (for the wall's shadow) */
static Rgb *px_at(int X, int y) { return (X >= 1 && X <= cw && y >= 0 && y < PH) ? &cv[(size_t)y * cw + X - 1] : 0; }
static void put(int X, int y, Rgb c) { Rgb *p = px_at(X, y); if (p) *p = c; }

static void draw_wall(int side)
{
    for (int y = 0; y < FLOOR; y++) {
        int k = fdiv(y - COURSE0, COURSE), t = fmodi(y - COURSE0, COURSE);      /* course k: rows below mortar k */
        int odd = k & 1, j0 = odd ? JOINT_ODD[side] : JOINT_EVEN[side];
        int base = course_tone[side][clampi(k + 1, 0, 13)];
        for (int X = 1; X <= cw; X++) {
            int n = fdiv(X - j0, BRICK), u = fmodi(X - j0, BRICK);        /* brick n of the course, column u in it */
            u32 h = hash3(side * 1000 + k, n, 17);
            int jit = (int)(h >> 24) % 3 - 1;                               /* joints wander by a column */
            if (u < 1 + jit && n > fdiv(1 - j0, BRICK)) { n--; u += BRICK; h = hash3(side * 1000 + k, n, 17); }
            int w = BRICK + ((int)(hash3(side * 1000 + k, n + 1, 17) >> 24) % 3 - 1) - (int)(h >> 24) % 3 + 1;
            Rgb c;
            if (t < MORTAR || u == 0 || u >= w) {                              /* mortar */
                int m = noise(X, y, 3, 2, 5) - 128;
                c = mix(C_mortar, ramp(&R_wall, 0), 64 + m / 4);
            } else {
                int tone = base + (int)(h & 63) - 32;                       /* per-brick tone */
                int bt = t - MORTAR, bh = COURSE - MORTAR;
                tone += noise(X + n * 7, y, 6, 4, 11 + side) / 5 - 25;     /* mottling */
                tone += noise(X, y, 2, 2, 13) / 8 - 16;                     /* grain */
                if ((h >> 8 & 7) == 0) { int s = noise(X, y, 5, 3, 29); if (s > 150) tone -= (s - 150) / 2; }   /* stain */
                if (bt == 0) tone += 34; else if (bt == 1) tone += 14;     /* bevel: lit top edge */
                if (bt == bh - 1) tone -= 42; else if (bt == bh - 2) tone -= 18;
                if (u == 1) tone += 20;                                     /* lit left edge, shadowed right */
                if (u == w - 1) tone -= 34; else if (u == w - 2) tone -= 12;
                if ((h >> 12 & 15) == 0) {                                   /* a crack: a dark diagonal hairline */
                    int cx = (int)(h >> 16 & 15) + 4, cy = (int)(h >> 20 & 7) + 3;
                    if ((u - cx) == (bt - cy) / 2 && bt >= cy && bt < cy + 8) tone -= 60;
                }
                c = ramp(&R_wall, tone);
            }
            put(X, y, c);
        }
    }
}

static void draw_floor(int side)
{
    for (int y = FLOOR; y < PH; y++)
        for (int X = 1; X <= cw; X++) {
            int tone = floor_tone[side][y];
            tone += noise(X, y, 13, 2, 31 + side) / 2 - 64;                 /* streaks */
            tone += noise(X, y, 5, 2, 37) / 4 - 32;                         /* slabs */
            tone += noise(X, y, 1, 1, 41) / 10 - 12;                        /* grain */
            tone += noise(X, y, 3, 1, 43) / 5 - 25;                         /* pebbles: short light dashes */
            if (y == FLOOR) tone -= 70; else if (y == FLOOR + 1) tone -= 30; /* the wall's foot */
            put(X, y, ramp(&R_floor, tone));
        }
}

/* a tapered curve (root / trunk): quadratic bezier p0 -> p2 via p1 (pixels), radius r0 -> r1 (1/16 px) */
static void draw_root(int x0, int y0, int x1, int y1, int x2, int y2, int r0, int r1, int seed)
{
    for (int i = 0; i <= 96; i++) {
        int a = (96 - i) * (96 - i), b = 2 * (96 - i) * i, c = i * i;      /* /9216 */
        int cx = (x0 * a + x1 * b + x2 * c) * 16 / 9216, cy = (y0 * a + y1 * b + y2 * c) * 16 / 9216;
        int r = r0 + (r1 - r0) * i / 96;
        for (int py = (cy - r) / 16 - 1; py <= (cy + r) / 16 + 1; py++)
            for (int X = (cx - r) / 16 - 1; X <= (cx + r) / 16 + 1; X++) {
                int dx = X * 16 + 8 - cx, dy = py * 16 + 8 - cy;
                if (dx * dx + dy * dy > r * r) continue;
                int v = dy * 128 / (r + 1);                                  /* -128 top .. 128 bottom */
                int tone = 125 - v - (dx * dx + dy * dy) * 70 / (r * r + 1) - i * 2 / 3;   /* darker towards the tip */
                tone += noise(i * 2 + seed * 50, dy / 4 + 64, 3, 1, 43) / 4 - 32;  /* bark streaks along the root */
                put(X, py, ramp(&R_root, tone));
            }
    }
}

static void draw_roots(void)
{
    /* the trunk behind the edge ivy: a dark column next to the picture's own */
    for (int y = 0; y < 150; y++) for (int X = 1; X <= 9 + noise(0, y, 1, 9, 47) / 40; X++) {
        int tone = 50 + noise(X, y, 2, 7, 53) / 3 - X * 3;
        put(X, y, ramp(&R_root, tone));
    }
    /* the fan: roots leaving the trunk outwards, down to the floor */
    for (int i = 0; i < 3; i++) {
        Rng r = rng_at(901, i, 3);
        int y0 = 132 + i * 10 + rndi(&r, 5), x2 = 20 + i * 7 + rndi(&r, 6), y2 = 181 + rndi(&r, 4);
        int x0 = 1 + rndi(&r, 3), x1 = x2 * 3 / 4 + rndi(&r, 4), y1 = y0 + 2 + rndi(&r, 4), r0 = 132 - i * 16 + rndi(&r, 16);
        draw_root(x0, y0, x1, y1, x2, y2, r0, 24, i);
    }
}

/* --- ivy */
typedef struct { short x16, y16; u8 rx, ry, ang, tone, layer; } Leaf;
static Leaf *leaves; static int nleaves, capleaves;
static void leaf_add(int x16, int y16, int rx, int ry, int ang, int tone, int layer)
{
    if (x16 < -64 || x16 > (cw + 6) * 16 || y16 < -64 || y16 > (PH + 4) * 16) return;
    if (nleaves == capleaves) {
        int n = capleaves ? capleaves * 2 : 4096; Leaf *l = realloc(leaves, (size_t)n * sizeof *l);
        if (!l) return;
        leaves = l; capleaves = n;
    }
    Leaf *l = &leaves[nleaves++];
    l->x16 = (short)x16; l->y16 = (short)y16; l->rx = (u8)rx; l->ry = (u8)ry; l->ang = (u8)(ang & 15);
    l->tone = (u8)clampi(tone, 0, 255); l->layer = (u8)layer;
}
static const int COS16[16] = { 256, 237, 181, 98, 0, -98, -181, -237, -256, -237, -181, -98, 0, 98, 181, 237 };
static const int SIN16[16] = { 0, 98, 181, 237, 256, 237, 181, 98, 0, -98, -181, -237, -256, -237, -181, -98 };
/* one leaf: a tapered ellipse (rx along its axis, ry across, 1/16 px; the tip at +u is narrower), lit from the top
   left, the far rim darker; mode 0 = shadow mask only (occ), 1 = draw */
static void leaf_draw(const Leaf *l, int mode)
{
    int c = COS16[l->ang], s = SIN16[l->ang], rx = l->rx, ry = l->ry, R = (rx > ry ? rx : ry) / 16 + 2;
    int X0 = l->x16 / 16, Y0 = l->y16 / 16;
    for (int py = Y0 - R; py <= Y0 + R; py++)
        for (int X = X0 - R; X <= X0 + R; X++) {
            int dx = X * 16 + 8 - l->x16, dy = py * 16 + 8 - l->y16;
            int u = (dx * c + dy * s) / 256, v = (dy * c - dx * s) / 256;
            int ryu = ry * (256 - 80 * u / (rx + 1)) / 256;                 /* taper towards the tip */
            if (ryu < 4) continue;
            long long q = (long long)u * u * 256 / (rx * rx) + (long long)v * v * 256 / (ryu * ryu);
            if (q > 256) continue;
            if (mode == 0) { if (X >= 1 && X <= cw && py >= 0 && py < PH) occ[(size_t)py * cw + X - 1] = 1; continue; }
            int tone = l->tone - (dx + dy) * 3 / 2;                         /* light from the top left */
            if (q > 150 && dx + dy > 0) tone -= 50;                          /* the far rim */
            if (q < 40 && dx + dy < 0) tone += 18;                           /* the sheen */
            tone += (int)(hash3(X, py, l->x16) & 31) - 16;                  /* the painted grain */
            put(X, py, ramp(&R_ivy, tone));
        }
}
static void stem_line(int x0, int y0, int x1, int y1, Rgb c)      /* 1/16 px endpoints */
{
    int ax = x0 / 16, ay = y0 / 16, bx = x1 / 16, by = y1 / 16;
    int dx = bx > ax ? bx - ax : ax - bx, dy = by > ay ? by - ay : ay - by, sx = ax < bx ? 1 : -1, sy = ay < by ? 1 : -1, e = dx - dy;
    for (int i = 0; i < 400; i++) {
        put(ax, ay, c);
        if (ax == bx && ay == by) break;
        { int e2 = 2 * e; if (e2 > -dy) { e -= dy; ax += sx; } if (e2 < dx) { e += dx; ay += sy; } }
    }
}

/* density of the ivy at (X, y): the edge's own ivy carried a little way out, falling off */
static int edge_dens(int side, int X, int y)
{
    int yy = clampi(y, 0, PH - 1), d = edge_depth[side][yy], e = edge_ivy[side][yy];
    int reach = 3 + d * 3 / 4 + noise(0, y, 1, 11, 61 + side) * (4 + d / 3) / 256;  /* ragged boundary */
    if (X > reach) return 0;
    return e * (reach - X + 2) / (reach + 2);
}

/* a stem growing from (x16, y16) in 1/16 px: dir 1 hangs (down), -1 climbs (up); drift = 1/16 px per row */
static void grow(int side, Rng *r, int x16, int y16, int dir, int len, int drift, int spread0, int spread1, int dens,
                 int depth, Rgb sc)
{
    int px = x16, py = y16;
    for (int i = 0; i < len; i++) {
        int t = i * 256 / (len + 1), spread = spread0 + (spread1 - spread0) * t / 256;
        int sway = (noise(i, depth * 37 + x16 / 16, 9, 1, 71) - 128) / 16;
        int nx = px + drift + sway, ny = py + 16 * dir;
        stem_line(px, py, nx, ny, sc);
        px = nx; py = ny;
        /* leaves along the stem, thinning towards the tip */
        int want = dens * (256 - t / 2) / 256;
        while (want > 0) {
            if (rndi(r, 256) < (want > 256 ? 256 : want)) {
                int ox = rndr(r, -spread, spread), oy = rndr(r, -spread / 2 - 8, spread / 2 + 8);
                int front = rndi(r, 2);
                int lx = rndr(r, 18, 34), ly = rndr(r, 13, 22), la = rndi(r, 16), lt = front ? rndr(r, 90, 230) : rndr(r, 0, 80);
                leaf_add(px + ox, py + oy, lx, ly, la, lt, front ? 2 : 1);
            }
            want -= 256;
        }
        /* a branch now and then */
        if (depth < 2 && i > 4 && rndi(r, 96) == 0)
        {
            int bl = len / 3 + rndi(r, len / 3 + 1), bs = rndi(r, 2) ? 1 : -1, bd = bs * rndr(r, 3, 8);
            grow(side, r, px, py, dir, bl, bd, spread0 / 2 + 16, 16, dens * 3 / 4, depth + 1, sc);
        }
        if (px / 16 < -6 || px / 16 > cw + 6) break;
    }
}

static void grow_ivy(int side)
{
    Rgb sc = C_stem, sc2 = mix(C_stem, C_gap, 128);
    nleaves = 0;
    /* 1. the mass at the edge: the picture's ivy carried on, as deep as it is on each row */
    for (int y = -2; y < PH; y += 2)
        for (int X = 1; X <= 40 && X <= cw + 3; X += 2) {
            int dn = edge_dens(side, X, y);
            Rng r = rng_at(side * 7 + 1, X, y);
            if (dn <= 0 || rndi(&r, 256) >= dn) continue;
            int front = rndi(&r, 2), et = edge_tone[side][clampi(y, 0, PH - 1)];
            if (et < 0) et = 120;
            int lx = X * 16 + rndr(&r, -16, 16), ly = y * 16 + rndr(&r, -16, 16), rx = rndr(&r, 18, 34), ry = rndr(&r, 13, 22);
            int la = rndi(&r, 16), lt = front ? et + rndr(&r, -10, 90) : et - rndr(&r, 30, 100);
            leaf_add(lx, ly, rx, ry, la, lt, front ? 2 : 1);
        }
    /* 2. hanging from the top: cascades (several stems side by side, the middle ones longest), short ones along the
       whole wall top, long ones near the edge and here and there further out */
    for (int slot = -1; slot * 22 <= cw + 22; slot++) {
        Rng r = rng_at(side * 7 + 2, slot, 0);
        int X = slot * 22 + rndi(&r, 10), near = X < 48 ? 256 - X * 4 : 70;
        int W = rndr(&r, 8, 22), L = 8 + rndi(&r, 22), n = W / 4 + 1;
        if (rndi(&r, 256) < near) L += 40 + rndi(&r, 100);
        if (X < 30) L += 50;
        for (int j = 0; j < n; j++) {
            int off = 2 * j - (n - 1), m = n > 1 ? (off < 0 ? -off : off) * 256 / n : 0;   /* 0 middle .. 256 sides */
            int len = L * (256 - m * 5 / 8) / 256 * rndr(&r, 180, 256) / 256;
            int sx = (X + j * 4) * 16 + rndr(&r, -24, 24), dr = rndr(&r, -3, 3);
            grow(side, &r, sx, -32, 1, len, dr, 44, 28, 300, 0, sc);
        }
    }
    /* 3. climbing: from the roots (left) / the floor at the edge (right) up the wall */
    for (int k = 0; k < 3 + cw / 60; k++) {
        Rng r = rng_at(side * 7 + 3, k, 0);
        int X = side == 0 ? (k < 3 ? 6 + k * 9 + rndi(&r, 6) : 30 + rndi(&r, cw)) : (k < 2 ? 3 + k * 10 + rndi(&r, 6) : 30 + rndi(&r, cw));
        int y = side == 0 && k < 3 ? 150 + rndi(&r, 20) : FLOOR - 2;
        if (k >= 3 && rndi(&r, 2)) continue;
        int cl = 30 + rndi(&r, 70), dr = rndr(&r, -4, 4);
        grow(side, &r, X * 16, y * 16, -1, cl, dr, 24, 40, 260, 1, sc2);
    }
    /* draw: shadow on the wall under the leaves, then back leaves, then front leaves */
    memset(occ, 0, (size_t)cw * PH);
    for (int i = 0; i < nleaves; i++) leaf_draw(&leaves[i], 0);
    for (int y = PH - 1; y >= 0; y--)
        for (int X = cw; X >= 1; X--) {
            int sx = X - 1, sy = y - 2, o = sx >= 1 && sy >= 0 && occ[(size_t)sy * cw + sx - 1];
            if (o && !occ[(size_t)y * cw + X - 1]) { Rgb *p = px_at(X, y); *p = scale(*p, 150); }
        }
    for (int i = 0; i < nleaves; i++) if (leaves[i].layer == 1) { Leaf g = leaves[i]; g.rx += 6; g.ry += 6; g.tone = 0; leaf_draw(&g, 1); }
    for (int i = 0; i < nleaves; i++) if (leaves[i].layer == 1) leaf_draw(&leaves[i], 1);
    for (int i = 0; i < nleaves; i++) if (leaves[i].layer == 2) { Leaf g = leaves[i]; g.x16 += 12; g.y16 += 16; g.tone = 10; leaf_draw(&g, 1); }
    for (int i = 0; i < nleaves; i++) if (leaves[i].layer == 2) leaf_draw(&leaves[i], 1);
}

/* nearest colour among those the picture uses (its CLUT), cached per 15-bit colour */
static u16 pal[256]; static int npal; static u16 qcache[32768]; static u8 qhave[32768];
static void pal_make(void)
{
    static u8 seen[32768];
    memset(seen, 0, sizeof seen); memset(qhave, 0, sizeof qhave); npal = 0;
    for (int y = 0; y < PH; y++) for (int x = 0; x < PW; x++) {
        u16 c = pic[y][x] & 0x7FFF;
        if (!seen[c] && npal < 256 && c) { seen[c] = 1; pal[npal++] = pic[y][x]; }
    }
}
static u16 quant(Rgb c)
{
    int r = clampi(c.r, 0, 255) >> 3, g = clampi(c.g, 0, 255) >> 3, b = clampi(c.b, 0, 255) >> 3, k = r | g << 5 | b << 10;
    if (!qhave[k]) {
        int bd = 1 << 30, best = 0;
        for (int i = 0; i < npal; i++) {
            int dr = r5(pal[i]) - r, dg = g5(pal[i]) - g, db = b5(pal[i]) - b, d = 3 * dr * dr + 4 * dg * dg + 2 * db * db;
            if (d < bd) { bd = d; best = i; }
        }
        qcache[k] = npal ? pal[best] : (u16)k; qhave[k] = 1;
    }
    return qcache[k];
}

static void build_redraw(int pad)
{
    analyse(); pal_make();
    cw = pad; cv = calloc((size_t)pad * PH, sizeof *cv); occ = calloc((size_t)pad * PH, 1);
    if (!cv || !occ || !R_wall.ok || !R_floor.ok || !R_ivy.ok) { free(cv); free(occ); cv = 0; occ = 0; return; }
    for (int side = 0; side < 2; side++) {
        draw_wall(side); draw_floor(side);
        if (side == 0) draw_roots();
        grow_ivy(side);
        for (int y = 0; y < PH; y++)
            for (int X = 1; X <= pad; X++) {
                Rgb c = cv[(size_t)y * pad + X - 1];
                if (X <= 3) c = mix(rgb15(picx(side, 0, y)), c, X == 1 ? 110 : X == 2 ? 180 : 230);   /* the join */
                strip[(size_t)y * 2 * pad + (side ? pad + (X - 1) : pad - X)] = quant(c);
            }
    }
    free(cv); free(occ); cv = 0; occ = 0; free(leaves); leaves = 0; nleaves = capleaves = 0;
}

static void build(int pad)
{
    free(strip); strip = calloc((size_t)2 * pad * PH, 2); strip_pad = pad;
    if (!strip) return;
    if (enabled == 2) build_mirror(pad); else build_redraw(pad);
}

/* the picture's texels: 8 bpp page at (tx, ty), CLUT at (cx, cy); vram is 1024x512 words */
static u32 grab(const u16 *vram, int tx, int ty, int cx, int cy)
{
    u32 h = 2166136261u;
    for (int y = 0; y < PH; y++)
        for (int x = 0; x < PW; x++) {
            u16 w = vram[(size_t)((ty + y) & 511) * 1024 + ((tx + x / 2) & 1023)];
            int i = x & 1 ? w >> 8 : w & 255;
            u16 c = vram[(size_t)(cy & 511) * 1024 + ((cx + i) & 1023)];
            pic[y][x] = c; h = (h ^ c) * 16777619u;
        }
    return h;
}

#ifndef WS_BACKDROP_MAIN
/* sprite (cmd 0x64/0x65) at screen (x, y) of w x h texels from (u, v), page (tx, ty, depth), CLUT (cx, cy):
   the strip buffer when it is the left part of the room's picture (left strip at columns 0..pad-1, right strip
   at pad..2pad-1, PH rows), else 0 */
const u16 *port_ws_backdrop(const u16 *vram, int pad, int x, int y, int w, int h, int u, int v, int tx, int ty,
                            int depth, int cx, int cy)
{
    if (enabled < 0) { const char *e = getenv("PORT_WS_BACKDROP"); enabled = e && *e >= '1' && *e <= '2' ? *e - '0' : port_ws_backdrop_on; }
    if (!enabled || pad <= 0 || x != 0 || y != 16 || w != 256 || h != PH || u || v || tx != 128 || ty || depth != 1
        || cx != 128 || cy != 511) return 0;
    {
        u8 *g = port_ram_ptr(PORT_A_GAME);
        if (*(u16 *)(g + 0) != 2 || *(u16 *)(g + 2) != 5) return 0;
    }
    {
        u32 s = grab(vram, 128, 0, cx, cy);
        if (!strip || strip_pad != pad || s != strip_sum) {
            strip_sum = s; build(pad);
            {   const char *e = getenv("PORT_WS_BACKDROP_DUMP");      /* research: the picture, raw 15-bit LE */
                FILE *f = e ? fopen(e, "wb") : 0;
                if (f) {
                    for (int yy = 0; yy < PH; yy++) for (int xx = 0; xx < PW; xx++) { u8 b[2] = { (u8)pic[yy][xx], (u8)(pic[yy][xx] >> 8) }; fwrite(b, 1, 2, f); }
                    fclose(f);
                }
            }
        }
    }
    return strip;
}
#else
int main(int argc, char **argv)
{
    FILE *f; int pad;
    if (argc < 4 || !(f = fopen(argv[1], "rb"))) { fprintf(stderr, "usage: wsb PIC.raw PAD OUT.ppm [mirror]\n"); return 1; }
    for (int y = 0; y < PH; y++) for (int x = 0; x < PW; x++) { u8 b[2]; if (fread(b, 1, 2, f) != 2) return 1; pic[y][x] = (u16)(b[0] | b[1] << 8); }
    fclose(f);
    pad = atoi(argv[2]); enabled = argc > 4 ? 2 : 1;
    build(pad);
    if (!strip || !(f = fopen(argv[3], "wb"))) return 1;
    fprintf(f, "P6 %d %d 255\n", PW + 2 * pad, PH);
    for (int y = 0; y < PH; y++)
        for (int x = 0; x < PW + 2 * pad; x++) {
            u16 c = x < pad ? strip[(size_t)y * 2 * pad + x] : x >= pad + PW ? strip[(size_t)y * 2 * pad + x - PW] : pic[y][x - pad];
            u8 rgb[3] = { (u8)(r5(c) << 3), (u8)(g5(c) << 3), (u8)(b5(c) << 3) }; fwrite(rgb, 1, 3, f);
        }
    fclose(f);
    { u32 h = 2166136261u; for (size_t i = 0; i < (size_t)2 * pad * PH; i++) h = (h ^ strip[i]) * 16777619u; printf("strip hash %08x\n", h); }
    return 0;
}
#endif
