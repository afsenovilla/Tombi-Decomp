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
 * The strips (made once per picture, ck_gen; ck_take lays the frame's strips from them and the picture's columns).
 * Second version: the first one was the wall one period deeper everywhere (ck_deep), i.e. the picture's texels
 * enlarged 1.2 times in height and 1.5 in width, and the join showed as a change of texture ("se nota mucho el
 * empalme").  Now nothing that carries texture is enlarged:
 *   - both walls from the cornice to the floor go on along their rays with the tone the ray has in the picture's
 *     first 8 columns (ck_profile / ck_ray: a profile at quarter rows, so the cornice, the plaster's frame lines,
 *     the chair rail, the wainscot's inlay and the skirting leave the picture where they are, to a fraction of a
 *     row, and as sharp as they are there; no tone to correct at the join);
 *     the profile also keeps each line's hue, and the panels' inlay lines take the speckle of the picture's own
 *     texels on their ray (third version, the right wall: the lower line had lost its orange at the join; both
 *     walls since the fourth);
 *   - the right strip goes on from what the frame shows there, which is not the picture but the game's copy of the
 *     wall past the door drawn over it, one row lower from the chair rail down (fourth version, ck_wall: the strip
 *     went on from the picture underneath and every line under the rail stepped 0.6 to 0.8 rows at the join);
 *   - on that, the picture's own texture texel for texel: the plaster's grain in patches (ck_grain), the wood's
 *     streaks of the cornice, rail, wainscot and skirting column by column along the ray (ck_streak);
 *   - the left wainscot's stiles are the wall one period deeper (ck_deep: the picture seen through the perspective
 *     of a wall moved along its length), so the cut panel is closed by its stile at the measured period and phase
 *     and the next one starts; the right wainscot's panel runs on (that wall shows no period);
 *   - the lunettes (the plaster between the wall arch and the cornice) are the deeper wall's tones, smoothed, with
 *     the lunette's own grain texel for texel and its tone at the join per row;
 *   - the wall arch goes on along its ellipse, the rib 2.9 rows thick as at the edge, the ceiling over it the
 *     picture's own 72 columns further in; past its end (wider than 16:9) the ceiling comes down to the cornice;
 *   - the floor is the same row, whole tiles further in;
 *   - where the deeper wall is used, the join's tone (per ray, the picture's first 6 columns less the strip's)
 *     fades out, halved every 32 columns.
 * Nothing is mirrored; clock, table, chandeliers, fireplace and door are not in the strips.  Every colour is the
 * nearest of the picture's CLUT.  Integers only; a texel depends on its place, not on the strip's width. */
#define CK_HM 1226 /* the ROOMS row's mark (no picture has this height) */
#define CK_W 480
#define CK_H 288
static void gen_ck(int pad) { (void)pad; } /* (the table's gen: ck_take makes the strips) */
static u16 (*ck_src)[CK_W];
static u16 ck_clut[256];
static u16 (*ck_qc);
static u8 (*ck_qh);
static u16 *ck_ext;
static int ck_side;
typedef long long ck_i64;

#define CK_VY16 12075008                     /* the vanishing point's row, 184.25 (16.16); its column is 239.5 */
#define CK_P2 2439                           /* the walls' period: 0.000820 per column -> 1219.5 columns, x 2 */
#define CK_UST 4396000                       /* the wainscot stile next to each edge: 1e6 / its distance */
#define CK_UT 820000                         /* the period in those units */
#define CK_V(v1000) ((v1000) * 65536 / 1000) /* a ray: rows per column from the vanishing point (16.16) */
#define CK_VCT CK_V(-421)                    /* the cornice's top */
#define CK_VCB CK_V(-299)                    /* its underside (the wall's first row) */
#define CK_VRT CK_V(158)                     /* the chair rail's top */
#define CK_VFL CK_V(379)                     /* the wall's foot on the floor */
#define CK_TILE 487                          /* the floor tiles: 0.476 columns per row under the horizon (x 1024) */
/* the wall arch (where the vault meets the side wall): an ellipse in the wall's own space (1e6 / distance, ray) */
#define CK_AUC 4651000
#define CK_AA 3110400
#define CK_AVC 85007
#define CK_AB 133602
#define CK_AW 1675000 /* half its chord at the cornice's top */
#define CK_RIB 190054 /* the rib along it: 2.9 rows at the edge (16.16) */
#define CK_TY 20      /* the join's tone: from this row (the wall under the arch) */
#define CK_TF 32      /* ... halved this many columns out */
#define CK_CS 72      /* the ceiling: the picture's, this many columns further in */
#define CK_LAM 923    /* the rib's other edge: the same curve at 0.923 of the distance */

static u16 ck_quant(Rgb c)
{
    int r = clampi(c.r + 4, 0, 255) >> 3,
        g = clampi(c.g + 4, 0, 255) >> 3,
        b = clampi(c.b + 4, 0, 255) >> 3,
        k = r | g << 5 | b << 10;
    if (!ck_qh[k]) {
        int best_d = 1 << 30,
            best = k;
        for (int i = 0; i < 256; i++) {
            int q = ck_clut[i] & 0x7FFF,
                dr, dg, db, d;
            if (!q) continue;
            dr = r5((u16)q) - r;
            dg = g5((u16)q) - g;
            db = b5((u16)q) - b;
            d = 3 * dr * dr + 4 * dg * dg + 2 * db * db;
            if (d < best_d) {
                best_d = d;
                best = q;
            }
        }
        ck_qc[k] = (u16)best;
        ck_qh[k] = 1;
    }
    return ck_qc[k];
}
/* the picture's texel at column m counted from the side's edge inwards, row y */
static Rgb ck_t(int m, int y)
{
    m = clampi(m, 0, CK_W - 1);
    return rgb15(ck_src[clampi(y, 0, CK_H - 1)][ck_side ? CK_W - 1 - m : m]);
}
static Rgb ck_bil(int m16, int y16)
{
    int m, y, fx, fy;
    Rgb a, b, c, d, o;
    m16 = clampi(m16, 0, (CK_W - 1) << 16);
    y16 = clampi(y16, 0, (CK_H - 1) << 16);
    m = m16 >> 16;
    y = y16 >> 16;
    fx = (m16 >> 8) & 255;
    fy = (y16 >> 8) & 255;
    a = ck_t(m, y);
    b = ck_t(m + 1, y);
    c = ck_t(m, y + 1);
    d = ck_t(m + 1, y + 1);
    o.r = ((a.r * (256 - fx) + b.r * fx) * (256 - fy) + (c.r * (256 - fx) + d.r * fx) * fy + 32768) >> 16;
    o.g = ((a.g * (256 - fx) + b.g * fx) * (256 - fy) + (c.g * (256 - fx) + d.g * fx) * fy + 32768) >> 16;
    o.b = ((a.b * (256 - fx) + b.b * fx) * (256 - fy) + (c.b * (256 - fx) + d.b * fx) * fy + 32768) >> 16;
    return o;
}
static ck_i64 ck_fdiv(ck_i64 a, ck_i64 b)
{
    ck_i64 q = a / b;
    if ((a % b != 0) && ((a < 0) != (b < 0))) q--;
    return q;
}
static unsigned ck_isqrt(unsigned long long v)
{
    unsigned long long root = 0,
                  bit = 1ull << 62;
    while (bit > v) bit >>= 2;
    while (bit) {
        if (v >= root + bit) {
            v -= root + bit;
            root = (root >> 1) + bit;
        } else
            root >>= 1;
        bit >>= 2;
    }
    return (unsigned)root;
}
/* the wall arch's row (16.16) at d16 columns (16.16) from the vanishing point's column; past its near end (where it
   comes down on the cornice) the cornice's top */
static int ck_arch(ck_i64 d16)
{
    ck_i64 inv_d = ((ck_i64)1000000000 << 16) / d16, q, v;
    unsigned r;
    if (inv_d <= CK_AUC - CK_AW) return (int)(CK_VY16 + (((ck_i64)CK_VCT * d16) >> 16));
    q = ((inv_d - CK_AUC) << 16) / CK_AA;
    r = ck_isqrt((unsigned long long)(((ck_i64)1 << 32) - q * q));
    v = CK_AVC - (((ck_i64)CK_AB * r) >> 16);
    return (int)(CK_VY16 + ((v * d16) >> 16));
}
/* the wall n periods deeper than the point d2 half columns from the vanishing column on row y16 */
static Rgb ck_deep(int d2, int y16, int n)
{
    ck_i64 scale = ((ck_i64)CK_P2 << 16) / (CK_P2 + n * d2);
    int m16 = (int)(((ck_i64)479 << 15) - (((ck_i64)d2 * scale) >> 1));
    return ck_bil(m16, (int)(CK_VY16 + (((ck_i64)(y16 - CK_VY16) * scale) >> 16)));
}
static int ck_deep_n(int d2) /* the fewest periods that land in the picture */
{
    int n = 1;
    while ((ck_i64)d2 * CK_P2 > (ck_i64)476 * (CK_P2 + n * d2)) n++;
    return n;
}
/* What runs along the rays (cornice, the plaster's frame lines, chair rail; on the right wall everything down to the
   floor): the side's own tone per ray, from the picture's first CK_PN columns, at quarter rows of the edge column
   (the columns cross a ray at different heights, so together they sample it finer than one column does: the
   strip's lines come out as sharp as the picture's, and where they are in the picture to a fraction of a row;
   each texel is spread over half a row each way, 0.75 until the fourth version, which left the wall foot's dark
   line a shade paler than the picture's). */
#define CK_PN 8
#define CK_PB (CK_H * 4 + 8)
static Rgb (*ck_pr);
static short (*ck_pl);
static int (*ck_profile__acc)[4];
static Rgb (*ck_profile__raw);
static void ck_profile(void)
{
    int (*const acc)[4] = ck_profile__acc;
    Rgb (*const raw) = ck_profile__raw;
    int last = -1;
    memset(acc, 0, (sizeof *acc * (CK_PB)));
    for (int m = 0; m < CK_PN; m++)
        for (int y = 0; y < CK_H; y++) {
            ck_i64 pos = (CK_VY16 + ck_fdiv((ck_i64)((y << 16) - CK_VY16) * 479, 479 - 2 * m)) * 4;
            int bin0, frac;
            Rgb c = ck_t(m, y);
            if (pos < 0) continue;
            bin0 = (int)(pos >> 16);
            frac = (int)(pos >> 8) & 255;
            for (int k = -2; k <= 3; k++) {
                int b = bin0 + k,
                    wt = k * 256 - frac;
                wt = 512 - (wt < 0 ? -wt : wt);
                if (b < 0 || b >= CK_PB || wt <= 0) continue;
                acc[b][0] += c.r * wt;
                acc[b][1] += c.g * wt;
                acc[b][2] += c.b * wt;
                acc[b][3] += wt;
            }
        }
    for (int b = 0; b < CK_PB; b++)
        if (acc[b][3]) {
            raw[b].r = acc[b][0] / acc[b][3];
            raw[b].g = acc[b][1] / acc[b][3];
            raw[b].b = acc[b][2] / acc[b][3];
            if (last < 0)
                for (int k = 0; k < b; k++) raw[k] = raw[b];
            last = b;
        } else if (last >= 0)
            raw[b] = raw[last];
    /* smoothed along the height, not across an edge: over 3 rows, then over 8 and 20 where it is even (a ray's noise
       would be a line, a blotch of the plaster a band) */
    for (int pass = 0; pass < 3; pass++) {
        int reach = pass == 2 ? 40 : pass ? 16 : 6,
            tol = pass ? 3 : 6;
        for (int b = 0; b < CK_PB; b++) {
            int r = 0,
                g = 0,
                bl = 0,
                n = 0,
                luma0 = lum(raw[b]);
            for (int k = -reach; k <= reach; k++) {
                int bb = clampi(b + k, 0, CK_PB - 1),
                    dl = lum(raw[bb]) - luma0,
                    dc = raw[bb].r - raw[bb].b - raw[b].r + raw[b].b;
                if (dl > tol || dl < -tol) continue;
                /* (nor across a hue: the inlay's orange has the skirting's luma) */
                if (dc > 2 * tol || dc < -2 * tol) continue;
                r += raw[bb].r;
                g += raw[bb].g;
                bl += raw[bb].b;
                n++;
            }
            ck_pr[b].r = r / n;
            ck_pr[b].g = g / n;
            ck_pr[b].b = bl / n;
            ck_pl[b] = (short)lum(ck_pr[b]);
        }
        memcpy(raw, ck_pr, (sizeof *raw * (CK_PB)));
    }
}
/* the ray through (d2 half columns from the vanishing column, row y16): its place in the profile (16.16 quarter
   rows) */
static ck_i64 ck_ppos(int d2, int y16) { return (CK_VY16 + ck_fdiv((ck_i64)(y16 - CK_VY16) * 479, d2)) * 4; }
static Rgb ck_pget(ck_i64 p)
{
    int b = clampi((int)(p >> 16), 0, CK_PB - 2),
        f = (int)(p >> 8) & 255;
    Rgb o;
    o.r = ck_pr[b].r + (ck_pr[b + 1].r - ck_pr[b].r) * f / 256;
    o.g = ck_pr[b].g + (ck_pr[b + 1].g - ck_pr[b].g) * f / 256;
    o.b = ck_pr[b].b + (ck_pr[b + 1].b - ck_pr[b].b) * f / 256;
    return o;
}
/* how even the profile is within n quarter rows of p: 256 where it changes less than t0, 0 from t1 */
static int ck_flat(ck_i64 p, int n, int t0, int t1)
{
    int b = clampi((int)((p + 32768) >> 16), 0, CK_PB - 1),
        spread = 0;
    for (int k = -n; k <= n; k++) {
        int d = ck_pl[clampi(b + k, 0, CK_PB - 1)] - ck_pl[b];
        if (d < 0) d = -d;
        if (d > spread) spread = d;
    }
    return clampi((t1 - spread) * 256 / (t1 - t0), 0, 256);
}
/* The plaster's grain, texel for texel (not enlarged): a clean piece of the picture's plaster less its mean over
   13 x 13 texels, laid in patches of 24 x 22 texels, each from its own place in the piece (a hash of the patch),
   cross-faded over 6 texels.  The walls': the left wall's panel (columns 1-62, rows 144-201: nothing but plaster;
   not mirrored on the right wall: both walls' streaks run the same way in the picture).  The lunettes': each
   side's own, between the wall arch and the cornice (42 columns from the third, rows 42-77). */
#define CK_BW 24
#define CK_BH 22
#define CK_BF 6
#define CK_GAW 218 /* the grain's strength: the panel's at the edge is 0.85 of its mean */
#define CK_GAU 176 /* ... the lunettes': 0.7 (the CLUT's steps do the rest) */
/* g: 58 x 62 texels (ws_heap) */
typedef struct {
    int x0, y0, w, h;
    short (*g)[3];
} CkGrain;
static CkGrain ck_gw = { 1, 144, 62, 58 }, ck_gu = { 2, 42, 42, 36 };
static int ck_gin(const CkGrain *G, int x, int y) /* plaster and nothing else? */
{
    int m = G == &ck_gu && ck_side ? CK_W - 1 - x : x,
        v16;
    if (m < 0 || m > 66 || y < 0 || y >= CK_H) return 0;
    v16 = (int)ck_fdiv((ck_i64)((y << 16) - CK_VY16) * 2, 479 - 2 * m);
    if (G == &ck_gu) return v16 < CK_V(-440) && (y << 16) > ck_arch((ck_i64)(479 - 2 * m) << 15) + 3 * 65536;
    return v16 > CK_V(-231) && v16 < CK_V(101);
}
static void ck_grain_make(CkGrain *G)
{
    G->x0 = G == &ck_gu && ck_side ? CK_W - 2 - G->w : G == &ck_gu ? 2 : G->x0;
    for (int y = 0; y < G->h; y++)
        for (int x = 0; x < G->w; x++) {
            int r = 0,
                g = 0,
                b = 0,
                n = 0;
            Rgb c;
            short *grain_px = G->g[y * G->w + x];
            for (int j = -10; j <= 10; j++)
                for (int i = -10; i <= 10; i++) {
                    if (!ck_gin(G, G->x0 + x + i, G->y0 + y + j)) continue;
                    c = rgb15(ck_src[G->y0 + y + j][G->x0 + x + i]);
                    r += c.r;
                    g += c.g;
                    b += c.b;
                    n++;
                }
            c = rgb15(ck_src[G->y0 + y][G->x0 + x]);
            if (!n || !ck_gin(G, G->x0 + x, G->y0 + y)) {
                grain_px[0] = grain_px[1] = grain_px[2] = 0;
                continue;
            }
            grain_px[0] = (short)(c.r - r / n);
            grain_px[1] = (short)(c.g - g / n);
            grain_px[2] = (short)(c.b - b / n);
        }
}
static Rgb ck_grain(const CkGrain *G, int X, int y)
{
    int u = ck_side ? X : 4080 - X,
        bx = u / CK_BW,
        by = y / CK_BH,
        fx = u % CK_BW,
        fy = y % CK_BH;
    int r = 0,
        g = 0,
        b = 0,
        w2 = 0,
        norm;
    Rgb o;
    for (int j = 0; j < 2; j++)
        for (int i = 0; i < 2; i++) {
            int lx = fx + i * CK_BW,
                ly = fy + j * CK_BH,
                wx, wy, w;
            u32 rnd;
            const short *tex;
            if (lx >= CK_BW + CK_BF || ly >= CK_BH + CK_BF) continue;
            wx = lx < CK_BF ? 2 * lx + 1 : lx >= CK_BW ? 2 * (CK_BW + CK_BF - lx) - 1 : 2 * CK_BF;
            wy = ly < CK_BF ? 2 * ly + 1 : ly >= CK_BH ? 2 * (CK_BH + CK_BF - ly) - 1 : 2 * CK_BF;
            w = wx * wy;
            rnd = (u32)(bx - i + 64) * 73856093u ^ (u32)(by - j + 64) * 19349663u;
            rnd ^= rnd >> 13;
            rnd *= 0x5bd1e995u;
            rnd ^= rnd >> 15;
            tex = G->g[((rnd >> 10) % (u32)(G->h - CK_BH - CK_BF + 1) + ly) * G->w +
                       rnd % (u32)(G->w - CK_BW - CK_BF + 1) + lx];
            r += tex[0] * w;
            g += tex[1] * w;
            b += tex[2] * w;
            w2 += w * w;
        }
    /* (the patches' grains do not add up: by the root of the weights) */
    norm = (int)ck_isqrt((unsigned long long)w2 << 8);
    o.r = r * 16 / norm;
    o.g = g * 16 / norm;
    o.b = b * 16 / norm;
    return o;
}
/* a moulding's own streaks: side's texel on the ray v16, at one of the n columns up to mh (the picture's columns as
   they are, further out as X grows), less its mean along the ray */
static Rgb ck_streak(int X, int v16, int side, int mh, int n)
{
    int col0 = mh - (X - 1) % n,
        saved_side = ck_side,
        r = 0,
        g = 0,
        b = 0;
    Rgb c = { 0, 0, 0 },
        o;
    ck_side = side;
    for (int k = -4; k <= 4; k++) {
        int m = col0 + k;
        Rgb t = ck_bil(m << 16, (int)(CK_VY16 + (((ck_i64)v16 * (479 - 2 * m)) >> 1)));
        r += t.r;
        g += t.g;
        b += t.b;
        if (!k) c = t;
    }
    ck_side = saved_side;
    o.r = c.r - r / 9;
    o.g = c.g - g / 9;
    o.b = c.b - b / 9;
    return o;
}
#define CK_VPT CK_V(-436) /* the rays' profile: from just over the cornice down to the floor */
/* how much of a texel on the ray v16 is the profile's (0-256): not the left wainscot's stiles, which are the deeper
   wall's (its columns 9.5 to 22.5: the stile with the ends of both panels' inlay) */
static int ck_pw(int d2, int v16)
{
    int share = clampi((v16 - CK_VPT) * 256 / CK_V(8), 0, 256);
    if (v16 > CK_VFL + CK_V(12)) return 0;
    if (!ck_side && v16 > CK_V(200) && v16 < CK_V(334)) {
        ck_i64 scale = ((ck_i64)CK_P2 << 16) / (CK_P2 + ck_deep_n(d2) * d2);
        int m16 = (int)(((ck_i64)479 << 15) - (((ck_i64)d2 * scale) >> 1));
        int stile = clampi((m16 - 622592) / 384, 0, 256) * clampi((1474560 - m16) / 384, 0, 256) / 256;
        stile = stile * clampi((v16 - CK_V(200)) * 256 / CK_V(8), 0, 256) / 256;
        stile = stile * clampi((CK_V(334) - v16) * 256 / CK_V(8), 0, 256) / 256;
        share = share * (256 - stile) / 256;
    }
    return share;
}
static Rgb ck_ray(int X, int d2, int y16, int v16)
{
    ck_i64 p = ck_ppos(d2, y16);
    Rgb o = ck_pget(p),
        grain = { 0, 0, 0 };
    int amount = 0;
    /* the cornice */
    if (v16 <= CK_VCB) {
        grain = ck_streak(X, v16, ck_side, 35, 32);
        amount = ck_flat(p, 3, 10, 30);
    }
    /* the plaster */
    else if (v16 < CK_VRT) {
        grain = ck_grain(&ck_gw, X, y16 >> 16);
        amount = ck_flat(p, 8, 8, 16) * CK_GAW / 256;
    }
    /* the chair rail */
    else if (v16 < CK_V(228)) {
        grain = ck_streak(X, v16, 0, 35, 32);
        amount = ck_flat(p, 5, 6, 14) / 2;
    }
    /* the wainscot's wood */
    else if (v16 < CK_V(320)) {
        grain = ck_streak(X, v16, 0, 41, 18);
        amount = ck_flat(p, 5, 6, 14) * 5 / 8;
    }
    /* the skirting */
    else if (v16 < CK_VFL) {
        grain = ck_streak(X, v16, 0, 35, 32);
        amount = ck_flat(p, 5, 6, 14) / 2;
    }
    o.r += grain.r * amount / 256;
    o.g += grain.g * amount / 256;
    o.b += grain.b * amount / 256;
    if (v16 > CK_V(200) && v16 < CK_V(334)) { /* the panels' inlay: its own texels' speckle along the line */
        int m = CK_PN - 1 - (X - 1) % CK_PN,
            inlay = clampi((o.r - o.b - 96) * 16, 0, 256);
        if (inlay) {
            Rgb t = ck_bil(m << 16, (int)(CK_VY16 + (((ck_i64)v16 * (479 - 2 * m)) >> 1))),
                q = ck_pget(p);
            o.r += (t.r - q.r) * inlay / 256;
            o.g += (t.g - q.g) * inlay / 256;
            o.b += (t.b - q.b) * inlay / 256;
        }
    }
    return o;
}
/* the side wall and the floor X columns out of the picture's edge on row y16 (no vault); X = 0: the picture's last
   column */
static Rgb ck_wall0(int X, int y16)
{
    int d2 = 479 + 2 * X,
        v16 = (int)ck_fdiv((ck_i64)(y16 - CK_VY16) * 2, d2),
        n = ck_deep_n(d2),
        prof_w = ck_pw(d2, v16);
    Rgb c = { 0, 0, 0 };
    if (!X) return ck_bil(0, y16);
    if (prof_w < 256) {
        int k = 1;
        c = ck_deep(d2, y16, n);
        while (k <= 8 && ck_deep_n(d2 + 2 * k) == n) k++;
        /* (one more period from 8 columns before it is needed) */
        if (k <= 8) c = mix(c, ck_deep(d2, y16, n + 1), (9 - k) * 256 / 9);
    }
    if (prof_w) c = mix(c, ck_ray(X, d2, y16, v16), prof_w);
    if (v16 > CK_VFL) { /* the floor: the same row, whole tiles further in */
        int tile = (int)(((ck_i64)(y16 - CK_VY16) * CK_TILE) >> 10),
            m16 = -X * 65536 + tile,
            w;
        Rgb floor;
        while (m16 < 65536) m16 += tile;
        floor = ck_bil(m16, y16);
        w = (int)((((ck_i64)(v16 - CK_VFL) * d2) >> 9) - 256) / 2; /* rows under the wall's foot (x 256): 1 to 3 */
        c = mix(c, floor, clampi(w, 0, 256));
    }
    return c;
}
/* What the frame shows at the right join is not the picture: the game lays a copy of the wall past the door over it
   (a quad of 32 x 128 pixels at the picture's columns 448-479, rows 160-287, in front of Tombi; its texels are the
   picture's, cut out along the wall's foot: the floor is not in it), and the quad's texture coordinates span
   31 x 127 texels, so its right half shows the picture's column before and its lower half the row above: from row
   225 down the frame has row 224 twice and then every row one lower, from column 465 on every column one further
   out (column 479 is never seen).  The right strip goes on from what is seen: under the ray through row 224 at
   the edge the wall is one row lower and one column later (its first column is the picture's last), down to the
   foot's ray, where the copy is cut; under that, the wall and the floor in their place, as in the frame. */
#define CK_OVY 224 /* the copy's last row in its place */
static Rgb ck_wall(int X, int y16)
{
    if (ck_side) {
        int cut = (int)(CK_VY16 + ck_fdiv((ck_i64)((CK_OVY << 16) - CK_VY16) * (479 + 2 * X), 479));
        if (y16 > cut) {
            int src_y = y16 - 65536 > cut ? y16 - 65536 : cut;
            if ((int)ck_fdiv((ck_i64)(src_y - CK_VY16) * 2, 477 + 2 * X) <= CK_VFL) return ck_wall0(X - 1, src_y);
        }
    }
    return ck_wall0(X, y16);
}
static Rgb ck_cv_px(const Rgb *cv, int pad, int X, int y)
{
    return cv[(size_t)clampi(y, 0, CK_H - 1) * pad + clampi(X, 1, pad) - 1];
}
static Rgb (*ck_gen__dl);
static Rgb (*ck_gen__ds);
static int (*ck_gen__ln);
static int (*ck_gen__lok);
/* both extensions: the side wall and the floor (ck_wall), the join's tone, the lunette, the vault over the wall arch;
   quantised into ck_ext */
static void ck_gen(int pad)
{
    if (!ws_need_ck_gen()) return;
    int out_w = pad;
    Rgb *cv, *cv_copy;
    Rgb (*const delta) = ck_gen__dl,
        (*const delta_s) = ck_gen__ds;
    int (*const lun_n) = ck_gen__ln,
        (*const lun_ok) = ck_gen__lok;
    /* (3 more columns for the lunette's means: a texel does not depend on the strip's width) */
    pad += 3;
    cv = malloc((size_t)CK_H * pad * sizeof *cv);
    cv_copy = malloc((size_t)CK_H * pad * sizeof *cv);
    if (!cv || !cv_copy) {
        free(cv);
        free(cv_copy);
        return;
    }
    memset(ck_qh, 0, (sizeof *ck_qh * (32768)));
    ck_side = 0;
    ck_grain_make(&ck_gw);
    for (ck_side = 0; ck_side < 2; ck_side++) {
        Rgb band[3];
        ck_profile();
        for (int k = 0; k < 3; k++) { /* the rib's rows over the wall arch at the edge */
            int r = 0,
                g = 0,
                b = 0;
            for (int m = 0; m < 3; m++) {
                Rgb c = ck_t(m, ((ck_arch((ck_i64)(479 - 2 * m) << 15) + 32768) >> 16) - 1 - k);
                r += c.r;
                g += c.g;
                b += c.b;
            }
            band[k].r = r / 3;
            band[k].g = g / 3;
            band[k].b = b / 3;
        }
        for (int y = 0; y < CK_H; y++)
            for (int X = 1; X <= pad; X++) cv[(size_t)y * pad + X - 1] = ck_wall(X, y << 16);
        /* the join's tone: per ray, the picture's first 6 columns less the strip's first 6, fading out */
        for (int y = 0; y < CK_H; y++) {
            int r = 0,
                g = 0,
                b = 0;
            for (int k = 0; k < 6; k++) {
                int ya = (int)(CK_VY16 + ck_fdiv((ck_i64)((y << 16) - CK_VY16) * (479 - 2 * k), 479));
                int yb = (int)(CK_VY16 + ck_fdiv((ck_i64)((y << 16) - CK_VY16) * (481 + 2 * k), 479));
                Rgb a = ck_bil(k << 16, ya),
                    s0 = ck_cv_px(cv, pad, k + 1, yb >> 16),
                    s1 = ck_cv_px(cv, pad, k + 1, (yb >> 16) + 1);
                int f = (yb >> 8) & 255;
                r += a.r - (s0.r + (s1.r - s0.r) * f / 256);
                g += a.g - (s0.g + (s1.g - s0.g) * f / 256);
                b += a.b - (s0.b + (s1.b - s0.b) * f / 256);
            }
            delta[y].r = r / 6;
            delta[y].g = g / 6;
            delta[y].b = b / 6;
        }
        for (int y = 0; y < CK_H; y++) {
            Rgb a = delta[clampi(y - 1, CK_TY, CK_H - 1)],
                b = delta[clampi(y, CK_TY, CK_H - 1)],
                c = delta[clampi(y + 1, CK_TY, CK_H - 1)];
            delta_s[y].r = (a.r + 2 * b.r + c.r) / 4;
            delta_s[y].g = (a.g + 2 * b.g + c.g) / 4;
            delta_s[y].b = (a.b + 2 * b.b + c.b) / 4;
        }
        for (int y = 0; y < CK_H; y++)
            for (int X = 1; X <= pad; X++) {
                int y0 = (int)(CK_VY16 + ck_fdiv((ck_i64)((y << 16) - CK_VY16) * 479, 479 + 2 * X)),
                    f = (y0 >> 8) & 255,
                    w = CK_TF * 256 / (CK_TF + X);
                /* (not the profile's rays: they are the join's) */
                w = w * (256 - ck_pw(479 + 2 * X, (int)ck_fdiv((ck_i64)((y << 16) - CK_VY16) * 2, 479 + 2 * X))) / 256;
                Rgb *c = &cv[(size_t)y * pad + X - 1],
                    d0 = delta_s[clampi(y0 >> 16, 0, CK_H - 1)],
                    d1 = delta_s[clampi((y0 >> 16) + 1, 0, CK_H - 1)];
                c->r += (d0.r * 256 + (d1.r - d0.r) * f) * w / 65536;
                c->g += (d0.g * 256 + (d1.g - d0.g) * f) * w / 65536;
                c->b += (d0.b * 256 + (d1.b - d0.b) * f) * w / 65536;
            }
        /* the lunette (the plaster between the wall arch and the cornice): the deeper wall's tones (its mean over
           7 x 7 texels) with the lunette's own grain, texel for texel ... */
        ck_grain_make(&ck_gu);
        memcpy(cv_copy, cv, (size_t)CK_H * pad * sizeof *cv);
        memset(delta_s, 0, (sizeof *delta_s * (CK_H)));
        memset(lun_n, 0, (sizeof *lun_n * (CK_H)));
        for (int X = 1; X <= pad; X++) {
            int d2 = 479 + 2 * X,
                y0 = (ck_arch((ck_i64)d2 << 15) >> 16) + 2;
            for (int y = y0; y < CK_H; y++) {
                int v16 = (int)ck_fdiv((ck_i64)((y << 16) - CK_VY16) * 2, d2),
                    r = 0,
                    g = 0,
                    b = 0,
                    n = 0,
                    w;
                Rgb o, t;
                if (v16 >= CK_VPT) break;
                for (int i = -3; i <= 3; i++) {
                    int Xi = clampi(X + i, 1, pad),
                        di = 479 + 2 * Xi,
                        ya = (ck_arch((ck_i64)di << 15) >> 16) + 2;
                    for (int j = -3; j <= 3; j++) {
                        if (y + j < ya || (int)ck_fdiv((ck_i64)(((y + j) << 16) - CK_VY16) * 2, di) >= CK_VPT) continue;
                        t = cv_copy[(size_t)(y + j) * pad + Xi - 1];
                        r += t.r;
                        g += t.g;
                        b += t.b;
                        n++;
                    }
                }
                t = ck_grain(&ck_gu, X, y);
                o.r = r / n;
                o.g = g / n;
                o.b = b / n;
                if (X <= 2) {
                    delta_s[y].r += o.r;
                    delta_s[y].g += o.g;
                    delta_s[y].b += o.b;
                    lun_n[y]++;
                }
                o.r += t.r * CK_GAU / 256;
                o.g += t.g * CK_GAU / 256;
                o.b += t.b * CK_GAU / 256;
                w = clampi((y - y0 + 1) * 64, 0, 256) * clampi((CK_VPT - v16) * 256 / CK_V(8), 0, 256) / 256;
                cv[(size_t)y * pad + X - 1] = mix(cv[(size_t)y * pad + X - 1], o, w);
            }
        }
        /* ... and its tone at the join, per row: the picture's first 4 columns, carried on to the strip's first
           two by 3/4 of what they are less the next 4 (3 at most), less the strip's; over 9 rows, halved every
           CK_TF columns */
        {
            int have = 0;
            for (int y = 0; y < CK_H; y++) {
                int a[3] = { 0, 0, 0 },
                    b[3] = { 0, 0, 0 },
                    c[3] = { 0, 0, 0 },
                    na = 0,
                    nb = 0,
                    nc = 0;
                for (int j = -4; j <= 4; j++) {
                    int yy = y + j;
                    if (yy < 0 || yy >= CK_H) continue;
                    for (int m = 0; m < 8; m++) {
                        Rgb t;
                        if (!ck_gin(&ck_gu, ck_side ? CK_W - 1 - m : m, yy)) continue;
                        t = ck_t(m, yy);
                        if (m < 4) {
                            a[0] += t.r;
                            a[1] += t.g;
                            a[2] += t.b;
                            na++;
                        } else {
                            b[0] += t.r;
                            b[1] += t.g;
                            b[2] += t.b;
                            nb++;
                        }
                    }
                    if (lun_n[yy] == 2) {
                        c[0] += delta_s[yy].r;
                        c[1] += delta_s[yy].g;
                        c[2] += delta_s[yy].b;
                        nc += 2;
                    }
                }
                lun_ok[y] = na >= 16 && nb >= 16 && nc >= 8;
                if (!lun_ok[y]) continue;
                delta[y].r = a[0] / na + clampi((a[0] * 3 / na - b[0] * 3 / nb) / 4, -3, 3) - c[0] / nc;
                delta[y].g = a[1] / na + clampi((a[1] * 3 / na - b[1] * 3 / nb) / 4, -3, 3) - c[1] / nc;
                delta[y].b = a[2] / na + clampi((a[2] * 3 / na - b[2] * 3 / nb) / 4, -3, 3) - c[2] / nc;
                have = 1;
            }
            for (int X = 1; X <= pad && have; X++) {
                int d2 = 479 + 2 * X,
                    y0 = (ck_arch((ck_i64)d2 << 15) >> 16) + 2;
                for (int y = y0; y < CK_H; y++) {
                    int v16 = (int)ck_fdiv((ck_i64)((y << 16) - CK_VY16) * 2, d2),
                        yy = y,
                        w;
                    Rgb *c = &cv[(size_t)y * pad + X - 1];
                    if (v16 >= CK_VPT) break;
                    while (yy < CK_H - 1 && !lun_ok[yy]) yy++;
                    while (yy > 0 && !lun_ok[yy]) yy--;
                    w = clampi((y - y0 + 1) * 64, 0, 256) * clampi((CK_VPT - v16) * 256 / CK_V(8), 0, 256) / 256 *
                        (CK_TF * 256 / (CK_TF + X)) / 256;
                    c->r += delta[yy].r * w / 256;
                    c->g += delta[yy].g * w / 256;
                    c->b += delta[yy].b * w / 256;
                }
            }
        }
        /* the ceiling's tone at the join, per row (the rows over the rib there) */
        for (int y = 0; y < CK_H; y++) {
            int r = 0,
                g = 0,
                b = 0,
                yy = y < 7 ? y : 7;
            for (int k = 0; k < 6; k++) {
                Rgb a = ck_t(k, yy),
                    c = ck_t(CK_CS - 1 - k, yy);
                r += a.r - c.r;
                g += a.g - c.g;
                b += a.b - c.b;
            }
            delta[y].r = r / 6;
            delta[y].g = g / 6;
            delta[y].b = b / 6;
        }
        /* the vault over the wall arch: the rib along it, the ceiling above */
        for (int X = 1; X <= pad; X++) {
            ck_i64 d16 = (ck_i64)(479 + 2 * X) << 15;
            int yi = ck_arch(d16),
                rib = (ck_i64)1000000000 * 2 / (479 + 2 * X) > CK_AUC - CK_AW,
                yt = rib ? yi - CK_RIB : yi,
                w = CK_TF * 256 / (CK_TF + X);
            for (int y = 0; y < CK_H && (y << 16) - 32768 < yi; y++) {
                int ci = clampi((yi - ((y << 16) - 32768)) >> 8, 0, 256),
                    cc = clampi((yt - ((y << 16) - 32768)) >> 8, 0, 256);
                int k = clampi((yi - (y << 16)) >> 16, 0, 2),
                    ms = CK_CS - X,
                    ys,
                    yr = yt >> 16;
                Rgb *c = &cv[(size_t)y * pad + X - 1],
                    ce;
                while (ms < 1) ms += CK_CS;
                /* (the ceiling's last whole row there) */
                ys = ((ck_arch((ck_i64)(479 - 2 * ms) << 15) - CK_RIB) >> 16) - 2;
                /* (more rows than it has: its rows spread) */
                ce = ck_t(ms, yr > ys ? y * ys / yr : y);
                ce.r += delta[y].r * w / 256;
                ce.g += delta[y].g * w / 256;
                ce.b += delta[y].b * w / 256;
                *c = mix(*c, band[k], ci);
                *c = mix(*c, ce, cc);
            }
        }
        for (int y = 0; y < CK_H; y++)
            for (int X = 1; X <= out_w; X++)
                ck_ext[(size_t)y * 2 * out_w + (ck_side ? out_w + X - 1 : out_w - X)] =
                    ck_quant(cv[(size_t)y * pad + X - 1]);
    }
    ck_side = 0;
    free(cv);
    free(cv_copy);
}
#ifndef WS_BACKDROP_MAIN
static u16 *ck_buf;
static int ck_pad = -1;
static u32 ck_sum;
/* the picture and its CLUT read from VRAM (into ck_src, ck_clut); returns their hash */
static u32 ck_grab(const u16 *vram)
{
    u32 fnv = 2166136261u;
    for (int i = 0; i < 256; i++) ck_clut[i] = vram[(size_t)511 * 1024 + 128 + i];
    for (int y = 0; y < 256; y++)
        for (int x = 0; x < CK_W; x++) ck_src[y][x] = tex8(vram, 128, x, y, 128, 511);
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 256; x++) ck_src[256 + y][x] = tex8(vram, 384, x, y, 128, 511);
        for (int x = 0; x < 224; x++) ck_src[256 + y][256 + x] = tex8(vram, 384, x, 32 + y, 128, 511);
    }
    for (int y = 0; y < CK_H; y++)
        for (int x = 0; x < CK_W; x++) fnv = (fnv ^ ck_src[y][x]) * 16777619u;
    return fnv;
}
static const u16 *ck_take(const u16 *vram, int pad, int x, int y, int *rows)
{
    if (!ws_need_ck_take()) return 0;
    static int last = -1;
    extern int port_frame;
    int first_row = y > 16 ? y : 16,
        end_row = y + CK_H < 240 ? y + CK_H : 240;
    if (last != port_frame || !ck_ext || ck_pad != pad) {
        u32 check = ck_grab(vram);
        last = port_frame;
        if (!ck_ext || ck_pad != pad || check != ck_sum) {
            ck_sum = check;
            ck_pad = pad;
            free(ck_ext);
            free(ck_buf);
            ck_ext = calloc((size_t)CK_H * 2 * pad, 2);
            ck_buf = calloc((size_t)PH * 2 * pad, 2);
            /* (the vault's peach, the floor's grey: this picture) */
            {
                Rgb vault = rgb15(ck_src[4][240]),
                    f = rgb15(ck_src[284][20]);
                if (!ck_ext || !ck_buf || vault.r < vault.b + 24 || f.r > f.b) {
                    free(ck_ext);
                    free(ck_buf);
                    ck_ext = ck_buf = 0;
                    return 0;
                }
            }
            {
                WsCPart entry = { ck_ext, (size_t)CK_H * 2 * pad * 2 }; /* (made once: ws_cache) */
                u32 key = wsc_hash(check, ck_clut, sizeof ck_clut);
                if (!wsc_get("ck", 9, 2, pad, key, 0, &entry, 1)) {
                    ck_gen(pad);
                    wsc_put("ck", 9, 2, pad, key, 0, &entry, 1);
                }
            }
            /* research: the CLUT, then the picture (raw 15-bit LE) */
            {
                const char *e = getenv("PORT_WS_BACKDROP_DUMP");
                FILE *f = e ? fopen(e, "wb") : 0;
                if (f) {
                    fwrite(ck_clut, 1, sizeof ck_clut, f);
                    fwrite(ck_src, 1, (sizeof *ck_src * (CK_H)), f);
                    fclose(f);
                }
            }
        }
    }
    if (!ck_ext || end_row <= first_row) return 0;
    if (x <= -pad && x + CK_W >= 320 + pad) return 0; /* the picture covers the wide frame */
    for (int j = first_row; j < end_row; j++) {
        u16 *row = ck_buf + (size_t)(j - first_row) * 2 * pad;
        const u16 *ext = ck_ext + (size_t)(j - y) * 2 * pad,
                  *src = ck_src[j - y];
        for (int c = 0; c < pad; c++) {
            /* the picture's column at the left / right strip's column c */
            int e = c - pad - x,
                f = 320 + c - x;
            row[c] = e < 0 ? (-e <= pad ? ext[pad + e] : 0) : e < CK_W ? src[e] : 0;
            row[pad + c] = f >= CK_W ? (f - CK_W < pad ? ext[pad + f - CK_W] : 0) : f >= 0 ? src[f] : 0;
        }
    }
    port_ws_backdrop_dy = first_row - y;
    *rows = end_row - first_row;
    return ck_buf;
}
#endif
