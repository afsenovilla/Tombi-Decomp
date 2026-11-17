/* runtime/ws_backdrop.c: AREA09 s2, the Haunted Mansion's grandfather clock room (included there; docs/PORT.md "More
 * rooms with picture strips").
 *
 * The room is one 480 x 288 picture, CLUT (128, 511): rows 0-255 from pages (128, 0) / (256, 0) (256 + 224 columns),
 * rows 256-287 from page (384, 0) (rows 0-31 and 32-63), four sprites moved by the camera: 160 columns sideways
 * (frame column 0 to -160) and 64 rows (frame row -48 at rest, up to 16).  At 16:9 the picture covers the wide frame
 * only in the middle third of the camera's range; at each end up to 53 columns are missing (nothing is culled: the
 * picture ends).  Clock, table, chairs, chandeliers, flames and the gem are objects drawn over it.
 *
 * What the picture's ends are: not the back wall with its ornaments, but the room's two side walls in perspective.
 * Measured (the picture is mirror-symmetric in its geometry about column 239.5):
 *   - every moulding of the side walls runs to one vanishing point, column 239.5, row 184.25 (texel centres; five
 *     lines fitted per side meet within 0.4 rows);
 *   - in the wall's own space (1 / distance from the vanishing column, ray) the left wainscot repeats every
 *     0.000820 per column (stiles at columns 12 and 48; the next one 40 columns out of the picture);
 *   - the vault meets each side wall along an arch that is an ellipse in that space (apex at column 24.5; fitted
 *     to 0.2 rows, and its nearer half is seen in the rib's other edge, the same curve at 0.923 of the distance);
 *     it would come down on the cornice 96 columns out;
 *   - the floor tiles' joints run to the vanishing point, 0.476 columns apart per row under the horizon.
 * The right wall is not the left one's mirror: a door stands in it, with a lamp, a narrow panel before it and a long
 * one after it that the edge cuts 18 columns in; its wainscot shows no stile after the door.
 *
 * The strips (made once per picture, ck_gen; ck_take lays the frame's strips from them and the picture's columns):
 *   - left wall, cornice to floor: the wall itself one period deeper (ck_deep: the picture seen through the
 *     perspective of a wall moved along its length), so the wainscot goes on with its period and phase (the cut
 *     panel is closed by its stile and the next one starts), the mouldings stay on their rays, the big plaster panel
 *     runs on (the picture never shows where it ends);
 *   - right wall: the cornice and the plaster over it the same way; from the cornice down the door is in the way,
 *     so the wall goes on along its rays with the tone of its last columns (ck_smear), the plaster's grain from
 *     the left panel and the wainscot's wood from the one whole panel of that wall; its panels run on;
 *   - the wall arch goes on along its ellipse, the rib 2.9 rows thick as at the edge, the ceiling over it the
 *     picture's own 72 columns further in; past its end (wider than 16:9) the ceiling comes down to the cornice;
 *   - the floor is the same row, whole tiles further in;
 *   - the join's tone (per ray, the picture's first 6 columns less the strip's) fades out, halved every 32 columns.
 * Nothing is mirrored; clock, table, chandeliers, fireplace and door are not in the strips.  Every colour is the
 * nearest of the picture's CLUT.  Integers only; a texel depends on its place, not on the strip's width. */
#define CK_HM 1226                                        /* the ROOMS row's mark (no picture has this height) */
#define CK_W 480
#define CK_H 288
static void gen_ck(int pad) { (void)pad; }               /* (the table's gen: ck_take makes the strips) */
static u16 ck_src[CK_H][CK_W];
static u16 ck_clut[256]; static u16 ck_qc[32768]; static u8 ck_qh[32768];
static u16 *ck_ext; static int ck_side;
typedef long long ck_i64;

#define CK_VY16 12075008                                  /* the vanishing point's row, 184.25 (16.16); its column is 239.5 */
#define CK_P2 2439                                        /* the walls' period: 0.000820 per column -> 1219.5 columns, x 2 */
#define CK_UST 4396000                                    /* the wainscot stile next to each edge: 1e6 / its distance */
#define CK_UT 820000                                      /* the period in those units */
#define CK_V(v1000) ((v1000) * 65536 / 1000)              /* a ray: rows per column from the vanishing point (16.16) */
#define CK_VCT CK_V(-421)                                 /* the cornice's top */
#define CK_VCB CK_V(-299)                                 /* its underside (the wall's first row) */
#define CK_VRT CK_V(158)                                  /* the chair rail's top */
#define CK_VFL CK_V(379)                                  /* the wall's foot on the floor */
#define CK_TILE 487                                       /* the floor tiles: 0.476 columns per row under the horizon (x 1024) */
/* the wall arch (where the vault meets the side wall): an ellipse in the wall's own space (1e6 / distance, ray) */
#define CK_AUC 4651000
#define CK_AA 3110400
#define CK_AVC 85007
#define CK_AB 133602
#define CK_AW 1675000                                     /* half its chord at the cornice's top */
#define CK_RIB 190054                                     /* the rib along it: 2.9 rows at the edge (16.16) */
#define CK_TY 20                                          /* the join's tone: from this row (the wall under the arch) */
#define CK_TF 32                                          /* ... halved this many columns out */
#define CK_CS 72                                          /* the ceiling: the picture's, this many columns further in */
#define CK_LAM 923                                        /* the rib's other edge: the same curve at 0.923 of the distance */

static u16 ck_quant(Rgb c)
{
    int r = clampi(c.r + 4, 0, 255) >> 3, g = clampi(c.g + 4, 0, 255) >> 3, b = clampi(c.b + 4, 0, 255) >> 3, k = r | g << 5 | b << 10;
    if (!ck_qh[k]) {
        int bd = 1 << 30, best = k;
        for (int i = 0; i < 256; i++) {
            int q = ck_clut[i] & 0x7FFF, dr, dg, db, d;
            if (!q) continue;
            dr = r5((u16)q) - r; dg = g5((u16)q) - g; db = b5((u16)q) - b; d = 3 * dr * dr + 4 * dg * dg + 2 * db * db;
            if (d < bd) { bd = d; best = q; }
        }
        ck_qc[k] = (u16)best; ck_qh[k] = 1;
    }
    return ck_qc[k];
}
/* the picture's texel at column m counted from the side's edge inwards, row y */
static Rgb ck_t(int m, int y) { m = clampi(m, 0, CK_W - 1); return rgb15(ck_src[clampi(y, 0, CK_H - 1)][ck_side ? CK_W - 1 - m : m]); }
static Rgb ck_bil(int m16, int y16)
{
    int m, y, fx, fy; Rgb a, b, c, d, o;
    m16 = clampi(m16, 0, (CK_W - 1) << 16); y16 = clampi(y16, 0, (CK_H - 1) << 16);
    m = m16 >> 16; y = y16 >> 16; fx = (m16 >> 8) & 255; fy = (y16 >> 8) & 255;
    a = ck_t(m, y); b = ck_t(m + 1, y); c = ck_t(m, y + 1); d = ck_t(m + 1, y + 1);
    o.r = ((a.r * (256 - fx) + b.r * fx) * (256 - fy) + (c.r * (256 - fx) + d.r * fx) * fy + 32768) >> 16;
    o.g = ((a.g * (256 - fx) + b.g * fx) * (256 - fy) + (c.g * (256 - fx) + d.g * fx) * fy + 32768) >> 16;
    o.b = ((a.b * (256 - fx) + b.b * fx) * (256 - fy) + (c.b * (256 - fx) + d.b * fx) * fy + 32768) >> 16;
    return o;
}
static ck_i64 ck_fdiv(ck_i64 a, ck_i64 b) { ck_i64 q = a / b; if ((a % b != 0) && ((a < 0) != (b < 0))) q--; return q; }
static unsigned ck_isqrt(unsigned long long v)
{
    unsigned long long r = 0, b = 1ull << 62;
    while (b > v) b >>= 2;
    while (b) { if (v >= r + b) { v -= r + b; r = (r >> 1) + b; } else r >>= 1; b >>= 2; }
    return (unsigned)r;
}
/* the wall arch's row (16.16) at d16 columns (16.16) from the vanishing point's column; past its near end (where it
   comes down on the cornice) the cornice's top */
static int ck_arch(ck_i64 d16)
{
    ck_i64 U = ((ck_i64)1000000000 << 16) / d16, q, v; unsigned r;
    if (U <= CK_AUC - CK_AW) return (int)(CK_VY16 + (((ck_i64)CK_VCT * d16) >> 16));
    q = ((U - CK_AUC) << 16) / CK_AA;
    r = ck_isqrt((unsigned long long)(((ck_i64)1 << 32) - q * q));
    v = CK_AVC - (((ck_i64)CK_AB * r) >> 16);
    return (int)(CK_VY16 + ((v * d16) >> 16));
}
/* the wall n periods deeper than the point d2 half columns from the vanishing column on row y16 */
static Rgb ck_deep(int d2, int y16, int n)
{
    ck_i64 s = ((ck_i64)CK_P2 << 16) / (CK_P2 + n * d2);
    int m16 = (int)(((ck_i64)479 << 15) - (((ck_i64)d2 * s) >> 1));
    return ck_bil(m16, (int)(CK_VY16 + (((ck_i64)(y16 - CK_VY16) * s) >> 16)));
}
static int ck_deep_n(int d2)                             /* the fewest periods that land in the picture */
{
    int n = 1;
    while ((ck_i64)d2 * CK_P2 > (ck_i64)476 * (CK_P2 + n * d2)) n++;
    return n;
}
/* the right wall between the cornice and the chair rail: the edge's own tone per ray (the door and its lamp are in
   the way further in), with the plaster's grain from the left wall's panel */
static Rgb ck_prof[CK_H];
static void ck_profile(void)
{
    static Rgb raw[CK_H];
    for (int y = 0; y < CK_H; y++) {
        int r = 0, g = 0, b = 0;
        for (int m = 1; m <= 6; m++) {
            Rgb c = ck_bil(m << 16, (int)(CK_VY16 + ck_fdiv((ck_i64)((y << 16) - CK_VY16) * (479 - 2 * m), 479)));
            r += c.r; g += c.g; b += c.b;
        }
        raw[y].r = r / 6; raw[y].g = g / 6; raw[y].b = b / 6;
    }
    for (int y = 0; y < CK_H; y++) {                     /* smoothed along the wall's height, not across an edge */
        int r = 0, g = 0, b = 0, n = 0, l0 = lum(raw[y]);
        for (int k = -4; k <= 4; k++) {
            int yy = clampi(y + k, 0, CK_H - 1), dl = lum(raw[yy]) - l0;
            if (dl > 10 || dl < -10) continue;
            r += raw[yy].r; g += raw[yy].g; b += raw[yy].b; n++;
        }
        ck_prof[y].r = r / n; ck_prof[y].g = g / n; ck_prof[y].b = b / n;
    }
}
static Rgb ck_grain(int X, int v16)                      /* the left panel's plaster less its mean round the texel */
{
    int x = 3 + (X - 1) % 60, vl = CK_V(-220) + (int)((((ck_i64)v16 - CK_VCB) % CK_V(320) + CK_V(320)) % CK_V(320));
    int y = (int)((CK_VY16 + (((ck_i64)vl * (479 - 2 * x)) >> 1) + 32768) >> 16), r = 0, g = 0, b = 0; Rgb c, o;
    for (int j = -2; j <= 2; j++) for (int i = -2; i <= 2; i++) { c = rgb15(ck_src[clampi(y + j, 0, CK_H - 1)][clampi(x + i, 0, CK_W - 1)]); r += c.r; g += c.g; b += c.b; }
    c = rgb15(ck_src[clampi(y, 0, CK_H - 1)][x]);
    o.r = c.r - r / 25; o.g = c.g - g / 25; o.b = c.b - b / 25;
    return o;
}
/* the right wainscot's wood: the corner panel's field (the one whole panel of that wall) less its mean along the ray */
static Rgb ck_wood(int d2, int v16)
{
    int U = 2000000000 / d2, r = 0, g = 0, b = 0; Rgb c = { 0, 0, 0 }, o;
    U = 5420000 + (5950000 - U) % 530000;
    for (int k = 0; k <= 8; k++) {
        int Uk = k < 8 ? 5420000 + k * 66250 + 33125 : U, d16 = (int)((((ck_i64)1000000000) << 16) / Uk);
        c = ck_bil((479 << 15) - d16, (int)(CK_VY16 + (((ck_i64)v16 * d16) >> 16)));
        if (k < 8) { r += c.r; g += c.g; b += c.b; }
    }
    o.r = c.r - r / 8; o.g = c.g - g / 8; o.b = c.b - b / 8;
    return o;
}
static Rgb ck_smear(int X, int d2, int y16, int v16)
{
    ck_i64 y0 = CK_VY16 + ck_fdiv((ck_i64)(y16 - CK_VY16) * 479, d2);
    int y = (int)(y0 >> 16), f = (int)(y0 >> 8) & 255; Rgb a = ck_prof[clampi(y, 0, CK_H - 1)], b = ck_prof[clampi(y + 1, 0, CK_H - 1)], g = { 0, 0, 0 }, o;
    if (v16 < CK_VRT) g = ck_grain(X, v16);
    else if (v16 > CK_V(225) && v16 < CK_V(318)) g = ck_wood(d2, v16);
    o.r = a.r + (b.r - a.r) * f / 256 + g.r; o.g = a.g + (b.g - a.g) * f / 256 + g.g; o.b = a.b + (b.b - a.b) * f / 256 + g.b;
    return o;
}
/* the side wall and the floor X columns out of the picture's edge on row y16 (no vault) */
static Rgb ck_wall(int X, int y16)
{
    int d2 = 479 + 2 * X, v16 = (int)ck_fdiv((ck_i64)(y16 - CK_VY16) * 2, d2), n = ck_deep_n(d2);
    Rgb c;
    if (ck_side && v16 >= CK_VCB && v16 <= CK_VFL + CK_V(12)) c = ck_smear(X, d2, y16, v16);
    else {
        int k = 1;
        c = ck_deep(d2, y16, n);
        while (k <= 8 && ck_deep_n(d2 + 2 * k) == n) k++;
        if (k <= 8) c = mix(c, ck_deep(d2, y16, n + 1), (9 - k) * 256 / 9);     /* (one more period from 8 columns before it is needed) */
    }
    if (v16 > CK_VFL) {                                  /* the floor: the same row, whole tiles further in */
        int px = (int)(((ck_i64)(y16 - CK_VY16) * CK_TILE) >> 10), m16 = -X * 65536 + px, w;
        Rgb f;
        while (m16 < 65536) m16 += px;
        f = ck_bil(m16, y16);
        w = (int)((((ck_i64)(v16 - CK_VFL) * d2) >> 9) - 256) / 2;      /* rows under the wall's foot (x 256): 1 to 3 */
        c = mix(c, f, clampi(w, 0, 256));
    }
    return c;
}
static Rgb ck_cv_px(const Rgb *cv, int pad, int X, int y) { return cv[(size_t)clampi(y, 0, CK_H - 1) * pad + clampi(X, 1, pad) - 1]; }
static void ck_gen(int pad)
{
    Rgb *cv = malloc((size_t)CK_H * pad * sizeof *cv); static Rgb dl[CK_H], ds[CK_H];
    if (!cv) return;
    memset(ck_qh, 0, sizeof ck_qh);
    for (ck_side = 0; ck_side < 2; ck_side++) {
        Rgb band[3];
        if (ck_side) ck_profile();
        for (int k = 0; k < 3; k++) {                    /* the rib's rows over the wall arch at the edge */
            int r = 0, g = 0, b = 0;
            for (int m = 0; m < 3; m++) { Rgb c = ck_t(m, ((ck_arch((ck_i64)(479 - 2 * m) << 15) + 32768) >> 16) - 1 - k); r += c.r; g += c.g; b += c.b; }
            band[k].r = r / 3; band[k].g = g / 3; band[k].b = b / 3;
        }
        for (int y = 0; y < CK_H; y++) for (int X = 1; X <= pad; X++) cv[(size_t)y * pad + X - 1] = ck_wall(X, y << 16);
        /* the join's tone: per ray, the picture's first 6 columns less the strip's first 6, fading out */
        for (int y = 0; y < CK_H; y++) {
            int r = 0, g = 0, b = 0;
            for (int k = 0; k < 6; k++) {
                int ya = (int)(CK_VY16 + ck_fdiv((ck_i64)((y << 16) - CK_VY16) * (479 - 2 * k), 479));
                int yb = (int)(CK_VY16 + ck_fdiv((ck_i64)((y << 16) - CK_VY16) * (481 + 2 * k), 479));
                Rgb a = ck_bil(k << 16, ya), s0 = ck_cv_px(cv, pad, k + 1, yb >> 16), s1 = ck_cv_px(cv, pad, k + 1, (yb >> 16) + 1);
                int f = (yb >> 8) & 255;
                r += a.r - (s0.r + (s1.r - s0.r) * f / 256); g += a.g - (s0.g + (s1.g - s0.g) * f / 256); b += a.b - (s0.b + (s1.b - s0.b) * f / 256);
            }
            dl[y].r = r / 6; dl[y].g = g / 6; dl[y].b = b / 6;
        }
        for (int y = 0; y < CK_H; y++) {
            Rgb a = dl[clampi(y - 1, CK_TY, CK_H - 1)], b = dl[clampi(y, CK_TY, CK_H - 1)], c = dl[clampi(y + 1, CK_TY, CK_H - 1)];
            ds[y].r = (a.r + 2 * b.r + c.r) / 4; ds[y].g = (a.g + 2 * b.g + c.g) / 4; ds[y].b = (a.b + 2 * b.b + c.b) / 4;
        }
        for (int y = 0; y < CK_H; y++) for (int X = 1; X <= pad; X++) {
            int y0 = (int)(CK_VY16 + ck_fdiv((ck_i64)((y << 16) - CK_VY16) * 479, 479 + 2 * X)), f = (y0 >> 8) & 255, w = CK_TF * 256 / (CK_TF + X);
            Rgb *c = &cv[(size_t)y * pad + X - 1], d0 = ds[clampi(y0 >> 16, 0, CK_H - 1)], d1 = ds[clampi((y0 >> 16) + 1, 0, CK_H - 1)];
            c->r += (d0.r * 256 + (d1.r - d0.r) * f) * w / 65536; c->g += (d0.g * 256 + (d1.g - d0.g) * f) * w / 65536; c->b += (d0.b * 256 + (d1.b - d0.b) * f) * w / 65536;
        }
        /* the ceiling's tone at the join, per row (the rows over the rib there) */
        for (int y = 0; y < CK_H; y++) {
            int r = 0, g = 0, b = 0, yy = y < 7 ? y : 7;
            for (int k = 0; k < 6; k++) { Rgb a = ck_t(k, yy), c = ck_t(CK_CS - 1 - k, yy); r += a.r - c.r; g += a.g - c.g; b += a.b - c.b; }
            dl[y].r = r / 6; dl[y].g = g / 6; dl[y].b = b / 6;
        }
        /* the vault over the wall arch: the rib along it, the ceiling above */
        for (int X = 1; X <= pad; X++) {
            ck_i64 d16 = (ck_i64)(479 + 2 * X) << 15;
            int yi = ck_arch(d16), rib = (ck_i64)1000000000 * 2 / (479 + 2 * X) > CK_AUC - CK_AW, yt = rib ? yi - CK_RIB : yi, w = CK_TF * 256 / (CK_TF + X);
            for (int y = 0; y < CK_H && (y << 16) - 32768 < yi; y++) {
                int ci = clampi((yi - ((y << 16) - 32768)) >> 8, 0, 256), cc = clampi((yt - ((y << 16) - 32768)) >> 8, 0, 256);
                int k = clampi((yi - (y << 16)) >> 16, 0, 2), ms = CK_CS - X, ys, yr = yt >> 16;
                Rgb *c = &cv[(size_t)y * pad + X - 1], ce;
                while (ms < 1) ms += CK_CS;
                ys = ((ck_arch((ck_i64)(479 - 2 * ms) << 15) - CK_RIB) >> 16) - 2;     /* (the ceiling's last whole row there) */
                ce = ck_t(ms, yr > ys ? y * ys / yr : y);                           /* (more rows than it has: its rows spread) */
                ce.r += dl[y].r * w / 256; ce.g += dl[y].g * w / 256; ce.b += dl[y].b * w / 256;
                *c = mix(*c, band[k], ci);
                *c = mix(*c, ce, cc);
            }
        }
        for (int y = 0; y < CK_H; y++) for (int X = 1; X <= pad; X++)
            ck_ext[(size_t)y * 2 * pad + (ck_side ? pad + X - 1 : pad - X)] = ck_quant(cv[(size_t)y * pad + X - 1]);
    }
    ck_side = 0;
    free(cv);
}
#ifndef WS_BACKDROP_MAIN
static u16 *ck_buf; static int ck_pad = -1; static u32 ck_sum;
static u32 ck_grab(const u16 *vram)
{
    u32 h = 2166136261u;
    for (int i = 0; i < 256; i++) ck_clut[i] = vram[(size_t)511 * 1024 + 128 + i];
    for (int y = 0; y < 256; y++) for (int x = 0; x < CK_W; x++) ck_src[y][x] = tex8(vram, 128, x, y, 128, 511);
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 256; x++) ck_src[256 + y][x] = tex8(vram, 384, x, y, 128, 511);
        for (int x = 0; x < 224; x++) ck_src[256 + y][256 + x] = tex8(vram, 384, x, 32 + y, 128, 511);
    }
    for (int y = 0; y < CK_H; y++) for (int x = 0; x < CK_W; x++) h = (h ^ ck_src[y][x]) * 16777619u;
    return h;
}
static const u16 *ck_take(const u16 *vram, int pad, int x, int y, int *rows)
{
    static int last = -1;
    extern int port_frame;
    int f0 = y > 16 ? y : 16, f1 = y + CK_H < 240 ? y + CK_H : 240;
    if (last != port_frame || !ck_ext || ck_pad != pad) {
        u32 s = ck_grab(vram);
        last = port_frame;
        if (!ck_ext || ck_pad != pad || s != ck_sum) {
            ck_sum = s; ck_pad = pad;
            free(ck_ext); free(ck_buf);
            ck_ext = calloc((size_t)CK_H * 2 * pad, 2); ck_buf = calloc((size_t)PH * 2 * pad, 2);
            {   Rgb t = rgb15(ck_src[4][240]), f = rgb15(ck_src[284][20]);   /* (the vault's peach, the floor's grey: this picture) */
                if (!ck_ext || !ck_buf || t.r < t.b + 24 || f.r > f.b) { free(ck_ext); free(ck_buf); ck_ext = ck_buf = 0; return 0; }
            }
            ck_gen(pad);
            {   const char *e = getenv("PORT_WS_BACKDROP_DUMP");      /* research: the CLUT, then the picture (raw 15-bit LE) */
                FILE *f = e ? fopen(e, "wb") : 0;
                if (f) { fwrite(ck_clut, 1, sizeof ck_clut, f); fwrite(ck_src, 1, sizeof ck_src, f); fclose(f); }
            }
        }
    }
    if (!ck_ext || f1 <= f0) return 0;
    if (x <= -pad && x + CK_W >= 320 + pad) return 0;    /* the picture covers the wide frame */
    for (int j = f0; j < f1; j++) {
        u16 *row = ck_buf + (size_t)(j - f0) * 2 * pad; const u16 *ext = ck_ext + (size_t)(j - y) * 2 * pad, *src = ck_src[j - y];
        for (int c = 0; c < pad; c++) {
            int e = c - pad - x, f = 320 + c - x;      /* the picture's column at the left / right strip's column c */
            row[c] = e < 0 ? (-e <= pad ? ext[pad + e] : 0) : e < CK_W ? src[e] : 0;
            row[pad + c] = f >= CK_W ? (f - CK_W < pad ? ext[pad + f - CK_W] : 0) : f >= 0 ? src[f] : 0;
        }
    }
    port_ws_backdrop_dy = f0 - y; *rows = f1 - f0;
    return ck_buf;
}
#endif
