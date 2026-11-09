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
 * and shown screen-fixed (at the integer scale nearest the view's height, bottom aligned, the top row continued above;
 * each band repeated sideways with its own period found in the capture, slow parallax) as the game shows it.
 * Not drawn: objects (signs, NPCs, enemies: drawn by their handlers), effects.
 *
 * Zoom: PANEL_MAP_ZOOMS levels; 1 = the whole map fitted, the last = about the game screen's framing, geometric in
 * between, each snapped to whole canvas pixels per world unit or whole world units per canvas pixel (pixel perfect).  Zoomed levels follow Tomba; drag (touch / mouse) or the arrows pan, OK re-centres.  -/+ buttons (with
 * the game's L1 / R1 glyphs), keys PageDown/PageUp or - / + in the PC panel window, --panel-input zoomout / zoomin.
 * Continuous zoom (map_zf) by a two-finger pinch (Android), the mouse wheel (PC) or --panel-input pinch@PERCENT@X@Y.
 * Markers: the events placed in the section (panel_eventmap.h, their icons at 2x, outlined) and Tomba's face (2x)
 * at his live position (PLAYER x / y), flipped to the side he faces (PLAYER +0xA6: 0 left, 2 right). */
#include <stdint.h>
extern const unsigned short *port_gpu_vram(int x, int y, int w, int h);   /* shims/gpu.c */
extern int port_panel_sky_req;                                            /* shims/gpu.c */
extern unsigned long long port_time_us(void);
#ifndef PANEL_MAP_ZOOMS
#define PANEL_MAP_ZOOMS 3
#endif
#define P_X ((s16)G16(PV_PLAYER_X))
#define P_Y ((s16)G16(PV_PLAYER_Y))
#define P_LEFT (G8(PV_PLAYER_FACING) == 0)  /* PLAYER +0xA6: the side he faces, 0 left, 2 right (measured) */
#define FACE_LEFT_FLIP 0               /* the sub-screen face sprite looks left */
#define FACE_RIGHT_FLIP PA_FLIPX
#define SIDE_GAME_W 440               /* world units across the game screen at Tomba's depth (zoom's last level) */

static int side_raster_n; static unsigned long long side_raster_us;   /* --log-fps: side views composed since the last line (panel.c) */
static int map_zoom, map_drag = -1, map_dx, map_dy, map_vx, map_vy, map_vw, map_vh, map_key = -1;
static float map_panx, map_pany;
static float map_zf = 1;   /* continuous zoom (pinch / wheel): the level's world units per pixel times this, clamped to the levels' range */

/* ---------------------------------------------------------------- far background (sky) */
#define SKY_W 320
#define SKY_H 256
static u32 sky_px[SKY_W * SKY_H]; static int sky_w, sky_h, sky_seq, sky_key = -1, sky_next, sky_tries;
static u32 c15(u16 w) { return 0xFF000000u | (u32)((w >> 10 & 31) * 255 / 31) << 16 | (u32)((w >> 5 & 31) * 255 / 31) << 8 | (u32)((w & 31) * 255 / 31); }
/* The far layer is strips the overlay places for the camera, textures that repeat sideways, and only the tiles the
   room does not hide are drawn (the capture has holes of the clear colour).  Each row gets the period its drawn pixels
   show (the smallest shift under which the pixels drawn on both sides are the same, 99.5 %): its holes are filled from
   the same column of the other periods, and past the captured width it repeats with the capture's own phase, so the
   strips line up as the game's do.  A row with no period is shown once, its holes and both sides the clear colour. */
static int sky_rp[SKY_H]; static u32 sky_bg; static float sky_scroll0;
#define SKY_SCROLL(wx) ((wx) / (SIDE_GAME_W / 320.f) * 0.25f)   /* slow parallax: capture pixels per world unit of the view */
static int sky_period(const u32 *r) {   /* 0: none */
    for (int P = 16; P <= sky_w - 16; P++) {
        int known = 0, bad = 0;
        for (int i = 0; i + P < sky_w; i++) if (r[i] != sky_bg && r[i + P] != sky_bg) { known++; bad += r[i] != r[i + P]; }
        if (known * 8 >= sky_w - P && known >= 32 && bad * 200 <= known) return P;
    }
    return 0;
}
static void sky_bands(void) {
    u32 best = sky_px[0]; int bn = 0;   /* the clear colour: the most common one of the top row */
    for (int i = 0; i < sky_w; i += 4) { int n = 0; for (int k = 0; k < sky_w; k += 4) n += sky_px[k] == sky_px[i]; if (n > bn) { bn = n; best = sky_px[i]; } }
    sky_bg = best;
    for (int j = 0; j < sky_h; j++) {
        u32 *r = sky_px + j * SKY_W;
        int P = sky_period(r);
        sky_rp[j] = P;
        if (P) for (int i = 0; i < sky_w; i++) if (r[i] == sky_bg) for (int x = i % P; x < sky_w; x += P) if (r[x] != sky_bg) { r[i] = r[x]; break; }
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
    sky_scroll0 = SKY_SCROLL(P_X);
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
    if (sky_h <= 0) return;
    int sc = (vh + sky_h / 2) / sky_h; if (sc < 1) sc = 1;   /* whole pixels, bottom aligned, the capture centred when the
                                                                view is where the camera was when it was taken */
    int top = vh - sky_h * sc, off = (int)((scroll - sky_scroll0) * sc) - (vw - sky_w * sc) / 2;
    for (int j = 0; j < vh; j++) {
        int sy = j - top; sy = sy < 0 ? 0 : sy / sc; if (sy >= sky_h) sy = sky_h - 1;   /* above the capture: its top row */
        int p = sky_rp[sy];
        const u32 *row = sky_px + sy * SKY_W;
        for (int i = 0; i < vw; i++) {
            int x = i + off, sx = x >= 0 ? x / sc : -((-x + sc - 1) / sc);
            if (sx < 0 || sx >= sky_w) { if (!p) { dst[j * vw + i] = sky_bg; continue; } sx %= p; if (sx < 0) sx += p; }
            dst[j * vw + i] = row[sx];
        }
    }
}

/* ---------------------------------------------------------------- side model */
typedef struct { s16 x[4], y[4]; u8 u[4], v[4], c[4][3]; u16 clut, tp; u8 nv, cmd; s16 bx0, by0, bx1, by1; int z; } SPoly;
static SPoly *sp; static int nsp, sp_cap, side_state, side_area = -1, side_sig, side_seen, side_x0, side_y0, side_x1, side_y1;
static u16 *side_vr;                  /* VRAM snapshot (1024 x 512) taken when the model was read */
static int side_snap;                 /* port_frame of the last VRAM check */
static u16 side_tps[64], side_cluts[256]; static int side_ntp, side_nclut;   /* the pages / CLUTs the polygons sample */
static unsigned side_model;           /* id of the model the view shows (changes when another is read) */
static unsigned side_geom, side_tex;  /* generations: the polygons read (side_build), the VRAM snapshot (and refreshes) */
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
    side_ntp = side_nclut = 0;
    for (int i = 0; i < nsp; i++) {
        if (!(sp[i].cmd & 4)) continue;
        u16 tp = sp[i].tp & 0x1FF, cl = sp[i].clut, mode = tp >> 7 & 3;
        int k;
        for (k = 0; k < side_ntp && side_tps[k] != tp; k++) ;
        if (k == side_ntp && side_ntp < 64) side_tps[side_ntp++] = tp;
        if (mode < 2) { for (k = 0; k < side_nclut && side_cluts[k] != (u16)(cl | mode << 15); k++) ; if (k == side_nclut && side_nclut < 256) side_cluts[side_nclut++] = (u16)(cl | mode << 15); }
    }
    side_snap = port_frame;
    if (!side_vr) side_vr = malloc(1024 * 512 * 2);
    memcpy(side_vr, port_gpu_vram(0, 0, 1024, 512), 1024 * 512 * 2);
    side_model++; side_geom++; side_tex++;
    port_printf("[panel] side map: area %d section %d, %d polygons, x %d..%d y %d..%d\n", G_AREA, G_SECTION, nsp, side_x0, side_x1, side_y0, side_y1);
    return 1;
}
/* the textures change under the same model: the game streams each section's texture pages / CLUTs into the same VRAM
   slots (and animates some), so a snapshot taken in one section shows another's ground as noise.  Every 25 frames out
   of loading screens, the rectangles the polygons sample are compared with the live VRAM and the snapshot refreshed */
static void side_refresh_vram(void) {
    extern int port_loading_frame;
    if (!side_vr || port_frame - side_snap < 25 || port_frame - port_loading_frame < 30) return;
    side_snap = port_frame;
    const u16 *v = port_gpu_vram(0, 0, 1024, 512);
    int diff = 0;
    for (int i = 0; i < side_ntp && !diff; i++) {
        int tp = side_tps[i], mode = tp >> 7 & 3, x = (tp & 15) * 64, y = (tp >> 4 & 1) * 256, w = mode == 0 ? 64 : mode == 1 ? 128 : 256;
        if (x + w > 1024) w = 1024 - x;
        for (int j = 0; j < 256 && !diff; j++) diff = memcmp(side_vr + (y + j) * 1024 + x, v + (y + j) * 1024 + x, (size_t)w * 2) != 0;
    }
    for (int i = 0; i < side_nclut && !diff; i++) {
        int cl = side_cluts[i] & 0x7FFF, n = side_cluts[i] >> 15 ? 256 : 16, o = (cl >> 6 & 511) * 1024 + (cl & 63) * 16;
        if (o + n > 1024 * 512) n = 1024 * 512 - o;
        diff = memcmp(side_vr + o, v + o, (size_t)n * 2) != 0;
    }
    if (diff) { memcpy(side_vr, v, 1024 * 512 * 2); side_model++; side_tex++; port_printf("[panel] side map: textures changed (area %d section %d), VRAM snapshot refreshed\n", G_AREA, G_SECTION); }
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
    if (side_state == 1 && sig == side_secs[31]) side_refresh_vram();
    return side_state == 1 && sig == side_secs[31];
}

/* one triangle into the supersampled RGBA buffer (alpha 0 = empty): buf holds the samples (ox..ox+bw-1, oy..oy+bh-1) of
   the picture whose sample (0, 0) is where px / py are 0 */
static void side_tri(u32 *buf, int ox, int oy, int bw, int bh, const float *px, const float *py, const SPoly *o, const int *ix) {
    float den = (py[1] - py[2]) * (px[0] - px[2]) + (px[2] - px[1]) * (py[0] - py[2]);
    if (den > -1e-6f && den < 1e-6f) return;
    float mnx = px[0], mxx = px[0], mny = py[0], mxy = py[0];
    for (int i = 1; i < 3; i++) { if (px[i] < mnx) mnx = px[i]; if (px[i] > mxx) mxx = px[i]; if (py[i] < mny) mny = py[i]; if (py[i] > mxy) mxy = py[i]; }
    int x0 = (int)(mnx - .5f), x1 = (int)(mxx + .5f), y0 = (int)(mny - .5f), y1 = (int)(mxy + .5f);
    if (x0 < ox) x0 = ox; if (y0 < oy) y0 = oy; if (x1 >= ox + bw) x1 = ox + bw - 1; if (y1 >= oy + bh) y1 = oy + bh - 1;
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
            u32 *d = &buf[(y - oy) * bw + (x - ox)];
            if (semi && stp) {
                u32 o2 = *d; int al = o2 >> 24;
                if (al) { r = (r + (int)(o2 & 255)) / 2; g = (g + (int)(o2 >> 8 & 255)) / 2; bl = (bl + (int)(o2 >> 16 & 255)) / 2; *d = (u32)(al > 128 ? al : 128) << 24 | (u32)bl << 16 | (u32)g << 8 | (u32)r; }
                else *d = 0x80000000u | (u32)bl << 16 | (u32)g << 8 | (u32)r;
            } else *d = 0xFF000000u | (u32)bl << 16 | (u32)g << 8 | (u32)r;
        }
    }
}
/* ---------------------------------------------------------------- side map cache
   The rasterised room is a picture: it is drawn once per zoom into tiles of SC_T x SC_T view pixels and the view is
   composed from them (the far background under them, then the markers), so a view that follows Tomba or is dragged
   costs a copy, not a raster.  The tiles are built on the game thread (the panel runs in port_gpu_vblank; the wasm2c
   build has no threads in the module) in slices of at most SC_BUDGET_US a vblank, resumable between two polygons:
   first the tiles the view shows (the nearest to its centre first), then a margin around it, then the rest of the
   room while the cache is under 3/4 of SC_MEM.  A tile not built yet shows the far background, or the same tile drawn
   with the textures before the last refresh; after a zoom change the previous picture stays (with its markers) until
   the new one's visible tiles are done, at most 2 s.
   Same picture as the live raster: an image (one per model and world units per pixel) has an anchor, the view origin
   it was first shown at, and every tile is that anchored view's raster clipped to the tile (side_tri with the sample
   origin moved), so the view at the anchor is pixel for pixel what side_render drew; views panned from it are the
   same raster translated by whole pixels.  Each tile pixel keeps what the box filter needs to blend over the far
   background exactly: the colour when all samples are covered, nothing when none, else the sums (part[]).
   Tiles are keyed by image and texture generation (side_tex: the VRAM snapshot refreshed, panel.c "side-map texture
   refresh"), so a refresh rebuilds them without blanking the map; least recently used tiles go past SC_MEM. */
#define SC_T 64
#define SC_BUDGET_US 1500
#define SC_MEM (24 << 20)
#define SC_IMGS 8
typedef struct STile { unsigned img, tex; int tx, ty, used, bytes, npart; u32 *px; int *part; struct STile *next; } STile;
typedef struct { unsigned id, geom; float upp; int ax, ay, used, cursor, done; } SImg;
static SImg sc_img[SC_IMGS]; static unsigned sc_img_id;
static STile *sc_hash[1024]; static long sc_bytes; static unsigned sc_gen; static int sc_ntiles;
static struct { int on; unsigned img, tex, geom; int tx, ty, poly; } sc_job;
static struct { unsigned img; int x0, y0, w, h, mx0, my0, mx1, my1, frame; } sc_want;   /* page_side: the view and the map (image pixels) */
static unsigned long long sc_build_us, sc_build_max; static int sc_build_tiles, sc_build_n;   /* --log-fps (panel.c panel_time_log) */
static int sc_fdiv(int a, int b) { return a >= 0 ? a / b : -((-a + b - 1) / b); }
static unsigned sc_h(unsigned img, int tx, int ty) { return (img * 2654435761u ^ (unsigned)tx * 40503u ^ (unsigned)ty * 9973u) & 1023; }
static STile *sc_find(unsigned img, unsigned tex, int tx, int ty, int any) {   /* any: the newest texture generation */
    STile *best = 0;
    for (STile *t = sc_hash[sc_h(img, tx, ty)]; t; t = t->next)
        if (t->img == img && t->tx == tx && t->ty == ty) { if (t->tex == tex) return t; if (any && (!best || t->tex > best->tex)) best = t; }
    return best;
}
static void sc_free_tile(STile *t) {
    STile **pp = &sc_hash[sc_h(t->img, t->tx, t->ty)];
    while (*pp != t) pp = &(*pp)->next;
    *pp = t->next; sc_bytes -= t->bytes; sc_ntiles--;
    free(t->px); free(t->part); free(t);
}
static void sc_drop(int (*f)(const STile *, unsigned), unsigned arg) {
    for (int i = 0; i < 1024; i++) for (STile *t = sc_hash[i], *n; t; t = n) { n = t->next; if (f(t, arg)) sc_free_tile(t); }
}
static int sc_of_img(const STile *t, unsigned img) { return t->img == img; }
static void sc_evict(void) {   /* the least recently used tiles, never one of the view's */
    while (sc_bytes > SC_MEM) {
        STile *lru = 0;
        for (int i = 0; i < 1024; i++) for (STile *t = sc_hash[i]; t; t = t->next) if (!lru || t->used < lru->used) lru = t;
        if (!lru || lru->used >= port_frame - 2) break;
        sc_free_tile(lru);
    }
}
static SImg *sc_image(unsigned geom, float upp, int nx, int ny) {   /* the image of this model and scale (anchored at nx, ny if new) */
    SImg *old = &sc_img[0];
    for (int i = 0; i < SC_IMGS; i++) {
        SImg *m = &sc_img[i];
        if (m->id && m->geom == geom && m->upp == upp) { m->used = port_frame; return m; }
        if (!m->id || (old->id && m->used < old->used)) old = m;
    }
    if (old->id) sc_drop(sc_of_img, old->id);
    old->id = ++sc_img_id; old->geom = geom; old->upp = upp; old->ax = nx; old->ay = ny; old->used = port_frame; old->cursor = 0; old->done = 0;
    return old;
}
static SImg *sc_img_by_id(unsigned id) { for (int i = 0; i < SC_IMGS; i++) if (sc_img[i].id == id && id) return &sc_img[i]; return 0; }
/* one slice of the tile job: polygons from sc_job.poly until the deadline; 1 when the tile is done */
static int sc_job_step(unsigned long long deadline) {
    static u32 sb[SC_T * 3 * SC_T * 3];
    SImg *m = sc_img_by_id(sc_job.img);
    if (!m || sc_job.geom != side_geom || sc_job.tex != side_tex || m->geom != side_geom) { sc_job.on = 0; return 0; }   /* the model or textures changed */
    float upp = m->upp;
    int ss = upp >= 4 ? 3 : upp >= 1.5f ? 2 : 1, bw = SC_T * ss, ox = sc_job.tx * bw, oy = sc_job.ty * bw;
    float wxa = (float)m->ax * upp, wya = (float)m->ay * upp, k = ss / upp;   /* the anchored view's origin, as page_side snaps it */
    float lx = wxa + (sc_job.tx * SC_T - 1) * upp, hx = wxa + ((sc_job.tx + 1) * SC_T + 1) * upp,
          ly = wya + (sc_job.ty * SC_T - 1) * upp, hy = wya + ((sc_job.ty + 1) * SC_T + 1) * upp;
    if (sc_job.poly == 0) memset(sb, 0, (size_t)bw * bw * 4);
    int i = sc_job.poly, chk = 0;
    for (; i < nsp; i++) {
        const SPoly *o = &sp[i];
        if (o->bx1 < lx || o->bx0 > hx || o->by1 < ly || o->by0 > hy) { if ((++chk & 255) == 0 && port_time_us() > deadline) break; continue; }
        float px[4], py[4];
        for (int j = 0; j < o->nv; j++) { px[j] = (o->x[j] - wxa) * k; py[j] = (o->y[j] - wya) * k; }
        static const int i0[3] = { 0, 1, 2 }, i1[3] = { 1, 2, 3 };
        side_tri(sb, ox, oy, bw, bw, px, py, o, i0);
        if (o->nv == 4) { float qx[3] = { px[1], px[2], px[3] }, qy[3] = { py[1], py[2], py[3] }; side_tri(sb, ox, oy, bw, bw, qx, qy, o, i1); }
        if ((++chk & 3) == 0 && port_time_us() > deadline) { i++; break; }
    }
    sc_job.poly = i;
    if (i < nsp) return 0;
    /* the box filter of side_render, kept unblended: colour (all samples covered), empty, or the sums */
    static int part[SC_T * SC_T * 4]; int np = 0, n = ss * ss * 255, any = 0;
    u32 *px = malloc(SC_T * SC_T * 4);
    for (int y = 0; y < SC_T; y++) for (int x = 0; x < SC_T; x++) {
        int r = 0, g = 0, b = 0, a = 0;
        for (int j = 0; j < ss; j++) for (int q = 0; q < ss; q++) {
            u32 c = sb[(y * ss + j) * bw + x * ss + q]; int al = c >> 24;
            r += (c & 255) * al; g += (c >> 8 & 255) * al; b += (c >> 16 & 255) * al; a += al;
        }
        u32 v = 0;
        if (a == n) v = 0xFF000000u | (u32)(b / n) << 16 | (u32)(g / n) << 8 | (u32)(r / n);
        else if (a) { v = 0x01000000u | (u32)np; part[4 * np] = r; part[4 * np + 1] = g; part[4 * np + 2] = b; part[4 * np + 3] = a; np++; }
        px[y * SC_T + x] = v; any |= v != 0;
    }
    STile *t = calloc(1, sizeof *t);
    t->img = sc_job.img; t->tex = sc_job.tex; t->tx = sc_job.tx; t->ty = sc_job.ty; t->used = port_frame; t->npart = np;
    if (any) { t->px = px; if (np) { t->part = malloc((size_t)np * 16); memcpy(t->part, part, (size_t)np * 16); } }
    else free(px);   /* nothing of the room here */
    t->bytes = (int)sizeof *t + (any ? SC_T * SC_T * 4 : 0) + np * 16;
    unsigned h = sc_h(t->img, t->tx, t->ty); t->next = sc_hash[h]; sc_hash[h] = t;
    sc_bytes += t->bytes; sc_ntiles++; sc_gen++; sc_build_tiles++;
    for (STile *u = sc_hash[h], *nx; u; u = nx) { nx = u->next; if (u != t && u->img == t->img && u->tx == t->tx && u->ty == t->ty) sc_free_tile(u); }   /* older textures */
    sc_job.on = 0;
    sc_evict();
    return 1;
}
/* the next tile to build for the view sc_want (0: none) */
static int sc_pick(SImg *m, int *otx, int *oty) {
    int vx0 = sc_want.x0, vy0 = sc_want.y0, cx = vx0 + sc_want.w / 2, cy = vy0 + sc_want.h / 2;
    int mtx0 = sc_fdiv(sc_want.mx0, SC_T), mty0 = sc_fdiv(sc_want.my0, SC_T), mtx1 = sc_fdiv(sc_want.mx1 - 1, SC_T), mty1 = sc_fdiv(sc_want.my1 - 1, SC_T);
    for (int pass = 0; pass < 2; pass++) {   /* the view, then a margin (half a view across, a quarter down) */
        int ex = pass ? sc_want.w / 2 : 0, ey = pass ? sc_want.h / 4 : 0;
        int tx0 = sc_fdiv(vx0 - ex, SC_T), ty0 = sc_fdiv(vy0 - ey, SC_T), tx1 = sc_fdiv(vx0 + sc_want.w + ex - 1, SC_T), ty1 = sc_fdiv(vy0 + sc_want.h + ey - 1, SC_T);
        if (pass) { if (tx0 < mtx0) tx0 = mtx0; if (ty0 < mty0) ty0 = mty0; if (tx1 > mtx1) tx1 = mtx1; if (ty1 > mty1) ty1 = mty1; }
        long bd = -1;
        for (int ty = ty0; ty <= ty1; ty++) for (int tx = tx0; tx <= tx1; tx++) {
            if (sc_find(m->id, side_tex, tx, ty, 0)) continue;
            long dx = tx * SC_T + SC_T / 2 - cx, dy = ty * SC_T + SC_T / 2 - cy, d = dx * dx + dy * dy;
            if (bd < 0 || d < bd) { bd = d; *otx = tx; *oty = ty; }
        }
        if (bd >= 0) return 1;
    }
    if (m->done || sc_bytes > SC_MEM / 4 * 3) return 0;   /* the rest of the room, row by row */
    int nw = mtx1 - mtx0 + 1, nh = mty1 - mty0 + 1;
    for (; m->cursor < nw * nh; m->cursor++) {
        int tx = mtx0 + m->cursor % nw, ty = mty0 + m->cursor / nw;
        if (!sc_find(m->id, side_tex, tx, ty, 0)) { *otx = tx; *oty = ty; return 1; }
    }
    m->done = 1;
    return 0;
}
/* every vblank while the side view is shown: build tiles for at most SC_BUDGET_US */
static void side_cache_work(void) {
    if (port_frame - sc_want.frame > 4 || !sc_want.img) return;
    SImg *m = sc_img_by_id(sc_want.img);
    if (!m || m->geom != side_geom) return;
    unsigned long long t0 = port_time_us(), deadline = t0 + SC_BUDGET_US;
    for (int guard = 0; guard < 4096; guard++) {
        if (sc_job.on && sc_job.img != m->id) sc_job.on = 0;   /* the view moved to another zoom: its tile first */
        if (!sc_job.on) {
            int tx, ty;
            if (!sc_pick(m, &tx, &ty)) break;
            sc_job.on = 1; sc_job.img = m->id; sc_job.tex = side_tex; sc_job.geom = side_geom; sc_job.tx = tx; sc_job.ty = ty; sc_job.poly = 0;
        }
        sc_job_step(deadline);
        if (port_time_us() >= deadline) break;
    }
    unsigned long long dt = port_time_us() - t0;
    sc_build_us += dt; sc_build_n++; if (dt > sc_build_max) sc_build_max = dt;
}
/* the view (vw x vh, image pixels from (nx, ny)) from the tiles over the far background; 1 when every tile was there */
static int side_compose(u32 *dst, int vw, int vh, SImg *m, int nx, int ny, float wx0, float upp) {   /* dst NULL: only the answer */
    if (dst) sky_draw(dst, vw, vh, SKY_SCROLL(wx0 + vw * upp / 2));
    int ss = upp >= 4 ? 3 : upp >= 1.5f ? 2 : 1, n = ss * ss * 255, all = 1;
    int X0 = nx - m->ax, Y0 = ny - m->ay;
    for (int ty = sc_fdiv(Y0, SC_T); ty * SC_T < Y0 + vh; ty++) for (int tx = sc_fdiv(X0, SC_T); tx * SC_T < X0 + vw; tx++) {
        STile *t = sc_find(m->id, side_tex, tx, ty, 1);
        if (!t) { all = 0; continue; }
        if (t->tex != side_tex) all = 0;
        if (!dst) continue;
        t->used = port_frame;
        if (!t->px) continue;
        int x0 = tx * SC_T - X0, y0 = ty * SC_T - Y0, i0 = x0 < 0 ? -x0 : 0, j0 = y0 < 0 ? -y0 : 0;
        int i1 = vw - x0 < SC_T ? vw - x0 : SC_T, j1 = vh - y0 < SC_T ? vh - y0 : SC_T;
        for (int j = j0; j < j1; j++) {
            const u32 *s = t->px + j * SC_T; u32 *d = dst + (y0 + j) * vw + x0;
            for (int i = i0; i < i1; i++) {
                u32 c = s[i];
                if (!c) continue;
                if (c >> 24 == 0xFF) { d[i] = c; continue; }
                const int *p = t->part + 4 * (c & 0xFFFFFF); int a = p[3];
                u32 bg = d[i];
                int r = (p[0] + (int)(bg & 255) * (n - a)) / n, g = (p[1] + (int)(bg >> 8 & 255) * (n - a)) / n, b = (p[2] + (int)(bg >> 16 & 255) * (n - a)) / n;
                d[i] = 0xFF000000u | (u32)b << 16 | (u32)g << 8 | (u32)r;
            }
        }
    }
    return all;
}
/* the side view into view (vw x vh) at image pixel (nx, ny) = world (*wx0, *wy0), *upp world units per pixel; when the
   previous picture is kept (a zoom change whose tiles are not built yet) the transform it was drawn with is returned */
static void side_view(u32 *view, int vw, int vh, int nx, int ny, float x0, float y0, float x1, float y1, float *wx0, float *wy0, float *upp) {
    static unsigned s_img, s_geom, s_tex, s_gen, hold_img; static int s_nx, s_ny, s_vw, s_vh, s_seq, s_ok, valid, hold_from;
    static float s_wx0, s_wy0, s_upp;
    float u = *upp;
    SImg *m = sc_image(side_geom, u, nx, ny);
    sc_want.img = m->id; sc_want.x0 = nx - m->ax; sc_want.y0 = ny - m->ay; sc_want.w = vw; sc_want.h = vh; sc_want.frame = port_frame;
    sc_want.mx0 = (int)(x0 / u) - 1 - m->ax; sc_want.my0 = (int)(y0 / u) - 1 - m->ay; sc_want.mx1 = (int)(x1 / u) + 2 - m->ax; sc_want.my1 = (int)(y1 / u) + 2 - m->ay;
    if (valid && m->id == s_img && nx == s_nx && ny == s_ny && vw == s_vw && vh == s_vh && sky_seq == s_seq && sc_gen == s_gen && side_tex == s_tex) return;
    if (valid && s_ok && m->id != s_img && s_geom == side_geom && vw == s_vw && vh == s_vh && !side_compose(NULL, vw, vh, m, nx, ny, *wx0, u)) {
        if (hold_img != m->id) { hold_img = m->id; hold_from = port_frame; }
        if (port_frame - hold_from < 100) { *wx0 = s_wx0; *wy0 = s_wy0; *upp = s_upp; return; }   /* the previous zoom's picture */
    }
    unsigned long long t0 = port_time_us();
    s_ok = side_compose(view, vw, vh, m, nx, ny, *wx0, u);
    side_raster_us += port_time_us() - t0; side_raster_n++;
    s_img = m->id; s_geom = side_geom; s_tex = side_tex; s_gen = sc_gen; s_nx = nx; s_ny = ny; s_vw = vw; s_vh = vh; s_seq = sky_seq; valid = 1;
    s_wx0 = *wx0; s_wy0 = *wy0; s_upp = u;
}
static void side_cache_desc(char *b, int n) {   /* --log-fps */
    snprintf(b, n, "%d tiles %ld KB, built %d tiles in %llu.%02llu ms (%d vblanks, max %llu.%02llu ms a vblank)", sc_ntiles, sc_bytes >> 10, sc_build_tiles,
             sc_build_us / 1000, sc_build_us / 10 % 100, sc_build_n, sc_build_max / 1000, sc_build_max / 10 % 100);
    sc_build_us = sc_build_max = 0; sc_build_tiles = sc_build_n = 0;
}

#include "panel_topdown.inc"

/* ---------------------------------------------------------------- view, zoom, input */
static int map_topdown(void) { return in_gameplay() && G_AREA < 20 && td_section(G_AREA, G_SECTION); }
static const char *map_tab_name(void) { return map_topdown() ? T("Top-down", "Cenital") : T("Map", "Mapa"); }
static void map_set_zoom(int z) {
    if (z < 0) z = 0; if (z > PANEL_MAP_ZOOMS - 1) z = PANEL_MAP_ZOOMS - 1;
    if (z != map_zoom || map_zf != 1) { map_zoom = z; map_zf = 1; map_panx = map_pany = 0; }
}
static int map_zoomed_now;
static float map_upp_now = 1;
static float map_ulev[PANEL_MAP_ZOOMS];   /* the levels' world units per pixel in the last view */
/* -/+ : the next level out / in from the current view (after a pinch, the level just past where the view is) */
static void map_step(int dir) {
    if (map_zf == 1 || !map_ulev[0]) { map_set_zoom(map_zoom + dir); return; }
    float u = map_upp_now; int z = dir > 0 ? PANEL_MAP_ZOOMS - 1 : 0;
    if (dir > 0) { for (int k = 0; k < PANEL_MAP_ZOOMS; k++) if (map_ulev[k] < u * .999f) { z = k; break; } }
    else { for (int k = PANEL_MAP_ZOOMS - 1; k >= 0; k--) if (map_ulev[k] > u * 1.001f) { z = k; break; } }
    map_zoom = z; map_zf = 1; map_panx = map_pany = 0;
}   /* the last view was closer than level 1 (follows Tomba, pans) */
static void map_pan(float dx, float dy) { if (map_zoomed_now) { map_panx += dx; map_pany += dy; } }
/* a pinch: the distance between the fingers grew by f (> 1: closer), centred on canvas (x, y) (-1: the view's centre) */
static void map_pinch(float f, int x, int y) {
    if (f < .2f || f > 5) return;
    float u = map_upp_now, u2 = u / f;
    map_zf /= f;                                      /* clamped by map_view */
    if (x >= map_vx && y >= map_vy && x < map_vx + map_vw && y < map_vy + map_vh && map_zoomed_now) {
        /* keep the world point under the pinch where it is */
        map_panx += (x - map_vx - map_vw / 2.f) * (u - u2); map_pany += (y - map_vy - map_vh / 2.f) * (u - u2);
    }
    map_drag = -1;
}
static int map_action(int a) {   /* keys / pad on the map tab (1: used) */
    float st = 48 * map_upp_now;
    switch (a) {
    case IN_ZOOM_OUT: map_step(-1); return 1;
    case IN_ZOOM_IN: map_step(1); return 1;
    case IN_LEFT: map_pan(-st, 0); return 1;
    case IN_RIGHT: map_pan(st, 0); return 1;
    case IN_UP: map_pan(0, -st); return 1;
    case IN_DOWN: map_pan(0, st); return 1;
    case IN_OK: case IN_BACK: map_panx = map_pany = 0; return 1;
    }
    return 0;
}
static void map_hit(int k) { if (k == 16) map_step(-1); else if (k == 17) map_step(1); }
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
    /* geometric levels u0 * (ug / u0) ^ (k / (N - 1)) (no libm: the port's math shims), each snapped pixel perfect:
       whole world units per canvas pixel (n) or whole canvas pixels per world unit (1 / m); the last level rounds,
       the others round towards the wider view (level 1: the whole map fits), and every level is closer than the one
       before it */
    float s = 1, u = u0, prev = 1e9f;
    if (PANEL_MAP_ZOOMS > 1) {
        float r = ug / u0, lo = r, hi = 1;
        for (int it = 0; it < 40; it++) { float m = (lo + hi) / 2, p = 1; for (int k = 0; k < PANEL_MAP_ZOOMS - 1; k++) p *= m; if (p > r) hi = m; else lo = m; }
        s = (lo + hi) / 2;
    }
    float ulev[PANEL_MAP_ZOOMS];
    for (int k = 0, last = PANEL_MAP_ZOOMS - 1; k <= last; k++) {
        float g = u0, q;
        for (int i = 0; i < k; i++) g *= s;
        if (g >= 1) { int n = k == last ? (int)(g + .5f) : (int)(g + .999f); q = (float)(n < 1 ? 1 : n); }
        else { int m = k == last ? (int)(1 / g + .5f) : (int)(1 / g + .001f); q = 1.f / (m < 1 ? 1 : m > 8 ? 8 : m); }
        if (q >= prev) q = prev > 1.5f ? (float)((int)(prev + .5f) - 1) : 1.f / ((int)(1 / prev + .5f) + 1);   /* one step closer */
        ulev[k] = prev = q;
    }
    for (int k = 0; k < PANEL_MAP_ZOOMS; k++) map_ulev[k] = ulev[k];
    u = ulev[map_zoom];
    if (map_zf != 1) {   /* the pinch's factor, kept inside level 1 .. the last level */
        float uz = u * map_zf;
        if (uz > ulev[0]) uz = ulev[0]; if (uz < ulev[PANEL_MAP_ZOOMS - 1]) uz = ulev[PANEL_MAP_ZOOMS - 1];
        map_zf = uz / u; u = uz;
    }
    int zoomed = u < ulev[0] * .999f;
    map_zoomed_now = zoomed;
    float cx = (x0 + x1) / 2, cy = (y0 + y1) / 2;
    if (zoomed && here) { cx = tx + map_panx; cy = ty - vh * u / 6 + map_pany; }
    else if (zoomed) { cx += map_panx; cy += map_pany; }
    float hw = vw * u / 2, hh = vh * u / 2;
    if (W > 2 * hw) { if (cx < x0 + hw) cx = x0 + hw; if (cx > x1 - hw) cx = x1 - hw; } else cx = (x0 + x1) / 2;
    if (H > 2 * hh) { if (cy < y0 + hh) cy = y0 + hh; if (cy > y1 - hh) cy = y1 - hh; } else cy = anchor_bottom ? y1 - hh : (y0 + y1) / 2;
    if (zoomed) { map_panx = (cx - (here ? tx : (x0 + x1) / 2)); map_pany = cy - (here ? ty - vh * u / 6 : (y0 + y1) / 2); }   /* clamp the pan too */
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

/* "Ver en el mapa" (the event's detail page): the view centred on its marker, which hops for a few seconds.  The map tab
   shows the room Tomba is in, so only the events placed there (or in a section sharing its model) can be shown. */
static int map_focus_ev = -1, map_flash_ev = -1, map_flash_t;
static int map_shows_event(int e) {
    const PanelEventLoc *L = ev_loc(e);
    if (!L || !in_gameplay() || G_AREA >= 20 || L->area != G_AREA || L->x == -32768 || ev_hidden_now(e)) return 0;
    int sec = G_SECTION;
    if (map_topdown()) return L->sec == sec;
    return L->sec == sec || (side_area == G_AREA && L->sec < 31 && side_secs[L->sec] >= 0 && side_secs[L->sec] == side_secs[31]);
}
static void map_focus(int e) { if (map_shows_event(e)) map_focus_ev = e; }
static void map_marks(int vx, int vy, int vh, float wx0, float wy0, float upp, int here, int area, int sec, int topdown) {
    int sc = mk_scale(vh);
#define WX(x) (vx + (int)(((x) - wx0) / upp))
#define WY(y) (vy + (int)(((y) - wy0) / upp))
    for (unsigned i = 0; i < sizeof panel_event_locs / sizeof panel_event_locs[0]; i++) {
        const PanelEventLoc *L = &panel_event_locs[i];
        if (L->area != area || L->x == -32768) continue;
        if (topdown ? L->sec != sec : !(L->sec == sec || (L->sec < 31 && side_secs[L->sec] >= 0 && side_secs[L->sec] == side_secs[31]))) continue;
        if (ev_hidden_now(L->ev)) continue;
        int st = ev_state(L->ev);
        int hop = L->ev == map_flash_ev && port_frame - map_flash_t < 240 ? ((port_frame / 8) & 1) * 3 * sc : 0;
        event_mark(WX(L->x), WY(L->y) + 3 * sc - hop, L->ev, sc, st == F_PENDING ? PA_DIM : 0);
    }
    if (here) {
        int tx = WX(P_X), ty = WY(P_Y), bob = ((port_frame / 10) & 1) * sc;
        face_mark(tx, ty - 6 - bob, sc, P_LEFT ? FACE_LEFT_FLIP : FACE_RIGHT_FLIP);
    }
#undef WX
#undef WY
}

static void page_side(int play) {
    int W = pa_cw, H = pa_ch;
    zoom_bar(W);
    int mx = 8, my = 84, mw = W - 16, mh = H - my - 8;
    frame_(mx, my, mw, mh);   /* its rim: the view (below) covers all of the inside */
    int vx = mx + 7, vy = my + 7, vw = mw - 14, vh = mh - 14;
    map_vx = vx; map_vy = vy; map_vw = vw; map_vh = vh;
    static u32 *view; static int view_cap; static float last[3]; static int last_vw, last_vh, last_id = -1;
    if (vw * vh > view_cap) { free(view); view_cap = vw * vh; view = malloc((size_t)view_cap * 4); last_id = -1; }
    int area = G_AREA, sec = G_SECTION, key = area * 32 + sec;
    if (key != map_key) { map_key = key; map_panx = map_pany = 0; }
    if (!play) { sky_draw(view, vw, vh, sky_scroll0); for (int j = 0; j < vh; j++) for (int i = 0; i < vw; i++) pa_put(vx + i, vy + j, view[j * vw + i]); pa_text(vx + 8, vy + 8, 16, T("Waiting for the game...", "Esperando a la partida..."), C_DARK, C_EDGE); return; }
    int td = map_topdown(), ok, id;
    float x0, y0, x1, y1, gw, wx0, wy0, upp;
    if (td) { ok = td_update(area, sec); id = 1000000 + td_model; x0 = td_x0; y0 = td_y0; x1 = td_x0 + td_w; y1 = td_y0 + td_h; gw = 320; }
    else { sky_tick(key); ok = side_update(); id = (int)side_model; x0 = side_x0; y0 = side_y0; x1 = side_x1; y1 = side_y1; gw = SIDE_GAME_W; }
    if (!ok) {
        sky_draw(view, vw, vh, sky_scroll0);
        for (int j = 0; j < vh; j++) for (int i = 0; i < vw; i++) pa_put(vx + i, vy + j, view[j * vw + i]);
        pa_text(vx + 8, vy + 8, 16, T("Reading the map...", "Leyendo el mapa..."), C_DARK, C_EDGE);
        return;
    }
    if (map_focus_ev >= 0) {   /* centre the event (map_view puts Tomba a sixth of the view below the centre) */
        const PanelEventLoc *L = ev_loc(map_focus_ev);
        if (map_zoom == 0 && PANEL_MAP_ZOOMS > 1) map_set_zoom(1);
        map_view(x0, y0, x1, y1, gw, !td, vw, vh, 1, P_X, P_Y, &wx0, &wy0, &upp);   /* (the levels of this view) */
        float u = map_ulev[map_zoom] * map_zf;
        map_panx = L->x - P_X; map_pany = L->y - P_Y + vh * u / 6;
        map_flash_ev = map_focus_ev; map_flash_t = port_frame; map_focus_ev = -1;
    }
    map_view(x0, y0, x1, y1, gw, !td, vw, vh, 1, P_X, P_Y, &wx0, &wy0, &upp);
    map_upp_now = upp;
    if (!td && map_zoomed_now) {   /* zoomed: the view's bottom on the ground under it, not the whole area's lowest point */
        int lb = -32768;
        for (int i = 0; i < nsp; i++) if (sp[i].bx1 >= wx0 && sp[i].bx0 <= wx0 + vw * upp && sp[i].by1 > lb) lb = sp[i].by1;
        if (lb > -32768 && wy0 + vh * upp > lb) { wy0 = lb - vh * upp; if (wy0 < y0) wy0 = y0; }
    }
    int nx = (int)(wx0 / upp), ny = (int)(wy0 / upp);
    wx0 = (float)nx * upp; wy0 = (float)ny * upp;   /* whole output pixels: no shimmer */
    if (!td) { side_view(view, vw, vh, nx, ny, x0, y0, x1, y1, &wx0, &wy0, &upp); last_id = -1; }   /* from the tile cache */
    else if (id != last_id || vw != last_vw || vh != last_vh || wx0 != last[0] || wy0 != last[1] || upp != last[2]) {
        td_render(view, vw, vh, wx0, wy0, upp);
        last_id = id; last_vw = vw; last_vh = vh; last[0] = wx0; last[1] = wy0; last[2] = upp;
    }
    for (int j = 0; j < vh; j++) memcpy(pa_cv + (vy + j) * pa_cw + vx, view + j * vw, (size_t)vw * 4);
    int cx0 = pa_clip_x0, cy0 = pa_clip_y0, cx1 = pa_clip_x1, cy1 = pa_clip_y1;
    pa_clip_x0 = vx; pa_clip_y0 = vy; pa_clip_x1 = vx + vw; pa_clip_y1 = vy + vh;
    map_marks(vx, vy, vh, wx0, wy0, upp, 1, area, sec, td);
    pa_clip_x0 = cx0; pa_clip_y0 = cy0; pa_clip_x1 = cx1; pa_clip_y1 = cy1;
}
