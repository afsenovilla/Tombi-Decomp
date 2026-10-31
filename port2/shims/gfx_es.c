/* Spanish (--lang 5) graphic redraws, generated natively from the user's own disc data: the C twin of tools/gfx_es
 * (CARGANDO, CARGAR / GUARDAR, VUELVE A INTENTARLO; see the module docstrings there for what each redraw is).
 *
 * The output is byte-identical to `python3 -m gfx_es DATA OUT.tbtx` (tools/) for the same data: every Pillow operation
 * the Python uses is reimplemented here exactly as Pillow 12 does it (ImageDraw rectangle / rounded_rectangle / polygon /
 * ellipse / pieslice, Image.transform AFFINE + rotate(expand) with BICUBIC, resize BOX, GaussianBlur), as are the parts
 * of Python the result depends on: random.Random (MT19937, seeded with an int) and, where the Python iterates a set
 * while drawing random numbers (tryagain.colour), CPython's set iteration order (tuple hash + open addressing).  Any
 * change to tools/gfx_es must be mirrored here.  Check (from the repository root, DATA = a tools/extract_data.py folder):
 *   cc -O2 -DGFX_ES_MAIN port2/shims/gfx_es.c -lm -o /tmp/gfx_es && /tmp/gfx_es DATA /tmp/c.tbtx
 *   (cd tools && python3 -c "import gfx_es, sys; gfx_es.build(sys.argv[1], sys.argv[2])" DATA /tmp/py.tbtx)
 *   cmp /tmp/c.tbtx /tmp/py.tbtx
 *
 * No pixels are shipped: main.c calls port_gfx_es_load() at startup with --lang 5, which reads the three source files
 * through port_cd_read_named (data/ folder or disc image) and hands the TBTK image to shims/gpu.c
 * (port_tex_patch_add_mem: the same keyed rects --patch-tex FILE loads; rects a --patch-tex file already provides win).
 *
 * Floating point must be plain IEEE double / float with no contraction (CMakeLists: -ffp-contract=off, and SSE2 math on
 * 32-bit x86): Pillow's C and Python's float do no fused multiply-adds and no x87 excess precision.  No libm: the port
 * links its own symbol resolution (port2/tools/gen2.py stubs what libc does not define), and a platform's sin / cos
 * could round differently anyway; the few functions needed are below.  sin / cos come from fdlibm: for a few of the
 * arguments used here they differ from glibc's in the last bit (the standalone build with -DGFX_ES_CHECKMATH lists
 * them), but nothing derived from them changes (ellipse bounds truncated to pixels, a matrix rounded to 15 digits,
 * half-plane tests), and every trig argument is a constant of the generator, independent of the disc data, so the
 * byte-identity check above covers them all.
 *
 * The drawing / resampling / blur code below is adapted from Pillow (src/libImaging Draw.c, Geometry.c, Resample.c,
 * BoxBlur.c and PIL/ImageDraw.py, Image.py), under its MIT-CMU licence:
 *   Copyright (c) 1997-2011 by Secret Labs AB, Copyright (c) 1995-2011 by Fredrik Lundh and contributors,
 *   Copyright (c) 2010 by Jeffrey A. Clark and contributors.
 *   Permission to use, copy, modify and distribute this software and its documentation for any purpose and without fee
 *   is hereby granted, provided that the above copyright notice appears in all copies, and that both that copyright
 *   notice and this permission notice appear in supporting documentation, and that the name of Secret Labs AB or the
 *   author not be used in advertising or publicity pertaining to distribution of the software without specific,
 *   written prior permission.  SECRET LABS AB AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 *   INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.  IN NO EVENT SHALL SECRET LABS AB OR THE AUTHOR BE
 *   LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA
 *   OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION
 *   WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#if defined(__clang__)
#pragma STDC FP_CONTRACT OFF
#endif
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

static int g_err;    /* set by anything the Python would have raised on: that part is skipped */

/* ================================================================== math without libm */
static double g_fabs(double x) { return x < 0 ? -x : x; }
/* exact for |x| < 2^31 (every use here); larger values are returned as they are */
static double g_trunc(double x) { return (x > -2147483648.0 && x < 2147483647.0) ? (double)(int)x : x; }
static double g_floor(double x) { double t = g_trunc(x); return t > x ? t - 1.0 : t; }
static double g_ceil(double x) { double t = g_trunc(x); return t < x ? t + 1.0 : t; }
static double g_fmax(double a, double b) { return a > b ? a : b; }
static double g_fmin(double a, double b) { return a < b ? a : b; }
static long g_lround(double x) { double t = g_trunc(x), d = x - t; if (d >= 0.5) t += 1.0; else if (d <= -0.5) t -= 1.0; return (long)t; }
static float g_roundf(float x) { float t = (float)g_trunc(x), d = x - t; if (d >= 0.5f) t += 1.0f; else if (d <= -0.5f) t -= 1.0f; return t; }
static double g_fmod(double x, double y) {     /* y > 0, |x| / y < 2^31 */
    double ax = g_fabs(x), r = ax - g_trunc(ax / y) * y;
    if (r < 0) r += y; else if (r >= y) r -= y;
    return x < 0 ? -r : r;
}
static double g_sqrt(double x) { return __builtin_sqrt(x); }     /* an instruction (IEEE, correctly rounded) */
/* sin / cos: fdlibm 5.3 (k_sin.c, k_cos.c, e_rem_pio2.c medium path, s_sin.c, s_cos.c), for |x| < 2^19 * pi / 2:
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.  Developed at SunSoft, a Sun Microsystems, Inc.
 * business.  Permission to use, copy, modify, and distribute this software is freely granted, provided that this
 * notice is preserved. */
static uint32_t g_hi(double x) { uint64_t u; memcpy(&u, &x, 8); return (uint32_t)(u >> 32); }
static double g_with_hi(uint32_t hi) { uint64_t u = (uint64_t)hi << 32; double x; memcpy(&x, &u, 8); return x; }
static double k_sin(double x, double y, int iy) {
    const double S1 = -1.66666666666666324348e-01, S2 = 8.33333333332248946124e-03, S3 = -1.98412698298579493134e-04,
                 S4 = 2.75573137070700676789e-06, S5 = -2.50507602534068634195e-08, S6 = 1.58969099521155010221e-10;
    double z, r, v;
    if ((g_hi(x) & 0x7fffffff) < 0x3e400000 && (int)x == 0) return x;
    z = x * x; v = z * x;
    r = S2 + z * (S3 + z * (S4 + z * (S5 + z * S6)));
    if (iy == 0) return x + v * (S1 + z * r);
    return x - ((z * (0.5 * y - v * r) - y) - v * S1);
}
static double k_cos(double x, double y) {
    const double C1 = 4.16666666666666019037e-02, C2 = -1.38888888888741095749e-03, C3 = 2.48015872894767294178e-05,
                 C4 = -2.75573143513906633035e-07, C5 = 2.08757232129817482790e-09, C6 = -1.13596475577881948265e-11;
    double a, hz, z, r, qx;
    uint32_t ix = g_hi(x) & 0x7fffffff;
    if (ix < 0x3e400000 && (int)x == 0) return 1.0;
    z = x * x;
    r = z * (C1 + z * (C2 + z * (C3 + z * (C4 + z * (C5 + z * C6)))));
    if (ix < 0x3FD33333) return 1.0 - (0.5 * z - (z * r - x * y));
    qx = ix > 0x3fe90000 ? 0.28125 : g_with_hi(ix - 0x00200000);
    hz = 0.5 * z - qx; a = 1.0 - qx;
    return a - (hz - (z * r - x * y));
}
static int rem_pio2(double x, double *y) {     /* pi/4 < |x| < 2^19 * pi / 2 */
    const double invpio2 = 6.36619772367581382433e-01, pio2_1 = 1.57079632673412561417e+00,
                 pio2_1t = 6.07710050650619224932e-11, pio2_2 = 6.07710050630396597660e-11,
                 pio2_2t = 2.02226624879595063154e-21, pio2_3 = 2.02226624871116645580e-21,
                 pio2_3t = 8.47842766036889956997e-32;
    double t = g_fabs(x), r, w, fn; int n, i, j;
    n = (int)(t * invpio2 + 0.5); fn = (double)n;
    r = t - fn * pio2_1; w = fn * pio2_1t;
    j = (int)((g_hi(x) & 0x7fffffff) >> 20);
    y[0] = r - w;
    i = j - (int)((g_hi(y[0]) >> 20) & 0x7ff);
    if (i > 16) {
        t = r; w = fn * pio2_2; r = t - w; w = fn * pio2_2t - ((t - r) - w); y[0] = r - w;
        i = j - (int)((g_hi(y[0]) >> 20) & 0x7ff);
        if (i > 49) { t = r; w = fn * pio2_3; r = t - w; w = fn * pio2_3t - ((t - r) - w); y[0] = r - w; }
    }
    y[1] = (r - y[0]) - w;
    if (x < 0) { y[0] = -y[0]; y[1] = -y[1]; return -n; }
    return n;
}
static double g_sin(double x) {
    double y[2];
    if ((g_hi(x) & 0x7fffffff) <= 0x3fe921fb) return k_sin(x, 0.0, 0);
    switch (rem_pio2(x, y) & 3) {
        case 0: return k_sin(y[0], y[1], 1);
        case 1: return k_cos(y[0], y[1]);
        case 2: return -k_sin(y[0], y[1], 1);
        default: return -k_cos(y[0], y[1]);
    }
}
static double g_cos(double x) {
    double y[2];
    if ((g_hi(x) & 0x7fffffff) <= 0x3fe921fb) return k_cos(x, 0.0);
    switch (rem_pio2(x, y) & 3) {
        case 0: return k_cos(y[0], y[1]);
        case 1: return -k_sin(y[0], y[1], 1);
        case 2: return -k_cos(y[0], y[1]);
        default: return k_sin(y[0], y[1], 1);
    }
}
#ifdef GFX_ES_CHECKMATH
#include <math.h>
static double chk(const char *f, double x, double a, double b) { if (a != b) fprintf(stderr, "MATH MISMATCH %s(%.17g): %.17g vs libm %.17g\n", f, x, a, b); return a; }
static long chk_sin, chk_cos;
static double g_sin_chk(double x) { chk_sin++; return chk("sin", x, g_sin(x), sin(x)); }
static double g_cos_chk(double x) { chk_cos++; return chk("cos", x, g_cos(x), cos(x)); }
#define g_sin g_sin_chk
#define g_cos g_cos_chk
#endif

/* ================================================================== Python helpers */
static int py_round(double x) {             /* round(x): half to even */
    double f = g_floor(x), d = x - f;
    if (d > 0.5 || (d == 0.5 && g_fmod(f, 2.0) != 0)) f += 1.0;
    return (int)f;
}
static int py_floordiv(int a, int b) { int q = a / b; if ((a % b) && ((a < 0) != (b < 0))) q--; return q; }
static double py_round15(double x) {        /* round(x, 15): correctly rounded decimal, as Python's dtoa */
    char buf[64];
    snprintf(buf, sizeof buf, "%.15f", x);
    return strtod(buf, 0);
}

/* random.Random(seed) for a small non-negative int seed (init_by_array with one key word) */
typedef struct { u32 mt[624]; int mti; } PyRand;
static void pr_seed(PyRand *r, u32 seed) {
    u32 *mt = r->mt; int i, j, k;
    mt[0] = 19650218u;
    for (i = 1; i < 624; i++) mt[i] = 1812433253u * (mt[i - 1] ^ (mt[i - 1] >> 30)) + (u32)i;
    i = 1; j = 0;
    for (k = 624; k; k--) {
        mt[i] = (mt[i] ^ ((mt[i - 1] ^ (mt[i - 1] >> 30)) * 1664525u)) + seed + (u32)j;
        i++; j++;
        if (i >= 624) { mt[0] = mt[623]; i = 1; }
        if (j >= 1) j = 0;
    }
    for (k = 623; k; k--) {
        mt[i] = (mt[i] ^ ((mt[i - 1] ^ (mt[i - 1] >> 30)) * 1566083941u)) - (u32)i;
        i++;
        if (i >= 624) { mt[0] = mt[623]; i = 1; }
    }
    mt[0] = 0x80000000u;
    r->mti = 624;
}
static u32 pr_u32(PyRand *r) {
    u32 y, *mt = r->mt;
    if (r->mti >= 624) {
        int kk;
        for (kk = 0; kk < 624; kk++) {
            y = (mt[kk] & 0x80000000u) | (mt[(kk + 1) % 624] & 0x7fffffffu);
            mt[kk] = mt[(kk + 397) % 624] ^ (y >> 1) ^ ((y & 1) ? 0x9908b0dfu : 0);
        }
        r->mti = 0;
    }
    y = mt[r->mti++];
    y ^= y >> 11; y ^= (y << 7) & 0x9d2c5680u; y ^= (y << 15) & 0xefc60000u; y ^= y >> 18;
    return y;
}
static double pr_random(PyRand *r) { u32 a = pr_u32(r) >> 5, b = pr_u32(r) >> 6; return (a * 67108864.0 + b) * (1.0 / 9007199254740992.0); }
static double pr_uniform(PyRand *r, double a, double b) { return a + (b - a) * pr_random(r); }

/* a CPython set of (int, int) tuples: insertion and iteration order exactly as setobject.c (64-bit hashes) */
typedef struct { uint64_t h; int x, y, used; } SetEnt;
typedef struct { SetEnt *t; uint64_t mask; long fill, used; } PySet;
static uint64_t tuple_hash(int x, int y) {
    const uint64_t P1 = 11400714785074694791ull, P2 = 14029467366897019727ull, P5 = 2870177450012600261ull;
    uint64_t acc = P5, lane;
    lane = (uint64_t)(int64_t)(x == -1 ? -2 : x); acc += lane * P2; acc = (acc << 31) | (acc >> 33); acc *= P1;
    lane = (uint64_t)(int64_t)(y == -1 ? -2 : y); acc += lane * P2; acc = (acc << 31) | (acc >> 33); acc *= P1;
    acc += 2 ^ (P5 ^ 3527539ull);
    if (acc == (uint64_t)-1) return 1546275796;
    return acc;
}
static void ps_init(PySet *s) { s->mask = 7; s->t = (SetEnt *)calloc(8, sizeof(SetEnt)); s->fill = s->used = 0; if (!s->t) g_err = 1; }
static void ps_free(PySet *s) { free(s->t); s->t = 0; }
static void ps_insert_clean(SetEnt *t, uint64_t mask, const SetEnt *e) {
    uint64_t perturb = e->h, i = e->h & mask, j;
    for (;;) {
        if (!t[i].used) { t[i] = *e; return; }
        if (i + 9 <= mask) for (j = 1; j <= 9; j++) if (!t[i + j].used) { t[i + j] = *e; return; }
        perturb >>= 5;
        i = (i * 5 + 1 + perturb) & mask;
    }
}
static void ps_resize(PySet *s, long minused) {
    uint64_t n = 8, k;
    SetEnt *old = s->t, *nt;
    while (n <= (uint64_t)minused) n <<= 1;
    nt = (SetEnt *)calloc((size_t)n, sizeof(SetEnt));
    if (!nt) { g_err = 1; return; }
    for (k = 0; k <= s->mask; k++) if (old[k].used) ps_insert_clean(nt, n - 1, &old[k]);
    free(old); s->t = nt; s->mask = n - 1;
}
static void ps_add_h(PySet *s, int x, int y, uint64_t h) {
    uint64_t mask = s->mask, i = h & mask, perturb = h, k;
    if (g_err) return;
    for (;;) {
        int probes = i + 9 <= mask ? 9 : 0;
        k = i;
        for (;;) {
            SetEnt *e = &s->t[k];
            if (!e->used) {
                e->used = 1; e->h = h; e->x = x; e->y = y; s->fill++; s->used++;
                if ((uint64_t)s->fill * 5 < mask * 3) return;
                ps_resize(s, s->used > 50000 ? s->used * 2 : s->used * 4);
                return;
            }
            if (e->h == h && e->x == x && e->y == y) return;
            k++;
            if (!probes--) break;
        }
        perturb >>= 5;
        i = (i * 5 + 1 + perturb) & mask;
    }
}
static void ps_add(PySet *s, int x, int y) { ps_add_h(s, x, y, tuple_hash(x, y)); }
static void ps_merge(PySet *s, const PySet *o) {      /* s |= o (s not empty) */
    uint64_t k;
    if (!o->used) return;
    if ((uint64_t)(s->fill + o->used) * 5 >= s->mask * 3) ps_resize(s, (s->used + o->used) * 2);
    for (k = 0; k <= o->mask && !g_err; k++) if (o->t[k].used) ps_add_h(s, o->t[k].x, o->t[k].y, o->t[k].h);
}

/* ================================================================== images (mode "L") and Pillow operations */
typedef struct { int w, h; u8 *p; } Img;
static Img img_new(int w, int h) {
    Img im; im.w = w; im.h = h; im.p = (w > 0 && h > 0) ? (u8 *)calloc((size_t)w * h, 1) : 0;
    if (!im.p) { g_err = 1; im.w = im.h = 0; }
    return im;
}
static void img_free(Img *im) { free(im->p); im->p = 0; }
#define PIX(im, x, y) ((im)->p[(size_t)(y) * (im)->w + (x)])

/* ---- Draw.c */
static void hline8(Img *im, int x0, int y0, int x1) {
    if (y0 >= 0 && y0 < im->h) {
        if (x0 < 0) x0 = 0; else if (x0 >= im->w) return;
        if (x1 < 0) return; else if (x1 >= im->w) x1 = im->w - 1;
        if (x0 <= x1) memset(&PIX(im, x0, y0), 255, (size_t)(x1 - x0 + 1));
    }
}
static void draw_rectangle(Img *im, double fx0, double fy0, double fx1, double fy1) {
    int x0 = (int)fx0, y0 = (int)fy0, x1 = (int)fx1, y1 = (int)fy1, y, tmp;
    if (fx1 < fx0 || fy1 < fy0) { g_err = 1; return; }
    if (y0 > y1) { tmp = y0; y0 = y1; y1 = tmp; }
    if (y0 < 0) y0 = 0; else if (y0 >= im->h) return;
    if (y1 < 0) return; else if (y1 > im->h) y1 = im->h;
    for (y = y0; y <= y1; y++) hline8(im, x0, y, x1);
}

typedef struct { int32_t a, b, cx, cy, ex, ey; int64_t a2, b2, a2b2; int8_t finished; } quarter_state;
static void quarter_init(quarter_state *s, int32_t a, int32_t b) {
    if (a < 0 || b < 0) s->finished = 1;
    else {
        s->a = a; s->b = b; s->cx = a; s->cy = b % 2; s->ex = a % 2; s->ey = b;
        s->a2 = (int64_t)a * a; s->b2 = (int64_t)b * b; s->a2b2 = s->a2 * s->b2; s->finished = 0;
    }
}
static int64_t quarter_delta(quarter_state *s, int64_t x, int64_t y) {
    int64_t v = s->a2 * y * y + s->b2 * x * x - s->a2b2;
    return v < 0 ? -v : v;
}
static int8_t quarter_next(quarter_state *s, int32_t *ret_x, int32_t *ret_y) {
    if (s->finished) return -1;
    *ret_x = s->cx; *ret_y = s->cy;
    if (s->cx == s->ex && s->cy == s->ey) s->finished = 1;
    else {
        int32_t nx = s->cx, ny = s->cy + 2;
        int64_t ndelta = quarter_delta(s, nx, ny);
        if (nx > 1) {
            int64_t newdelta = quarter_delta(s, s->cx - 2, s->cy + 2);
            if (ndelta > newdelta) { nx = s->cx - 2; ny = s->cy + 2; ndelta = newdelta; }
            newdelta = quarter_delta(s, s->cx - 2, s->cy);
            if (ndelta > newdelta) { nx = s->cx - 2; ny = s->cy; }
        }
        s->cx = nx; s->cy = ny;
    }
    return 0;
}
typedef struct {
    quarter_state st_o, st_i;
    int32_t py, pl, pr, cy[4], cl[4], cr[4];
    int8_t bufcnt, finished, leftmost;
} ellipse_state;
static void ellipse_init(ellipse_state *s, int32_t a, int32_t b, int32_t w) {
    s->bufcnt = 0; s->leftmost = a % 2;
    quarter_init(&s->st_o, a, b);
    if (w < 1 || quarter_next(&s->st_o, &s->pr, &s->py) == -1) s->finished = 1;
    else { s->finished = 0; quarter_init(&s->st_i, a - 2 * (w - 1), b - 2 * (w - 1)); s->pl = s->leftmost; }
}
static int8_t ellipse_next(ellipse_state *s, int32_t *ret_x0, int32_t *ret_y, int32_t *ret_x1) {
    if (s->bufcnt == 0) {
        int32_t y, l, r, cx = 0, cy = 0; int8_t next_ret;
        if (s->finished) return -1;
        y = s->py; l = s->pl; r = s->pr;
        while ((next_ret = quarter_next(&s->st_o, &cx, &cy)) != -1 && cy <= y) {}
        if (next_ret == -1) s->finished = 1; else { s->pr = cx; s->py = cy; }
        while ((next_ret = quarter_next(&s->st_i, &cx, &cy)) != -1 && cy <= y) l = cx;
        s->pl = next_ret == -1 ? s->leftmost : cx;
        if ((l > 0 || l < r) && y > 0) { s->cl[s->bufcnt] = l == 0 ? 2 : l; s->cy[s->bufcnt] = y; s->cr[s->bufcnt] = r; ++s->bufcnt; }
        if (y > 0) { s->cl[s->bufcnt] = -r; s->cy[s->bufcnt] = y; s->cr[s->bufcnt] = -l; ++s->bufcnt; }
        if (l > 0 || l < r) { s->cl[s->bufcnt] = l == 0 ? 2 : l; s->cy[s->bufcnt] = -y; s->cr[s->bufcnt] = r; ++s->bufcnt; }
        s->cl[s->bufcnt] = -r; s->cy[s->bufcnt] = -y; s->cr[s->bufcnt] = -l; ++s->bufcnt;
    }
    --s->bufcnt;
    *ret_x0 = s->cl[s->bufcnt]; *ret_y = s->cy[s->bufcnt]; *ret_x1 = s->cr[s->bufcnt];
    return 0;
}
/* ImageDraw.ellipse(xy, fill) */
static void draw_ellipse(Img *im, double fx0, double fy0, double fx1, double fy1) {
    int x0 = (int)fx0, y0 = (int)fy0, x1 = (int)fx1, y1 = (int)fy1, a = x1 - x0, b = y1 - y0;
    ellipse_state st; int32_t X0, Y, X1;
    if (fx1 < fx0 || fy1 < fy0) { g_err = 1; return; }
    if (a < 0 || b < 0) return;
    ellipse_init(&st, a, b, a + b);
    while (ellipse_next(&st, &X0, &Y, &X1) != -1) hline8(im, x0 + (X0 + a) / 2, y0 + (Y + b) / 2, x0 + (X1 + a) / 2);
}
/* half-plane clip of a segment (clip_tree_do_clip, CT_CLIP): 0 = empty */
static int clip_half(double A, double B, double C, int32_t y, int32_t *px0, int32_t *px1) {
    double eps = 1e-9; int32_t x0 = *px0, x1 = *px1;
    if (g_fabs(A) < eps) {
        if (B * y + C < -eps) { x0 = 1; x1 = 0; }
    } else {
        double ix = -(B * y + C) / A;
        if (A * x0 + B * y + C < eps) x0 = (int32_t)g_lround(g_fmax(x0, ix));
        if (A * x1 + B * y + C < eps) x1 = (int32_t)g_lround(g_fmin(x1, ix));
    }
    *px0 = x0; *px1 = x1;
    return x0 <= x1;
}
static void normalize_angles(float *al, float *ar) {
    if (*ar - *al >= 360) { *al = 0; *ar = 360; }
    else {
        *al = g_fmod(*al < 0 ? 360 - (g_fmod(-*al, 360)) : *al, 360);
        *ar = *al + g_fmod(*ar < *al ? 360 - g_fmod(*al - *ar, 360) : *ar - *al, 360);
    }
}
/* ImageDraw pieslice, fill only (ImagingDrawPieslice + pie_init; the clip tree of a < 180 degree pie is an AND of its
   half-planes, so each ellipse segment is cut to their intersection) */
static void draw_pieslice(Img *im, double fx0, double fy0, double fx1, double fy1, float start, float end) {
    int x0 = (int)fx0, y0 = (int)fy0, x1 = (int)fx1, y1 = (int)fy1, a, b;
    double xl, xr, yl, yr, cl[3][3]; int nc = 2, i;
    ellipse_state st; int32_t X0, Y, X1;
    if (fx1 < fx0 || fy1 < fy0) { g_err = 1; return; }
    normalize_angles(&start, &end);
    if (start + 360 == end) { draw_ellipse(im, x0, y0, x1, y1); return; }
    if (start == end) return;
    a = x1 - x0; b = y1 - y0;
    if (a < 0 || b < 0) return;
    if (!(end - start < 180)) { g_err = 1; return; }       /* (an OR tree: not used by gfx_es) */
    ellipse_init(&st, a, b, x1 + y1 - x0 - y0);
    xl = a * g_cos(start * M_PI / 180.0); xr = a * g_cos(end * M_PI / 180.0);
    yl = b * g_sin(start * M_PI / 180.0); yr = b * g_sin(end * M_PI / 180.0);
    cl[0][0] = -yl; cl[0][1] = xl; cl[0][2] = 0;
    cl[1][0] = yr; cl[1][1] = -xr; cl[1][2] = 0;
    if (end - start < 90) { cl[2][0] = (xl + xr) / 2.0; cl[2][1] = (yl + yr) / 2.0; cl[2][2] = 0; nc = 3; }
    while (ellipse_next(&st, &X0, &Y, &X1) >= 0) {
        int32_t lo = X0, hi = X1, ok = 1;
        for (i = 0; i < nc && ok; i++) {
            int32_t s0 = X0, s1 = X1;
            if (!clip_half(cl[i][0], cl[i][1], cl[i][2], Y, &s0, &s1)) ok = 0;
            else { if (s0 > lo) lo = s0; if (s1 < hi) hi = s1; }
        }
        if (ok && lo <= hi) hline8(im, x0 + (lo + a) / 2, y0 + (Y + b) / 2, x0 + (hi + a) / 2);
    }
}
/* ImageDraw.rounded_rectangle(xy, radius, fill) (all corners) */
static void draw_rounded_rectangle(Img *im, double fx0, double fy0, double fx1, double fy1, double radius) {
    double d = fx1 - fx0, dd; int x0, y0, x1, y1, full_x, full_y, r;
    if (fx1 < fx0 || fy1 < fy0) { g_err = 1; return; }
    if (fy1 - fy0 < d) d = fy1 - fy0;
    if (radius * 2 < d) d = radius * 2;
    x0 = py_round(fx0); y0 = py_round(fy0); x1 = py_round(fx1); y1 = py_round(fy1);
    full_x = d >= x1 - x0 - 1;
    if (full_x) d = x1 - x0;
    full_y = d >= y1 - y0 - 1;
    if (full_y) d = y1 - y0;
    if (full_x && full_y) { draw_ellipse(im, fx0, fy0, fx1, fy1); return; }
    if (d == 0) { draw_rectangle(im, fx0, fy0, fx1, fy1); return; }
    r = (int)g_floor(d / 2);
    dd = d;
    if (full_x) {
        draw_pieslice(im, x0, y0, x0 + dd, y0 + dd, 180, 360);
        draw_pieslice(im, x0, y1 - dd, x0 + dd, y1, 0, 180);
    } else if (full_y) {
        draw_pieslice(im, x0, y0, x0 + dd, y0 + dd, 90, 270);
        draw_pieslice(im, x1 - dd, y0, x1, y0 + dd, 270, 90);
    } else {
        draw_pieslice(im, x0, y0, x0 + dd, y0 + dd, 180, 270);
        draw_pieslice(im, x1 - dd, y0, x1, y0 + dd, 270, 360);
        draw_pieslice(im, x1 - dd, y1 - dd, x1, y1, 0, 90);
        draw_pieslice(im, x0, y1 - dd, x0 + dd, y1, 90, 180);
    }
    if (full_x) draw_rectangle(im, x0, y0 + r + 1, x1, y1 - r - 1);
    else if (x1 - r - 1 >= x0 + r + 1) draw_rectangle(im, x0 + r + 1, y0, x1 - r - 1, y1);
    if (!full_x && !full_y) {
        draw_rectangle(im, x0, y0 + r + 1, x0 + r, y1 - (r + 1));
        draw_rectangle(im, x1 - r, y0 + r + 1, x1, y1 - (r + 1));
    }
}
/* ImageDraw.polygon(xy, fill): ImagingDrawPolygon + polygon_generic (8-bit, no alpha) */
typedef struct { int d, x0, y0, xmin, ymin, xmax, ymax; float dx; } Edge;
static int x_cmp(const void *a, const void *b) {
    float diff = *((const float *)a) - *((const float *)b);
    return diff < 0 ? -1 : diff > 0 ? 1 : 0;
}
#define ROUND_UP(f) ((int)((f) >= 0.0 ? g_floor((f) + 0.5F) : -g_floor(g_fabs(f) + 0.5F)))
#define ROUND_DOWN(f) ((int)((f) >= 0.0 ? g_ceil((f) - 0.5F) : -g_ceil(g_fabs(f) - 0.5F)))
static void add_edge(Edge *e, int x0, int y0, int x1, int y1) {
    if (x0 <= x1) e->xmin = x0, e->xmax = x1; else e->xmin = x1, e->xmax = x0;
    if (y0 <= y1) e->ymin = y0, e->ymax = y1; else e->ymin = y1, e->ymax = y0;
    if (y0 == y1) { e->d = 0; e->dx = 0.0; }
    else { e->dx = ((float)(x1 - x0)) / (y1 - y0); e->d = y0 == e->ymin ? 1 : -1; }
    e->x0 = x0; e->y0 = y0;
}
static void polygon_generic(Img *im, int n, Edge *e) {
    Edge **edge_table; float *xx, adjacent_line_x, adjacent_line_x_other_edge;
    int edge_count = 0, ymin = im->h - 1, ymax = 0, i, j, k;
    if (n <= 0) return;
    edge_table = (Edge **)calloc((size_t)n, sizeof(Edge *));
    if (!edge_table) { g_err = 1; return; }
    for (i = 0; i < n; i++) {
        if (ymin > e[i].ymin) ymin = e[i].ymin;
        if (ymax < e[i].ymax) ymax = e[i].ymax;
        if (e[i].ymin == e[i].ymax) { hline8(im, e[i].xmin, e[i].ymin, e[i].xmax); continue; }
        edge_table[edge_count++] = e + i;
    }
    if (ymin < 0) ymin = 0;
    if (ymax > im->h) ymax = im->h;
    xx = (float *)calloc((size_t)edge_count * 2 + 1, sizeof(float));
    if (!xx) { free(edge_table); g_err = 1; return; }
    for (; ymin <= ymax; ymin++) {
        j = 0;
        for (i = 0; i < edge_count; i++) {
            Edge *current = edge_table[i];
            if (ymin >= current->ymin && ymin <= current->ymax) {
                xx[j++] = (ymin - current->y0) * current->dx + current->x0;
                if (ymin == current->ymax && ymin < ymax) { xx[j] = xx[j - 1]; j++; }
                else if ((ymin == current->ymin || ymin == current->ymax) && current->dx != 0) {
                    for (k = 0; k < i; k++) {
                        Edge *other_edge = edge_table[k];
                        if ((ymin != other_edge->ymin && ymin != other_edge->ymax) || other_edge->dx == 0) continue;
                        if (g_roundf(xx[j - 1]) == g_roundf((ymin - other_edge->y0) * other_edge->dx + other_edge->x0)) {
                            int offset = ymin == current->ymax ? -1 : 1;
                            adjacent_line_x = (ymin + offset - current->y0) * current->dx + current->x0;
                            if (ymin + offset >= other_edge->ymin && ymin + offset <= other_edge->ymax) {
                                adjacent_line_x_other_edge = (ymin + offset - other_edge->y0) * other_edge->dx + other_edge->x0;
                                if (xx[j - 1] > adjacent_line_x + 1 && xx[j - 1] > adjacent_line_x_other_edge + 1)
                                    xx[j - 1] = g_roundf(g_fmax(adjacent_line_x, adjacent_line_x_other_edge)) + 1;
                                else if (xx[j - 1] < adjacent_line_x - 1 && xx[j - 1] < adjacent_line_x_other_edge - 1)
                                    xx[j - 1] = g_roundf(g_fmin(adjacent_line_x, adjacent_line_x_other_edge)) - 1;
                                break;
                            }
                        }
                    }
                }
            }
        }
        qsort(xx, (size_t)j, sizeof(float), x_cmp);
        for (i = 1; i < j; i += 2) hline8(im, ROUND_UP(xx[i - 1]), ymin, ROUND_DOWN(xx[i]));
    }
    free(xx); free(edge_table);
}
static void draw_polygon(Img *im, int count, const double *fxy) {
    int *xy, i, n, x0, y0, x1, y1; Edge *e;
    if (count < 2) { g_err = 1; return; }
    xy = (int *)calloc((size_t)count, 2 * sizeof(int)); e = (Edge *)calloc((size_t)count, sizeof(Edge));
    if (!xy || !e) { free(xy); free(e); g_err = 1; return; }
    for (i = 0; i < 2 * count; i++) xy[i] = (int)fxy[i];
    for (i = n = 0; i < count - 1; i++) {
        x0 = xy[i * 2]; y0 = xy[i * 2 + 1]; x1 = xy[i * 2 + 2]; y1 = xy[i * 2 + 3];
        if (y0 == y1 && i != 0 && y0 == xy[i * 2 - 1]) {
            Edge *last_e = &e[n - 1];
            if (x1 > x0 && x0 > xy[i * 2 - 2]) { last_e->xmax = x1; continue; }
            else if (x1 < x0 && x0 < xy[i * 2 - 2]) { last_e->xmin = x1; continue; }
        }
        add_edge(&e[n++], x0, y0, x1, y1);
    }
    if (xy[i * 2] != xy[0] || xy[i * 2 + 1] != xy[1]) add_edge(&e[n++], xy[i * 2], xy[i * 2 + 1], xy[0], xy[1]);
    polygon_generic(im, n, e);
    free(e); free(xy);
}

/* ---- Geometry.c: Image.transform(size, AFFINE, a, BICUBIC) (fill black outside) */
#define XCLIP(im, x) (((x) < 0) ? 0 : ((x) < (im)->w) ? (x) : (im)->w - 1)
#define YCLIP(im, y) (((y) < 0) ? 0 : ((y) < (im)->h) ? (y) : (im)->h - 1)
#define FLOORI(v) ((v) < 0.0 ? ((int)g_floor(v)) : ((int)(v)))
#define BICUBIC(v, v1, v2, v3, v4, d)                \
    {                                                \
        double p1 = v2;                              \
        double p2 = -v1 + v3;                        \
        double p3 = 2 * (v1 - v2) + v3 - v4;         \
        double p4 = -v1 + v2 - v3 + v4;              \
        v = p1 + (d) * (p2 + (d) * (p3 + (d) * p4)); \
    }
static int bicubic_filter8(u8 *out, const Img *im, double xin, double yin) {
    int x, y, x0, x1, x2, x3; double v1, v2, v3, v4, dx, dy; const u8 *in;
    if (xin < 0.0 || xin >= im->w || yin < 0.0 || yin >= im->h) return 0;
    xin -= 0.5; yin -= 0.5;
    x = FLOORI(xin); y = FLOORI(yin);
    dx = xin - x; dy = yin - y;
    x--; y--;
    in = im->p + (size_t)YCLIP(im, y) * im->w;
    x0 = XCLIP(im, x + 0); x1 = XCLIP(im, x + 1); x2 = XCLIP(im, x + 2); x3 = XCLIP(im, x + 3);
    BICUBIC(v1, in[x0], in[x1], in[x2], in[x3], dx);
    if (y + 1 >= 0 && y + 1 < im->h) { in = im->p + (size_t)(y + 1) * im->w; BICUBIC(v2, in[x0], in[x1], in[x2], in[x3], dx); } else v2 = v1;
    if (y + 2 >= 0 && y + 2 < im->h) { in = im->p + (size_t)(y + 2) * im->w; BICUBIC(v3, in[x0], in[x1], in[x2], in[x3], dx); } else v3 = v2;
    if (y + 3 >= 0 && y + 3 < im->h) { in = im->p + (size_t)(y + 3) * im->w; BICUBIC(v4, in[x0], in[x1], in[x2], in[x3], dx); } else v4 = v3;
    BICUBIC(v1, v1, v2, v3, v4, dy);
    out[0] = v1 <= 0.0 ? 0 : v1 >= 255.0 ? 255 : (u8)v1;
    return 1;
}
static Img transform_affine(const Img *in, int w, int h, const double a[6]) {
    Img out = img_new(w, h); int x, y;
    if (g_err) return out;
    for (y = 0; y < h; y++)
        for (x = 0; x < w; x++) {
            double xin = x + 0.5, yin = y + 0.5;
            double xx = a[0] * xin + a[1] * yin + a[2], yy = a[3] * xin + a[4] * yin + a[5];
            if (!bicubic_filter8(&PIX(&out, x, y), in, xx, yy)) PIX(&out, x, y) = 0;
        }
    return out;
}
/* Image.rotate(angle, BICUBIC, expand=True) */
static Img rotate_expand(const Img *in, double angle) {
    int w = in->w, h = in->h, nw, nh, i;
    double m[6], cx = w / 2.0, cy = h / 2.0, xx[4], yy[4], mnx, mxx, mny, mxy;
    static const int cxs[4] = {0, 1, 1, 0}, cys[4] = {0, 0, 1, 1};
    angle = g_fmod(angle, 360.0);
    if (angle < 0) angle += 360.0;
    if (angle == 0 || angle == 90 || angle == 180 || angle == 270) { Img e = {0, 0, 0}; g_err = 1; return e; }  /* (transpose paths: unused) */
    angle = -(angle * (M_PI / 180.0));
    m[0] = py_round15(g_cos(angle)); m[1] = py_round15(g_sin(angle)); m[2] = 0.0;
    m[3] = py_round15(-g_sin(angle)); m[4] = py_round15(g_cos(angle)); m[5] = 0.0;
    {   double x = -cx - 0, y = -cy - 0, t2 = m[0] * x + m[1] * y + m[2], t5 = m[3] * x + m[4] * y + m[5];
        m[2] = t2; m[5] = t5; }
    m[2] += cx; m[5] += cy;
    for (i = 0; i < 4; i++) {
        double x = cxs[i] * w, y = cys[i] * h;
        xx[i] = m[0] * x + m[1] * y + m[2]; yy[i] = m[3] * x + m[4] * y + m[5];
    }
    mnx = mxx = xx[0]; mny = mxy = yy[0];
    for (i = 1; i < 4; i++) { if (xx[i] < mnx) mnx = xx[i]; if (xx[i] > mxx) mxx = xx[i]; if (yy[i] < mny) mny = yy[i]; if (yy[i] > mxy) mxy = yy[i]; }
    nw = (int)g_ceil(mxx) - (int)g_floor(mnx); nh = (int)g_ceil(mxy) - (int)g_floor(mny);
    {   double x = -(nw - w) / 2.0, y = -(nh - h) / 2.0, t2 = m[0] * x + m[1] * y + m[2], t5 = m[3] * x + m[4] * y + m[5];
        m[2] = t2; m[5] = t5; }
    return transform_affine(in, nw, nh, m);
}

/* ---- Resample.c: Image.resize(size, BOX) */
#define PRECISION_BITS (32 - 8 - 2)
static u8 clip8(int in) { int v = in >> PRECISION_BITS; return v < 0 ? 0 : v > 255 ? 255 : (u8)v; }
static double box_filter(double x) { return (x > -0.5 && x <= 0.5) ? 1.0 : 0.0; }
static int precompute_coeffs(int inSize, float in0, float in1, int outSize, int **boundsp, int **kkp) {
    double support, scale, filterscale, *kk, *k, inv_filterscale;
    int xx, x, ksize, xmin, xmax, *bounds, *ik;
    filterscale = scale = (double)(in1 - in0) / outSize;
    if (filterscale < 1.0) filterscale = 1.0;
    support = 0.5 * filterscale;
    ksize = (int)g_ceil(support) * 2 + 1;
    kk = (double *)malloc((size_t)outSize * ksize * sizeof(double));
    bounds = (int *)malloc((size_t)outSize * 2 * sizeof(int));
    ik = (int *)malloc((size_t)outSize * ksize * sizeof(int));
    if (!kk || !bounds || !ik) { free(kk); free(bounds); free(ik); g_err = 1; return 0; }
    inv_filterscale = 1.0 / filterscale;
    for (xx = 0; xx < outSize; xx++) {
        double center = in0 + (xx + 0.5) * scale, ww = 0.0;
        xmin = (int)(center - support + 0.5);
        if (xmin < 0) xmin = 0;
        xmax = (int)(center + support + 0.5);
        if (xmax > inSize) xmax = inSize;
        xmax -= xmin;
        k = &kk[xx * ksize];
        for (x = 0; x < xmax; x++) { double w = box_filter((x + xmin - center + 0.5) * inv_filterscale); k[x] = w; ww += w; }
        if (ww != 0.0) for (x = 0; x < xmax; x++) k[x] /= ww;
        for (; x < ksize; x++) k[x] = 0;
        bounds[xx * 2 + 0] = xmin; bounds[xx * 2 + 1] = xmax;
    }
    for (x = 0; x < outSize * ksize; x++)       /* normalize_coeffs_8bpc */
        ik[x] = kk[x] < 0 ? (int)(-0.5 + kk[x] * (1 << PRECISION_BITS)) : (int)(0.5 + kk[x] * (1 << PRECISION_BITS));
    free(kk);
    *boundsp = bounds; *kkp = ik;
    return ksize;
}
static Img resize_box(const Img *in, int xsize, int ysize) {
    float box[4]; int need_h, need_v, ybox_first, ybox_last, ksv, ksh, *bv, *kv, *bh, *kh, i, xx, yy, x, y;
    Img tmp = {0, 0, 0}, out = {0, 0, 0}; const Img *src = in;
    if (g_err) return out;
    if (xsize == in->w && ysize == in->h) { out = img_new(xsize, ysize); if (!g_err) memcpy(out.p, in->p, (size_t)xsize * ysize); return out; }
    box[0] = 0; box[1] = 0; box[2] = (float)in->w; box[3] = (float)in->h;
    need_h = xsize != in->w || box[0] || box[2] != xsize;
    need_v = ysize != in->h || box[1] || box[3] != ysize;
    ksv = precompute_coeffs(in->h, box[1], box[3], ysize, &bv, &kv);
    if (!ksv) return out;
    ybox_first = bv[0];
    ybox_last = bv[ysize * 2 - 2] + bv[ysize * 2 - 1];
    if (need_h) {
        ksh = precompute_coeffs(in->w, box[0], box[2], xsize, &bh, &kh);
        if (!ksh) { free(bv); free(kv); return out; }
        for (i = 0; i < ysize; i++) bv[i * 2] -= ybox_first;
        tmp = img_new(xsize, ybox_last - ybox_first);
        if (!g_err)
            for (yy = 0; yy < tmp.h; yy++) {
                const u8 *lineIn = in->p + (size_t)(yy + ybox_first) * in->w;
                for (xx = 0; xx < xsize; xx++) {
                    int xmin = bh[xx * 2], xmax = bh[xx * 2 + 1], *k = &kh[xx * ksh], ss0 = 1 << (PRECISION_BITS - 1);
                    for (x = 0; x < xmax; x++) ss0 += lineIn[x + xmin] * k[x];
                    PIX(&tmp, xx, yy) = clip8(ss0);
                }
            }
        free(bh); free(kh);
        if (g_err) { free(bv); free(kv); img_free(&tmp); return out; }
        src = &tmp;
    }
    if (need_v) {
        out = img_new(src->w, ysize);
        if (!g_err)
            for (yy = 0; yy < ysize; yy++) {
                int *k = &kv[yy * ksv], ymin = bv[yy * 2], ymax = bv[yy * 2 + 1];
                for (xx = 0; xx < src->w; xx++) {
                    int ss0 = 1 << (PRECISION_BITS - 1);
                    for (y = 0; y < ymax; y++) ss0 += src->p[(size_t)(y + ymin) * src->w + xx] * k[y];
                    PIX(&out, xx, yy) = clip8(ss0);
                }
            }
        img_free(&tmp);
    } else out = tmp;
    free(bv); free(kv);
    return out;
}

/* ---- BoxBlur.c: ImageFilter.GaussianBlur(radius) (3 box passes each way) */
static void line_box_blur8(u8 *lineOut, const u8 *lineIn, int lastx, int radius, int edgeA, int edgeB, u32 ww, u32 fw) {
    int x; u32 acc, bulk;
#define MOVE_ACC(acc, subtract, add) acc += lineIn[add] - lineIn[subtract];
#define ADD_FAR(bulk, acc, left, right) bulk = (acc * ww) + (lineIn[left] + lineIn[right]) * fw;
#define SAVE(x, bulk) lineOut[x] = (u8)((bulk + (1 << 23)) >> 24)
    acc = lineIn[0] * (radius + 1);
    for (x = 0; x < edgeA - 1; x++) acc += lineIn[x];
    acc += lineIn[lastx] * (radius - edgeA + 1);
    if (edgeA <= edgeB) {
        for (x = 0; x < edgeA; x++) { MOVE_ACC(acc, 0, x + radius); ADD_FAR(bulk, acc, 0, x + radius + 1); SAVE(x, bulk); }
        for (x = edgeA; x < edgeB; x++) { MOVE_ACC(acc, x - radius - 1, x + radius); ADD_FAR(bulk, acc, x - radius - 1, x + radius + 1); SAVE(x, bulk); }
        for (x = edgeB; x <= lastx; x++) { MOVE_ACC(acc, x - radius - 1, lastx); ADD_FAR(bulk, acc, x - radius - 1, lastx); SAVE(x, bulk); }
    } else {
        for (x = 0; x < edgeB; x++) { MOVE_ACC(acc, 0, x + radius); ADD_FAR(bulk, acc, 0, x + radius + 1); SAVE(x, bulk); }
        for (x = edgeB; x < edgeA; x++) { MOVE_ACC(acc, 0, lastx); ADD_FAR(bulk, acc, 0, lastx); SAVE(x, bulk); }
        for (x = edgeA; x <= lastx; x++) { MOVE_ACC(acc, x - radius - 1, lastx); ADD_FAR(bulk, acc, x - radius - 1, lastx); SAVE(x, bulk); }
    }
#undef MOVE_ACC
#undef ADD_FAR
#undef SAVE
}
static void horizontal_box_blur(Img *im, float floatRadius) {     /* in place, row by row */
    int y, radius = (int)floatRadius;
    u32 ww = (u32)((u32)(1 << 24) / (floatRadius * 2 + 1));
    u32 fw = ((1 << 24) - (radius * 2 + 1) * ww) / 2;
    int edgeA = radius + 1 < im->w ? radius + 1 : im->w;
    int edgeB = im->w - radius - 1 > 0 ? im->w - radius - 1 : 0;
    u8 *line = (u8 *)malloc((size_t)im->w);
    if (!line) { g_err = 1; return; }
    for (y = 0; y < im->h; y++) {
        line_box_blur8(line, im->p + (size_t)y * im->w, im->w - 1, radius, edgeA, edgeB, ww, fw);
        memcpy(im->p + (size_t)y * im->w, line, (size_t)im->w);
    }
    free(line);
}
static float gaussian_blur_radius(float radius, int passes) {
    float sigma2, L, l, a;
    sigma2 = radius * radius / passes;
    L = g_sqrt(12.0 * sigma2 + 1.0);
    l = g_floor((L - 1.0) / 2.0);
    a = (2 * l + 1) * (l * (l + 1) - 3 * sigma2);
    a /= 6 * (sigma2 - (l + 1) * (l + 1));
    return l + a;
}
static Img transpose(const Img *in) {
    Img out = img_new(in->h, in->w); int x, y;
    if (!g_err) for (y = 0; y < in->h; y++) for (x = 0; x < in->w; x++) PIX(&out, y, x) = PIX(in, x, y);
    return out;
}
static Img gaussian_blur(const Img *in, float radius) {
    float r = gaussian_blur_radius(radius, 3);
    Img out = img_new(in->w, in->h), t; int i;
    if (g_err) return out;
    memcpy(out.p, in->p, (size_t)in->w * in->h);
    if (r != 0) {
        for (i = 0; i < 3; i++) horizontal_box_blur(&out, r);
        t = transpose(&out);
        if (g_err) return out;
        for (i = 0; i < 3; i++) horizontal_box_blur(&t, r);
        img_free(&out);
        out = transpose(&t);
        img_free(&t);
    }
    return out;
}

/* ================================================================== sparse 2D maps (Python dicts / sets of points) */
#define MAPN 320
#define MOFF 32                                   /* coordinates -32 .. 287 */
typedef struct { signed char v[MAPN * MAPN]; } Map;
static Map *map_new(void) { Map *m = (Map *)malloc(sizeof(Map)); if (!m) { g_err = 1; return 0; } memset(m->v, -1, sizeof m->v); return m; }
static int mget(const Map *m, int x, int y) {
    x += MOFF; y += MOFF;
    if (!m || x < 0 || y < 0 || x >= MAPN || y >= MAPN) return -1;
    return m->v[y * MAPN + x];
}
static void mset(Map *m, int x, int y, int v) {
    x += MOFF; y += MOFF;
    if (!m || x < 0 || y < 0 || x >= MAPN || y >= MAPN) { g_err = 1; return; }
    m->v[y * MAPN + x] = (signed char)v;
}
static int mhas(const Map *m, int x, int y) { return mget(m, x, y) >= 0; }
#define MFOR(m, x, y) for (y = -MOFF; y < MAPN - MOFF; y++) for (x = -MOFF; x < MAPN - MOFF; x++) if (mhas(m, x, y))
static int mbbox(const Map *m, int *x0, int *y0, int *x1, int *y1) {     /* -> count; min / max coordinates */
    int x, y, n = 0;
    *x0 = *y0 = 1 << 20; *x1 = *y1 = -(1 << 20);
    MFOR(m, x, y) { n++; if (x < *x0) *x0 = x; if (x > *x1) *x1 = x; if (y < *y0) *y0 = y; if (y > *y1) *y1 = y; }
    if (!n) g_err = 1;                            /* (min() / max() of an empty sequence) */
    return n;
}
static Map *map_shift(const Map *m, int dx, int dy) {
    Map *o = map_new(); int x, y;
    if (o) MFOR(m, x, y) mset(o, x + dx, y + dy, mget(m, x, y));
    return o;
}
static int n4(const Map *m, int x, int y) { return mhas(m, x + 1, y) + mhas(m, x - 1, y) + mhas(m, x, y + 1) + mhas(m, x, y - 1); }
static void map_free(Map **m) { free(*m); *m = 0; }

/* ================================================================== CARGAR / GUARDAR (cargar_guardar.py) */
static const int CG_BAND[16] = {0, 0, 0, 0, 9, 7, 7, 6, 6, 6, 1, 1, 8, 8, 0, 0};
static int cg_aa(int b) { return b == 9 ? 6 : b == 7 ? 6 : b == 6 ? 5 : b == 1 ? 5 : b == 8 ? 5 : -1; }
static const char *const CG_C[10] = {".+######", "+#######", "##+.....", "##......", "##......", "##......", "##......", "###.....", "########", ".#######"};
static const char *const CG_G[10] = {".+######", "+#######", "##+.....", "##......", "##..####", "##..####", "##....##", "###..###", "########", ".######."};
static const char *const CG_R[10] = {"######..", "#######.", "##...###", "##....##", "##...###", "#######.", "######..", "##..###.", "##...###", "##....##"};
static const char *const CG_U[10] = {"##....##", "##....##", "##....##", "##....##", "##....##", "##....##", "##....##", "###..###", "########", ".######."};
typedef struct { int w; char r[10][16]; } CgLetter;
static void cg_from(CgLetter *L, const char *const *rows) { int i; L->w = (int)strlen(rows[0]); for (i = 0; i < 10; i++) memcpy(L->r[i], rows[i], (size_t)L->w); }
static void cg_narrow(CgLetter *L, int w) {
    while (L->w > w) {
        int i, c, r, found = -1;
        for (i = 2; i < L->w - 2 && found < 0; i++) {
            int same = 1;
            for (r = 0; r < 10; r++) if (L->r[r][i] != L->r[r][i + 1]) { same = 0; break; }
            if (same) found = i;
        }
        if (found < 0) found = L->w / 2;
        for (r = 0; r < 10; r++) for (c = found; c < L->w - 1; c++) L->r[r][c] = L->r[r][c + 1];
        L->w--;
    }
}
static int cg_ink(char m[16][80], int w, int a, int b) { return b >= 0 && b < 16 && a >= 0 && a < w && (m[b][a] == '#' || m[b][a] == '+'); }
/* word -> render -> words (w/4 * 16) */
static void cg_word(const CgLetter *ls, int nl, int W, int lw, u16 *words) {
    char m[16][80]; int r, x, i, end = -1;
    memset(m, '.', sizeof m);
    {   char rows[10][160]; int len = 0;
        for (i = 0; i < nl; i++) {
            CgLetter L = ls[i];
            cg_narrow(&L, lw);
            for (r = 0; r < 10; r++) { memcpy(rows[r] + len, L.r[r], (size_t)L.w); rows[r][len + L.w] = '.'; }
            len += L.w + 1;
        }
        for (r = 0; r < 10; r++) for (x = 0; x < W; x++) m[4 + r][x] = x == 0 ? '.' : x - 1 < len ? rows[r][x - 1] : '.';
    }
    for (r = 0; r < 16; r++) for (x = 0; x < W; x++) if ((m[r][x] == '#' || m[r][x] == '+') && x > end) end = x;
    end += 1;
    for (r = 0; r < 16; r++)
        for (x = 0; x < W; x += 4) {
            int k, wv = 0;
            for (k = 0; k < 4; k++) {
                char ch = m[r][x + k]; int v;
                if ((ch == '#' || ch == '+') && !CG_BAND[r]) { g_err = 1; v = 0; }      /* (KeyError) */
                else if (ch == '#') v = CG_BAND[r];
                else if (ch == '+') v = cg_aa(CG_BAND[r]);
                else if (cg_ink(m, W, x + k - 1, r) || (cg_ink(m, W, x + k, r - 1) && (cg_ink(m, W, x + k - 1, r - 1) || r == 14))) v = 5;
                else v = (r >= 10 && x + k <= end) ? 12 : 0;
                wv |= v << (4 * k);
            }
            *words++ = (u16)wv;
        }
}

/* ================================================================== the TBTK image */
typedef struct { u8 *b; size_t n, cap; int count; } Out;
static void out_put(Out *o, const void *d, size_t n) {
    if (o->n + n > o->cap) {
        size_t c = o->cap ? o->cap * 2 : 65536; u8 *nb;
        while (c < o->n + n) c *= 2;
        nb = (u8 *)realloc(o->b, c);
        if (!nb) { g_err = 1; return; }
        o->b = nb; o->cap = c;
    }
    memcpy(o->b + o->n, d, n); o->n += n;
}
static void out_u16(Out *o, unsigned v) { u8 b[2]; b[0] = (u8)v; b[1] = (u8)(v >> 8); out_put(o, b, 2); }
typedef struct { int kx, ky, kw, kh; u32 hash; } Key;
/* page_key(): the key rect of a 4bpp page stored `stride` bytes a row, uploaded at VRAM word (vx, vy) */
static int page_key(Key *k, const u8 *page, size_t n, int stride, int vx, int vy, int u, int v, int w, int h) {
    int r, i; u32 hh = 2166136261u;
    if ((size_t)(v + h - 1) * stride + u / 2 + 2 * (w / 4) > n) return 0;
    for (r = v; r < v + h; r++) for (i = 0; i < w / 4; i++) {
        hh = (hh ^ page[r * stride + u / 2 + 2 * i]) * 16777619u;
        hh = (hh ^ page[r * stride + u / 2 + 2 * i + 1]) * 16777619u;
    }
    k->kx = vx + u / 4; k->ky = vy + v; k->kw = w / 4; k->kh = h; k->hash = hh;
    return 1;
}
static void out_rect(Out *o, int x, int y, int w, int h, const u16 *words, const Key *k) {
    int i; u8 hb[4];
    out_u16(o, (unsigned)x); out_u16(o, (unsigned)y); out_u16(o, (unsigned)w); out_u16(o, (unsigned)h);
    out_u16(o, (unsigned)k->kx); out_u16(o, (unsigned)k->ky); out_u16(o, (unsigned)k->kw); out_u16(o, (unsigned)k->kh);
    hb[0] = (u8)k->hash; hb[1] = (u8)(k->hash >> 8); hb[2] = (u8)(k->hash >> 16); hb[3] = (u8)(k->hash >> 24);
    out_put(o, hb, 4);
    for (i = 0; i < w * h; i++) out_u16(o, words[i]);
    o->count++;
}

static int part_cargar_guardar(Out *o, const u8 *page, size_t n) {
    Key k; CgLetter A, D, C, G, R, U, set1[6], set2[7]; u16 w1[16 * 16], w2[18 * 16];
    char load[16][40]; int r, x;
    if (!page_key(&k, page, n, 384, 64, 0, 168, 120, 40, 32)) return 0;
    if ((size_t)(136 + 15) * 384 + (168 + 39) / 2 >= n) return 0;
    for (r = 0; r < 16; r++)
        for (x = 0; x < 40; x++) {
            int u = 168 + x, v = 136 + r, c = (page[v * 384 + (u >> 1)] >> (4 * (u & 1))) & 15, b = CG_BAND[r];
            int inset = c == 1 || c == 6 || c == 7 || c == 8 || c == 9;
            load[r][x] = (inset && b && c == b) ? '#' : (b && c == cg_aa(b) && c != 5) ? '+' : inset ? '?' : '.';
        }
    A.w = 9; D.w = 8;
    for (r = 0; r < 10; r++) {
        for (x = 0; x < 9; x++) A.r[r][x] = load[4 + r][21 + x] == '?' ? '#' : load[4 + r][21 + x];
        for (x = 0; x < 8; x++) D.r[r][x] = load[4 + r][31 + x] == '?' ? '#' : load[4 + r][31 + x];
    }
    cg_from(&C, CG_C); cg_from(&G, CG_G); cg_from(&R, CG_R); cg_from(&U, CG_U);
    set1[0] = C; set1[1] = A; set1[2] = R; set1[3] = G; set1[4] = A; set1[5] = R;
    set2[0] = G; set2[1] = U; set2[2] = A; set2[3] = R; set2[4] = D; set2[5] = A; set2[6] = R;
    cg_word(set1, 6, 64, 8, w1);
    cg_word(set2, 7, 72, 7, w2);
    if (g_err) return 0;
    out_rect(o, 64, 160, 16, 16, w1, &k);
    out_rect(o, 64, 176, 18, 16, w2, &k);
    return 2;
}

/* ================================================================== CARGANDO (cargando.py) */
#define CA_PW 112
#define CA_PH 64
#define CA_K 8
static const u8 *ca_raw;
static int ca_px(int x, int y) { return (ca_raw[y * 128 + (x >> 1)] >> (4 * (x & 1))) & 15; }
static int ca_bg(int v) { return v == 0 || v == 2 || v == 3 || v == 10 || v == 11; }
static Map *ca_comp(int sx, int sy) {
    Map *c = map_new(); int *st = (int *)malloc(sizeof(int) * 2 * (CA_PW * CA_PH + 8)), sp = 0, d;
    static const int dx4[4] = {1, -1, 0, 0}, dy4[4] = {0, 0, 1, -1};
    if (!c || !st) { free(st); map_free(&c); g_err = 1; return 0; }
    mset(c, sx, sy, 1); st[sp++] = sx; st[sp++] = sy;
    while (sp) {
        int b = st[--sp], a = st[--sp];
        for (d = 0; d < 4; d++) {
            int px = a + dx4[d], py = b + dy4[d];
            if (px >= 0 && px < CA_PW && py >= 0 && py < CA_PH && !mhas(c, px, py) && !ca_bg(ca_px(px, py))) { mset(c, px, py, 1); st[sp++] = px; st[sp++] = py; }
        }
    }
    free(st);
    return c;
}
static int ca_near(int x, int y, unsigned cols) {
    int best = 99, xx, yy;
    for (yy = y - 3; yy <= y + 3; yy++) for (xx = x - 3; xx <= x + 3; xx++)
        if (xx >= 0 && xx < CA_PW && yy >= 0 && yy < CA_PH && (cols >> ca_px(xx, yy) & 1)) { int d = abs(xx - x) + abs(yy - y); if (d < best) best = d; }
    return best;
}
/* grab(seed, keep): keep 0 none, 1 = A rule, 2 = D rule */
static Map *ca_grab(int sx, int sy, int keep) {
    Map *c = ca_comp(sx, sy), *out = 0, *res = 0; int x, y, x0, y0, x1, y1;
    const unsigned S615 = 1u << 6 | 1u << 15, S714 = 1u << 7 | 1u << 14;
    if (!c) return 0;
    if (keep) MFOR(c, x, y) {
        int a = ca_near(x, y, S615), b = ca_near(x, y, S714);
        if (keep == 1 ? !(a <= b) : !(b < a)) mset(c, x, y, -1);
    }
    mbbox(c, &x0, &y0, &x1, &y1);
    x0--; x1++; y0--; y1++;
    out = map_new();
    {   int *st = (int *)malloc(sizeof(int) * 2 * ((x1 - x0 + 1) * (y1 - y0 + 1) + 8)), sp = 0, d;
        static const int dx4[4] = {1, -1, 0, 0}, dy4[4] = {0, 0, 1, -1};
        if (!st || !out) { free(st); map_free(&c); map_free(&out); g_err = 1; return 0; }
        mset(out, x0, y0, 1); st[sp++] = x0; st[sp++] = y0;
        while (sp) {
            int b = st[--sp], a = st[--sp];
            for (d = 0; d < 4; d++) {
                int px = a + dx4[d], py = b + dy4[d];
                if (px >= x0 && px <= x1 && py >= y0 && py <= y1 && !mhas(out, px, py) && !mhas(c, px, py)) { mset(out, px, py, 1); st[sp++] = px; st[sp++] = py; }
            }
        }
        free(st);
    }
    {   /* allp = c | holes; x0, y0 = its minimum (the holes lie inside c's box) */
        int ax0 = 1 << 20, ay0 = 1 << 20;
        for (y = y0; y <= y1; y++) for (x = x0; x <= x1; x++)
            if (mhas(c, x, y) || !mhas(out, x, y)) { if (x < ax0) ax0 = x; if (y < ay0) ay0 = y; }
        res = map_new();
        if (res) for (y = y0; y <= y1; y++) for (x = x0; x <= x1; x++) {
            if (mhas(c, x, y)) mset(res, x - ax0, y - ay0, ca_px(x, y));
            else if (!mhas(out, x, y)) mset(res, x - ax0, y - ay0, 2);
        }
    }
    map_free(&c); map_free(&out);
    return res;
}
static Map *ca_shade(const Map *m, int fill, int shd) {
    int cnt[16] = {0}, first[16], order = 0, x, y, top = -1, c; Map *o;
    for (c = 0; c < 16; c++) first[c] = 1 << 30;
    MFOR(m, x, y) { int v = mget(m, x, y); if (!cnt[v]) first[v] = order++; cnt[v]++; }
    for (c = 0; c < 16; c++)
        if (cnt[c] && c != 2 && c != 9 && (top < 0 || cnt[c] > cnt[top] || (cnt[c] == cnt[top] && first[c] < first[top]))) top = c;
    if (top < 0) { g_err = 1; return 0; }      /* (old[0] of an empty list) */
    o = map_new();
    if (o) MFOR(m, x, y) { int v = mget(m, x, y); mset(o, x, y, v == top ? fill : (v == 2 || v == 9) ? v : cnt[v] ? shd : v); }
    return o;
}
static Map *ca_upscale(const Map *m, double sc) {
    int x0, y0, W0, H0, W1, H1, x, y, i, j; Img im, bl, sm; Map *out;
    mbbox(m, &x0, &y0, &W0, &H0);
    W0 += 1; H0 += 1;
    im = img_new(W0 * 8, H0 * 8);
    if (g_err) return 0;
    MFOR(m, x, y) for (i = 0; i < 8; i++) for (j = 0; j < 8; j++) PIX(&im, x * 8 + i, y * 8 + j) = 255;
    W1 = py_round(W0 * sc); H1 = py_round(H0 * sc);
    bl = gaussian_blur(&im, 5.0f);
    sm = resize_box(&bl, W1, H1);
    img_free(&im); img_free(&bl);
    out = map_new();
    if (!g_err && out)
        for (y = 0; y < H1; y++) for (x = 0; x < W1; x++)
            if (PIX(&sm, x, y) >= 128) {
                int sx = (int)((x + 0.5) / sc), sy = (int)((y + 0.5) / sc), v, qx, qy;
                if (sx > W0 - 1) sx = W0 - 1;
                if (sy > H0 - 1) sy = H0 - 1;
                v = mget(m, sx, sy);
                if (v < 0) {
                    long best = -1;
                    MFOR(m, qx, qy) { long d = (long)(qx - sx) * (qx - sx) + (long)(qy - sy) * (qy - sy); if (best < 0 || d < best) { best = d; v = mget(m, qx, qy); } }
                }
                mset(out, x, y, v);
            }
    img_free(&sm);
    return out;
}
static Map *ca_rot_mask(const Map *pts, double deg) {
    const int k = 8; int x0, y0, W0, H0, x, y, W1, H1; Img im, rot, sm; Map *out, *res;
    mbbox(pts, &x0, &y0, &W0, &H0);
    W0 += 1; H0 += 1;
    im = img_new(W0 * k, H0 * k);
    if (g_err) return 0;
    MFOR(pts, x, y) draw_rectangle(&im, x * k, y * k, x * k + k - 1, y * k + k - 1);
    rot = rotate_expand(&im, deg);
    img_free(&im);
    if (g_err) return 0;
    W1 = rot.w / k; H1 = rot.h / k;
    sm = resize_box(&rot, W1, H1);
    img_free(&rot);
    out = map_new();
    if (!g_err && out) for (y = 0; y < H1; y++) for (x = 0; x < W1; x++) if (PIX(&sm, x, y) >= 128) mset(out, x, y, 1);
    img_free(&sm);
    if (g_err) { map_free(&out); return 0; }
    mbbox(out, &x0, &y0, &W1, &H1);
    res = map_shift(out, -x0, -y0);
    map_free(&out);
    return res;
}
static Map *despike(Map *body) {      /* takes body */
    int pass, x, y;
    for (pass = 0; pass < 2 && body; pass++) {
        Map *o = map_new();
        if (o) MFOR(body, x, y) if (n4(body, x, y) >= 2) mset(o, x, y, 1);
        map_free(&body); body = o;
    }
    return body;
}
static Map *ca_colour(const Map *body, const Map *holes, int fill, int shd) {
    Map *m = map_new(); int x, y;
    if (!m) return 0;
    MFOR(body, x, y) {
        int out4 = 0, hole4 = 0, d;
        static const int dx4[4] = {1, -1, 0, 0}, dy4[4] = {0, 0, 1, -1};
        for (d = 0; d < 4; d++) {
            int qx = x + dx4[d], qy = y + dy4[d];
            if (!mhas(body, qx, qy) && !mhas(holes, qx, qy)) out4 = 1;
            if (mhas(holes, qx, qy)) hole4 = 1;
        }
        if (out4) mset(m, x, y, 9);
        else if (hole4) mset(m, x, y, shd);
        else if (mhas(body, x + 1, y) && !mhas(body, x + 2, y) && !mhas(holes, x + 2, y)) mset(m, x, y, shd);
        else mset(m, x, y, fill);
    }
    if (holes) MFOR(holes, x, y) mset(m, x, y, 2);
    return m;
}
static void ca_orange_streak(Map *m) {
    Map *snap = map_new(); int x, y;
    if (!snap) return;
    memcpy(snap->v, m->v, sizeof m->v);
    MFOR(snap, x, y) if (mget(snap, x, y) == 8 && mget(snap, x - 1, y) == 9) mset(m, x, y, 12);
    map_free(&snap);
}
static Map *ca_big_c(const Map *G) {
    Map *c = map_new(), *up, *mask, *body, *res; int x, y;
    if (!c) return 0;
    MFOR(G, x, y) if (y <= 8) {
        int v = mget(G, x, y); v = v == 6 ? 8 : v == 15 ? 12 : v;
        mset(c, x, y, v); mset(c, x, 17 - y, v);
    }
    up = ca_upscale(c, 1.95);
    map_free(&c);
    if (!up) return 0;
    mask = ca_rot_mask(up, 12);
    map_free(&up);
    if (!mask) return 0;
    body = despike(mask);
    if (!body) return 0;
    res = ca_colour(body, 0, 8, 12);
    map_free(&body);
    if (res) ca_orange_streak(res);
    return res;
}
typedef void (*ca_draw_fn)(Img *);
static Map *ca_ss_mask(ca_draw_fn fn, int w, int h, double shear) {
    Img im = img_new(w * CA_K, h * CA_K), sm; Map *m; int x, y;
    if (g_err) return 0;
    fn(&im);
    if (shear != 0) {
        double a[6]; Img t;
        a[0] = 1; a[1] = shear; a[2] = -shear * h * CA_K; a[3] = 0; a[4] = 1; a[5] = 0;
        t = transform_affine(&im, im.w, im.h, a);
        img_free(&im); im = t;
    }
    sm = resize_box(&im, w, h);
    img_free(&im);
    m = map_new();
    if (!g_err && m) for (y = 0; y < h; y++) for (x = 0; x < w; x++) if (PIX(&sm, x, y) >= 128) mset(m, x, y, 1);
    img_free(&sm);
    return m;
}
static void ca_bez(double *out, double p0x, double p0y, double p1x, double p1y, double p2x, double p2y) {
    int i;
    for (i = 0; i <= 12; i++) {
        double t = i / 12.0, u = 1 - t;
        out[2 * i] = (u * u * p0x + 2 * u * t * p1x + t * t * p2x) * CA_K;
        out[2 * i + 1] = (u * u * p0y + 2 * u * t * p1y + t * t * p2y) * CA_K;
    }
}
static void ca_r_draw(Img *im) {
    double poly[52];
    draw_rectangle(im, 1 * CA_K, 0.5 * CA_K, 6.5 * CA_K, 20 * CA_K);
    draw_rounded_rectangle(im, 1 * CA_K, 0.3 * CA_K, 15.5 * CA_K, 12.2 * CA_K, 5.8 * CA_K);
    ca_bez(poly, 6.5, 10, 11.5, 12, 16.5, 20);
    ca_bez(poly + 26, 11.3, 20, 8.8, 14.6, 6.5, 14);
    draw_polygon(im, 26, poly);
}
static void ca_r_hole(Img *im) { draw_rounded_rectangle(im, 6.3 * CA_K, 4 * CA_K, 11 * CA_K, 8.5 * CA_K, 1.9 * CA_K); }
static Map *ca_big_r(void) {
    Map *body = ca_ss_mask(ca_r_draw, 18, 21, -0.12), *hh = ca_ss_mask(ca_r_hole, 18, 21, -0.12), *r, *res; int x, y, x0, y0, x1, y1;
    if (!body || !hh) { map_free(&body); map_free(&hh); return 0; }
    MFOR(hh, x, y) mset(body, x, y, -1);
    body = despike(body);
    if (!body) { map_free(&hh); return 0; }
    MFOR(body, x, y) mset(hh, x, y, -1);
    r = ca_colour(body, hh, 7, 14);
    map_free(&body); map_free(&hh);
    if (!r) return 0;
    mbbox(r, &x0, &y0, &x1, &y1);
    res = map_shift(r, -x0, 0);
    map_free(&r);
    return res;
}
static int part_cargando(Out *o, const u8 *raw, size_t n) {
    Key k; Map *O, *A, *D, *N, *G, *row[8] = {0}, *tmp; u16 words[36 * 71];
    static const int base[8] = {-14, 1, 1, 3, 4, 3, 6, 6}, arc[8] = {16, 7, 2, 0, 0, 2, 7, 16};
    int i, x, y, wd[8], sum = 0, ok = 0; signed char can[71][144];
    if (n < 72 * 128) return 0;
    if (!page_key(&k, raw, n, 128, 0, 0, 0, 0, 256, 72)) return 0;
    ca_raw = raw;
    O = ca_grab(32, 45, 0); A = ca_grab(48, 45, 1); D = ca_grab(60, 45, 2); N = ca_grab(80, 45, 0); G = ca_grab(93, 47, 0);
    if (g_err || !O || !A || !D || !N || !G) goto done;
    row[0] = ca_big_c(G);
    tmp = ca_shade(A, 5, 4); row[1] = tmp ? ca_upscale(tmp, 1.18) : 0; map_free(&tmp);
    row[2] = ca_big_r();
    row[3] = G; row[4] = ca_shade(A, 1, 9); row[5] = N; row[6] = D; row[7] = O;
    for (i = 0; i < 8; i++) { int x0, y0, y1; if (!row[i]) goto done; mbbox(row[i], &x0, &y0, &wd[i], &y1); wd[i] += 1; sum += wd[i]; }
    if (g_err) goto done;
    memset(can, 0, sizeof can);
    {   int ox[8], oy[8], xx = py_floordiv(144 - (sum + -3 * 7), 2), it;
        for (i = 0; i < 8; i++) { ox[i] = xx; oy[i] = 12 + base[i] + arc[i]; xx += wd[i] + -3; }
        for (it = 7; it >= 0; it--) {          /* back to front */
            const Map *m = row[it]; int kk, dx, dy;
            MFOR(m, x, y) for (kk = 0; kk <= 3; kk++) for (dx = -1; dx <= 1; dx++) for (dy = -1; dy <= 1; dy++) {
                int cx = ox[it] + x + dx - py_floordiv(kk + 1, 3), cy = oy[it] + y + dy + kk;
                if (cx >= 0 && cx < 144 && cy >= 0 && cy < 71) can[cy][cx] = 2;
            }
            MFOR(m, x, y) { int cx = ox[it] + x, cy = oy[it] + y; if (cx >= 0 && cx < 144 && cy >= 0 && cy < 71) can[cy][cx] = (signed char)mget(m, x, y); }
        }
    }
    for (y = 0; y < 71; y++) for (x = 0; x < 144; x += 4)
        words[y * 36 + x / 4] = (u16)(can[y][x] | can[y][x + 1] << 4 | can[y][x + 2] << 8 | can[y][x + 3] << 12);
    if (g_err) goto done;
    out_rect(o, 0, 72, 36, 71, words, &k);
    ok = 1;
done:
    map_free(&row[0]); map_free(&row[1]); map_free(&row[2]); map_free(&row[4]);
    map_free(&O); map_free(&A); map_free(&D); map_free(&N); map_free(&G);
    return ok;
}

/* ================================================================== VUELVE A INTENTARLO (tryagain.py) */
#define TA_S 8
static const u8 *ta_page;
static int ta_px(int x, int y) { return (ta_page[y * 128 + (x >> 1)] >> (4 * (x & 1))) & 15; }
static int ta_hl(int v) { return v == 13 || v == 15; }
enum { F_RED, F_YELLOW, F_ORANGE, F_PINK, F_GREEN, F_BLUE };
static const int TA_FAM[6][2] = {{8, 9}, {5, 4}, {4, 14}, {1, 10}, {6, 11}, {7, 12}};
/* comps(): 8-connected components of the non-zero pixels of a rect, in discovery order (maps of value 1) */
static int ta_comps(int x0, int y0, int w, int h, Map **out, int max) {
    Map *seen = map_new(); int n = 0, x, y, *st = (int *)malloc(sizeof(int) * 2 * (w * h * 9 + 8));
    if (!seen || !st) { free(st); map_free(&seen); g_err = 1; return 0; }
    for (y = y0; y < y0 + h; y++) for (x = x0; x < x0 + w; x++) {
        int sp = 0; Map *c;
        if (!ta_px(x, y) || mhas(seen, x, y)) continue;
        if (n >= max || !(c = map_new())) { g_err = 1; break; }
        st[sp++] = x; st[sp++] = y; mset(seen, x, y, 1);
        while (sp) {
            int b = st[--sp], a = st[--sp], dx, dy;
            mset(c, a, b, 1);
            for (dx = -1; dx <= 1; dx++) for (dy = -1; dy <= 1; dy++) {
                int px = a + dx, py = b + dy;
                if (px >= x0 && px < x0 + w && py >= y0 && py < y0 + h && !mhas(seen, px, py) && ta_px(px, py)) { mset(seen, px, py, 1); st[sp++] = px; st[sp++] = py; }
            }
        }
        out[n++] = c;
    }
    free(st); map_free(&seen);
    return n;
}
/* cut(c, keep) with y made relative to `top`: keep 0 all, 1 T rule, 2 R rule */
static Map *ta_cut(const Map *c, int keep, int top) {
    Map *m = map_new(), *res; int x, y, x0 = 1 << 20;
    if (!m) return 0;
    MFOR(c, x, y) {
        int v = ta_px(x, y), t = v == 8 || v == 9 || (ta_hl(v) && x <= 198);
        if (keep == 0 || (keep == 1 && t) || (keep == 2 && !t)) { mset(m, x, y, v); if (x < x0) x0 = x; }
    }
    if (x0 == 1 << 20) { g_err = 1; map_free(&m); return 0; }
    res = map_shift(m, -x0, -top);
    map_free(&m);
    return res;
}
static int ta_count(const Map *m) { int x, y, n = 0; MFOR(m, x, y) n++; return n; }
static int ta_minx(const Map *m) { int x, y, b = 1 << 20; MFOR(m, x, y) if (x < b) b = x; return b; }
static Map *ta_recolour(const Map *m, int fam) {
    int cnt[16] = {0}, x, y, fill = -1, c, f = TA_FAM[fam][0], s = TA_FAM[fam][1], hl = fam == F_BLUE ? 15 : 13; Map *o;
    MFOR(m, x, y) { int v = mget(m, x, y); if (!ta_hl(v)) cnt[v]++; }
    for (c = 0; c < 16; c++) if (cnt[c] && (fill < 0 || cnt[c] > cnt[fill])) fill = c;
    if (fill < 0) { g_err = 1; return 0; }
    o = map_new();
    if (o) MFOR(m, x, y) { int v = mget(m, x, y); mset(o, x, y, ta_hl(v) ? hl : v == fill ? f : s); }
    return o;
}
typedef struct { const double *pts; int n; } TaStroke;
typedef struct { const TaStroke *st; int nst, w; } TaGlyph;
static const double TA_V0[] = {0, 0, 1.2, 4.5, 3.6, 9.4}, TA_V1[] = {3.6, 9.4, 5.6, 4.8, 7.4, 0.2};
static const double TA_U0[] = {0, 0, 0.1, 6.8, 1.6, 9.4, 3.8, 9.8, 5.8, 8.8, 6.5, 6, 6.6, 0};
static const double TA_E0[] = {5.8, 0.2, 0.4, 0, 0, 4.6, 0.2, 9.6, 6, 9.8}, TA_E1[] = {0.2, 4.8, 4.6, 4.6};
static const double TA_L0[] = {0.2, 0, 0, 5, 0.4, 9.6, 5.6, 9.8};
static const double TA_O0[] = {3.6, 0, 6.4, 1.2, 7.2, 5, 6.2, 8.8, 3.4, 9.9, 0.8, 8.6, 0, 4.6, 1.0, 1.0, 3.6, 0};
static const TaStroke TA_SV[] = {{TA_V0, 3}, {TA_V1, 3}}, TA_SU[] = {{TA_U0, 7}}, TA_SE[] = {{TA_E0, 5}, {TA_E1, 2}},
                      TA_SL[] = {{TA_L0, 4}}, TA_SO[] = {{TA_O0, 9}};
static const TaGlyph *ta_glyph(char ch) {
    static const TaGlyph V = {TA_SV, 2, 8}, U = {TA_SU, 1, 7}, E = {TA_SE, 2, 6}, L = {TA_SL, 1, 6}, O = {TA_SO, 1, 7};
    return ch == 'V' ? &V : ch == 'U' ? &U : ch == 'E' ? &E : ch == 'L' ? &L : &O;
}
/* render(): the body as a CPython set (its iteration order drives colour()'s random draws) */
static int ta_render(const TaGlyph *g, double k, int h, double rot, int seed, double wid, PySet *res) {
    int w = g->w, W = (w + 6) * TA_S, H = (h + 6) * TA_S, s, i, kk, x, y, pass;
    double cx = w / 2.0, cy = h / 2.0, ca = g_cos(rot * (M_PI / 180.0)), sa = g_sin(rot * (M_PI / 180.0));
    PyRand rnd; Img im, small; PySet m; Map *mm;
    pr_seed(&rnd, (u32)seed);
    im = img_new(W, H);
    if (g_err) return 0;
    for (s = 0; s < g->nst; s++) {
        double sw = wid * pr_uniform(&rnd, .9, 1.12), pts[32];
        for (i = 0; i < g->st[s].n; i++) {
            double px = g->st[s].pts[2 * i], py = g->st[s].pts[2 * i + 1] * k, tx, ty;
            tx = px + pr_uniform(&rnd, -.35, .35) - cx;
            ty = py + pr_uniform(&rnd, -.35, .35) - cy;
            pts[2 * i] = (tx * ca - ty * sa + cx + 3) * TA_S;
            pts[2 * i + 1] = (tx * sa + ty * ca + cy + 3) * TA_S;
        }
        for (i = 0; i + 1 < g->st[s].n; i++) {
            double ax = pts[2 * i], ay = pts[2 * i + 1], bx = pts[2 * i + 2], by = pts[2 * i + 3];
            int n = (int)(g_sqrt((ax - bx) * (ax - bx) + (ay - by) * (ay - by)) / TA_S * 2);
            if (n < 2) n = 2;
            for (kk = 0; kk <= n; kk++) {
                double t = (double)kk / n, ex = ax + (bx - ax) * t, ey = ay + (by - ay) * t;
                double r = sw * TA_S / 2 * (1 + .12 * g_sin(t * 5 + seed));
                draw_ellipse(&im, ex - r, ey - r, ex + r, ey + r);
            }
        }
    }
    small = resize_box(&im, w + 6, h + 6);
    img_free(&im);
    if (g_err) return 0;
    ps_init(&m); mm = map_new();
    for (y = 0; y < h + 6; y++) for (x = 0; x < w + 6; x++) if (PIX(&small, x, y) >= 128) { ps_add(&m, x, y); mset(mm, x, y, 1); }
    img_free(&small);
    for (pass = 0; pass < 2 && !g_err; pass++) {
        PySet n2, add; Map *nm = map_new(); uint64_t e; int x0 = 1 << 20, x1 = -(1 << 20), y0 = 1 << 20, y1 = -(1 << 20);
        ps_init(&n2);
        for (e = 0; e <= m.mask && nm; e++) if (m.t[e].used && n4(mm, m.t[e].x, m.t[e].y) >= 2) { ps_add(&n2, m.t[e].x, m.t[e].y); mset(nm, m.t[e].x, m.t[e].y, 1); }
        ps_free(&m); map_free(&mm); m = n2; mm = nm;
        if (!m.used) { g_err = 1; break; }
        for (e = 0; e <= m.mask; e++) if (m.t[e].used) {
            if (m.t[e].x < x0) x0 = m.t[e].x;
            if (m.t[e].x > x1) x1 = m.t[e].x;
            if (m.t[e].y < y0) y0 = m.t[e].y;
            if (m.t[e].y > y1) y1 = m.t[e].y;
        }
        ps_init(&add);
        for (x = x0; x <= x1; x++) for (y = y0; y <= y1; y++) if (!mhas(mm, x, y) && n4(mm, x, y) >= 4) ps_add(&add, x, y);
        ps_merge(&m, &add);
        for (e = 0; e <= add.mask; e++) if (add.t[e].used) mset(mm, add.t[e].x, add.t[e].y, 1);
        ps_free(&add);
    }
    if (!g_err) {
        uint64_t e; int x0 = 1 << 20, y0 = 1 << 20;
        for (e = 0; e <= m.mask; e++) if (m.t[e].used) { if (m.t[e].x < x0) x0 = m.t[e].x; if (m.t[e].y < y0) y0 = m.t[e].y; }
        ps_init(res);
        for (e = 0; e <= m.mask; e++) if (m.t[e].used) ps_add(res, m.t[e].x - x0, m.t[e].y - y0);
    }
    ps_free(&m); map_free(&mm);
    return !g_err;
}
static Map *ta_letter(char ch, int fam, int seed, double rot, int dy, double wid) {
    const TaGlyph *g = ta_glyph(ch); double k = ch == 'E' ? 1 : .85; int h = ch == 'E' ? 10 : py_round(10 * k), x, y, hgt = 0;
    int f = TA_FAM[fam][0], s = TA_FAM[fam][1], hl = fam == F_BLUE ? 15 : 13; uint64_t e;
    PySet body; Map *bm, *m, *res; PyRand rnd;
    if (!ta_render(g, k, h, rot, seed, wid, &body)) return 0;
    bm = map_new(); m = map_new();
    if (!bm || !m) { ps_free(&body); map_free(&bm); map_free(&m); return 0; }
    for (e = 0; e <= body.mask; e++) if (body.t[e].used) { mset(bm, body.t[e].x, body.t[e].y, 1); if (body.t[e].y + 1 > hgt) hgt = body.t[e].y + 1; }
    pr_seed(&rnd, (u32)seed);
    for (e = 0; e <= body.mask; e++) if (body.t[e].used) {
        int bx = body.t[e].x, by = body.t[e].y, v;
        int left = !mhas(bm, bx - 1, by), below = !mhas(bm, bx, by + 1), right = !mhas(bm, bx + 1, by);
        if (left && pr_random(&rnd) < .8) v = s;
        else if (below && (left || right || pr_random(&rnd) < .5)) v = s;
        else v = f;
        mset(m, bx, by, v);
    }
    ps_free(&body);
    for (y = 1; y <= (int)(hgt * .55); y++) {
        int xs[4], nx = 0;
        for (x = -MOFF; x < MAPN - MOFF && nx < 4; x++) if (mhas(bm, x, y)) xs[nx++] = x;
        if (nx >= 3 && xs[1] == xs[0] + 1 && xs[2] == xs[0] + 2) {
            mset(m, xs[0] + 1, y, hl);
            if (y == 2 && nx >= 4 && xs[3] == xs[0] + 3) mset(m, xs[0] + 2, y, hl);
        }
    }
    map_free(&bm);
    res = map_shift(m, 0, dy);
    map_free(&m);
    return res;
}
/* kern(): letters left to right, each as far left as `gap` empty pixels to what is placed allow (0 = the space) */
static Map *ta_kern(Map *const *row, int n, int gap, int space, int dy0, int dy1) {
    Map *out = map_new(), *occ = map_new(); int i, x = 0, sp = 0, any = 0, maxx = 0, a, b, dx, dy;
    if (!out || !occ) { map_free(&out); map_free(&occ); return 0; }
    for (i = 0; i < n && !g_err; i++) {
        const Map *m = row[i]; int ox, lo, hit;
        if (!m) { sp = maxx + 1 + space; continue; }
        memset(occ->v, -1, sizeof occ->v);
        MFOR(out, a, b) for (dx = -gap; dx <= gap; dx++) for (dy = dy0; dy <= dy1; dy++) mset(occ, a + dx, b + dy, 1);
        lo = any ? maxx - 12 : 0;
        ox = lo; if (x - 6 > ox) ox = x - 6; if (sp > ox) ox = sp;
        do {
            hit = 0;
            MFOR(m, a, b) if (mhas(occ, a + ox, b)) { hit = 1; break; }
            if (hit) ox++;
        } while (hit && !g_err && ox < MAPN);
        MFOR(m, a, b) { mset(out, a + ox, b, mget(m, a, b)); if (!any || a + ox > maxx) maxx = a + ox; any = 1; }
        x = ox + 1;
    }
    map_free(&occ);
    return out;
}
static int part_tryagain(Out *o, const u8 *page, size_t n) {
    Key k; Map *t1[16], *t2[16], *L[8] = {0}, *l1[8] = {0}, *l2[10] = {0}, *r1 = 0, *r2 = 0;
    int n1 = 0, n2 = 0, i, ok = 0, big = 0, W, x, y, x0, y1; u16 *words = 0; u8 g[32][256];
    enum { LT, LR, LY, LA1, LG, LA2, LI, LN };
    if (n < 32768) return 0;
    if (!page_key(&k, page, n, 128, 256, 256, 184, 192, 56, 32)) return 0;
    ta_page = page;
    n1 = ta_comps(184, 192, 40, 16, t1, 16); n2 = ta_comps(184, 208, 56, 16, t2, 16);
    if (g_err || n1 < 2 || n2 != 5) goto done;
    for (i = 1; i < n1; i++) if (ta_count(t1[i]) > ta_count(t1[big])) big = i;
    L[LT] = ta_cut(t1[big], 1, 192); L[LR] = ta_cut(t1[big], 2, 192);
    L[LY] = ta_cut(t1[big == 0 ? 1 : 0], 0, 192);
    {   int ord[5] = {0, 1, 2, 3, 4}, a, b;      /* sorted by min x (stable) */
        for (a = 1; a < 5; a++) for (b = a; b > 0 && ta_minx(t2[ord[b - 1]]) > ta_minx(t2[ord[b]]); b--) { int t = ord[b]; ord[b] = ord[b - 1]; ord[b - 1] = t; }
        for (a = 0; a < 5; a++) L[LA1 + a] = ta_cut(t2[ord[a]], 0, 208);
    }
    if (g_err) goto done;
    l1[0] = ta_letter('V', F_PINK, 3, -6, 1, 3.7); l1[1] = ta_letter('U', F_GREEN, 7, 5, 2, 3.7);
    l1[2] = ta_letter('E', F_YELLOW, 11, -4, 1, 3.0); l1[3] = ta_letter('L', F_ORANGE, 5, 7, 2, 3.7);
    l1[4] = ta_letter('V', F_BLUE, 9, 4, 1, 3.7); l1[5] = ta_letter('E', F_RED, 13, -6, 2, 3.0); l1[6] = 0;
    l1[7] = ta_recolour(L[LA1], F_PINK);
    l2[0] = L[LI]; l2[1] = L[LN]; l2[2] = L[LT]; l2[3] = ta_letter('E', F_YELLOW, 21, 5, 2, 3.0);
    l2[4] = ta_recolour(L[LN], F_GREEN); l2[5] = ta_recolour(L[LT], F_PINK); l2[6] = L[LA2];
    l2[7] = ta_recolour(L[LR], F_RED); l2[8] = ta_letter('L', F_BLUE, 23, -5, 2, 3.7); l2[9] = ta_letter('O', F_ORANGE, 29, 8, 2, 3.7);
    if (g_err) goto done;
    for (i = 0; i < 8; i++) if (i != 6 && !l1[i]) goto done;
    for (i = 0; i < 10; i++) if (!l2[i]) goto done;
    r1 = ta_kern(l1, 8, 1, 5, -1, 1); r2 = ta_kern(l2, 10, 1, 5, 0, 0);
    if (g_err || !r1 || !r2) goto done;
    {   int w1, w2, t1y, t2y, rr;
        mbbox(r1, &x0, &t1y, &w1, &y1); mbbox(r2, &x0, &t2y, &w2, &y1);
        w1 += 1; w2 += 1;
        W = (w1 > w2 ? w1 : w2) + 2 + 3; W = py_floordiv(W, 4) * 4; if (W < 112) W = 112;
        if (W > 256) goto done;
        memset(g, 0, sizeof g);
        for (rr = 0; rr < 2; rr++) {
            const Map *r = rr ? r2 : r1; int yo = rr ? 17 - t2y : 1 - t1y, xo = py_floordiv(W - (rr ? w2 : w1), 2);
            MFOR(r, x, y) {
                int gy = y + yo, gx = x + xo;
                if (gy >= 32 || gx >= W) { g_err = 1; goto done; }        /* (IndexError) */
                if (gy < 0) gy += 32;                                      /* (Python's negative index) */
                if (gx < 0) gx += W;
                if (gy < 0 || gx < 0) { g_err = 1; goto done; }
                g[gy][gx] = (u8)mget(r, x, y);
            }
        }
    }
    words = (u16 *)malloc(sizeof(u16) * (W / 4) * 32);
    if (!words) goto done;
    for (y = 0; y < 32; y++) for (x = 0; x < W / 4; x++)
        words[y * (W / 4) + x] = (u16)(g[y][x * 4] | g[y][x * 4 + 1] << 4 | g[y][x * 4 + 2] << 8 | g[y][x * 4 + 3] << 12);
    out_rect(o, 256 + 92 / 4, 256 + 223, W / 4, 32, words, &k);
    ok = 1;
done:
    free(words); map_free(&r1); map_free(&r2);
    for (i = 0; i < 8; i++) map_free(&l1[i]);
    map_free(&l2[3]); map_free(&l2[4]); map_free(&l2[5]); map_free(&l2[7]); map_free(&l2[8]); map_free(&l2[9]);
    for (i = 0; i < 8; i++) map_free(&L[i]);
    for (i = 0; i < n1; i++) map_free(&t1[i]);
    for (i = 0; i < n2; i++) map_free(&t2[i]);
    return ok;
}

/* ================================================================== build */
/* `GAM\0`, u32 size, then 16-bit flag words (bit 0 first; 1 = copy: back offset byte, length byte), as area000.gam_unpack */
static u8 *gam_unpack(const u8 *d, size_t n, size_t *out_n) {
    size_t size, len = 0, p = 8, cap; u8 *out; unsigned flags = 0; int left = 0;
    if (n < 8 || memcmp(d, "GAM", 4)) return 0;
    size = d[4] | d[5] << 8 | d[6] << 16 | (size_t)d[7] << 24;
    cap = size + 600;
    if (size > 0x1000000 || !(out = (u8 *)malloc(cap))) return 0;
    while (len < size) {
        int bit;
        if (left == 0) { if (p + 1 >= n) goto bad; flags = d[p] | d[p + 1] << 8; p += 2; left = 16; }
        bit = flags & 1; flags >>= 1; left--;
        if (bit) {
            unsigned off, ln, i;
            if (p + 1 >= n) goto bad;
            off = d[p]; ln = d[p + 1]; p += 2;
            for (i = 0; i < ln; i++) {
                if (len >= cap || (off ? off > len : len == 0)) goto bad;
                out[len] = out[off ? len - off : 0]; len++;
            }
        } else {
            if (p >= n || len >= cap) goto bad;
            out[len++] = d[p++];
        }
    }
    *out_n = size;
    return out;
bad:
    free(out);
    return 0;
}

typedef u8 *(*gfx_es_reader)(const char *name, unsigned *size);
typedef void (*gfx_es_logger)(const char *msg);
/* the TBTK image (tools/gfx_es build()): malloc'd, *count rects; 0 when no part built */
u8 *gfx_es_build(gfx_es_reader read, gfx_es_logger log, size_t *len, int *count) {
    Out o; static const char *const labels[3] = {"VUELVE A INTENTARLO", "CARGAR / GUARDAR", "CARGANDO"};
    int part; char msg[160];
    memset(&o, 0, sizeof o);
    out_put(&o, "TBTK\0\0\0\0", 8);
    for (part = 0; part < 3; part++) {
        unsigned sz = 0; u8 *d = 0, *page = 0; size_t pn = 0; size_t mark = o.n; int cnt = o.count, got = 0;
        g_err = 0;
        if (part == 0) {
            if ((d = read("SYSTEM/A10003", &sz))) { page = d; pn = sz; }
            else if ((d = read("SYSTEM/A10003.GAM", &sz))) { page = gam_unpack(d, sz, &pn); free(d); d = page; }
            else if ((d = read("SYSTEM/A10003.000", &sz))) { page = d; pn = sz; }
        } else if ((d = read(part == 1 ? "SYSTEM/A00400.000" : "SYSTEM/A00900.000", &sz))) { page = d; pn = sz; }
        if (page) got = part == 0 ? part_tryagain(&o, page, pn) : part == 1 ? part_cargar_guardar(&o, page, pn) : part_cargando(&o, page, pn);
        free(d);
        if (!got || g_err) {
            o.n = mark; o.count = cnt;
            if (log) { snprintf(msg, sizeof msg, "gfx_es: %s skipped (%s)", labels[part], page ? "unexpected data" : "source file missing"); log(msg); }
        }
    }
    g_err = 0;
    if (!o.count || !o.b) { free(o.b); *len = 0; *count = 0; return 0; }
    o.b[4] = (u8)o.count; o.b[5] = (u8)(o.count >> 8); o.b[6] = 0; o.b[7] = 0;
    *len = o.n; *count = o.count;
    return o.b;
}

#ifdef GFX_ES_MAIN
/* standalone build for the check in the header comment: gfx_es DATA_DIR OUT.tbtx */
static const char *g_dir;
static u8 *dir_read(const char *name, unsigned *size) {
    char path[2048]; FILE *f; long n; u8 *d;
    snprintf(path, sizeof path, "%s/%s", g_dir, name);
    if (!(f = fopen(path, "rb"))) return 0;
    fseek(f, 0, SEEK_END); n = ftell(f); fseek(f, 0, SEEK_SET);
    d = (u8 *)malloc((size_t)n + 4);
    if (!d || fread(d, 1, (size_t)n, f) != (size_t)n) { free(d); fclose(f); return 0; }
    fclose(f); *size = (unsigned)n;
    return d;
}
static void stderr_log(const char *m) { fprintf(stderr, "%s\n", m); }
int main(int argc, char **argv) {
    size_t n; int count; u8 *b; FILE *f;
    if (argc != 3) { fprintf(stderr, "usage: %s DATA_DIR OUT.tbtx\n", argv[0]); return 2; }
    g_dir = argv[1];
    b = gfx_es_build(dir_read, stderr_log, &n, &count);
    if (!b) { printf("0 rects\n"); return 1; }
    if (!(f = fopen(argv[2], "wb")) || fwrite(b, 1, n, f) != n) { perror(argv[2]); return 1; }
    fclose(f);
    printf("%d rects\n", count);
    return 0;
}
#else
/* ------------------------------------------------------------------ in the port */
extern unsigned char *port_cd_read_named(const char *path, unsigned *size);
extern void port_printf(const char *fmt, ...);
extern int port_tex_patch_add_mem(const char *name, unsigned char *buf, unsigned n);
static void port_log(const char *m) { port_printf("[gfx_es] %s\n", m); }
/* --lang 5: build the redraws from the disc data now (a few ms) and hand them to the --patch-tex loader */
void port_gfx_es_load(void) {
    size_t n; int count; u8 *b = gfx_es_build(port_cd_read_named, port_log, &n, &count);
    if (!b) { port_printf("[gfx_es] no redraws built: the original English sprites stay\n"); return; }
    if (port_tex_patch_add_mem("gfx_es (built from the disc data)", b, (unsigned)n) < 0) free(b);
}
#endif
