/* Second screen panel: the map tab (included by panel.c): the side view of side-scrolling sections, the game's own
 * top-down picture in the top-down ones (panel_topdown.inc), chosen by section; discrete zoom, follow Tomba, pan.
 *
 * Side view: the room model the game has loaded (WMD_DATA[0], scratch 0x1F800330: sectors of GPU-like packets
 * {clut, tpage, rgb cmd, (gouraud: 2/3 more rgb words), 3/4 x {s16 x, y, z, u8 u, v}}, tools/room.py,
 * notes/formats.md "WMD rooms") rasterised orthographically (x right, y down, far polygons first by mean z) the way
 * the GPU draws them: texels through the polygon's CLUT / page from a snapshot of the VRAM taken while the section is
 * loaded, modulated by the vertex colours (0x80 = 1.0, Gouraud interpolated) unless the packet is raw, STP texels of
 * semi-transparent packets blended 1/2 + 1/2, colour 0x0000 transparent.  Supersampled and box filtered when zoomed out.
 * Behind it, the area's far background: the frame as the game has drawn it once the ordering table's far slots
 * (0..99: the overlay's sky / scenery, notes/formats.md) are done, read back by shims/gpu.c (port_panel_sky_req)
 * and shown screen-fixed (scaled to the view's height, each band repeated sideways, slow parallax) as the game shows it.
 * Not drawn: objects (signs, NPCs, enemies: drawn by their handlers), effects.
 *
 * Zoom: PANEL_MAP_ZOOMS levels; 1 = the whole map fitted, the last = about the game screen's framing, geometric in
 * between.  Zoomed levels follow Tomba; drag (touch / mouse) or the arrows pan, OK re-centres.  -/+ buttons (with
 * the game's L1 / R1 glyphs), keys PageDown/PageUp or - / + in the PC panel window, --panel-input zoomout / zoomin.
 * Markers: the events placed in the section (panel_eventmap.h, their icons at 3/4 size, outlined) and Tomba's face
 * at his live position (PLAYER x / y), flipped to the side he faces (PLAYER +0xA6: 0 left, 2 right). */
#include <stdint.h>
extern const unsigned short *port_gpu_vram(int x, int y, int w, int h);   /* shims/gpu.c */
extern int port_panel_sky_req;                                            /* shims/gpu.c */
#ifndef PANEL_MAP_ZOOMS
#define PANEL_MAP_ZOOMS 3
#endif
#define P_X ((s16)G16(0x800A604Au))
#define P_Y ((s16)G16(0x800A604Eu))
#define P_LEFT (G8(0x800A60DEu) == 0)  /* PLAYER +0xA6: the side he faces, 0 left, 2 right (measured) */
#define FACE_LEFT_FLIP 0               /* the sub-screen face sprite looks left */
#define FACE_RIGHT_FLIP PA_FLIPX
#define SIDE_GAME_W 440               /* world units across the game screen at Tomba's depth (zoom's last level) */

static int map_zoom, map_drag = -1, map_dx, map_dy, map_vx, map_vy, map_vw, map_vh, map_key = -1;
static float map_panx, map_pany;

/* ---------------------------------------------------------------- far background (sky) */
#define SKY_W 320
#define SKY_H 256
static u32 sky_px[SKY_W * SKY_H]; static int sky_w, sky_h, sky_seq, sky_key = -1, sky_next, sky_tries;
static u32 c15(u16 w) { return 0xFF000000u | (u32)((w >> 10 & 31) * 255 / 31) << 16 | (u32)((w >> 5 & 31) * 255 / 31) << 8 | (u32)((w & 31) * 255 / 31); }
/* The far layer is strips the overlay places for the camera (parts may be missing where the room hides them):
   rows of the same band (consecutive rows holding anything but the clear colour) repeat their drawn span sideways,
   so the backdrop continues past the captured screen as the game's strips do. */
static int sky_ra[SKY_H], sky_rp[SKY_H]; static u32 sky_bg;
static void sky_bands(void) {
    u32 best = sky_px[0]; int bn = 0;   /* the clear colour: the most common one of the top row */
    for (int i = 0; i < sky_w; i += 4) { int n = 0; for (int k = 0; k < sky_w; k += 4) n += sky_px[k] == sky_px[i]; if (n > bn) { bn = n; best = sky_px[i]; } }
    sky_bg = best;
    for (int j = 0; j < sky_h;) {
        int a = sky_w, b = -1, j0 = j;
        for (; j < sky_h; j++) {
            int ra = sky_w, rb = -1;
            for (int i = 0; i < sky_w; i++) if (sky_px[j * SKY_W + i] != best) { if (i < ra) ra = i; rb = i; }
            if (rb < 0) break;
            if (j > j0 && (ra - a > sky_w / 5 || a - ra > sky_w / 5 || rb - b > sky_w / 5 || b - rb > sky_w / 5)) break;   /* another strip */
            if (ra < a) a = ra;
            if (rb > b) b = rb;
        }
        for (int k = j0; k < j; k++) { sky_ra[k] = a; sky_rp[k] = b - a + 1; }
        if (j == j0) { sky_ra[j] = 0; sky_rp[j] = 0; j++; }
    }
}
void port_panel_sky_take(const unsigned short *vr, int x, int y, int w, int h) {
    if (w > SKY_W) { x += (w - SKY_W) / 2; w = SKY_W; }
    if (h > SKY_H) h = SKY_H;
    int top = 0, bot = h;   /* the letterbox rows the game leaves black */
    while (top < h / 4) { int k = 0; for (int i = 0; i < w; i += 8) k |= vr[((y + top) & 511) * 1024 + ((x + i) & 1023)] & 0x7FFF; if (k) break; top++; }
    while (bot > h * 3 / 4) { int k = 0; for (int i = 0; i < w; i += 8) k |= vr[((y + bot - 1) & 511) * 1024 + ((x + i) & 1023)] & 0x7FFF; if (k) break; bot--; }
    for (int j = top; j < bot; j++) for (int i = 0; i < w; i++) sky_px[(j - top) * SKY_W + i] = c15(vr[((y + j) & 511) * 1024 + ((x + i) & 1023)]);
    sky_w = w; sky_h = bot - top; sky_seq++;
    sky_bands();
}
static void sky_tick(int key) {
    if (key != sky_key) { sky_key = key; sky_h = 0; sky_tries = 0; sky_seq++; sky_next = port_frame + 45; }
    if (!sky_h && sky_tries < 6 && port_frame >= sky_next) {
        if (++sky_tries == 6) { sky_seq++; return; }   /* no far layer in this section: the game shows its clear colour (black) */
        port_panel_sky_req = 1; sky_next = port_frame + 30;
    }
}
static void sky_draw(u32 *dst, int vw, int vh, float scroll) {
    if (sky_h <= 0 && sky_tries >= 6) { for (int i = 0; i < vw * vh; i++) dst[i] = RGB(8, 6, 12); return; }
    if (sky_h <= 0) {
        for (int j = 0; j < vh; j++) { int t = j * 255 / (vh ? vh : 1); u32 c = RGB(120 + 80 * t / 255, 170 + 58 * t / 255, 255); for (int i = 0; i < vw; i++) dst[j * vw + i] = c; }
        return;
    }
    int sc256 = vh * 256 / sky_h, off = (int)scroll;   /* cover the view's height; scale in 1/256 */
    if (sc256 < 1) sc256 = 1;
    for (int j = 0; j < vh; j++) {
        int sy = j * 256 / sc256; if (sy >= sky_h) sy = sky_h - 1;
        for (int i = 0; i < vw; i++) {
            int sx = (i + off) * 256 / sc256, p = sky_rp[sy];
            if (!p) { dst[j * vw + i] = sky_bg; continue; }
            sx = (sx - sky_ra[sy]) % p; if (sx < 0) sx += p;
            dst[j * vw + i] = sky_px[sy * SKY_W + sky_ra[sy] + sx];
        }
    }
}

/* ---------------------------------------------------------------- side model */
typedef struct { s16 x[4], y[4]; u8 u[4], v[4], c[4][3]; u16 clut, tp; u8 nv, cmd; s16 bx0, by0, bx1, by1; int z; } SPoly;
static SPoly *sp; static int nsp, sp_cap, side_state, side_area = -1, side_sig, side_seen, side_x0, side_y0, side_x1, side_y1;
static u16 *side_vr;                  /* VRAM snapshot (1024 x 512) taken when the model was read */
static unsigned side_model;           /* id of the model the view shows (changes when another is read) */
static int side_secs[32];              /* per section of the area: the model signature seen there (events of those sections) */
static int sp_cmp(const void *a, const void *b) { return ((const SPoly *)b)->z - ((const SPoly *)a)->z; }
static const u8 *side_wmd(void) {
    u32 a = SPAD32(0x330);
    const u8 *d = (const u8 *)(uintptr_t)a;
    if (!a || !port_is_ram_ptr((void *)d) || !port_is_ram_ptr((void *)(d + 0x40000))) return NULL;
    return d;
}
static int side_signature(const u8 *d) {   /* cheap: the sector count, offsets and the first packets */
    unsigned h = 2166136261u;
    for (int i = 0; i < 64; i++) h = (h ^ d[i]) * 16777619u;
    return (int)(h & 0x7FFFFFFF);
}
static int side_build(const u8 *d) {
    unsigned ns = rd32(d, 0);
    if (ns == 0 || ns > 2048) return 0;
    if (!sp) { sp_cap = 8192; sp = malloc(sizeof(SPoly) * sp_cap); }
    nsp = 0; side_x0 = side_y0 = 1 << 20; side_x1 = side_y1 = -(1 << 20);
    for (unsigned s = 0; s < ns; s++) {
        unsigned p = rd32(d, 4 + 4 * s);
        if (p > 0x200000 || !port_is_ram_ptr((void *)(d + p))) return 0;
        for (int g = 0; g < 4; g++) {
            unsigned c = rd32(d, p); p += 4;
            if (c > 8192) return 0;
            for (unsigned k = 0; k < c; k++) {
                const u8 *q = d + p;
                int cmd = q[7], nv = cmd & 8 ? 4 : 3, gr = cmd & 0x10, ex = gr ? (nv - 1) * 4 : 0;
                if ((cmd & 0xE0) != 0x20) return 0;
                if (nsp == sp_cap) { sp_cap *= 2; sp = realloc(sp, sizeof(SPoly) * sp_cap); }
                SPoly *o = &sp[nsp]; int z = 0;
                o->clut = (u16)rd16(q, 0); o->tp = (u16)rd16(q, 2); o->cmd = (u8)cmd; o->nv = (u8)nv;
                o->bx0 = o->by0 = 32767; o->bx1 = o->by1 = -32768;
                for (int i = 0; i < nv; i++) {
                    const u8 *v = q + 8 + ex + 8 * i, *col = i && gr ? q + 4 + 4 * i : q + 4;
                    o->x[i] = rd16(v, 0); o->y[i] = rd16(v, 2); z += rd16(v, 4); o->u[i] = v[6]; o->v[i] = v[7];
                    o->c[i][0] = col[0]; o->c[i][1] = col[1]; o->c[i][2] = col[2];
                    if (o->x[i] < o->bx0) o->bx0 = o->x[i]; if (o->x[i] > o->bx1) o->bx1 = o->x[i];
                    if (o->y[i] < o->by0) o->by0 = o->y[i]; if (o->y[i] > o->by1) o->by1 = o->y[i];
                }
                o->z = z * 12 / nv;   /* mean depth (x12: exact for 3 and 4 vertices) */
                if (o->bx0 < side_x0) side_x0 = o->bx0; if (o->bx1 > side_x1) side_x1 = o->bx1;
                if (o->by0 < side_y0) side_y0 = o->by0; if (o->by1 > side_y1) side_y1 = o->by1;
                if ((o->tp >> 7 & 3) <= 1 || !(cmd & 4)) nsp++;
                p += 8 + ex + 8 * nv;
            }
        }
    }
    if (nsp < 16 || side_x1 <= side_x0 || side_y1 <= side_y0) return 0;
    qsort(sp, nsp, sizeof *sp, sp_cmp);
    if (!side_vr) side_vr = malloc(1024 * 512 * 2);
    memcpy(side_vr, port_gpu_vram(0, 0, 1024, 512), 1024 * 512 * 2);
    side_model++;
    port_printf("[panel] side map: area %d section %d, %d polygons, x %d..%d y %d..%d\n", G_AREA, G_SECTION, nsp, side_x0, side_x1, side_y0, side_y1);
    return 1;
}
/* the model of the current section: (re)read once the loaded one has stayed the same for a moment */
static int side_update(void) {
    const u8 *d = side_wmd();
    int area = G_AREA, sec = G_SECTION, sig = d ? side_signature(d) : -1;
    if (area != side_area) { side_area = area; side_state = 0; for (int i = 0; i < 32; i++) side_secs[i] = -1; }
    if (sig != side_sig) { side_sig = sig; side_seen = port_frame; }
    if (d && port_frame - side_seen > 40 && (side_state == 0 || (side_state == 1 && sig != side_secs[31]))) {
        side_state = side_build(d) ? 1 : -1;
        side_secs[31] = side_state == 1 ? sig : -1;   /* (slot 31: the model in memory) */
    }
    if (side_state == 1 && sec < 31 && sig == side_secs[31]) side_secs[sec] = side_secs[31];
    return side_state == 1 && sig == side_secs[31];
}

/* one triangle into the supersampled RGBA buffer (alpha 0 = empty) */
static void side_tri(u32 *buf, int bw, int bh, const float *px, const float *py, const SPoly *o, const int *ix) {
    float den = (py[1] - py[2]) * (px[0] - px[2]) + (px[2] - px[1]) * (py[0] - py[2]);
    if (den > -1e-6f && den < 1e-6f) return;
    float mnx = px[0], mxx = px[0], mny = py[0], mxy = py[0];
    for (int i = 1; i < 3; i++) { if (px[i] < mnx) mnx = px[i]; if (px[i] > mxx) mxx = px[i]; if (py[i] < mny) mny = py[i]; if (py[i] > mxy) mxy = py[i]; }
    int x0 = (int)(mnx - .5f), x1 = (int)(mxx + .5f), y0 = (int)(mny - .5f), y1 = (int)(mxy + .5f);
    if (x0 < 0) x0 = 0; if (y0 < 0) y0 = 0; if (x1 >= bw) x1 = bw - 1; if (y1 >= bh) y1 = bh - 1;
    if (x0 > x1 || y0 > y1) return;
    int mode = o->tp >> 7 & 3, tx = (o->tp & 15) * 64, ty = (o->tp >> 4 & 1) * 256, tex = o->cmd & 4, raw = o->cmd & 1, semi = o->cmd & 2;
    const u16 *pal = side_vr + (o->clut >> 6 & 511) * 1024 + (o->clut & 63) * 16;
    float U[3], V[3], R[3], G[3], B[3];
    for (int i = 0; i < 3; i++) { U[i] = o->u[ix[i]]; V[i] = o->v[ix[i]]; R[i] = o->c[ix[i]][0]; G[i] = o->c[ix[i]][1]; B[i] = o->c[ix[i]][2]; }
    float inv = 1.f / den;
    for (int y = y0; y <= y1; y++) {
        float cy = y + .5f;
        for (int x = x0; x <= x1; x++) {
            float cx = x + .5f;
            float a = ((py[1] - py[2]) * (cx - px[2]) + (px[2] - px[1]) * (cy - py[2])) * inv;
            float b = ((py[2] - py[0]) * (cx - px[2]) + (px[0] - px[2]) * (cy - py[2])) * inv, c = 1 - a - b;
            if (a < -1e-4f || b < -1e-4f || c < -1e-4f) continue;
            int r, g, bl, stp = 0;
            int mr = (int)(a * R[0] + b * R[1] + c * R[2]), mg = (int)(a * G[0] + b * G[1] + c * G[2]), mb = (int)(a * B[0] + b * B[1] + c * B[2]);
            if (tex) {
                int u = (int)(a * U[0] + b * U[1] + c * U[2]) & 255, v = (int)(a * V[0] + b * V[1] + c * V[2]) & 255;
                u16 w = mode == 0 ? pal[(side_vr[((ty + v) & 511) * 1024 + ((tx + (u >> 2)) & 1023)] >> ((u & 3) * 4)) & 15]
                                  : pal[(side_vr[((ty + v) & 511) * 1024 + ((tx + (u >> 1)) & 1023)] >> ((u & 1) * 8)) & 255];
                if (!w) continue;
                stp = w >> 15;
                r = (w & 31) << 3; g = (w >> 5 & 31) << 3; bl = (w >> 10 & 31) << 3;
                if (!raw) { r = r * mr >> 7; g = g * mg >> 7; bl = bl * mb >> 7; if (r > 255) r = 255; if (g > 255) g = 255; if (bl > 255) bl = 255; }
            } else { r = mr; g = mg; bl = mb; stp = 1; }
            u32 *d = &buf[y * bw + x];
            if (semi && stp) {
                u32 o2 = *d; int al = o2 >> 24;
                if (al) { r = (r + (int)(o2 & 255)) / 2; g = (g + (int)(o2 >> 8 & 255)) / 2; bl = (bl + (int)(o2 >> 16 & 255)) / 2; *d = (u32)(al > 128 ? al : 128) << 24 | (u32)bl << 16 | (u32)g << 8 | (u32)r; }
                else *d = 0x80000000u | (u32)bl << 16 | (u32)g << 8 | (u32)r;
            } else *d = 0xFF000000u | (u32)bl << 16 | (u32)g << 8 | (u32)r;
        }
    }
}
/* the model over the sky into dst (vw x vh): world (wx0, wy0) at the top left, upp world units per pixel */
static void side_render(u32 *dst, int vw, int vh, float wx0, float wy0, float upp) {
    int ss = upp >= 4 ? 3 : upp >= 1.5f ? 2 : 1, bw = vw * ss, bh = vh * ss;
    static u32 *buf; static int cap;
    if (bw * bh > cap) { free(buf); cap = bw * bh; buf = malloc((size_t)cap * 4); }
    memset(buf, 0, (size_t)bw * bh * 4);
    float k = ss / upp, wx1 = wx0 + vw * upp, wy1 = wy0 + vh * upp;
    for (int i = 0; i < nsp; i++) {
        const SPoly *o = &sp[i];
        if (o->bx1 < wx0 || o->bx0 > wx1 || o->by1 < wy0 || o->by0 > wy1) continue;
        float px[4], py[4];
        for (int j = 0; j < o->nv; j++) { px[j] = (o->x[j] - wx0) * k; py[j] = (o->y[j] - wy0) * k; }
        static const int i0[3] = { 0, 1, 2 }, i1[3] = { 1, 2, 3 };
        side_tri(buf, bw, bh, px, py, o, i0);
        if (o->nv == 4) { float qx[3] = { px[1], px[2], px[3] }, qy[3] = { py[1], py[2], py[3] }; side_tri(buf, bw, bh, qx, qy, o, i1); }
    }
    sky_draw(dst, vw, vh, (wx0 + vw * upp / 2) / (SIDE_GAME_W / 320.f) * 0.25f);
    for (int y = 0; y < vh; y++) for (int x = 0; x < vw; x++) {   /* box filter, coverage over the sky */
        int r = 0, g = 0, b = 0, a = 0;
        for (int j = 0; j < ss; j++) for (int i = 0; i < ss; i++) {
            u32 c = buf[(y * ss + j) * bw + x * ss + i]; int al = c >> 24;
            r += (c & 255) * al; g += (c >> 8 & 255) * al; b += (c >> 16 & 255) * al; a += al;
        }
        if (!a) continue;
        u32 bg = dst[y * vw + x]; int n = ss * ss * 255;
        r = (r + (int)(bg & 255) * (n - a)) / n; g = (g + (int)(bg >> 8 & 255) * (n - a)) / n; b = (b + (int)(bg >> 16 & 255) * (n - a)) / n;
        dst[y * vw + x] = 0xFF000000u | (u32)b << 16 | (u32)g << 8 | (u32)r;
    }
}

#include "panel_topdown.inc"

/* ---------------------------------------------------------------- view, zoom, input */
static int map_topdown(void) { return in_gameplay() && G_AREA < 20 && td_section(G_AREA, G_SECTION); }
static const char *map_tab_name(void) { return map_topdown() ? T("Top-down", "Cenital") : T("Side map", "Lateral"); }
static void map_set_zoom(int z) {
    if (z < 0) z = 0; if (z > PANEL_MAP_ZOOMS - 1) z = PANEL_MAP_ZOOMS - 1;
    if (z != map_zoom) { map_zoom = z; map_panx = map_pany = 0; }
}
static void map_pan(float dx, float dy) { if (map_zoom) { map_panx += dx; map_pany += dy; } }
static float map_upp_now = 1;
static int map_action(int a) {   /* keys / pad on the map tab (1: used) */
    float st = 48 * map_upp_now;
    switch (a) {
    case IN_ZOOM_OUT: map_set_zoom(map_zoom - 1); return 1;
    case IN_ZOOM_IN: map_set_zoom(map_zoom + 1); return 1;
    case IN_LEFT: map_pan(-st, 0); return 1;
    case IN_RIGHT: map_pan(st, 0); return 1;
    case IN_UP: map_pan(0, -st); return 1;
    case IN_DOWN: map_pan(0, st); return 1;
    case IN_OK: case IN_BACK: map_panx = map_pany = 0; return 1;
    }
    return 0;
}
static void map_hit(int k) { if (k == 16) map_set_zoom(map_zoom - 1); else if (k == 17) map_set_zoom(map_zoom + 1); }
static int map_touch(int x, int y, int action) {   /* drag inside the view pans (1: used) */
    if (action == 0) {
        if (x < map_vx || y < map_vy || x >= map_vx + map_vw || y >= map_vy + map_vh) { map_drag = -1; return 0; }
        map_drag = 1; map_dx = x; map_dy = y; return 1;
    }
    if (map_drag < 0) return 0;
    if (action == 2) { map_pan(-(x - map_dx) * map_upp_now, -(y - map_dy) * map_upp_now); map_dx = x; map_dy = y; }
    else map_drag = -1;
    return 1;
}

/* the view of a map whose world bounds are (x0, y0)-(x1, y1); gw = world units across the game screen; anchor_bottom:
   a map shorter than the view sits on its bottom edge (side view: the sky above) */
static void map_view(float x0, float y0, float x1, float y1, float gw, int anchor_bottom, int vw, int vh, int here, float tx, float ty,
                     float *wx0, float *wy0, float *upp) {
    float W = x1 - x0, H = y1 - y0, u0 = W / vw > H / vh ? W / vw : H / vh, ug = gw / vw;
    if (ug > u0) ug = u0;
    float u = u0;
    if (PANEL_MAP_ZOOMS > 1 && map_zoom) {   /* u0 * (ug / u0) ^ (zoom / (N - 1)), geometric (no libm: the port's math shims) */
        float r = ug / u0, lo = r, hi = 1, s = 1;
        for (int it = 0; it < 40; it++) { float m = (lo + hi) / 2, p = 1; for (int k = 0; k < PANEL_MAP_ZOOMS - 1; k++) p *= m; if (p > r) hi = m; else lo = m; }
        s = (lo + hi) / 2;
        for (int k = 0; k < map_zoom; k++) u *= s;
    }
    float cx = (x0 + x1) / 2, cy = (y0 + y1) / 2;
    if (map_zoom && here) { cx = tx + map_panx; cy = ty - vh * u / 6 + map_pany; }
    else if (map_zoom) { cx += map_panx; cy += map_pany; }
    float hw = vw * u / 2, hh = vh * u / 2;
    if (W > 2 * hw) { if (cx < x0 + hw) cx = x0 + hw; if (cx > x1 - hw) cx = x1 - hw; } else cx = (x0 + x1) / 2;
    if (H > 2 * hh) { if (cy < y0 + hh) cy = y0 + hh; if (cy > y1 - hh) cy = y1 - hh; } else cy = anchor_bottom ? y1 - hh : (y0 + y1) / 2;
    if (map_zoom) { map_panx = (cx - (here ? tx : (x0 + x1) / 2)); map_pany = cy - (here ? ty - vh * u / 6 : (y0 + y1) / 2); }   /* clamp the pan too */
    *upp = u; *wx0 = cx - hw; *wy0 = cy - hh;
}

/* - / + and the slash drawn in the plank text's style (cream / yellow, 1 px dark outline): the WFM fonts have none */
static void zb_sign(int cx, int cy, int plus, u32 c) {
    pa_fill(cx - 6, cy - 2, 13, 5, C_DARK); if (plus) pa_fill(cx - 2, cy - 6, 5, 13, C_DARK);
    pa_fill(cx - 5, cy - 1, 11, 3, c); if (plus) pa_fill(cx - 1, cy - 5, 3, 11, c);
}
static void zoom_bar(int W) {   /* [<L1  -]  Zoom n/N  [+  R1>] */
    char b[32]; int bw = 96, y = 56;
    plank(8, y, bw); hot(8, y, bw, 24, HS_FILTER + 16);
    plank(W - 8 - bw, y, bw); hot(W - 8 - bw, y, bw, 24, HS_FILTER + 17);
    pa_spr(16, y + 7, SUB, 56, 206, 22, 9, 288, 500, PA_OUTLINE);            /* the game's "<L1" / "R1>" glyphs (sub-screen page) */
    pa_spr(W - 16 - 24, y + 7, SUB, 80, 206, 24, 9, 288, 500, PA_OUTLINE);
    zb_sign(8 + bw - 28, y + 11, 0, map_zoom == 0 ? RGB(150, 120, 100) : C_WHITE);
    zb_sign(W - 8 - bw + 28, y + 11, 1, map_zoom == PANEL_MAP_ZOOMS - 1 ? RGB(150, 120, 100) : C_WHITE);
    int mw = W - 2 * bw - 32; plank(8 + bw + 8, y, mw);
    snprintf(b, sizeof b, "Zoom %d", map_zoom + 1);
    char n[8]; snprintf(n, sizeof n, "%d", PANEL_MAP_ZOOMS);
    int w1 = pa_text_w(16, b), w2 = pa_text_w(16, n), x = W / 2 - (w1 + 10 + w2) / 2;
    pa_text_ol(x, y + 3, 16, b, C_YELLOW, C_WOODE, C_DARK);
    line(x + w1 + 8, y + 5, x + w1 + 2, y + 18, 3, C_DARK); line(x + w1 + 8, y + 5, x + w1 + 2, y + 18, 1, C_YELLOW);
    pa_text_ol(x + w1 + 10, y + 3, 16, n, C_YELLOW, C_WOODE, C_DARK);
}

static void map_marks(int vx, int vy, float wx0, float wy0, float upp, int here, int area, int sec, int topdown) {
#define WX(x) (vx + (int)(((x) - wx0) / upp))
#define WY(y) (vy + (int)(((y) - wy0) / upp))
    for (unsigned i = 0; i < sizeof panel_event_locs / sizeof panel_event_locs[0]; i++) {
        const PanelEventLoc *L = &panel_event_locs[i];
        if (L->area != area || L->x == -32768) continue;
        if (topdown ? L->sec != sec : !(L->sec == sec || (L->sec < 31 && side_secs[L->sec] >= 0 && side_secs[L->sec] == side_secs[31]))) continue;
        int st = ev_state(L->ev);
        pa_event_icon(WX(L->x) - 9, WY(L->y) - 13, L->ev, PA_OUTLINE | PA_SMALL | (st == F_PENDING ? PA_DIM : 0));
    }
    if (here) {
        int tx = WX(P_X), ty = WY(P_Y), bob = (port_frame / 10) & 1;
        face_f(tx - 9, ty - 6 - bob - 18, PA_SMALL | (P_LEFT ? FACE_LEFT_FLIP : FACE_RIGHT_FLIP));
    }
#undef WX
#undef WY
}

static void page_side(int play) {
    int W = pa_cw, H = pa_ch;
    zoom_bar(W);
    int mx = 8, my = 84, mw = W - 16, mh = H - my - 8;
    window_(mx, my, mw, mh);
    int vx = mx + 7, vy = my + 7, vw = mw - 14, vh = mh - 14;
    map_vx = vx; map_vy = vy; map_vw = vw; map_vh = vh;
    static u32 *view; static int view_cap; static float last[3]; static int last_vw, last_vh, last_id = -1, last_seq = -1;
    if (vw * vh > view_cap) { free(view); view_cap = vw * vh; view = malloc((size_t)view_cap * 4); last_id = -1; }
    int area = G_AREA, sec = G_SECTION, key = area * 32 + sec;
    if (key != map_key) { map_key = key; map_panx = map_pany = 0; }
    if (!play) { sky_draw(view, vw, vh, 0); for (int j = 0; j < vh; j++) for (int i = 0; i < vw; i++) pa_put(vx + i, vy + j, view[j * vw + i]); pa_text(vx + 8, vy + 8, 16, T("Waiting for the game...", "Esperando a la partida..."), C_DARK, C_EDGE); return; }
    int td = map_topdown(), ok, id;
    float x0, y0, x1, y1, gw, wx0, wy0, upp;
    if (td) { ok = td_update(area, sec); id = 1000000 + td_model; x0 = td_x0; y0 = td_y0; x1 = td_x0 + td_w; y1 = td_y0 + td_h; gw = 320; }
    else { sky_tick(key); ok = side_update(); id = (int)side_model; x0 = side_x0; y0 = side_y0; x1 = side_x1; y1 = side_y1; gw = SIDE_GAME_W; }
    if (!ok) {
        sky_draw(view, vw, vh, 0);
        for (int j = 0; j < vh; j++) for (int i = 0; i < vw; i++) pa_put(vx + i, vy + j, view[j * vw + i]);
        pa_text(vx + 8, vy + 8, 16, T("Reading the map...", "Leyendo el mapa..."), C_DARK, C_EDGE);
        return;
    }
    map_view(x0, y0, x1, y1, gw, !td, vw, vh, 1, P_X, P_Y, &wx0, &wy0, &upp);
    map_upp_now = upp;
    if (!td && map_zoom) {   /* zoomed: the view's bottom on the ground under it, not the whole area's lowest point */
        int lb = -32768;
        for (int i = 0; i < nsp; i++) if (sp[i].bx1 >= wx0 && sp[i].bx0 <= wx0 + vw * upp && sp[i].by1 > lb) lb = sp[i].by1;
        if (lb > -32768 && wy0 + vh * upp > lb) { wy0 = lb - vh * upp; if (wy0 < y0) wy0 = y0; }
    }
    wx0 = (float)(int)(wx0 / upp) * upp; wy0 = (float)(int)(wy0 / upp) * upp;   /* whole output pixels: no shimmer */
    if (id != last_id || vw != last_vw || vh != last_vh || wx0 != last[0] || wy0 != last[1] || upp != last[2] || (!td && sky_seq != last_seq)) {
        if (td) td_render(view, vw, vh, wx0, wy0, upp); else side_render(view, vw, vh, wx0, wy0, upp);
        last_id = id; last_vw = vw; last_vh = vh; last[0] = wx0; last[1] = wy0; last[2] = upp; last_seq = sky_seq;
    }
    for (int j = 0; j < vh; j++) memcpy(pa_cv + (vy + j) * pa_cw + vx, view + j * vw, (size_t)vw * 4);
    int cx0 = pa_clip_x0, cy0 = pa_clip_y0, cx1 = pa_clip_x1, cy1 = pa_clip_y1;
    pa_clip_x0 = vx; pa_clip_y0 = vy; pa_clip_x1 = vx + vw; pa_clip_y1 = vy + vh;
    map_marks(vx, vy, wx0, wy0, upp, 1, area, sec, td);
    pa_clip_x0 = cx0; pa_clip_y0 = cy0; pa_clip_x1 = cx1; pa_clip_y1 = cy1;
}
