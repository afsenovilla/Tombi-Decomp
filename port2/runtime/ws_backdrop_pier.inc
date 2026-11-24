/* --widescreen: the side strips of Baccus Pier, AREA08 s1 / s3 (included by ws_backdrop.c; docs/PORT.md "More rooms
 * with picture strips", "Baccus Pier").
 *
 * The room is drawn in three layers: the water (two quads of page 256, u 64.., 320 x 120 texels at picture row 104),
 * the picture over it (the usual 256 + 64 column sprites, texel 0 clear: trees, far shore, the pier) and the light
 * rays (a 4 bpp layer, pages 384 / 448, CLUT (128, 510), added to the frame).  pr_take() reads the water and the rays
 * from VRAM next to the picture grab() took.
 *
 * The strips, as the user approved them on real renders (2026-11-17):
 *   - the picture carried on with its own trees: the scene without the pier (the water layer under it) goes on past
 *     each edge from the other side of the picture (never mirrored), from the column that matches the edge column
 *     best, and its tone is brought to the edge's over 28 columns;
 *   - a nearer tree right at each join, behind the leaves the edge cuts (the user: "a tree that connects with those
 *     leaves"): the picture's big middle trunk drawn 1.7 times nearer (1.5 at the right), scaled about the horizon;
 *     its bark is the trunk's shading at that scale (smooth) plus the bark's grain at the picture's own texel size
 *     (a plain enlargement showed horizontal bands), it meets the water in a U (the near side of its round foot lower
 *     than its flanks) with a dark contact line, and its reflection follows that curve;
 *   - the leaves cut by the edge are completed: each one's texels are found in the picture, its axis and width
 *     measured, and it goes on along that axis to a tip (a lens as long as its width asks for), in its own colours
 *     across its width;
 *   - the light: the glow's radial profile about its centre and the rays that cross an edge continued along their
 *     lines from the rays' common point, added like the game adds its layer.
 * Integers only (fixed point), fixed seeds and position hashes: the same strips on i386, x86-64 and wasm2c. */

#define PR_HZ 113 /* the horizon row */
#define PR_YT 30  /* the middle trunk: its rows in the picture */
#define PR_YB 166
/* the frame without rays; the scene without the pier; the water */
static int (*pr_C)[PW][3], (*pr_S)[PW][3], (*pr_W)[PW][3];
static u8 (*pr_lab)[PW];  /* 1 sky, 2 foliage, 3 trunk / shore, 4 water */
static int (*pr_ray)[PW]; /* the ray layer (0..248) */
static int pr_have_layers;

/* the layers read from VRAM: the water picture (pr_W, rows 104 on) and the ray layer (pr_ray, from a 4-bit page) */
static void pr_take(const u16 *vram)
{
    if (!ws_need_pr_take()) return;
    memset(pr_W, 0, (sizeof *pr_W * (PH)));
    for (int y = 0; y < 120; y++)
        for (int x = 0; x < PW; x++) {
            Rgb c = rgb15(x < 192 ? tex8(vram, 256, 64 + x, y, 128, 511)
                                  : tex8(vram, 256, 64 + x - 192, 120 + y, 128, 511));
            pr_W[104 + y][x][0] = c.r;
            pr_W[104 + y][x][1] = c.g;
            pr_W[104 + y][x][2] = c.b;
        }
    for (int y = 0; y < PH; y++)
        for (int x = 0; x < PW; x++) {
            u16 word = vram[(size_t)y * 1024 + (x < 256 ? 384 + x / 4 : 448 + (x - 256) / 4)];
            int i = (word >> (4 * (x & 3))) & 15;
            pr_ray[y][x] = (vram[(size_t)510 * 1024 + 128 + i] & 31) << 3;
        }
    pr_have_layers = 1;
}

static int pr_dist(const int *a, const int *b) { return abs(a[0] - b[0]) + abs(a[1] - b[1]) + abs(a[2] - b[2]); }
/* the frame's colours (pr_C; the water where the picture is clear), their labels (pr_lab) and the scene without the
   pier (pr_S) */
static void pr_scene(void)
{
    for (int y = 0; y < PH; y++)
        for (int x = 0; x < PW; x++) {
            Rgb texel = rgb15(pic[y][x]);
            int *c = pr_C[y][x],
                label = 0;
            if (!pic[y][x]) {
                c[0] = pr_W[y][x][0];
                c[1] = pr_W[y][x][1];
                c[2] = pr_W[y][x][2];
            } else {
                c[0] = texel.r;
                c[1] = texel.g;
                c[2] = texel.b;
            }
            {
                int r = c[0],
                    g = c[1],
                    b = c[2],
                    leafy = g >= r + 16 && g >= b + 8;
                if (b >= r && b >= g - 8 && !leafy) label = 1;
                if (leafy) label = 2;
                if (r >= g + 24 && g >= b && r - b >= 48) label = 3;
                if (!pic[y][x]) label = 4;
            }
            pr_lab[y][x] = (u8)label;
        }
    memcpy(pr_S, pr_C, (sizeof *pr_S * (PH)));
    /* the pier out: its texels back to water (the middle trunk's foot stays) */
    for (int y = 148; y < PH; y++)
        for (int x = 84; x < 228; x++) {
            if (!pic[y][x] || (y < 166 && x >= 205)) continue;
            if (y >= 146 && y < 166 && x >= 190 && x < 208 && pr_lab[y - 20][x] == 3)
                memcpy(pr_S[y][x], pr_S[y - 20][x], sizeof pr_S[y][x]);
            else
                memcpy(pr_S[y][x], pr_W[y][x], sizeof pr_S[y][x]);
        }
}

/* --- the canvas of one side: pr_cv[y][j], j = 0 next to the join, outwards */
static int (*pr_cv)[3];
static int pr_pad;
static int *pr_px(int j, int y) { return pr_cv[(size_t)y * pr_pad + j]; }

/* the picture carried on from its other side, from the column that matches the edge best; the tone at the join */
static int (*pr_base__d)[3];
static void pr_base(int side)
{
    int best = 1;
    long long best_err = -1;
    for (int off = 1; off < 70; off++) {
        long long err = 0;
        int src_col = side ? off - 1 : PW - off,
            edge_col = side ? PW - 1 : 0;
        for (int y = 0; y < PH; y++) err += pr_dist(pr_S[y][src_col], pr_C[y][edge_col]);
        if (best_err < 0 || err < best_err) {
            best_err = err;
            best = off;
        }
    }
    for (int y = 0; y < PH; y++)
        for (int j = 0; j < pr_pad; j++) {
            int src_x = fmodi(side ? best + j : PW - best - 1 - j, PW);
            memcpy(pr_px(j, y), pr_S[y][src_x], sizeof(int[3]));
        }
    {
        int (*const d)[3] = pr_base__d;
        int edge_col = side ? PW - 1 : 0;
        for (int y = 0; y < PH; y++)
            for (int c = 0; c < 3; c++) d[y][c] = clampi(pr_C[y][edge_col][c] - pr_px(0, y)[c], -48, 48);
        for (int y = 0; y < PH; y++)
            for (int c = 0; c < 3; c++) {
                int sum = 0;
                for (int k = -6; k <= 6; k++) sum += d[clampi(y + k, 0, PH - 1)][c];
                for (int j = 0; j < pr_pad && j < 28; j++) {
                    int *p = pr_px(j, y);
                    p[c] = clampi(p[c] + sum * (28 - j) * (28 - j) / (13 * 784), 0, 248);
                }
            }
    }
}

/* --- the middle trunk: its flanks per row (x 256), its bark's shading (5 x 5 means of trunk texels, x 16) and grain */
static int pr_lo[PH], pr_hi[PH], (*pr_B)[PW][3], (*pr_D)[PW][3];
static u8 (*pr_trunk__M)[PW];
static void pr_trunk(void)
{
    static int lo[PH], hi[PH];
    u8 (*const in_trunk)[PW] = pr_trunk__M;
    int first = -1,
        last = -1;
    for (int y = 0; y < PH; y++) lo[y] = hi[y] = -1;
    for (int y = PR_YT; y < PR_YB; y++) {
        int n = 0,
            a = 0,
            b = 0;
        for (int x = 188; x < 224; x++)
            if (pr_lab[y][x] == 3 || (y >= 146 && pr_S[y][x][0] > pr_S[y][x][2] + 30)) {
                if (!n) a = x;
                b = x;
                n++;
            }
        if (n >= 4) {
            lo[y] = a * 256;
            hi[y] = (b + 1) * 256;
            if (first < 0) first = y;
            last = y;
        }
    }
    if (first < 0) {
        for (int y = 0; y < PH; y++) {
            pr_lo[y] = 196 * 256;
            pr_hi[y] = 218 * 256;
        }
    } else {
        static int lo_fill[PH], hi_fill[PH];
        for (int y = 0; y < PH; y++) { /* rows without a measure: between their neighbours' */
            if (y <= first) {
                lo_fill[y] = lo[first];
                hi_fill[y] = hi[first];
            } else if (y >= last) {
                lo_fill[y] = lo[last];
                hi_fill[y] = hi[last];
            } else if (lo[y] >= 0) {
                lo_fill[y] = lo[y];
                hi_fill[y] = hi[y];
            } else {
                int a = y,
                    b = y;
                while (lo[a] < 0) a--;
                while (lo[b] < 0) b++;
                lo_fill[y] = lo[a] + (lo[b] - lo[a]) * (y - a) / (b - a);
                hi_fill[y] = hi[a] + (hi[b] - hi[a]) * (y - a) / (b - a);
            }
        }
        for (int y = 0; y < PH; y++) {
            int lo_sum = 0,
                hi_sum = 0;
            for (int k = -4; k <= 4; k++) {
                lo_sum += lo_fill[clampi(y + k, 0, PH - 1)];
                hi_sum += hi_fill[clampi(y + k, 0, PH - 1)];
            }
            pr_lo[y] = lo_sum / 9;
            pr_hi[y] = hi_sum / 9;
        }
        {
            int lo_sum = 0,
                hi_sum = 0;
            for (int y = 136; y < 140; y++) {
                lo_sum += pr_lo[y];
                hi_sum += pr_hi[y];
            }
            for (int y = 140; y < PH; y++) {
                pr_lo[y] = lo_sum / 4;
                pr_hi[y] = hi_sum / 4;
            }
        }
    }
    memset(in_trunk, 0, (sizeof *in_trunk * (PH)));
    memset(pr_B, 0, (sizeof *pr_B * (PH)));
    memset(pr_D, 0, (sizeof *pr_D * (PH)));
    for (int y = PR_YT; y < 138; y++)
        for (int x = (pr_lo[y] + 255) >> 8; x < pr_hi[y] >> 8; x++)
            if (x >= 0 && x < PW) in_trunk[y][x] = 1;
    for (int y = PR_YT; y < 138; y++) {
        int a = (pr_lo[y] + 255) >> 8,
            b = (pr_hi[y] >> 8) - 1;
        if (a > b || a < 2 || b >= PW - 2) continue;
        for (int x = a; x <= b; x++) {
            int s[3] = { 0, 0, 0 },
                n = 0;
            for (int dy = -2; dy <= 2; dy++)
                for (int dx = -2; dx <= 2; dx++)
                    if (in_trunk[y + dy][x + dx]) {
                        const int *c = pr_S[y + dy][x + dx];
                        s[0] += c[0];
                        s[1] += c[1];
                        s[2] += c[2];
                        n++;
                    }
            for (int c = 0; c < 3; c++) {
                pr_B[y][x][c] = s[c] * 16 / n;
                pr_D[y][x][c] = pr_S[y][x][c] - s[c] / n;
            }
        }
        /* past a flank: the flank's bark */
        for (int x = 0; x < a; x++) memcpy(pr_B[y][x], pr_B[y][a], sizeof pr_B[y][x]);
        for (int x = b + 1; x < PW; x++) memcpy(pr_B[y][x], pr_B[y][b], sizeof pr_B[y][x]);
    }
}
/* the bark's shading at (x, y) (x 256, texel centres at .5), bilinear between the trunk's rows */
static void pr_bark(int x8, int y8, int *o)
{
    int x = clampi(x8 - 128, 0, (PW - 1) * 256 - 1),
        y = clampi(y8 - 128, PR_YT * 256, 137 * 256 - 1);
    int xi = x >> 8,
        yi = y >> 8,
        fx = x & 255,
        fy = y & 255;
    for (int c = 0; c < 3; c++) {
        int upper = pr_B[yi][xi][c] * (256 - fx) + pr_B[yi][xi + 1][c] * fx,
            lower = pr_B[yi + 1][xi][c] * (256 - fx) + pr_B[yi + 1][xi + 1][c] * fx;
        o[c] = (int)(((long long)upper * (256 - fy) + (long long)lower * fy) >> 20);
    }
}
static int pr_flank(const int *f, int y8) /* a flank at row y (x 256), between the rows' centres */
{
    int y = clampi(y8 - 128, 0, (PH - 1) * 256 - 1),
        yi = y >> 8,
        fy = y & 255;
    return (f[yi] * (256 - fy) + f[yi + 1] * fy) >> 8;
}
static int pr_sin(int a) /* sine x 256 of a / 4096 turns */
{
    int i = (a >> 8) & 15,
        f = a & 255;
    return (SIN16[i] * (256 - f) + SIN16[(i + 1) & 15] * f) >> 8;
}
static int pr_folish(const int *c) { return c[1] >= c[0] + 16 && c[1] >= c[2] + 8; }

/* the near tree of one side: the middle trunk s (x 65536) times nearer, gap columns from the join */
static void pr_tree(int side, int s, int gap, int seed)
{
    int side_x,
        flank_l = pr_lo[136],
        flank_r = pr_hi[136],
        mid = (flank_l + flank_r) / 2,
        half_w = (flank_r - flank_l) / 2;
    int foot_y = PR_HZ * 256 + (int)(((long long)(PR_YB - PR_HZ) * 256 * s) >> 16),
        sag = (int)(((long long)(flank_r - flank_l) * 41 / 256 * s) >> 16);
    /* the trunk's side towards the picture: the median flank of its lower rows */
    {
        int v[70],
            n = 0;
        for (int y = PR_YB - 70; y < PR_YB; y++) v[n++] = side ? pr_lo[y] : pr_hi[y];
        for (int i = 1; i < n; i++) {
            int t = v[i],
                k = i;
            while (k > 0 && v[k - 1] > t) {
                v[k] = v[k - 1];
                k--;
            }
            v[k] = t;
        }
        side_x = (v[34] + v[35]) / 2 + (side ? -256 : 256);
    }
    if (half_w < 256) return;
    for (int j = 0; j < pr_pad; j++) {
        int X = j * 256 + 128 - gap * 256,
            dx = (int)(((long long)X << 16) / s),
            src_x = side ? side_x + dx : side_x - dx;
        int across = (src_x - mid) * 256 / half_w,
            across_abs = across < 0 ? -across : across,
            water_y;
        if (across_abs > 294) continue;
        {
            int q = across_abs > 256 ? 256 : across_abs;
            water_y = foot_y - sag + sag * isqrt_i(65536 - q * q) / 256;
        } /* the waterline here */
        for (int y = 0; y < PH; y++) {
            int yy = y * 256 + 128,
                *p = pr_px(j, y),
                c[3];
            /* (floor) */
            int src_y =
                PR_HZ * 256 + (int)((((long long)(yy - PR_HZ * 256)) * 65536 - (yy < PR_HZ * 256 ? s - 1 : 0)) / s);
            if (yy <= water_y) {
                int bark_y = src_y >= PR_YT * 256 ? src_y : PR_YT * 256 + fmodi(PR_YT * 256 - src_y, 12 * 256),
                    bark_x = src_x,
                    grain;
                if (bark_y >= PR_YB * 256) bark_y = (PR_YB - 1) * 256 - (bark_y - PR_YB * 256);
                /* the foot: whole down to the waterline, in the bark of the clean rows above it (the pier's post
                   hides the picture's) */
                if (bark_y >= 138 * 256) {
                    if (across_abs > 256) continue;
                    bark_y = 137 * 256 + 128 - (bark_y - 138 * 256) / 2;
                    bark_x = clampi(src_x, flank_l + 256, flank_r - 512);
                } else if (src_x < pr_flank(pr_lo, bark_y) || src_x > pr_flank(pr_hi, bark_y))
                    continue;
                if (bark_y > 137 * 256 + 102) bark_y = 137 * 256 + 102;
                pr_bark(bark_x, bark_y, c);
                for (int k = 0; k < 3; k++) {
                    grain =
                        clampi(pr_D[60 + (y + seed * 31) % 76][200 + (j + (y / 76) * 5 + seed * 3) % 14][k], -22, 22);
                    c[k] += grain;
                    if (water_y - yy < 384) c[k] = c[k] * 154 / 256; /* the dark line where it meets the water */
                }
                /* the canopy stays in front */
                if (y < 34 && pr_folish(p) && (int)(hash3(j, y, seed) % 34u) >= y) continue;
                for (int k = 0; k < 3; k++) p[k] = clampi(c[k], 0, 255);
            } else { /* its reflection, from the waterline's curve down */
                int d, bark_y, wob, alpha, down;
                const int *q;
                if (across_abs > 256) continue;
                d = (int)(((long long)(yy - water_y) << 16) / s);
                bark_y = 137 * 256 - d * 230 / 256;
                if (bark_y < PR_YT * 256) continue;
                wob = (pr_sin(y * 587) * 205 + pr_sin(y * 241 + 652) * 154) >> 8;
                q = pr_S[clampi(bark_y >> 8, 0, PH - 1)]
                        [clampi((clampi(src_x + wob, flank_l + 256, flank_r - 512)) >> 8, 0, PW - 1)];
                /* 0..256 down to the frame's foot */
                down = (yy - water_y) / (PH - (water_y >> 8) > 1 ? PH - (water_y >> 8) : 1);
                alpha = 159 * (256 - down / 2) / 256;
                if (hash3(j / 3, y, seed + 40) % 100u < 14) alpha = alpha * 90 / 256; /* broken by the ripples */
                for (int k = 0; k < 3; k++) p[k] = (p[k] * (256 - alpha) + (q[k] * 3 / 4 + 55) * alpha) >> 8;
            }
        }
    }
}

/* --- the leaves cut by the edge, completed along their own axis */
static const short PR_DC[32] = { 1024, 1019, 1004, 980, 946, 903, 851, 792, 724, 650, 569, 483, 392, 297, 200, 100, 0, -100, -200,
    -297, -392, -483, -569, -650, -724, -792, -851, -903, -946, -980, -1004, -1019 };
static const short PR_DS[32] = { 0, 100, 200, 297, 392, 483, 569, 650, 724, 792, 851, 903, 946, 980, 1004, 1019, 1024, 1019, 1004,
    980, 946, 903, 851, 792, 724, 650, 569, 483, 392, 297, 200, 100 };
/* a lens' half width along its length (x 1024 of half its width; sin(pi s) ^ 0.7 x 1.05, 64 steps) */
static const short PR_LN[65] = { 0, 130, 212, 281, 342, 399, 452, 502, 549, 593, 635, 675, 713, 748, 782, 814, 844, 872, 898, 922, 945,
    966, 985, 1002, 1017, 1031, 1043, 1053, 1061, 1067, 1072, 1074, 1075, 1074, 1072, 1067, 1061, 1053, 1043, 1031, 1017, 1002,
    985, 966, 945, 922, 898, 872, 844, 814, 782, 748, 713, 675, 635, 593, 549, 502, 452, 399, 342, 281, 212, 130, 0 };
#define PR_MAXP 1024
#define PR_MAXL 96
/* centre x 256, axis x 1024, width / start / length x 256; colours across it from bin b0 */
typedef struct {
    int key, cx, cy, ax, ay, wid, u0, lt, nb, b0;
    int col[24][3];
    u8 has[24];
} PrLeaf;
static void pr_sorti(int *v, int n)
{
    for (int i = 1; i < n; i++) {
        int t = v[i],
            k = i;
        while (k > 0 && v[k - 1] > t) {
            v[k] = v[k - 1];
            k--;
        }
        v[k] = t;
    }
}
static int pr_pct(const int *v, int n, int p) /* the p % point of sorted v, between its order statistics */
{
    int pos = (n - 1) * p * 256 / 100,
        i = pos >> 8,
        f = pos & 255;
    return i + 1 < n ? v[i] + (v[i + 1] - v[i]) * f / 256 : v[n - 1];
}
static PrLeaf (*pr_leaves__L);
static short (*pr_leaves__px);
static short (*pr_leaves__py);
static u8 (*pr_leaves__seen)[PW];
static int (*pr_leaves__us);
static int (*pr_leaves__vs);
static int (*pr_leaves__tmp);
static void pr_leaves(int side)
{
    PrLeaf (*const L) = pr_leaves__L;
    short (*const px) = pr_leaves__px,
          (*const py) = pr_leaves__py;
    u8 (*const seen)[PW] = pr_leaves__seen;
    int (*const us) = pr_leaves__us,
        (*const vs) = pr_leaves__vs,
        (*const tmp) = pr_leaves__tmp;
    int edge = side ? PW - 1 : 0,
        inward = side ? -1 : 1,
        leaf_n = 0,
        y = 0;
    while (y < PH && leaf_n < PR_MAXL) {
        int run0 = y,
            run1,
            col[3] = { 0, 0, 0 },
            npx = 0,
            next, n;
        if (pr_lab[y][edge] != 2) {
            y++;
            continue;
        }
        run1 = y + 1;
        while (run1 < PH && pr_lab[run1][edge] == 2 && pr_dist(pr_C[run1][edge], pr_C[run1 - 1][edge]) < 70 &&
               pr_dist(pr_C[run1][edge], pr_C[run0][edge]) < 110)
            run1++;
        y = run1;
        n = run1 - run0;
        for (int k = run0; k < run1; k++)
            for (int c = 0; c < 3; c++) col[c] += pr_C[k][edge][c];
        for (int c = 0; c < 3; c++) col[c] /= n;
        {
            int m = 0; /* cut?  the strip beside it is something else */
            for (int k = run0; k < run1 && m < PR_MAXP; k++) tmp[m++] = pr_dist(pr_px(0, k), col);
            pr_sorti(tmp, m);
            if ((m & 1 ? tmp[m / 2] : (tmp[m / 2 - 1] + tmp[m / 2]) / 2) < 60) continue;
        }
        /* the leaf's texels: similar colours joined to the run, 26 columns in */
        memset(seen, 0, (sizeof *seen * (PH)));
        for (int k = run0; k < run1 && npx < PR_MAXP; k++) {
            px[npx] = (short)edge;
            py[npx] = (short)k;
            seen[k][edge] = 1;
            npx++;
        }
        next = 0;
        while (next < npx) {
            static const signed char DX[4] = { 1, -1, 0, 0 },
                                DY[4] = { 0, 0, 1, -1 };
            int x = px[next],
                yy = py[next];
            next++;
            for (int d = 0; d < 4; d++) {
                int nx = x + DX[d],
                    ny = yy + DY[d];
                if (nx < 0 || nx >= PW || ny < 0 || ny >= PH || abs(nx - edge) >= 26 || seen[ny][nx] ||
                    pr_lab[ny][nx] != 2)
                    continue;
                if (pr_dist(pr_C[ny][nx], col) >= 70 || pr_dist(pr_C[ny][nx], pr_C[yy][x]) >= 60 || npx >= PR_MAXP)
                    continue;
                seen[ny][nx] = 1;
                px[npx] = (short)nx;
                py[npx] = (short)ny;
                npx++;
            }
        }
        {
            PrLeaf *l = &L[leaf_n];
            int ok = 0,
                cx = 0,
                cy = 0,
                bd = 0,
                wid = 0,
                seen_len = 0,
                umin = 0;
            long long best_var = -1,
                      perp_var = 0;
            if (npx >= 4 && npx < 160) {
                for (int i = 0; i < npx; i++) {
                    cx += px[i] * 256 + 128;
                    cy += py[i] * 256 + 128;
                }
                cx /= npx;
                cy /= npx;
                for (int d = 0; d < 32; d++) { /* the axis: the direction its texels spread most along */
                    long long v = 0;
                    for (int i = 0; i < npx; i++) {
                        long long u = ((long long)(px[i] * 256 + 128 - cx) * PR_DC[d] +
                                       (long long)(py[i] * 256 + 128 - cy) * PR_DS[d]) >>
                                      10;
                        v += u * u;
                    }
                    if (v > best_var) {
                        best_var = v;
                        bd = d;
                    }
                }
                {
                    int ax = PR_DC[bd],
                        ay = PR_DS[bd],
                        umax = 0;
                    /* pointing out of the picture */
                    if (ax * -inward < 0) {
                        ax = -ax;
                        ay = -ay;
                    }
                    for (int i = 0; i < npx; i++) {
                        int ddx = px[i] * 256 + 128 - cx,
                            ddy = py[i] * 256 + 128 - cy;
                        us[i] = (ddx * ax + ddy * ay) >> 10;
                        vs[i] = (-ddx * ay + ddy * ax) >> 10;
                        perp_var += (long long)vs[i] * vs[i];
                        if (!i || us[i] < umin) umin = us[i];
                        if (!i || us[i] > umax) umax = us[i];
                    }
                    memcpy(tmp, vs, sizeof(int) * (size_t)npx);
                    pr_sorti(tmp, npx);
                    wid = pr_pct(tmp, npx, 95) - pr_pct(tmp, npx, 5) + 256;
                    if (wid < 256) wid = 256;
                    seen_len = umax - umin + 256;
                    ok = best_var * 2 > perp_var * 5 && wid <= 1664 && abs(ax) > 205;
                    if (ok) {
                        int want = wid * 42 / 10 + (int)(hash3(run0, run1, side + 7) % 4u) * 256,
                            lt = seen_len + 768;
                        if (want > lt) lt = want;
                        if (lt > seen_len + 4096) lt = seen_len + 4096;
                        l->cx = cx;
                        l->cy = cy;
                        l->ax = ax;
                        l->ay = ay;
                        l->wid = wid;
                        l->u0 = umin;
                        l->lt = lt;
                    }
                }
            }
            if (!ok) { /* no clear leaf: a short tip, straight out */
                npx = 0;
                for (int k = run0; k < run1 && npx < PR_MAXP; k++) {
                    px[npx] = (short)edge;
                    py[npx] = (short)k;
                    npx++;
                }
                l->cx = edge * 256 + 128;
                l->cy = (run0 + run1) * 128;
                l->ax = -inward * 1024;
                l->ay = 0;
                l->wid = n < 6 ? n * 256 : 1536;
                l->lt = l->wid * 3 / 2 + 512;
                l->u0 = -l->lt * 3 / 10;
                for (int i = 0; i < npx; i++) {
                    us[i] = 0;
                    vs[i] = ((py[i] * 256 + 128 - l->cy) * l->ax) >> 10;
                }
            }
            {
                /* its colours across its width, from its texels */
                int cnt[24],
                    lo = 0,
                    hi = 0;
                for (int i = 0; i < npx; i++) {
                    int b = fdiv(vs[i] + 128, 256);
                    if (!i || b < lo) lo = b;
                    if (!i || b > hi) hi = b;
                }
                if (hi - lo > 23) hi = lo + 23;
                l->b0 = lo;
                l->nb = hi - lo + 1;
                memset(cnt, 0, sizeof cnt);
                memset(l->col, 0, sizeof l->col);
                memset(l->has, 0, sizeof l->has);
                for (int i = 0; i < npx; i++) {
                    int b = fdiv(vs[i] + 128, 256) - lo;
                    if (b < 0 || b > 23) continue;
                    for (int c = 0; c < 3; c++) l->col[b][c] += pr_C[py[i]][px[i]][c];
                    cnt[b]++;
                }
                for (int b = 0; b < l->nb; b++)
                    if (cnt[b]) {
                        l->has[b] = 1;
                        for (int c = 0; c < 3; c++) l->col[b][c] /= cnt[b];
                    }
            }
            l->key = (col[0] + col[1] + col[2]) * 256 + leaf_n; /* darker leaves first, the lighter ones over them */
            leaf_n++;
        }
    }
    for (int i = 1; i < leaf_n; i++) {
        PrLeaf t = L[i];
        int k = i;
        while (k > 0 && L[k - 1].key > t.key) {
            L[k] = L[k - 1];
            k--;
        }
        L[k] = t;
    }
    for (int i = 0; i < leaf_n; i++) {
        const PrLeaf *l = &L[i];
        for (int j = 0; j < pr_pad; j++) {
            int x8 = (side ? PW + j : -1 - j) * 256 + 128,
                any = 0;
            for (int yy = 0; yy < PH; yy++) {
                int ddx = x8 - l->cx,
                    ddy = yy * 256 + 128 - l->cy;
                int uu = (ddx * l->ax + ddy * l->ay) >> 10,
                    vv = (-ddx * l->ay + ddy * l->ax) >> 10,
                    av = vv < 0 ? -vv : vv,
                    s, half_w, q, b,
                    bestb = -1,
                    bd = 0;
                if (av > l->wid || uu < l->u0 || uu > l->u0 + l->lt) continue;
                s = (uu - l->u0) * 64 / l->lt;
                {
                    int f = ((uu - l->u0) * 64 * 256 / l->lt) & 255;
                    half_w = l->wid / 2 * (PR_LN[s] * (256 - f) + PR_LN[s < 64 ? s + 1 : 64] * f) / (1024 * 256);
                }
                if (av > half_w) continue;
                q = vv * (l->wid / 2) / (half_w > 128 ? half_w : 128); /* across the lens as across the leaf */
                for (b = 0; b < l->nb; b++) {
                    int d = abs((l->b0 + b) * 256 - q);
                    if (l->has[b] && (bestb < 0 || d < bd)) {
                        bestb = b;
                        bd = d;
                    }
                }
                if (bestb < 0) continue;
                memcpy(pr_px(j, yy), l->col[bestb], sizeof(int[3]));
                any = 1;
            }
            if (!any && j > 2) break;
        }
    }
}

/* --- the light: the glow about G = (12, -44) by its radial profile, the rays that cross the edge along their lines
   from C = (30.25, -15.25), added to the strip */
static int (*pr_light__g);
static int (*pr_light__g2);
static int (*pr_light__hist)[32];
static int (*pr_light__cnt);
static void pr_light(int side)
{
    static int ray_edge[PH];
    int (*const g) = pr_light__g,
        (*const g2) = pr_light__g2,
        (*const hist)[32] = pr_light__hist,
        (*const cnt) = pr_light__cnt;
    int edge_x = side ? PW - 1 : 0;
    memset(hist, 0, (sizeof *hist * (400)));
    memset(cnt, 0, (sizeof *cnt * (400)));
    for (int y = 0; y < PH; y++)
        for (int x = 0; x < PW; x++) {
            int d2 = (x - 12) * (x - 12) + (y + 44) * (y + 44);
            for (int r = isqrt_i(d2) - 3; r <= isqrt_i(d2) + 2; r++) {
                if (r < 0 || r >= 400) continue;
                if ((r >= 2 ? d2 >= (r - 2) * (r - 2) : 1) && d2 < (r + 3) * (r + 3)) {
                    hist[r][pr_ray[y][x] >> 3]++;
                    cnt[r]++;
                }
            }
        }
    for (int r = 0; r < 400; r++) { /* the 15 % point of each ring: the glow without the rays */
        g[r] = 0;
        if (cnt[r] > 20) {
            int pos = (cnt[r] - 1) * 15,
                i = pos / 100,
                f = pos % 100,
                ray = -1,
                b = -1,
                acc = 0;
            for (int light = 0; light < 32 && b < 0; light++) {
                acc += hist[r][light];
                if (ray < 0 && acc > i) ray = light;
                if (acc > i + 1) b = light;
            }
            if (b < 0) b = ray;
            g[r] = (ray * 8 * (100 - f) + b * 8 * f) * 256 / 100;
        }
    }
    for (int r = 0; r < 400; r++) {
        int s = 0;
        for (int k = -8; k <= 8; k++) s += g[clampi(r + k, 0, 399)];
        g2[r] = s / 17;
    }
#define PR_GLOW(x, y) g2[clampi(isqrt_i(((x) - 12) * ((x) - 12) + ((y) + 44) * ((y) + 44)), 0, 399)]
    for (int y = 0; y < PH; y++)
        ray_edge[y] =
            (pr_ray[clampi(y - 1, 0, PH - 1)][edge_x] + pr_ray[y][edge_x] + pr_ray[clampi(y + 1, 0, PH - 1)][edge_x]) *
                256 / 3 -
            PR_GLOW(edge_x, y);
    for (int y = 0; y < PH; y++)
        for (int j = 0; j < pr_pad; j++) {
            int x = side ? PW + j : -1 - j,
                num = 4 * edge_x - 121,
                den = 4 * x + 2 - 121;
            /* x 256 */
            int ray_y = -61 * 64 + (int)((long long)(4 * y + 2 + 61) * 64 * num / den),
                light = PR_GLOW(x, y),
                *p = pr_px(j, y);
            if (ray_y >= 0 && ray_y < (PH - 1) * 256) {
                int k = ray_y >> 8,
                    f = ray_y & 255,
                    ray = (ray_edge[k] * (256 - f) + ray_edge[k + 1] * f) >> 8;
                if (ray > 0) light += ray;
            } else if (ray_y < 0 && ray_edge[0] > 0) {
                int w = 256 + ray_y / 12;
                if (w > 0) light += ray_edge[0] * w / 256;
            }
            for (int c = 0; c < 3; c++) {
                p[c] += light >> 8;
                if (p[c] > 248) p[c] = 248;
            }
        }
#undef PR_GLOW
}

/* both strips: the picture carried on, a near tree, the leaves the edge cuts completed, then the light */
static void gen_pr(int pad)
{
    if (!ws_need_gen_pr()) return;
    static const int NEAR[2] = { 111411, 98042 }; /* the near trees: 1.7 and 1.496 times nearer */
    if (!pr_have_layers) return;
    pr_scene();
    pr_trunk();
    pr_cv = malloc(sizeof(int[3]) * (size_t)PH * (size_t)pad);
    pr_pad = pad;
    if (!pr_cv) return;
    for (int side = 0; side < 2; side++) {
        pr_base(side);
        pr_tree(side, NEAR[side], side ? 2 : 1, side + 1);
        pr_leaves(side);
        pr_light(side);
        for (int y = 0; y < PH; y++)
            for (int j = 0; j < pad; j++) {
                const int *p = pr_px(j, y);
                strip[(size_t)y * 2 * pad + (side ? pad + j : pad - 1 - j)] =
                    (u16)(clampi(p[0], 0, 248) >> 3 | (clampi(p[1], 0, 248) >> 3) << 5 |
                          (clampi(p[2], 0, 248) >> 3) << 10);
            }
    }
    free(pr_cv);
    pr_cv = 0;
}
