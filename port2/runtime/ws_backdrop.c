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
 *     leaves, and the per-course tone of the bricks next to each edge;
 *   - the room is a box: both strips are its side walls in perspective (vanishing point and horizon measured from the
 *     picture, see "the side walls in perspective" below), laid with the picture's bond (18-row courses, 27-column
 *     bricks, half bond) in wall coordinates, so courses get taller, bricks wider and the mortar lines diverge from
 *     the vanishing point towards the viewer; every brick with its own tone, bevel (lit top-left, shadowed
 *     bottom-right), grain (value noise), stains and the odd crack, laid out by a hash of its course / index (no copy
 *     of the picture's bricks); the right wall on the plane of the picture's slanted face (its foot continues the
 *     line from the room corner through the vanishing point), the left one from a corner behind the trunk;
 *   - the room's light measured on the picture ("the room's light" below), gently: the walls a little darker towards
 *     the viewer (at most 14 %) and at the foot's corner; the floor at the picture's lateral falloff (a line fitted per
 *     side, at most 1/8 darker than at the edge) with a soft contact line at the walls' foot; the ivy too; then
 *     balance(): per region (wall, floor, ivy) the strip's luminance next to the join mapped onto the mean and spread
 *     of the picture's 40 columns next to the edge, so the strips are as light and as contrasted as the room there
 *     (the user: "far too dark");
 *   - the floor up to the side wall's foot, streaky value noise in floor coordinates (perspective) over the floor
 *     ramp;
 *   - no roots: the user rejected every generated root ("never draw roots again"), so the left strip is side wall,
 *     floor and ivy only; the picture's own root ends at its edge, where the edge ivy comes down to the floor over it;
 *   - ivy grown as stems: a fringe hanging from the top of the side wall, curtains (long stems that sway and branch,
 *     spaced along the wall, thicker near the corner), climbers from the wall's foot, and a mass next to
 *     each edge as deep as the picture's own ivy on that row so the join closes; leaves sized by the wall's scale where
 *     they hang relative to the edge's (bigger towards the viewer); every leaf drawn one by one (a tapered ellipse at one of 16 angles, lit top-left, rim shadow), back leaves darker
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
#define VPX16 (172 * 16)    /* the vanishing point (x 16) and the camera's focal length, see "the side walls" below */
#define VPY16 (85 * 16)
#define FOC16 (320 * 16)
static const int JOINT_EVEN[2] = { 5, 4 }, JOINT_ODD[2] = { 19, 18 };   /* X of the first joint, even / odd courses */
static int pcol(int side, int X) { return side ? PW - 1 + X : -X; }      /* X <= 0: inside the picture */
static u16 picx(int side, int X, int y) { int x = clampi(pcol(side, X), 0, PW - 1); return pic[clampi(y, 0, PH - 1)][x]; }

static Ramp R_wall, R_floor, R_ivy;
static Rgb C_mortar, C_stem, C_gap;
static int course_tone[2][14];          /* mean tone (ramp index 0..255) of the bricks next to each edge, per course */
static int floor_tone[2][PH];           /* the floor's row tone next to each edge */
static int edge_ivy[2][PH];             /* 0..256: share of ivy texels in the 6 columns at the edge (+-2 rows) */
static int edge_depth[2][PH];           /* how far (columns) the picture's ivy reaches in from the edge on that row */
static int edge_tone[2][PH];            /* the tone (on R_ivy) of the picture's ivy at the edge on that row */
static int side_k[2];                   /* light of each side wall against the back wall (x 256) */
static int floor_fit[2][2];             /* the floor's light: L(|f|) = a - b (|f| - 1), fitted per side on |f| 0.6..1.15 */
static u8 lum_tone[256];                /* luminance -> tone on R_floor */
static int pic_lum[2][3], pic_dev[2][3]; /* mean luminance and its standard deviation (x 16) of the picture's 40 columns next
                                           to each edge: wall, floor, ivy */
enum { REG_WALL, REG_FLOOR, REG_IVY, REG_NONE };

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

static int isqrt_i(int v)
{
    int r = 0, b = 1 << 30;
    if (v <= 0) return 0;
    while (b > v) b >>= 2;
    while (b) { if (v >= r + b) { v -= r + b; r = (r >> 1) + b; } else r >>= 1; b >>= 2; }
    return r;
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
    /* the side walls' light: the picture's one wall face turned sideways (x 298-301, between the mortar rows) against
       the back wall beside it (x 284-291); the left wall a little darker still, in the tree's shade */
    {
        static const int rows[3][2] = { { 127, 138 }, { 147, 156 }, { 166, 174 } };
        int sa = 0, sn = 0, ba = 0, bn = 0;
        for (int i = 0; i < 3; i++) for (int y = rows[i][0]; y < rows[i][1]; y++) {
            for (int x = 298; x < 302; x++) { sa += lum(rgb15(pic[y][x])); sn++; }
            for (int x = 284; x < 292; x++) { ba += lum(rgb15(pic[y][x])); bn++; }
        }
        side_k[1] = clampi(sa * 256 / sn * bn / (ba ? ba : 1), 160, 256);
        side_k[0] = side_k[1] * 15 / 16;
    }
    /* the floor's light across the room: a line through its luminance against |f| (0.6..1.15, rows 186-223, no ivy),
       per side; past the edge the strip follows that line (the pool of light fading towards the side walls) */
    for (int side = 0; side < 2; side++) {
        long long n = 0, sf = 0, sl = 0, sff = 0, sfl = 0;
        for (int y = 186; y < PH; y++)
            for (int x = 0; x < PW; x++) {
                int f = ((x * 16 + 8 - VPX16) * 256) / (y * 16 + 8 - VPY16), l;
                if ((side ? f : -f) < 154 || (side ? f : -f) > 294 || greenish(pic[y][x])) continue;
                f = f < 0 ? -f : f; l = lum(rgb15(pic[y][x]));
                n++; sf += f; sl += l; sff += (long long)f * f; sfl += (long long)f * l;
            }
        if (n > 100 && n * sff - sf * sf != 0) {
            long long den = n * sff - sf * sf, num = n * sfl - sf * sl;             /* slope x 256 = num * 256 / den */
            int b = (int)(-num * 256 / den);                                          /* L(256) = mean + slope (256 - mean f) */
            int a = (int)((sl * 256 + (long long)b * sf) / (n * 256)) - b;
            floor_fit[side][0] = clampi(a, 50, 140); floor_fit[side][1] = clampi(b, 0, 120);
        } else { floor_fit[side][0] = 85; floor_fit[side][1] = 40; }
    }
    for (int l = 0; l < 256; l++) { Rgb c = { l, l, l }; lum_tone[l] = (u8)(R_floor.ok ? tone_of(&R_floor, c) : l); }
    /* the brightness the strips are matched to: the picture's 40 columns next to each edge, per region (ivy: the green
       texels; floor: rows 196-223; wall: the rest of rows 4-169, on the left above the root, rows 4-121) */
    for (int side = 0; side < 2; side++) {
        int sum[3] = { 0, 0, 0 }, n[3] = { 0, 0, 0 }; long long sq[3] = { 0, 0, 0 };
        for (int X = -39; X <= 0; X++)
            for (int y = 0; y < PH; y++) {
                u16 c = picx(side, X, y); int r = greenish(c) ? REG_IVY : y >= 196 ? REG_FLOOR : y >= 4 && y < (side ? 170 : 122) ? REG_WALL : REG_NONE;
                if (r != REG_NONE) { int l = lum(rgb15(c)); sum[r] += l; sq[r] += l * l; n[r]++; }
            }
        for (int r = 0; r < 3; r++) {
            pic_lum[side][r] = n[r] ? sum[r] * 16 / n[r] : 0;
            pic_dev[side][r] = n[r] ? isqrt_i((int)((sq[r] * 256 - (long long)sum[r] * sum[r] * 256 / n[r]) / n[r])) : 0;
        }
    }
}

/* --- the canvas of one strip: X = 1..pad, y = 0..PH-1 */
static Rgb *cv; static u8 *occ; static int cw;         /* occ: leaf coverage (for the wall's shadow) */
static Rgb *px_at(int X, int y) { return (X >= 1 && X <= cw && y >= 0 && y < PH) ? &cv[(size_t)y * cw + X - 1] : 0; }
static void put(int X, int y, Rgb c) { Rgb *p = px_at(X, y); if (p) *p = c; }

/* --- the side walls in perspective.  The room is a box: at the right edge the back wall turns towards the viewer
   (the narrow face at x 297-302 whose mortar lines slant down-right from the back wall's rows 121 / 139 / 157 / 176 to
   the lit front face's rows 124 / 143.5 / 163 / 181), and both strips are the room's side walls, receding towards the
   camera.  Measured: the back wall is frontal (the same mortar rows at both edges); the front face's courses are 19.5
   rows against the back wall's 18 (scale 1.083) and each of its mortar rows lies on the line from a back-wall row
   through the horizon at row 85 (124 = 85 + 1.083 * (121 - 85), 143.5, 163 alike); a least-squares fit of the slanted
   face's mortar slopes (0.33 / 0.65 / 0.75 / 0.8 at rows 122 / 141 / 160 / 178) puts the vanishing point at x 173, the
   door's axis is x 160: VP = (172, 85).  The right side wall starts at the front face's plane (scale 1.083 at the
   picture's edge), the left one at the back wall's (the tree hides that corner).  A wall pixel at strip column X has
   the scale s = (D + X - 1/2) / D (D: the corner's distance from the VP), its back-wall height yb = 85 + (y - 85) /
   (s * kc), and its place along the wall U = F (1 - 1 / s) / kc (focal length F 320 px): bricks get wider, courses
   taller and their mortar lines diverge from the VP as the wall comes towards the viewer; verticals stay vertical. */
/* the side wall's plane: the room corner's column (x 16; scale 1 there, on the back wall's foot row) and the strip
   column's distance from the VP.  Right: the corner at x 296.5 (the dark vertical line where the back wall meets the
   narrow slanted face; the slanted face's foot runs from (296.5, 176) through the VP, and the dark ivy-covered wall
   beside the pillar, x 315-319, ends on the floor on the same line: rows 190 / 193 at x 315 / 319), so the pillar's
   lit front face (x 303-314) stands out of that wall.  Left: the corner hidden behind the trunk (bricks of the back
   wall show from x 22), at x 19.5, where the room's mirror of the right corner about the door's axis (x 23.5) would
   be too; its foot passes x 0 at row 187.6, under the ivy that drapes onto the floor down to row ~194 there. */
static const int CORNER16[2] = { 19 * 16 + 8, 296 * 16 + 8 };
static int DC16(int side) { return side ? CORNER16[1] - VPX16 : VPX16 - CORNER16[0]; }
static int DE16(int side, int X) { return side ? (PW - 1) * 16 + 8 + X * 16 - VPX16 : VPX16 - 8 + X * 16; }   /* column centre */
static int *S256, *WU16;                                   /* per strip column 0..cw+1: scale (x 256), U (back-wall px x 16) */
static int s_at(int X) { return S256[clampi(X, 0, cw + 1)]; }
static int ivy_s(int X) { return s_at(X) * 256 / s_at(1); }   /* the ivy's size: as the picture's own at the edge, growing outwards */
static void persp_cols(int side)
{
    for (int X = 0; X <= cw + 1; X++) {
        int s = DE16(side, X) * 256 / DC16(side);
        S256[X] = s;
        WU16[X] = (int)((long long)(s - 256) * FOC16 / s);
    }
}
/* screen row (x 16) of back-wall height yb16 at column X, and back-wall height (x 16) of screen row y16 */
static int wall_y16(int side, int X, int yb16) { (void)side; return VPY16 + (int)((long long)(yb16 - VPY16) * s_at(X) / 256); }
static int wall_yb16(int side, int X, int y16) { (void)side; return VPY16 + (int)((long long)(y16 - VPY16) * 256 / s_at(X)); }
#define FOOT16 (FLOOR * 16 - 8)                            /* the back wall's foot: between rows 175 and 176 */
static int foot_y16(int side, int X) { return wall_y16(side, X, FOOT16); }
static int foot_y(int side, int X) { return (foot_y16(side, X) + 8) / 16; }   /* first floor row of the column */
/* strip column of a place U16 along the wall (the inverse of WU16) */
static int X_of_U(int side, int U16)
{
    int u = U16; if (u > FOC16 * 15 / 16) u = FOC16 * 15 / 16;
    { int s = (int)((long long)256 * FOC16 / (FOC16 - u)); return (int)(((long long)s * DC16(side) / 256 - DE16(side, 0)) / 16); }
}

/* --- the room's light (measured on the picture).  One soft pool from above the camera, centred on the door: the floor
   is brightest on the door's axis (luminance 130 at floor lateral f = 0, f = (x - 172) / (y - 85)) and falls to ~82 at
   |f| 1.2 near both edges, lateral only (rows of equal f keep their tone from row 186 to 223); the back wall the same
   (130 by the arch, 50-65 by the edges, 30-40 under the edge ivy); faces turned to the camera are lit most (the
   pillar's front face, 80, brighter than the back wall beside it, 57), faces turned sideways less (the slanted face of
   the right wall, ~0.7 of the back wall next to it).  The wall beside the pillar (x 315-319) lies in the pillar's
   shadow (~30); the left edge is dark under the tree.  Corners and the wall's foot are darker (the back wall's mortar
   row over the floor, rows 174-175, is its darkest).  The first strips followed all of that (the pillar's / the
   trunk's shadow by the edges, dark under the top ivy, a dark foot, a steep falloff) and the user found them "far too
   dark"; now only a gentle falloff and a soft foot are kept, and balance() sets the level.
   wall_light: the side wall's light (x 256) at strip column X, row y (foot16: its foot, 1/16 px) */
static int wall_light(int side, int X, int y, int foot16)
{
    int s = s_at(X), k = side_k[side];
    k = k * (256 - clampi((s - 256) * 40 / 256, 0, 36)) / 256;               /* a gentle falloff towards the viewer (<= 14 %) */
    {   int up = foot16 - (y * 16 + 8), sc = s / 16;                        /* height over the foot (1/16 px), scale */
        if (up < 2 * sc) k = k * 208 / 256; else if (up < 4 * sc) k = k * 236 / 256;   /* the foot's corner, soft */
    }
    return k;
}

static void draw_wall(int side)
{
    for (int X = 1; X <= cw; X++) {
        int U = WU16[X], dU = WU16[X + 1] - WU16[X], foot16 = foot_y16(side, X);
        int jw = dU > 16 ? dU : 16;                                         /* a joint is at least a column wide */
        for (int y = 0; y < PH; y++) {
            int yb = wall_yb16(side, X, y * 16 + 8);
            if (yb >= FOOT16) break;
            int k = fdiv(yb - COURSE0 * 16, COURSE * 16), t = yb - COURSE0 * 16 - k * COURSE * 16;  /* course k, t in it */
            int odd = k & 1, j0 = odd ? JOINT_ODD[side] : JOINT_EVEN[side];
            int base = course_tone[side][clampi(k + 1, 0, 13)];
            int n = fdiv(U - j0 * 16, BRICK * 16), u = U - j0 * 16 - n * BRICK * 16;   /* brick n, u in it */
            u32 h = hash3(side * 1000 + k, n, 17);
            int w = (BRICK + (int)(h >> 24) % 3 - 1) * 16;                  /* bricks a column longer or shorter */
            Rgb c;
            if (t < MORTAR * 16 || u < jw || u >= w) {                           /* mortar */
                int m = noise(U, yb, 48, 32, 5) - 128;
                c = mix(C_mortar, ramp(&R_wall, 0), 64 + m / 4);
            } else {
                int tone = base + (int)(h & 63) - 32;
                int bt = t - MORTAR * 16, bh = (COURSE - MORTAR) * 16;
                int lo = side ? u - jw : w - u, hi = side ? w - u : u - jw;     /* screen-left / -right edge of the brick */
                tone += noise(U + n * 112, yb, 96, 64, 11 + side) / 5 - 25;     /* mottling */
                tone += noise(U, yb, 32, 32, 13) / 8 - 16;                       /* grain */
                if ((h >> 8 & 7) == 0) { int st = noise(U, yb, 80, 48, 29); if (st > 150) tone -= (st - 150) / 2; }   /* stain */
                if (bt < 16) tone += 34; else if (bt < 32) tone += 14;            /* bevel: lit top edge */
                if (bt >= bh - 16) tone -= 42; else if (bt >= bh - 32) tone -= 18;
                if (lo < jw) tone += 20;                                         /* lit left edge, shadowed right */
                if (hi < jw) tone -= 34; else if (hi < 2 * jw) tone -= 12;
                if ((h >> 12 & 15) == 0) {                                       /* a crack: a dark diagonal hairline */
                    int cx = (int)(h >> 16 & 15) + 4, cy = (int)(h >> 20 & 7) + 3, uu = u / 16, tt = bt / 16;
                    if ((uu - cx) == (tt - cy) / 2 && tt >= cy && tt < cy + 8) tone -= 60;
                }
                c = ramp(&R_wall, tone);
            }
            put(X, y, scale(c, wall_light(side, X, y, foot16)));
        }
    }
}

/* the floor between the side wall's foot and the frame, in perspective: its light from the picture's floor (the
   lateral profile, carried on past the edge with its own slope), streaks across (rows of constant depth stay
   horizontal), slabs and pebbles scaled with the distance (texture coordinates at the scale of picture row 200), and
   the wall's foot darker (the contact shadow, a few rows, wider towards the viewer) */
static int floor_lum(int side, int f256)                   /* |f| x 256 -> luminance */
{
    int a = floor_fit[side][0], b = floor_fit[side][1];    /* L = a - b (|f| - 1) */
    return clampi(a - b * (f256 - 256) / 256, 40, 140);
}
static void draw_floor(int side)
{
    for (int X = 1; X <= cw; X++) {
        int foot16 = foot_y16(side, X), fy = foot_y(side, X), x16 = pcol(side, X) * 16 + 8, sc = s_at(X);
        for (int y = fy < 0 ? 0 : fy; y < PH; y++) {
            int dy = y * 16 + 8 - VPY16; if (dy < 16) dy = 16;
            int fu = (int)((long long)(x16 - VPX16) * 115 * 16 / dy) + 8192, fv = 115 * 115 * 256 / dy;
            int f256 = (int)((long long)(x16 - VPX16) * 256 / dy); if (f256 < 0) f256 = -f256;
            int e256 = (int)((long long)(pcol(side, 0) * 16 + 8 - VPX16) * 256 / dy), le, lf;
            if (e256 < 0) e256 = -e256;
            le = floor_lum(side, e256); lf = floor_lum(side, f256);                   /* the edge's light, this column's */
            if (lf < le - le / 8) lf = le - le / 8;                                   /* at most 1/8 darker than the edge */
            int tone = lum_tone[lf];
            tone += noise(fu, fv, 13 * 16, 2 * 16, 31 + side) / 2 - 64;              /* streaks */
            tone += noise(fu, fv, 5 * 16, 2 * 16, 37) / 4 - 32;                      /* slabs */
            tone += noise(X, y, 1, 1, 41) / 10 - 12;                                  /* grain */
            tone += noise(fu, fv, 3 * 16, 16, 43) / 5 - 25;                           /* pebbles: short light dashes */
            {   int dn = (y * 16 + 8 - foot16) * 256 / sc;                       /* rows below the foot, at the corner's scale */
                Rgb c = ramp(&R_floor, tone);
                if (dn < 16) c = scale(c, 200); else if (dn < 32) c = scale(c, 232);   /* a soft contact line at the foot */
                put(X, y, c);
            }
        }
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
static int cur_side;                                       /* the strip being drawn */
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
            {   int k = wall_light(cur_side, clampi(X, 1, cw), py, (PH + 64) * 16) * 256 / (side_k[cur_side] ? side_k[cur_side] : 256);
                put(X, py, scale(ramp(&R_ivy, tone), clampi(k * 9 / 8, 120, 256)));   /* the room's light on the leaves too */
            }
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
    int yy = clampi(y, 0, PH - 1), d = edge_depth[side][yy], e = edge_ivy[side][yy], reach;
    if (side == 0 && y >= 112 && y < 202) { if (d < 12) d = 12; if (e < 236) e = 236; }  /* the ivy down to the floor over
                                                                                       the picture's root, cut at the edge */
    reach = 3 + d * 3 / 4 + noise(0, y, 1, 11, 61 + side) * (4 + d / 3) / 256;       /* ragged boundary */
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
                int lx = rndr(r, 18, 34), ly = rndr(r, 13, 22), la = rndi(r, 16), lt = front ? rndr(r, 90, 220) : rndr(r, 40, 120);
                int ks = ivy_s((px + ox) / 16);                              /* nearer the viewer, bigger */
                leaf_add(px + ox * ks / 256, py + oy * ks / 256, lx * ks / 256, ly * ks / 256, la, lt, front ? 2 : 1);
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
            int la = rndi(&r, 16), lt = front ? et + rndr(&r, -10, 90) : et - rndr(&r, 30, 100), ks = ivy_s(X);
            leaf_add(lx, ly, rx * ks / 256, ry * ks / 256, la, lt, front ? 2 : 1);
        }
    /* 2. hanging from the top of the side wall: cascades (several stems side by side, the middle ones longest), laid
       out along the wall (every 22 back-wall columns, so further apart and wider towards the viewer), short ones along
       the whole wall top, long ones near the corner and here and there further out */
    for (int slot = -1; ; slot++) {
        Rng r = rng_at(side * 7 + 2, slot, 0);
        int X = X_of_U(side, WU16[1] + (slot * 22 + rndi(&r, 10)) * 16), ks, near;
        if (slot < 0) X = -8 + rndi(&r, 6);
        if (X > cw + 22) break;
        ks = ivy_s(X); near = X < 48 ? 256 - X * 4 : 70;
        int W = rndr(&r, 8, 22) * ks / 256, L = 8 + rndi(&r, 22), n = W / 4 + 1;
        if (rndi(&r, 256) < near) L += 40 + rndi(&r, 100);
        if (X < 30) L += 50;
        L = L * ks / 256;
        for (int j = 0; j < n; j++) {
            int off = 2 * j - (n - 1), m = n > 1 ? (off < 0 ? -off : off) * 256 / n : 0;   /* 0 middle .. 256 sides */
            int len = L * (256 - m * 5 / 8) / 256 * rndr(&r, 180, 256) / 256;
            int sx = (X + j * 4) * 16 + rndr(&r, -24, 24), dr = rndr(&r, -3, 3);
            grow(side, &r, sx, -32, 1, len, dr, 44, 28, 300, 0, sc);
        }
    }
    /* 3. climbing: from the wall's foot up the wall */
    for (int k = 0; k < 3 + cw / 60; k++) {
        Rng r = rng_at(side * 7 + 3, k, 0);
        int X = k < 2 ? 3 + k * 10 + rndi(&r, 6) : 30 + rndi(&r, cw);
        int y = foot_y(side, X) - 2;
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
            if (o && !occ[(size_t)y * cw + X - 1]) { Rgb *p = px_at(X, y); *p = scale(*p, 208); }
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
    int r = clampi(c.r + 4, 0, 255) >> 3, g = clampi(c.g + 4, 0, 255) >> 3, b = clampi(c.b + 4, 0, 255) >> 3, k = r | g << 5 | b << 10;   /* rounded */
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

/* the strip's region at (X, y): green = ivy, below the side wall's foot = floor, above it = wall (the same split as
   pic_lum's, so like is compared with like) */
static int region(int side, int X, int y, Rgb c)
{
    int f16 = foot_y16(side, X);
    if (c.g >= c.r + 8 && c.g >= c.b + 8) return REG_IVY;
    if (y * 16 + 8 >= f16 + 32) return REG_FLOOR;
    if (y >= 4 && y * 16 + 8 < f16 - 48) return REG_WALL;
    return y * 16 + 8 >= f16 ? REG_FLOOR : REG_WALL;
}
/* match the strip's brightness and contrast to the room's: per region, the luminance of the strip's 10 columns next to
   the join (4-13) mapped (L' = m + (L - ms) k, k = dev / devs) onto the mean and spread of the picture's 40 columns next to the edge
   (pic_lum / pic_dev), each pixel's colour scaled by L' / L; the whole strip takes the same map, so its own falloff
   towards the viewer stays (gentle: wall_light, draw_floor) */
static void balance(int side)
{
    int sum[3] = { 0, 0, 0 }, n[3] = { 0, 0, 0 }, ms[3], k[3]; long long sq[3] = { 0, 0, 0 };
    for (int X = 4; X <= 13 && X <= cw; X++)                              /* past the 3 columns blended with the edge */
        for (int y = 0; y < PH; y++) {
            Rgb c = cv[(size_t)y * cw + X - 1]; int f16 = foot_y16(side, X), r = region(side, X, y, c), l = lum(c);
            if (r == REG_IVY || y * 16 + 8 >= f16 + 32 || (y >= 4 && y * 16 + 8 < f16 - 48)) { sum[r] += l; sq[r] += l * l; n[r]++; }
        }
    for (int r = 0; r < 3; r++) {
        int dev = n[r] ? isqrt_i((int)((sq[r] * 256 - (long long)sum[r] * sum[r] * 256 / n[r]) / n[r])) : 0;   /* x 16 */
        ms[r] = n[r] ? sum[r] * 16 / n[r] : 0;
        k[r] = dev && pic_dev[side][r] ? clampi(pic_dev[side][r] * 256 / dev, 192, 512) : 256;
        if (!n[r] || !pic_lum[side][r]) { ms[r] = 0; k[r] = 256; }
    }
    for (int y = 0; y < PH; y++)
        for (int X = 1; X <= cw; X++) {
            Rgb *p = &cv[(size_t)y * cw + X - 1]; int r = region(side, X, y, *p), l = lum(*p), l2;
            if (!ms[r]) continue;
            l2 = clampi((pic_lum[side][r] + (l * 16 - ms[r]) * k[r] / 256) / 16, 2, 255);   /* L' */
            *p = l ? scale(*p, clampi(l2 * 256 / l, 0, 1024)) : (Rgb){ l2, l2, l2 };
        }
#ifdef WS_BACKDROP_MAIN
    if (getenv("WSB_INFO")) for (int r = 0; r < 3; r++)
        fprintf(stderr, "side %d region %d: picture mean %d dev %d, strip mean %d -> contrast x%d/256\n", side, r,
                pic_lum[side][r] / 16, pic_dev[side][r] / 16, ms[r] / 16, k[r]);
#endif
}

static void build_redraw(int pad)
{
    analyse(); pal_make();
    cw = pad; cv = calloc((size_t)pad * PH, sizeof *cv); occ = calloc((size_t)pad * PH, 1);
    S256 = calloc((size_t)pad + 2, sizeof *S256); WU16 = calloc((size_t)pad + 2, sizeof *WU16);
    if (!cv || !occ || !S256 || !WU16 || !R_wall.ok || !R_floor.ok || !R_ivy.ok) {
        free(cv); free(occ); free(S256); free(WU16); cv = 0; occ = 0; S256 = WU16 = 0; return;
    }
    for (int side = 0; side < 2; side++) {
        persp_cols(side); cur_side = side;
        draw_wall(side); draw_floor(side);
#ifdef WS_BACKDROP_MAIN
        if (!getenv("WSB_NOIVY"))
#endif
        grow_ivy(side);
        balance(side);
        for (int y = 0; y < PH; y++)
            for (int X = 1; X <= pad; X++) {
                Rgb c = cv[(size_t)y * pad + X - 1];
                if (X <= 3) c = mix(rgb15(picx(side, 0, y)), c, X == 1 ? 110 : X == 2 ? 180 : 230);   /* the join */
                strip[(size_t)y * 2 * pad + (side ? pad + (X - 1) : pad - X)] = quant(c);
            }
    }
    free(cv); free(occ); free(S256); free(WU16); cv = 0; occ = 0; S256 = WU16 = 0; free(leaves); leaves = 0; nleaves = capleaves = 0;
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
    if (getenv("WSB_INFO")) fprintf(stderr, "side_k %d %d floor_fit L %d-%d R %d-%d\n", side_k[0], side_k[1], floor_fit[0][0], floor_fit[0][1], floor_fit[1][0], floor_fit[1][1]);
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
