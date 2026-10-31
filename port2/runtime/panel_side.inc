/* Second screen panel: side-view map PROTOTYPE (AREA00 sections 0..2 only; included by panel.c).
 * The room model (WMD, AREA00/D001.GAM from the user's disc: sectors of GPU-like packets {clut, tpage, rgb cmd, 3/4 x
 * {s16 x, y, z, u8 u, v}}, tools/room.py) rasterised orthographically (x right, y down, far polygons first) with a small
 * software rasteriser at 1/2 scale, its texels and palettes read from the game's VRAM while the area is loaded (the room
 * pages are there then).  Built once when the player is in the area; markers: the events placed there
 * (panel_eventmap.h, their icons) and Tomba at his live position (PLAYER.obj x / y, 0x800A604A / 0x800A604E).
 * Views: whole area (fit to width) and room (1:2 around Tomba). */
extern const unsigned short *port_gpu_vram(int x, int y, int w, int h);   /* shims/gpu.c */
#define SIDE_SC 2
static u32 *side_img; static int side_w, side_h, side_x0, side_y0, side_state;   /* 0 not built, 1 built, -1 failed */

static void side_tri(const s16 *X, const s16 *Y, const u8 *U, const u8 *V, const u16 *vr, int tx, int ty, int mode, const u16 *pal) {
    float px[3], py[3];
    for (int i = 0; i < 3; i++) { px[i] = (float)(X[i] - side_x0) / SIDE_SC; py[i] = (float)(Y[i] - side_y0) / SIDE_SC; }
    float den = (py[1] - py[2]) * (px[0] - px[2]) + (px[2] - px[1]) * (py[0] - py[2]);
    if (den == 0) return;
    int x0 = (int)px[0], x1 = x0, y0 = (int)py[0], y1 = y0;
    for (int i = 1; i < 3; i++) { if ((int)px[i] < x0) x0 = (int)px[i]; if ((int)px[i] > x1) x1 = (int)px[i]; if ((int)py[i] < y0) y0 = (int)py[i]; if ((int)py[i] > y1) y1 = (int)py[i]; }
    if (x0 < 0) x0 = 0; if (y0 < 0) y0 = 0; if (x1 >= side_w) x1 = side_w - 1; if (y1 >= side_h) y1 = side_h - 1;
    for (int y = y0; y <= y1; y++) for (int x = x0; x <= x1; x++) {
        float a = ((py[1] - py[2]) * (x - px[2]) + (px[2] - px[1]) * (y - py[2])) / den;
        float b = ((py[2] - py[0]) * (x - px[2]) + (px[0] - px[2]) * (y - py[2])) / den, c = 1 - a - b;
        if (a < -.01f || b < -.01f || c < -.01f) continue;
        int u = (int)(a * U[0] + b * U[1] + c * U[2]) & 255, v = (int)(a * V[0] + b * V[1] + c * V[2]) & 255;
        u16 w = mode == 0 ? pal[(vr[((ty + v) & 511) * 1024 + ((tx + (u >> 2)) & 1023)] >> ((u & 3) * 4)) & 15]
                          : pal[(vr[((ty + v) & 511) * 1024 + ((tx + (u >> 1)) & 1023)] >> ((u & 1) * 8)) & 255];
        if (!w) continue;
        side_img[y * side_w + x] = 0xFF000000u | (u32)((w >> 10 & 31) * 255 / 31) << 16 | (u32)((w >> 5 & 31) * 255 / 31) << 8 | (u32)((w & 31) * 255 / 31);
    }
}
typedef struct { const u8 *p; int z; } SidePoly;
static int side_cmp(const void *a, const void *b) { return ((const SidePoly *)b)->z - ((const SidePoly *)a)->z; }
static void side_build(void) {
    unsigned n; u8 *d = pa_read("AREA00/D001.GAM", &n);
    side_state = -1;
    if (!d || n < 8) { free(d); return; }
    unsigned ns = rd32(d, 0);
    if (ns == 0 || ns > 4096 || 4 + 4 * ns > n) { free(d); return; }
    SidePoly *polys = malloc(sizeof(SidePoly) * 16384); int np = 0, xmin = 1 << 20, xmax = -(1 << 20), ymin = 1 << 20, ymax = -(1 << 20);
    for (unsigned s = 0; s < ns; s++) {
        unsigned p = rd32(d, 4 + 4 * s), e = s + 1 < ns ? rd32(d, 8 + 4 * s) : n;
        if (e > n || p > e) continue;
        for (int g = 0; g < 4 && p + 4 <= e; g++) {
            unsigned c = rd32(d, p); p += 4;
            for (unsigned k = 0; k < c && p + 8 <= e; k++) {
                int cmd = d[p + 7], nv = cmd & 8 ? 4 : 3, ex = cmd & 0x10 ? (nv - 1) * 4 : 0;
                unsigned sz = 8 + ex + 8 * nv;
                if (p + sz > e) break;
                int z = 0;
                for (int i = 0; i < nv; i++) {
                    const u8 *q = d + p + 8 + ex + 8 * i; int x = rd16(q, 0), y = rd16(q, 2);
                    z += rd16(q, 4);
                    if (x < xmin) xmin = x; if (x > xmax) xmax = x; if (y < ymin) ymin = y; if (y > ymax) ymax = y;
                }
                if (np < 16384) { polys[np].p = d + p; polys[np].z = z; np++; }
                p += sz;
            }
        }
    }
    if (!np) { free(polys); free(d); return; }
    side_x0 = xmin; side_y0 = ymin; side_w = (xmax - xmin) / SIDE_SC + 1; side_h = (ymax - ymin) / SIDE_SC + 1;
    side_img = calloc((size_t)side_w * side_h, 4);
    qsort(polys, np, sizeof *polys, side_cmp);
    const u16 *vr = port_gpu_vram(0, 0, 1024, 512);
    for (int i = 0; i < np; i++) {
        const u8 *q = polys[i].p;
        int cl = rd16(q, 0) & 0xFFFF, tp = rd16(q, 2) & 0xFFFF, cmd = q[7], nv = cmd & 8 ? 4 : 3, ex = cmd & 0x10 ? (nv - 1) * 4 : 0;
        int mode = tp >> 7 & 3;
        if (mode > 1) continue;
        u16 pal[256]; int cx = (cl & 63) * 16, cy = cl >> 6 & 511;
        for (int k = 0; k < (mode ? 256 : 16); k++) pal[k] = vr[cy * 1024 + ((cx + k) & 1023)];
        s16 X[4], Y[4]; u8 U[4], V[4];
        for (int k = 0; k < nv; k++) { const u8 *v = q + 8 + ex + 8 * k; X[k] = rd16(v, 0); Y[k] = rd16(v, 2); U[k] = v[6]; V[k] = v[7]; }
        side_tri(X, Y, U, V, vr, (tp & 15) * 64, (tp >> 4 & 1) * 256, mode, pal);
        if (nv == 4) side_tri(X + 1, Y + 1, U + 1, V + 1, vr, (tp & 15) * 64, (tp >> 4 & 1) * 256, mode, pal);
    }
    free(polys); free(d);
    side_state = 1;
    port_printf("[panel] side map: AREA00/D001 %d polygons, %d x %d (1/%d)\n", np, side_w, side_h, SIDE_SC);
}

#define P_X G16(0x800A604Au)
#define P_Y G16(0x800A604Eu)
static void page_side(int play) {
    int W = pa_cw, H = pa_ch;
    static const char *zn[2][2] = { { "Area", "\xC1rea" }, { "Room", "Sala" } };
    for (int i = 0; i < 2; i++) {
        int tw = (W - 20) / 2, x = 8 + i * (tw + 4);
        plank(x, 56, tw); hot(x, 56, tw, 24, HS_FILTER + 16 + i);
        plank_text(x + tw / 2, 60, zn[i][es()], i == side_zoom);
    }
    int mx = 8, my = 84, mw = W - 16, mh = H - my - 8;
    window_(mx, my, mw, mh);
    int vx = mx + 7, vy = my + 7, vw = mw - 14, vh = mh - 14;
    for (int j = 0; j < vh; j++) { int t = j * 255 / (vh ? vh : 1); pa_fill(vx, vy + j, vw, 1, RGB(120 + 80 * t / 255, 170 + 58 * t / 255, 255)); }
    if (!play || G_AREA != 0 || G_SECTION > 2) {
        if (side_state != 1) { pa_text(vx + 8, vy + 8, 16, T("Prototype: Village area only", "Prototipo: solo el \xE1rea del Pueblo"), C_DARK, C_EDGE); return; }
    } else if (side_state == 0 && port_frame > 60) side_build();
    if (side_state != 1) { pa_text(vx + 8, vy + 8, 16, T("Building...", "Preparando..."), C_DARK, C_EDGE); return; }
    int cx0 = pa_clip_x0, cy0 = pa_clip_y0, cx1 = pa_clip_x1, cy1 = pa_clip_y1;
    pa_clip_x0 = vx; pa_clip_y0 = vy; pa_clip_x1 = vx + vw; pa_clip_y1 = vy + vh;
    /* view: world units per canvas pixel (fixed point /256) and the world point at the view's top left */
    int upp, wx0, wy0, px = (s16)P_X, py = (s16)P_Y;
    int here = play && G_AREA == 0 && G_SECTION <= 2;
    if (!side_zoom) { upp = (side_w * SIDE_SC * 256) / vw; wx0 = side_x0; wy0 = side_y0 + side_h * SIDE_SC - vh * upp / 256; }
    else { upp = 256 * SIDE_SC; wx0 = (here ? px : side_x0) - vw * upp / 512; wy0 = (here ? py : 0) - vh * upp / 256 * 2 / 3; }
    for (int j = 0; j < vh; j++) {
        int sy = ((wy0 + j * upp / 256) - side_y0) / SIDE_SC;
        if (sy < 0 || sy >= side_h) continue;
        for (int i = 0; i < vw; i++) {
            int sx = ((wx0 + i * upp / 256) - side_x0) / SIDE_SC;
            if (sx < 0 || sx >= side_w) continue;
            u32 c = side_img[sy * side_w + sx];
            if (c) pa_put(vx + i, vy + j, c);
        }
    }
#define WX(x) (vx + ((x) - wx0) * 256 / upp)
#define WY(y) (vy + ((y) - wy0) * 256 / upp)
    for (unsigned i = 0; i < sizeof panel_event_locs / sizeof panel_event_locs[0]; i++) {
        const PanelEventLoc *L = &panel_event_locs[i];
        if (L->area != 0 || L->sec > 2 || L->x == -32768) continue;
        int ex = WX(L->x), ey = WY(L->y), st = ev_state(L->ev);
        pa_event_icon(ex - 12, ey - 17, L->ev, PA_OUTLINE | (st == F_PENDING ? PA_DIM : 0));
    }
    if (here) {
        int tx = WX(px), ty = WY(py), bob = (port_frame / 10) & 1;
        face(tx - 12, ty - 26 - bob);
        pa_text_ol(tx - 6, ty - 36 - bob, 8, es() ? "T\xDA" : "YOU", C_YELLOW, C_WOODE, C_DARK);
    }
    pa_clip_x0 = cx0; pa_clip_y0 = cy0; pa_clip_x1 = cx1; pa_clip_y1 = cy1;
}
