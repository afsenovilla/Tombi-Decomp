/* --fps N|auto|unlimited: high frame rates at the original speed (docs/PORT.md "High frame rates").  Included by gpu.c.
 *
 * The game keeps running one logic step per vblank (50 Hz PAL, or --hz 60); only the presents get their own clock: the
 * display's refresh (auto), a cap (60 / 90 / 120 / 144 ...) or as fast as possible (unlimited, vsync off).  Each
 * in-between present shows the displayed frame re-drawn with every primitive moved back towards where it was in the
 * frame displayed before it, so motion is smooth while the textures, colours and everything else come from the newest
 * frame.  No more latency than the 50 Hz path: the blend runs from the frame that path shows at a vblank to the one it
 * would show at the next (the frame the game finished and flipped to in between).
 *
 *  - Recording (both renderers, here, not in a backend): while --fps is on, every drawing operation the decoder hands to
 *    the backend (polygons as whole packets, sprites, lines, fills, VRAM copies, host uploads with their pixels, the
 *    wide-plane operations of --widescreen) is appended to the record of the current vblank, with the GP0 state it was
 *    drawn with.  A record starts with a snapshot of VRAM (and the wide plane): software, a copy of vram[] / wvram[];
 *    OpenGL, a copy of the textures (gpu_gl.c port_gl_hfr_snap).
 *  - Identity: each primitive's signature is what makes it the same thing from one frame to the next: its kind, vertex
 *    count, flags, blend mode, plane, texture page and CLUT, the texture coordinates of every vertex (and a sprite's
 *    size); its position is taken relative to the origin of its drawing area, so the two halves of the double buffer
 *    line up.  The game's C draws through the GTE into memory it then copies around, so a tag on the GTE result would
 *    not survive to the packet; the texture coordinates of a face are fixed by its model and are the stable part.
 *  - Which frame: the game flips right after the vblank (FlipFrame: PutDispEnv + DrawOTag), so at the first present
 *    after it (hfr_sync) the newest closed record that drew into the displayed area is the displayed frame C; when it
 *    changes it is paired with the frame displayed before it, P.
 *  - Pairing (once per new displayed frame, ~50 us), in rounds of increasing distance so the closest candidate wins:
 *      1. the same signature at the same place (static scenery, the HUD, text);
 *      2. the same signature within 2, 6, 16, 40 pixels, every vertex its own delta (3D faces turn and scale);
 *      3. animated sprites (texture coordinates changed: a new animation frame) by texture page / CLUT / kind and a
 *         similar size within 4, 24 pixels, moved as a whole by their centre.
 *    Primitives whose texels were rewritten between P and C (a background streamed into a ring of texture columns as
 *    the camera moves: the same u/v then shows another picture) only match in place; moved, they take the motion most
 *    of the other moved primitives share (the camera's).  A primitive without a partner (it appeared this frame) is
 *    drawn where it is now.  A frame where few primitives found a partner (a scene change) or where more of them
 *    jumped than moved (a camera cut, a teleport) is not blended, nor one where nothing moved.
 *  - Replay: the displayed record on its snapshot (software: a work copy of vram / wvram the arrays are pointed at;
 *    OpenGL: work targets standing in for the real ones), each vertex at C + (P - C) * (1 - alpha), then the normal
 *    present path shows it.  OpenGL above 1x internal resolution also gets the sub-pixel rest (port_gpu_subpx).
 *  - Not blended (shown as the game drew them): 24-bit frames (the movies), a blank display, frames where the --debug-menu
 *    overlay is up, the first frame after a scene change or cut.  The second-screen panel is its own window, untouched.
 *
 * Off (the default): hfr_rec stays 0 and nothing below runs; presents happen once per vblank as before. */
int port_hfr_fps;                   /* 0 = off; N > 0 = presents per second (cap); -1 = auto (display refresh); -2 = unlimited */
const char *port_hfr_dump_dir;      /* --dump-presents DIR[:FROM:TO]: every present of vblanks FROM..TO as present_N.png */
int port_hfr_dump_from, port_hfr_dump_to = 0x7FFFFFFF;
/* (hfr_overlay_frame, gpu.c: the vblank the --debug-menu overlay last drew into the displayed frame) */

enum { H_POLY, H_RECT, H_LINE, H_FILL, H_COPY, H_UP, H_WSDOWN };
enum { HF_SHADE = 1, HF_SEMI = 2, HF_RAW = 4, HF_TEX = 8 };
typedef struct {
    unsigned char kind, nv, flags, wide;
    int st;                         /* index of its GP0 state in HRec.st (drawing ops) */
    int v0;                         /* first vertex in HRec.v (drawing ops), first word in HRec.data (H_UP) */
    GpuTex t;
    int w, h;                       /* H_RECT */
    int a[6];                       /* H_FILL x y w h colour; H_COPY / H_WSDOWN sx sy dx dy w h; H_UP x y w h */
} HOp;
typedef struct { int op; u32 sig, lsig; float ox, oy, cx, cy; int bw, bh; short tr[4]; } HPrim;   /* tr: VRAM words its texels come from (x1 y1 x2 y2), x1 > x2 = none */
typedef struct {
    unsigned id;                    /* 0 = being recorded or empty */
    int bad;                        /* too big: not used */
    HOp *ops; int nops, capops;
    GpuVtx *v; int nv, capv;
    GpuState *st; int nst, capst;
    u16 *data; int ndata, capdata;
    HPrim *p; int np, capp, built;
    float *d; int capd;             /* per vertex: (earlier - this), set when this record is the displayed one of a pair */
    u16 *snapV, *snapW;             /* software: vram / wvram when the record started */
    short wr[8][5]; int nwr;        /* the rectangles its replay writes (x1 y1 x2 y2 wide), nwr < 0 = not worked out yet (OpenGL) */
} HRec;
#define HNREC 4
#define HFR_MAX_OPS 400000
static HRec hrec[HNREC];
static int hcur, h_init, h_pair_ok, h_since, h_span = 1, h_block, h_gl;
static unsigned hseq, h_disp, h_prev;
static int hd_x, hd_y, hd_w, hd_h, hd_wide;
static u16 *h_workV, *h_workW;
/* what the presents take (us), for port_hfr_wait, which starts none that would end past the next vblank: h_cost a replay
   (OpenGL: until the GPU has finished it), h_pc a present (the swap); each the running mean plus twice the running mean
   deviation (h_est), as TCP estimates a round trip */
typedef struct { double m, d; } HEst;
static HEst he_cost, he_pc;
static unsigned long long h_cost, h_pc;
static unsigned long long h_est(HEst *e, unsigned long long us) {
    double x = (double)us, dv = x > e->m ? x - e->m : e->m - x;
    if (e->m == 0 && e->d == 0) { e->m = x; e->d = x / 2; }
    else { e->d += (dv - e->d) / 4; e->m += (x - e->m) / 8; }
    return (unsigned long long)(e->m + 2 * e->d);
}
/* statistics (-v at exit, --dump-presents) */
static unsigned long long hs_pair_us, hs_replay_us, hs_presents, hs_interp, hs_pairs, hs_cuts, hs_scene, hs_vblanks, hs_late, hs_plain;
static unsigned long long hs_static, hs_exact, hs_loose, hs_new;

static void *h_grow(void *p, int *cap, int need, size_t sz) {
    if (need <= *cap) return p;
    int c = *cap ? *cap : 1024;
    while (c < need) c *= 2;
    p = realloc(p, (size_t)c * sz);
    if (!p) port_fatal("--fps: out of memory");
    *cap = c;
    return p;
}
static HOp *h_op(int kind) {
    HRec *r = &hrec[hcur];
    if (r->bad) return 0;
    if (r->nops >= HFR_MAX_OPS) { r->bad = 1; return 0; }
    r->ops = (HOp *)h_grow(r->ops, &r->capops, r->nops + 1, sizeof(HOp));
    HOp *o = &r->ops[r->nops++];
    memset(o, 0, sizeof *o);
    o->kind = (unsigned char)kind;
    return o;
}
static int h_state(void) {
    HRec *r = &hrec[hcur];
    if (r->nst && !memcmp(&r->st[r->nst - 1], &gs, sizeof gs)) return r->nst - 1;
    r->st = (GpuState *)h_grow(r->st, &r->capst, r->nst + 1, sizeof(GpuState));
    r->st[r->nst] = gs;
    return r->nst++;
}
static GpuVtx *h_vtx(HOp *o, int n) {
    HRec *r = &hrec[hcur];
    r->v = (GpuVtx *)h_grow(r->v, &r->capv, r->nv + n, sizeof(GpuVtx));
    o->v0 = r->nv; o->nv = (unsigned char)n; r->nv += n;
    return &r->v[o->v0];
}
static void hfr_poly(const Vtx *v, int nv, int shade, const GpuTex *t, int semi, int raw) {
    HOp *o = h_op(H_POLY);
    if (!o) return;
    o->flags = (unsigned char)((shade ? HF_SHADE : 0) | (semi ? HF_SEMI : 0) | (raw ? HF_RAW : 0) | (t ? HF_TEX : 0));
    o->st = h_state();
    if (t) o->t = *t;
    memcpy(h_vtx(o, nv), v, (size_t)nv * sizeof(GpuVtx));
}
static void hfr_rect(int x, int y, int w, int h, u32 col, const GpuTex *t, int u0, int v0, int semi, int raw) {
    HOp *o = h_op(H_RECT);
    if (!o) return;
    o->flags = (unsigned char)((semi ? HF_SEMI : 0) | (raw ? HF_RAW : 0) | (t ? HF_TEX : 0));
    o->st = h_state();
    if (t) o->t = *t;
    o->w = w; o->h = h;
    GpuVtx *p = h_vtx(o, 1);
    p->x = x; p->y = y; p->r = col & 255; p->g = (col >> 8) & 255; p->b = (col >> 16) & 255; p->u = u0; p->v = v0;
}
static void hfr_line(int x0, int y0, u32 c0, int x1, int y1, u32 c1, int shade, int semi) {
    HOp *o = h_op(H_LINE);
    if (!o) return;
    o->flags = (unsigned char)((shade ? HF_SHADE : 0) | (semi ? HF_SEMI : 0));
    o->st = h_state();
    GpuVtx *p = h_vtx(o, 2);
    memset(p, 0, 2 * sizeof *p);
    p[0].x = x0; p[0].y = y0; p[0].r = c0 & 255; p[0].g = (c0 >> 8) & 255; p[0].b = (c0 >> 16) & 255;
    p[1].x = x1; p[1].y = y1; p[1].r = c1 & 255; p[1].g = (c1 >> 8) & 255; p[1].b = (c1 >> 16) & 255;
}
static void hfr_fill(int x, int y, int w, int h, u16 c, int wide) {
    HOp *o = h_op(H_FILL);
    if (!o) return;
    o->wide = (unsigned char)wide; o->a[0] = x; o->a[1] = y; o->a[2] = w; o->a[3] = h; o->a[4] = c;
}
static void hfr_copy(int sx, int sy, int dx, int dy, int w, int h) {
    HOp *o = h_op(H_COPY);
    if (!o) return;
    o->a[0] = sx; o->a[1] = sy; o->a[2] = dx; o->a[3] = dy; o->a[4] = w; o->a[5] = h;
}
static void hfr_wsdown(int wx, int wy, int vx, int vy, int w, int h) {
    HOp *o = h_op(H_WSDOWN);
    if (!o) return;
    o->a[0] = wx; o->a[1] = wy; o->a[2] = vx; o->a[3] = vy; o->a[4] = w; o->a[5] = h;
}
/* a host write (already in vram / wvram): its pixels as they are now */
static void hfr_upload(int x, int y, int w, int h, int wide) {
    HRec *r = &hrec[hcur];
    if (w <= 0 || h <= 0 || w > 2048 || h > 512) return;
    if (wide && x + w > WS_STRIDE) w = WS_STRIDE - x;
    HOp *o = h_op(H_UP);
    if (!o) return;
    o->wide = (unsigned char)wide; o->a[0] = x; o->a[1] = y; o->a[2] = w; o->a[3] = h;
    r->data = (u16 *)h_grow(r->data, &r->capdata, r->ndata + w * h, sizeof(u16));
    o->v0 = r->ndata;
    u16 *d = r->data + r->ndata;
    for (int j = 0; j < h; j++)
        for (int i = 0; i < w; i++) *d++ = wide ? wvram[(y + j) & 511][x + i] : vram[(y + j) & 511][(x + i) & 1023];
    r->ndata += w * h;
}

/* ------------------------------------------------------------------ records */
static HRec *h_by_id(unsigned id) {
    for (int i = 0; i < HNREC; i++) if (id && hrec[i].id == id) return &hrec[i];
    return 0;
}
/* the next record starts now in slot i, with a snapshot of the targets */
static void h_start(int i) {
    HRec *r = &hrec[i];
    hcur = i;
    r->id = 0; r->bad = 0; r->nops = r->nv = r->nst = r->ndata = r->np = 0; r->built = 0; r->nwr = -1;
    if (h_gl) { extern void port_gl_hfr_snap(int); port_gl_hfr_snap(i); return; }
    if (!r->snapV && !(r->snapV = (u16 *)malloc(sizeof vram_store))) port_fatal("--fps: out of memory");
    memcpy(r->snapV, vram_store, sizeof vram_store);
    if (port_widescreen) {
        if (!r->snapW && !(r->snapW = (u16 *)malloc(sizeof wvram_store))) port_fatal("--fps: out of memory");
        memcpy(r->snapW, wvram_store, sizeof wvram_store);
    }
}
static u32 h_mix(u32 h, u32 v) { return (h ^ v) * 16777619u; }
static void h_build(HRec *r) {
    if (r->built) return;
    r->built = 1; r->np = 0;
    for (int i = 0; i < r->nops; i++) {
        const HOp *o = &r->ops[i];
        if (o->kind > H_LINE) continue;
        const GpuState *s = &r->st[o->st];
        const GpuVtx *v = &r->v[o->v0];
        r->p = (HPrim *)h_grow(r->p, &r->capp, r->np + 1, sizeof(HPrim));
        HPrim *p = &r->p[r->np++];
        u32 h = 2166136261u;
        h = h_mix(h, (u32)o->kind | (u32)o->nv << 4 | (u32)(o->flags & ~HF_SHADE) << 8 | (u32)(o->flags & HF_SEMI ? s->abr : 0) << 12 | (u32)s->wide << 14);
        if (o->flags & HF_TEX) { h = h_mix(h, (u32)o->t.tx | (u32)o->t.ty << 11 | (u32)o->t.depth << 20); h = h_mix(h, (u32)o->t.cx | (u32)o->t.cy << 11); }
        p->lsig = h;
        if (o->kind == H_RECT) h = h_mix(h, (u32)o->w | (u32)o->h << 12);
        if (o->flags & HF_TEX) for (int k = 0; k < o->nv; k++) h = h_mix(h, (u32)v[k].u | (u32)v[k].v << 8);
        p->sig = h;
        p->op = i; p->ox = (float)s->x1; p->oy = (float)s->y1;
        int x0 = v[0].x, x1 = v[0].x, y0 = v[0].y, y1 = v[0].y; float cx = 0, cy = 0;
        for (int k = 0; k < o->nv; k++) {
            cx += v[k].x; cy += v[k].y;
            if (v[k].x < x0) x0 = v[k].x; if (v[k].x > x1) x1 = v[k].x; if (v[k].y < y0) y0 = v[k].y; if (v[k].y > y1) y1 = v[k].y;
        }
        cx /= o->nv; cy /= o->nv;
        if (o->kind == H_RECT) { cx += o->w * 0.5f; cy += o->h * 0.5f; p->bw = o->w; p->bh = o->h; }
        else { p->bw = x1 - x0; p->bh = y1 - y0; }
        p->cx = cx - p->ox; p->cy = cy - p->oy;
        p->tr[0] = 1; p->tr[2] = 0;
        if (o->flags & HF_TEX) {
            int u0 = 255, u1 = 0, w0 = 255, w1 = 0, sh = o->t.depth == 0 ? 2 : o->t.depth == 1 ? 1 : 0;
            if (o->kind == H_RECT) { u0 = v[0].u; w0 = v[0].v; u1 = u0 + o->w - 1; w1 = w0 + o->h - 1; if (u1 > 255) { u0 = 0; u1 = 255; } if (w1 > 255) { w0 = 0; w1 = 255; } }
            else for (int k = 0; k < o->nv; k++) {
                if (v[k].u < u0) u0 = v[k].u; if (v[k].u > u1) u1 = v[k].u; if (v[k].v < w0) w0 = v[k].v; if (v[k].v > w1) w1 = v[k].v;
            }
            p->tr[0] = (short)(o->t.tx + (u0 >> sh)); p->tr[2] = (short)(o->t.tx + (u1 >> sh));
            p->tr[1] = (short)(o->t.ty + w0); p->tr[3] = (short)(o->t.ty + w1);
        }
    }
}
/* the deltas of C's vertices towards the matching primitives of P; 0 = do not blend this pair (scene change, cut) */
static int h_pair(HRec *C, HRec *P) {
    enum { HB = 8192, SCAN = 96 };
    static int head[HB], lhead[HB], *nx, *lnx, capn; static int *mt, capm; static unsigned char *used; static int capu;
    unsigned long long t0 = port_time_us();
    h_build(C); h_build(P);
    C->d = (float *)h_grow(C->d, &C->capd, (C->nv ? C->nv : 1) * 2, sizeof(float));
    memset(C->d, 0, (size_t)C->nv * 2 * sizeof(float));
    nx = (int *)h_grow(nx, &capn, P->np * 2 + 2, sizeof(int)); lnx = nx + P->np + 1;
    used = (unsigned char *)h_grow(used, &capu, P->np + 1, 1); memset(used, 0, (size_t)P->np + 1);
    mt = (int *)h_grow(mt, &capm, C->np + 1, sizeof(int));
    for (int i = 0; i < C->np; i++) mt[i] = -1;
    for (int i = 0; i < HB; i++) head[i] = lhead[i] = -1;
    for (int i = P->np - 1; i >= 0; i--) {
        u32 b = P->p[i].sig % HB, lb = P->p[i].lsig % HB;
        nx[i] = head[b]; head[b] = i; lnx[i] = lhead[lb]; lhead[lb] = i;
    }
    /* texels rewritten since P was drawn (a background streamed into a ring of texture columns as the camera moves, an
       animated texture): the same texture coordinates no longer mean the same picture, so those primitives are matched
       only at the very same place; moved, they follow the camera (the motion most of the other primitives share) */
    static unsigned char *flag; static int capf; static short (*up)[4]; static int capup; int nup = 0;
    flag = (unsigned char *)h_grow(flag, &capf, C->np + 1, 1); memset(flag, 0, (size_t)C->np + 1);
    for (unsigned id = P->id; id && id <= C->id; id++) {
        const HRec *q = h_by_id(id);
        if (!q) continue;
        for (int j = 0; j < q->nops; j++) {
            const HOp *o = &q->ops[j];
            if (o->kind != H_UP || o->wide || o->a[3] <= 1) continue;      /* (one-line uploads are CLUTs: colours only) */
            up = (short (*)[4])h_grow(up, &capup, nup + 1, sizeof *up);
            up[nup][0] = (short)o->a[0]; up[nup][1] = (short)o->a[1]; up[nup][2] = (short)(o->a[0] + o->a[2] - 1); up[nup][3] = (short)(o->a[1] + o->a[3] - 1);
            nup++;
        }
    }
    for (int i = 0; i < C->np && nup; i++) {
        const short *t = C->p[i].tr;
        if (t[0] > t[2]) continue;
        for (int k = 0; k < nup; k++)
            if (t[0] <= up[k][2] && t[2] >= up[k][0] && t[1] <= up[k][3] && t[3] >= up[k][1]) { flag[i] = 1; break; }
    }
    static const float exact_t[5] = { 0.0f, 2.0f, 6.0f, 16.0f, 40.0f }, loose_t[2] = { 4.0f, 24.0f };
    int n_static = 0, n_exact = 0, n_loose = 0, n_follow = 0;
    for (int round = 0; round < 7; round++) {
        int loose = round >= 5;
        float th = loose ? loose_t[round - 5] : exact_t[round];
        for (int i = 0; i < C->np; i++) {
            if (mt[i] >= 0 || (round && flag[i])) continue;
            const HPrim *c = &C->p[i]; const HOp *co = &C->ops[c->op]; const GpuVtx *cv = &C->v[co->v0];
            int best = -1, scan = 0; float bd = th + 0.001f;
            for (int j = loose ? lhead[c->lsig % HB] : head[c->sig % HB]; j >= 0 && scan < SCAN; j = loose ? lnx[j] : nx[j]) {
                const HPrim *q = &P->p[j];
                if (used[j] || (loose ? q->lsig != c->lsig : q->sig != c->sig)) continue;
                const HOp *qo = &P->ops[q->op];
                if (qo->nv != co->nv) continue;
                scan++;
                float m = 0;
                if (loose) {
                    int dw = q->bw - c->bw, dh = q->bh - c->bh, lim_w = c->bw / 3 > 4 ? c->bw / 3 : 4, lim_h = c->bh / 3 > 4 ? c->bh / 3 : 4;
                    if (dw < -lim_w || dw > lim_w || dh < -lim_h || dh > lim_h) continue;
                    float dx = q->cx - c->cx, dy = q->cy - c->cy;
                    m = dx < 0 ? -dx : dx; if ((dy < 0 ? -dy : dy) > m) m = dy < 0 ? -dy : dy;
                } else {
                    const GpuVtx *qv = &P->v[qo->v0];
                    for (int k = 0; k < co->nv && m < bd; k++) {
                        float dx = (qv[k].x - q->ox) - (cv[k].x - c->ox), dy = (qv[k].y - q->oy) - (cv[k].y - c->oy);
                        if (dx < 0) dx = -dx; if (dy < 0) dy = -dy;
                        if (dx > m) m = dx; if (dy > m) m = dy;
                    }
                }
                if (m < bd) { bd = m; best = j; if (m == 0) break; }
            }
            if (best < 0) continue;
            used[best] = 1; mt[i] = best;
            const HPrim *q = &P->p[best]; const HOp *qo = &P->ops[q->op]; const GpuVtx *qv = &P->v[qo->v0];
            float *d = C->d + (size_t)co->v0 * 2;
            for (int k = 0; k < co->nv; k++) {
                if (loose) { d[2 * k] = q->cx - c->cx; d[2 * k + 1] = q->cy - c->cy; }
                else { d[2 * k] = (qv[k].x - q->ox) - (cv[k].x - c->ox); d[2 * k + 1] = (qv[k].y - q->oy) - (cv[k].y - c->oy); }
            }
            if (loose) n_loose++; else if (round == 0) n_static++; else n_exact++;
        }
    }
    if (nup) {   /* the camera's motion: the delta most of the moved, unflagged primitives share (whole pixels) */
        enum { MB = 64 }; static int hx[MB], hy[MB], hn[MB]; int nb = 0, tot = 0, best = -1;
        for (int i = 0; i < C->np; i++) {
            if (mt[i] < 0 || flag[i]) continue;
            const HOp *co = &C->ops[C->p[i].op]; const float *d = C->d + (size_t)co->v0 * 2;
            float sx = 0, sy = 0; for (int k = 0; k < co->nv; k++) { sx += d[2 * k]; sy += d[2 * k + 1]; }
            int dx = (int)(sx / co->nv + (sx < 0 ? -0.5f : 0.5f)), dy = (int)(sy / co->nv + (sy < 0 ? -0.5f : 0.5f)), b;
            if (!dx && !dy) continue;
            tot++;
            for (b = 0; b < nb; b++) if (hx[b] == dx && hy[b] == dy) break;
            if (b == nb) { if (nb == MB) continue; hx[b] = dx; hy[b] = dy; hn[b] = 0; nb++; }
            hn[b]++;
        }
        for (int b = 0; b < nb; b++) if (best < 0 || hn[b] > hn[best]) best = b;
        if (best >= 0 && hn[best] >= 3 && hn[best] * 5 >= tot * 2)
            for (int i = 0; i < C->np; i++) {
                if (mt[i] >= 0 || !flag[i]) continue;
                const HOp *co = &C->ops[C->p[i].op]; float *d = C->d + (size_t)co->v0 * 2;
                for (int k = 0; k < co->nv; k++) { d[2 * k] = (float)hx[best]; d[2 * k + 1] = (float)hy[best]; }
                mt[i] = -2; n_follow++;
            }
    }
    /* frame-level checks: primitives left without a partner although P had unused ones of the same signature
       jumped further than any round allows (a camera cut or a teleport moves everything at once) */
    int jumped = 0, unmatched = 0;
    for (int i = 0; i < C->np; i++) {
        if (mt[i] >= 0 || mt[i] == -2) continue;
        unmatched++;
        for (int j = head[C->p[i].sig % HB], s = 0; j >= 0 && s < 8; j = nx[j], s++)
            if (!used[j] && P->p[j].sig == C->p[i].sig) { jumped++; break; }
    }
    int matched = n_static + n_exact + n_loose + n_follow, moving = n_exact + n_loose + n_follow, ok = 1, still = 1;
    for (int i = 0; i < C->nv * 2 && still; i++) if (C->d[i] != 0) still = 0;
    if (C->np >= 24 && matched * 4 < C->np) { ok = 0; hs_scene++; }                      /* a different picture */
    else if (jumped >= 16 && jumped > moving) { ok = 0; hs_cuts++; }                    /* a cut */
    if (ok && still) ok = 0;                                                            /* nothing moved: no replay needed */
    hs_static += n_static; hs_exact += n_exact + n_follow; hs_loose += n_loose; hs_new += unmatched; hs_pairs++;
    hs_pair_us += port_time_us() - t0;
    if (port_verbose > 1)
        port_printf("[fps] vblank %d: %d primitives: %d static, %d moved, %d animated sprites, %d with the camera, %d new (%d jumped)%s\n", port_frame,
                    C->np, n_static, n_exact, n_loose, n_follow, unmatched, jumped, ok ? "" : ": not blended");
    return ok;
}

/* ------------------------------------------------------------------ replay */
/* the rectangles a replay of r writes (inclusive, clipped to the plane): drawing ops their drawing area, fills, copies and
   uploads their destination; more than 8 are merged; nwr = -2 when one wraps around the plane (then: the whole plane) */
static void h_wadd(HRec *r, int x1, int y1, int x2, int y2, int wide) {
    int W = wide ? WS_STRIDE : 1024;
    if (r->nwr == -2) return;
    if (x1 < 0 || y1 < 0 || x2 >= W || y2 >= 512) { r->nwr = -2; return; }
    if (x2 < x1 || y2 < y1) return;
    for (int i = 0; i < r->nwr; i++) {
        short *q = r->wr[i];
        if (q[4] == wide && x1 >= q[0] && y1 >= q[1] && x2 <= q[2] && y2 <= q[3]) return;
    }
    if (r->nwr == 8) {   /* full: grow the one of the same plane that grows least */
        int best = -1; long long ba = 0;
        for (int i = 0; i < 8; i++) {
            short *q = r->wr[i];
            if (q[4] != wide) continue;
            long long a = (long long)((x2 > q[2] ? x2 : q[2]) - (x1 < q[0] ? x1 : q[0]) + 1) * ((y2 > q[3] ? y2 : q[3]) - (y1 < q[1] ? y1 : q[1]) + 1)
                          - (long long)(q[2] - q[0] + 1) * (q[3] - q[1] + 1);
            if (best < 0 || a < ba) { best = i; ba = a; }
        }
        if (best < 0) { r->nwr = -2; return; }
        short *q = r->wr[best];
        if (x1 < q[0]) q[0] = (short)x1; if (y1 < q[1]) q[1] = (short)y1; if (x2 > q[2]) q[2] = (short)x2; if (y2 > q[3]) q[3] = (short)y2;
        return;
    }
    short *q = r->wr[r->nwr++];
    q[0] = (short)x1; q[1] = (short)y1; q[2] = (short)x2; q[3] = (short)y2; q[4] = (short)wide;
}
static void h_wrects(HRec *r) {
    if (r->nwr != -1) return;
    r->nwr = 0;
    for (int i = 0; i < r->nops && r->nwr >= 0; i++) {
        const HOp *o = &r->ops[i];
        switch (o->kind) {
        case H_POLY: case H_RECT: case H_LINE: {
            const GpuState *st = &r->st[o->st]; int W = st->wide ? WS_STRIDE : 1024;
            h_wadd(r, st->x1 < 0 ? 0 : st->x1, st->y1 < 0 ? 0 : st->y1, st->x2 >= W ? W - 1 : st->x2, st->y2 > 511 ? 511 : st->y2, st->wide);
            break;
        }
        case H_FILL: h_wadd(r, o->a[0], o->a[1], o->a[0] + o->a[2] - 1, o->a[1] + o->a[3] - 1, o->wide); break;
        case H_COPY: h_wadd(r, o->a[2], o->a[3], o->a[2] + o->a[4] - 1, o->a[3] + o->a[5] - 1, 0); break;
        case H_UP: h_wadd(r, o->a[0], o->a[1], o->a[0] + o->a[2] - 1, o->a[1] + o->a[3] - 1, o->wide); break;
        case H_WSDOWN: h_wadd(r, o->a[2], o->a[3], o->a[2] + o->a[4] - 1, o->a[3] + o->a[5] - 1, 0); break;
        }
    }
}
static int h_replay_begin(int slot) {
    if (!h_workV && !(h_workV = (u16 *)malloc(sizeof vram_store))) return 0;
    if (port_widescreen && !h_workW && !(h_workW = (u16 *)malloc(sizeof wvram_store))) return 0;
    if (h_gl) {
        extern int port_gl_hfr_begin(int, const short (*)[5], int);
        HRec *r = &hrec[slot];
        h_wrects(r);
        if (!port_gl_hfr_begin(slot, (const short (*)[5])r->wr, r->nwr < 0 ? -1 : r->nwr)) return 0;
    }
    else {
        if (!hrec[slot].snapV || (port_widescreen && !hrec[slot].snapW)) return 0;
        memcpy(h_workV, hrec[slot].snapV, sizeof vram_store);
        if (port_widescreen) memcpy(h_workW, hrec[slot].snapW, sizeof wvram_store);
    }
    /* (OpenGL: the CPU arrays only carry the pixels of the replayed uploads and the readbacks of the test dumps) */
    vram = (u16 (*)[1024])h_workV;
    if (port_widescreen) wvram = (u16 (*)[WS_STRIDE])h_workW;
    return 1;
}
static void h_replay_end(void) {
    vram = vram_store; wvram = wvram_store;
    tgt = &vram_store[0][0]; tgt_stride = 1024;
    if (h_gl) { extern void port_gl_hfr_end(void); port_gl_hfr_end(); }
}
float port_gpu_subpx[3][2];
int port_gpu_subpx_on;
static void h_replay(const HRec *C, float f) {
    GpuState live = gs;
    port_gpu_subpx_on = 1;
    for (int i = 0; i < C->nops; i++) {
        const HOp *o = &C->ops[i];
        switch (o->kind) {
        case H_POLY: case H_RECT: case H_LINE: {
            GpuVtx v[4]; const GpuVtx *s = &C->v[o->v0]; const float *d = C->d + (size_t)o->v0 * 2;
            gs = C->st[o->st];
            if (gs.wide) { tgt = &wvram[0][0]; tgt_stride = WS_STRIDE; } else { tgt = &vram[0][0]; tgt_stride = 1024; }
            float fr[4][2];          /* the sub-pixel rest of each position (OpenGL at internal resolutions > 1x) */
            for (int k = 0; k < o->nv; k++) {
                v[k] = s[k];
                float dx = d[2 * k] * f, dy = d[2 * k + 1] * f;
                int ix = (int)(dx < 0 ? dx - 0.5f : dx + 0.5f), iy = (int)(dy < 0 ? dy - 0.5f : dy + 0.5f);
                v[k].x += ix; v[k].y += iy; fr[k][0] = dx - ix; fr[k][1] = dy - iy;
            }
            const GpuTex *t = o->flags & HF_TEX ? &o->t : 0;
            int semi = o->flags & HF_SEMI, raw = o->flags & HF_RAW, shade = o->flags & HF_SHADE;
            memcpy(port_gpu_subpx, fr, 3 * sizeof fr[0]);
            if (o->kind == H_POLY) {
                if (!too_big(v, 3)) be->tri(&v[0], &v[1], &v[2], shade, t, semi, raw);
                memcpy(port_gpu_subpx, fr + 1, 3 * sizeof fr[0]);
                if (o->nv == 4 && !too_big(v + 1, 3)) be->tri(&v[1], &v[2], &v[3], shade, t, semi, raw);
            } else if (o->kind == H_RECT)
                be->rect(v[0].x, v[0].y, o->w, o->h, (u32)v[0].r | (u32)v[0].g << 8 | (u32)v[0].b << 16, t, v[0].u, v[0].v, semi, raw);
            else if (!(v[1].x - v[0].x > 1023 || v[0].x - v[1].x > 1023 || v[1].y - v[0].y > 511 || v[0].y - v[1].y > 511))
                be->line(v[0].x, v[0].y, (u32)v[0].r | (u32)v[0].g << 8 | (u32)v[0].b << 16,
                         v[1].x, v[1].y, (u32)v[1].r | (u32)v[1].g << 8 | (u32)v[1].b << 16, shade, semi);
            break;
        }
        case H_FILL:
            if (!o->wide) be->fill(o->a[0], o->a[1], o->a[2], o->a[3], (u16)o->a[4]);
            else if (be->ws_fill) be->ws_fill(o->a[0], o->a[1], o->a[2], o->a[3], (u16)o->a[4]);
            else if (port_widescreen)
                for (int j = 0; j < o->a[3]; j++) for (int k = 0; k < o->a[2] && o->a[0] + k < WS_STRIDE; k++) wvram[(o->a[1] + j) & 511][o->a[0] + k] = (u16)o->a[4];
            break;
        case H_COPY: be->copy(o->a[0], o->a[1], o->a[2], o->a[3], o->a[4], o->a[5]); break;
        case H_UP: {
            const u16 *s = C->data + o->v0; int x = o->a[0], y = o->a[1], w = o->a[2], h = o->a[3];
            if (o->wide) {
                if (!port_widescreen) break;
                for (int j = 0; j < h; j++) memcpy(&wvram[(y + j) & 511][x], s + (size_t)j * w, (size_t)w * 2);
                if (be->ws_upload && y + h <= 512) be->ws_upload(x, y, w, h, &wvram[y][x], WS_STRIDE);
            } else {
                for (int j = 0; j < h; j++) for (int k = 0; k < w; k++) vram[(y + j) & 511][(x + k) & 1023] = *s++;
                be->vram_written(x, y, w, h);
            }
            break;
        }
        case H_WSDOWN:
            if (be->ws_copy_down) be->ws_copy_down(o->a[0], o->a[1], o->a[2], o->a[3], o->a[4], o->a[5]);
            else if (port_widescreen)
                for (int j = 0; j < o->a[5]; j++) memcpy(&vram[(o->a[3] + j) & 511][o->a[2]], &wvram[(o->a[1] + j) & 511][o->a[0]], (size_t)o->a[4] * 2);
            break;
        }
    }
    gs = live;
    port_gpu_subpx_on = 0;
    be->list_end();
}

/* ------------------------------------------------------------------ the vblank and the presents */
/* port_gpu_vblank: closes this vblank's record and starts the next one in a slot that holds none of the records the
   next presents can need (the one just closed, which the game puts on display right after this vblank, the displayed
   one and the one before it) */
static void hfr_vblank(void) {
    hs_vblanks++;
    if (!h_init) {
        char rate[16]; snprintf(rate, sizeof rate, "%d", port_hfr_fps);
        h_init = 1; h_gl = be == &port_gpu_gl;
        port_printf("[gpu] --fps %s: presents interpolated between the %d Hz logic frames (%s renderer)\n",
                    port_hfr_fps == -1 ? "auto" : port_hfr_fps == -2 ? "unlimited" : rate, port_hz, be->name);
        h_start(0); hfr_rec = 1;
        { const char *e = getenv("PORT_HFR_TEST_CUT"); if (e) sscanf(e, "%d:%d", &hfr_test_cut_frame, &hfr_test_cut_dx); }
        return;
    }
    HRec *r = &hrec[hcur];
    if (r->nops && !r->bad) r->id = ++hseq;
    int next = -1;
    for (int k = 1; k < HNREC && next < 0; k++) {
        int i = (hcur + k) % HNREC;
        if (!hrec[i].id || (hrec[i].id != h_disp && hrec[i].id != h_prev)) next = i;
    }
    if (next < 0) next = (hcur + 1) % HNREC;
    if (!r->id) next = hcur;                                /* (nothing recorded: the slot is reused) */
    h_start(next);
}
/* at the first present after a vblank (the game has flipped its buffers by then): the record that drew the frame now on
   display and, when that changed, its pairing with the one displayed before */
static void hfr_sync(void) {
    RECT *d = (RECT *)cur_dispenv;
    int w = d->w > 0 ? d->w : 320, h = d->h > 0 ? d->h : 240, rgb24 = cur_dispenv[0x11], wide, plain;
    if (w > 1024) w = 1024; if (h > 512) h = 512;
    wide = (be == &port_gpu_soft || be->ws_present) && ws_frame(d, w, rgb24);
    plain = rgb24 || !disp_mask;
    int x = wide ? ws_base(d->x & 1023) : d->x & 1023, y = d->y & 511;
    if (wide) w = 320 + 2 * port_widescreen;
    unsigned nd = 0;
    if (!plain)
        for (int i = 0; i < HNREC; i++) {
            const HRec *q = &hrec[i];
            if (!q->id || q->id <= nd) continue;
            for (int j = 0; j < q->nops; j++) {
                const HOp *o = &q->ops[j];
                if (o->kind > H_LINE) continue;
                const GpuState *s = &q->st[o->st];
                if (s->wide == wide && s->x1 < x + w && s->x2 >= x && s->y1 < y + h && s->y2 >= y) { nd = q->id; break; }
            }
        }
    if (nd != h_disp) {
        HRec *C = h_by_id(nd), *P = h_by_id(h_disp);
        h_span = h_since + 1 > 3 ? 3 : h_since + 1;
        h_since = 0;
        h_pair_ok = C && P && C != P && hd_w == w && hd_h == h && hd_wide == wide && h_pair(C, P);
        h_prev = h_disp; h_disp = nd;
    } else if (h_since < 1000) h_since++;
    h_block = hfr_overlay_frame >= port_frame - 1;
    hd_x = x; hd_y = y; hd_w = w; hd_h = h; hd_wide = wide;
}
/* whether a present at alpha would be a blend (an interpolated frame) */
static int hfr_can_blend(double alpha) {
    static int noblend = -1;      /* PORT_HFR_NOBLEND=1 (test hook): the same presents without the blend, for comparisons */
    if (noblend < 0) noblend = getenv("PORT_HFR_NOBLEND") != 0;
    int seen = !noblend && (port_window || (port_hfr_dump_dir && port_frame >= port_hfr_dump_from && port_frame <= port_hfr_dump_to));
    return h_pair_ok && !h_block && h_by_id(h_disp) && alpha < 0.999 && seen;
}
/* what a swap took: with vsync it also waits for the display's refresh, which is not work, so then the least of about
   the last second counts; without (unlimited) every one, rising at once and falling slowly */
static void h_note_present(unsigned long long us) {
    if (port_hfr_fps != -2) {
        static unsigned long long mn = ~0ull, prev = ~0ull, t0;
        unsigned long long now = port_time_us();
        if (us < mn) mn = us;
        if (now - t0 > 1000000ull) { prev = mn; mn = ~0ull; t0 = now; }
        h_pc = prev < mn ? prev : mn;
    } else h_pc = h_est(&he_pc, us);
}
/* one present at alpha (0 = the frame displayed before, 1 = the displayed one), a blend when blend is set; deadline
   (OpenGL with fences, realtime): the replay's GPU work must be done by then, or the frame is dropped (returns 0)
   rather than the swap waiting for it */
static int hfr_present(double alpha, int blend, unsigned long long deadline) {
    static int dump_n;
    HRec *C = h_by_id(h_disp);
    int interp = 0;
    if (blend && C) {
        unsigned long long t0 = port_time_us();
        if (h_replay_begin((int)(C - hrec))) {
            h_replay(C, (float)(1.0 - (alpha < 0 ? 0 : alpha))); interp = 1;
            if (h_gl && deadline) {
                extern int port_gl_hfr_fence(unsigned long long);
                unsigned long long t = port_time_us();
                if (!port_gl_hfr_fence(deadline > t ? deadline - t : 0)) interp = -1;   /* not finished in time */
            }
        }
        unsigned long long c = port_time_us() - t0;
        h_cost = h_est(&he_cost, c);
        hs_replay_us += c;
        if (interp < 0) { h_replay_end(); hs_late++; return 0; }
    }
    hs_presents++; hs_interp += interp;
    if (port_window) { unsigned long long t0 = port_time_us(); present_window(); h_note_present(port_time_us() - t0); }
    if (port_hfr_dump_dir && port_frame >= port_hfr_dump_from && port_frame <= port_hfr_dump_to) {
        char path[512]; int w, h;
        extern int port_internal_res;
        extern int port_gl_frame_hd(u8 *rgb, int x, int y, int w, int h, int wide, int *ow, int *oh);
        static u8 *hd;
        port_mkdir(port_hfr_dump_dir);
        snprintf(path, sizeof path, "%s/present_%06d.png", port_hfr_dump_dir, dump_n++);
        if (h_gl && port_internal_res > 1 && disp_mask && !cur_dispenv[0x11] && (hd || (hd = (u8 *)malloc((size_t)2048 * 512 * 3 * 16)))
            && !port_gl_frame_hd(hd, hd_x, hd_y, hd_w, hd_h, hd_wide, &w, &h))
            write_png(path, hd, w, h);                       /* (at the internal resolution: the sub-pixel positions show) */
        else { port_gpu_frame(frame_rgb, &w, &h); write_png(path, frame_rgb, w, h); }
        if (port_verbose) port_printf("[fps] %s: vblank %d alpha %.3f%s\n", path, port_frame, alpha, interp ? " (interpolated)" : "");
    }
    if (interp) h_replay_end();
    return 1;
}
static int hfr_rate(void) {
    extern int port_window_refresh(void);
    if (port_hfr_fps > 0) return port_hfr_fps;
    if (port_hfr_fps == -1) { int r = port_window ? port_window_refresh() : 0; return r > 0 ? r : 60; }
    return 1000;                    /* unlimited: as fast as the presents go (vsync off), at most one per millisecond */
}
/* psyq.c port_vblank, in place of its pacing sleep: the presents due before the next vblank at due_us (realtime: wall
   clock; unpaced, headless: a virtual clock at the --fps rate, auto = 60 without a display); alpha runs from 0 to 1 over
   the time the displayed frame's predecessor was shown (one vblank at the full logic rate).
   Realtime, the vblank comes first: the presents only fill the time before it.  A blend starts only when what a replay
   and a swap take (h_cost, h_pc, plus a margin of about 1 ms) ends before due_us, and on OpenGL only once the GPU has finished the last
   one (a fence: no glFinish, no readback); its swap waits for nothing past the deadline (the frame is dropped instead).
   Every vblank still gets one present: when no blend fits, the newest frame as drawn (alpha 1, as without --fps), after
   which nothing else is shown before the vblank (a blend would step back).  A slow GPU then falls back to the 50/60 Hz
   presents instead of slowing the game down. */
void port_hfr_wait(unsigned long long due_us, unsigned long long period_us, int realtime) {
    if (!h_init) { if (realtime && due_us > port_time_us()) port_sleep_until_us(due_us); return; }
    hfr_sync();
    double span = (double)h_span;
    if (!realtime) {
        static unsigned long long k, vb;
        unsigned long long rate = (unsigned long long)hfr_rate();
        double vdue = (double)++vb * (double)period_us;
        for (; (double)k * 1e6 / (double)rate < vdue; k++) {
            double frac = 1.0 - (vdue - (double)k * 1e6 / (double)rate) / (double)period_us;
            double a = (h_since + frac) / span;
            if (a > 1) a = 1;
            hfr_present(a, hfr_can_blend(a), 0);
        }
        return;
    }
    static unsigned long long p, last;
    unsigned long long P = 1000000ull / (unsigned long long)hfr_rate(), now = port_time_us(), margin = 500 + period_us / 32;
    int n = 0, refused = 0;
    double shown = -1;                        /* the alpha of the last present before this vblank */
    if (!p || p + 4 * P < now || p > due_us + 2 * P) p = now;
    for (;;) {
        unsigned long long t = port_time_us();
        if (t >= due_us) break;
        if (p > t) { port_sleep_until_us(p < due_us ? p : due_us); continue; }
        double frac = 1.0 - (double)(due_us - t) / (double)period_us;
        double a = (h_since + (frac < 0 ? 0 : frac)) / span;
        if (a > 1) a = 1;
        int want = hfr_can_blend(a), blend = want && a > shown;
        if (want && !blend) break;            /* the newest frame is up already */
        if (blend) {
            extern int port_gl_hfr_busy(void);
            int busy = h_gl && port_gl_hfr_busy();
            if (busy || t + h_cost + h_pc + margin > due_us) {
                if (!busy && !refused++) { he_cost.m -= he_cost.m / 32; he_cost.d -= he_cost.d / 16; h_cost = (unsigned long long)(he_cost.m + 2 * he_cost.d); }   /* (so a slow spell does not keep the blends off for good) */
                if (n) break;
                blend = 0; a = 1; hs_plain++;
            }
        } else if (n && t + h_pc + margin > due_us) break;
        if (!hfr_present(a, blend, blend ? due_us - h_pc - margin / 2 : 0)) continue;   /* dropped: what time is left */
        n++; last = port_time_us();
        shown = blend ? a : 1.0;
        p += P;
        if (last > p + P) p = last;           /* the swap blocked for longer (vsync at a lower rate): follow it */
    }
    if (!n && (h_since == 0 || port_time_us() > last + P)) { hfr_present(1.0, 0, 0); last = port_time_us(); }
    if (due_us > port_time_us()) port_sleep_until_us(due_us);
}
/* the --show-fps line (psyq.c): presents and interpolated presents since the last call */
void port_hfr_second(char *out, int n) {
    static unsigned long long p0, i0, r0;
    static unsigned long long l0, f0;
    unsigned long long p = hs_presents - p0, i = hs_interp - i0, r = hs_replay_us - r0, l = hs_late - l0, f = hs_plain - f0;
    snprintf(out, (size_t)n, " presents %llu/s (%llu interpolated, replay %llu.%llu ms each; %llu vblanks without a blend in time, %llu dropped; "
             "expected replay %llu.%llu swap %llu.%llu ms)", p, i, i ? r / i / 1000 : 0, i ? r / i / 100 % 10 : 0, f, l,
             h_cost / 1000, h_cost / 100 % 10, h_pc / 1000, h_pc / 100 % 10);
    p0 = hs_presents; i0 = hs_interp; r0 = hs_replay_us; l0 = hs_late; f0 = hs_plain;
}
void port_hfr_report(void) {
    if (!h_init || !hs_vblanks) return;
    unsigned long long c = hs_static + hs_exact + hs_loose + hs_new; if (!c) c = 1;
    port_printf("[fps] %llu vblanks, %llu presents (%llu interpolated; %llu vblanks without a blend in time, %llu dropped); pairing %llu us per new frame, replay %llu us per "
                "interpolated present; primitives: %llu%% static, %llu%% moved, %llu%% animated sprites, %llu%% new; %llu scene changes, %llu cuts not blended\n",
                hs_vblanks, hs_presents, hs_interp, hs_plain, hs_late, hs_pairs ? hs_pair_us / hs_pairs : 0, hs_interp ? hs_replay_us / hs_interp : 0,
                hs_static * 100 / c, hs_exact * 100 / c, hs_loose * 100 / c, hs_new * 100 / c, hs_scene, hs_cuts);
}
