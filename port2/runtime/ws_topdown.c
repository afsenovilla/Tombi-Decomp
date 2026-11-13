/* --widescreen in the top-down areas (I0NFO Dwarf Village, I0NFO2 Baccus Village / Baccus Lake and the other
 * overhead rooms; docs/PORT.md "Widescreen", "Top-down map").
 *
 * The overhead background is not a model: it is the room's whole map picture (an 8 bpp TIM kept in RAM, scratch
 * 0x1F800394) of which the game uploads, every frame, a 384 x 288 window (12 x 9 tiles of 32 px around the camera;
 * USA / JP: 384 x 256) into fixed VRAM (tpages 0x300 / 0x380 and a strip page), and draws it as five SPRTs
 * (`topdownDrawMapLayers` / `overheadDrawMapLayers`, two in the USA / JP builds).  The window is 32 px wider than the
 * 4:3 screen on each side at most, so on a wide frame the columns past it stayed black, and as the camera moved the
 * map, and the objects standing on it, appeared in 32-px pieces.
 *
 * The VRAM has no room for a wider window, so the same VRAM is used again in the same frame: the hook at the end of
 * the draw function (port/fixes.txt -> port_ws_overhead_map) links, ahead of the game's map packets in its ordering
 * table bucket, for the window one width to the left and one to the right (as far as each shows on the wide frame):
 * a marker packet (two GP0 no-ops), then a copy of the game's DR_MODE + SPRT pairs moved by the window width and
 * clipped to the wide frame and the picture.  When the GPU shim reaches a marker (shims/gpu.c DrawOTag ->
 * port_ws_overhead_mark) the picture pixels of that window are uploaded into the game's VRAM rects (LoadImage, so
 * both renderers, --fps replay and the texture tools see an ordinary upload); a last marker puts back what the game
 * uploaded before its own map packets run.  So the game's picture, tiles and palette, nothing else.
 *
 * Identity when off: nothing is linked and no marker exists without --widescreen; the gpu.c test is one compare. */
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32; typedef short s16;
typedef struct { s16 x, y, w, h; } RECT;

extern int port_widescreen, port_ws_cull;
extern int port_printf(const char *, ...);
extern int port_is_ram_ptr(const void *);
extern void *port_ram_ptr(unsigned int);
extern int LoadImage(RECT *, u32 *);
extern u16 *port_vram(void);
extern char *NEXT_PRIM;
extern char *CURRENT_OT;
extern void (*port_ot_mark_fn)(u32);

#define OT_MARK 0x00575354u               /* GP0 0x00 (no-op) with "WST" in its parameter bits: shims/gpu.c */
#define MAXR 5                            /* map SPRTs: 5 (PAL), 2 (USA / JP) */
#define SLOTS 4                           /* parameter sets in flight (one per ordering table, double-buffered) */

typedef struct {
    int x, y, w, h;                       /* VRAM rect of the game's SPRT (halfwords) */
    int ex, ey;                           /* its place in the map window (px) */
} Rect;
typedef struct {
    int used;
    const u8 *pic; int stride, pw, ph;    /* picture pixels, bytes per row, size (px) */
    int px0, py0;                         /* picture px at the window's top left */
    int ww;                               /* window width (px) */
    int n; Rect r[MAXR];
    struct { int k, ri, du, dw, j0, j1; } up[2 * MAXR * 2]; int nup;   /* uploads per batch (k = -2..2) */
    u16 save[MAXR][256 * 256 / 2 + 8 * 128]; int saved[MAXR];
} Slot;
static Slot slots[SLOTS];
static int slot_next;

static u32 rd32(const u8 *p) { return p[0] | p[1] << 8 | p[2] << 16 | (u32)p[3] << 24; }
static u16 rd16(const u8 *p) { return (u16)(p[0] | p[1] << 8); }

static void upload(Slot *s, int k)
{
    static u16 buf[256 * 288];
    for (int i = 0; i < s->nup; i++) {
        if (s->up[i].k != k) continue;
        Rect *r = &s->r[s->up[i].ri];
        int du = s->up[i].du, dw = s->up[i].dw, j0 = s->up[i].j0, j1 = s->up[i].j1;
        if (!s->saved[s->up[i].ri]) {     /* what the game uploaded there, put back by the last marker */
            const u16 *v = port_vram();
            for (int j = 0; j < r->h; j++) memcpy(&s->save[s->up[i].ri][j * r->w], v + (r->y + j) * 1024 + r->x, (size_t)r->w * 2);
            s->saved[s->up[i].ri] = 1;
        }
        int pcol = s->px0 + r->ex + k * s->ww + du;
        for (int j = j0; j < j1; j++)
            memcpy((u8 *)buf + (size_t)(j - j0) * dw, s->pic + (size_t)(s->py0 + r->ey + j) * s->stride + pcol, (size_t)dw);
        RECT rc = { (s16)(r->x + du / 2), (s16)(r->y + j0), (s16)(dw / 2), (s16)(j1 - j0) };
        LoadImage(&rc, (u32 *)buf);
    }
}

/* shims/gpu.c, at a marker packet in DrawOTag: id = slot * 8 + batch (k + 2), batch 7 = put the game's VRAM back */
static void mark(u32 id)
{
    Slot *s = &slots[(id >> 3) % SLOTS];
    int b = id & 7;
    if (!s->used) return;
    if (b < 5) { upload(s, b - 2); return; }
    for (int i = 0; i < s->n; i++) if (s->saved[i]) {
        Rect *r = &s->r[i];
        RECT rc = { (s16)r->x, (s16)r->y, (s16)r->w, (s16)r->h };
        LoadImage(&rc, (u32 *)s->save[i]);
        s->saved[i] = 0;
    }
    s->used = 0;
}

static char *prim(int size) { char *p = NEXT_PRIM; NEXT_PRIM += size; return p; }
static void link_prim(u32 *ot, char *p)          /* addPrim: keeps the packet's length byte */
{
    *(u32 *)p = (*(u32 *)p & 0xFF000000u) | (*ot & 0xFFFFFF);
    *ot = (*ot & 0xFF000000u) | ((u32)(uintptr_t)p & 0xFFFFFF);
}

/* The frame inside the picture.  The game clamps its camera so the 4:3 view stays inside the map picture, so at a
   map edge the wide frame ran up to `pad` columns past the picture (black).  While the scene draws
   (drawGameplayScene: port_ws_topdown_begin after the lighting, port_ws_topdown_end before the pause menu, fades and
   HUD), everything the scene draws moves dx columns: the objects through the GTE (gte.c port_gte_dx, added to every
   RTPS / RTPT screen X, so their own culls see the moved position), the map through its packets below.  dx puts the
   wide frame's edge on the picture's edge; a picture narrower than the wide frame (but at least 4:3 wide) is centred.
   The game's camera, logic and 4:3 picture are untouched; message boxes (2D packets) stay put, except a balloon that
   follows its speaker (below); the pause menu, fades and HUD come after port_ws_topdown_end.  The culls (port_ws_cull, PORT_WSX) keep pad + |dx| columns, so what enters
   from the far edge is there.  dx follows the camera of the frame being drawn: picture column c lies at screen x
   K - SCREEN_ORIGIN_X + c, with K and the picture width taken from the previous map draw (the room's map must have
   drawn in the previous scene draw, else dx = 0). */
extern int port_gte_dx;                           /* runtime/gte.c */
static int td_seq, td_hook_seq = -100, td_K, td_pw, td_dx, td_cull = -1;
void port_ws_topdown_begin(void)
{
    int pad = port_widescreen, dx = 0;
    if (td_cull < 0) td_cull = port_ws_cull;
    td_seq++;
    if (pad > 0 && td_cull > 0 && td_hook_seq == td_seq - 1) {
        int L0 = td_K - *(const s16 *)port_ram_ptr(0x1F800176u), pw = td_pw;   /* picture column 0, 4:3 frame px */
        if (pw >= 320 + 2 * pad) {
            if (L0 > -pad) dx = -pad - L0;
            else if (L0 + pw < 320 + pad) dx = 320 + pad - (L0 + pw);
        } else if (pw >= 320) dx = (320 - pw) / 2 - L0;
        if (dx > pad) dx = pad;
        if (dx < -pad) dx = -pad;
    }
    td_dx = dx;
    port_gte_dx = dx;
    if (td_cull > 0) port_ws_cull = td_cull + (dx < 0 ? -dx : dx);
}
void port_ws_topdown_end(void) { port_gte_dx = 0; }
/* fixes.txt drawMessageBox: a balloon that follows its speaker (openMessageBox's anim, +0x2E, nonzero: the game
   places it from the speaker's world position each frame) moves with the scene, so the tail stays on the speaker */
int port_ws_balloon_dx(const void *o) { return *(const s16 *)((const char *)o + 0x2E) ? port_gte_dx : 0; }

/* fixes.txt, at the end of the map draw (inside its `o->b6b != 0`): n = the game's SPRT count, row_off = picture row
   of tile row 0 (PAL -24: topdownBuildMapStrip's `(v / w) * 32 - 24`; USA / JP 0).  The game's packets are the last
   0xC + n * 0x20 bytes before NEXT_PRIM: DR_MODE (0xC), then n times SPRT (0x14) + DR_MODE (0xC). */
void port_ws_overhead_map(void *o, int n, int row_off)
{
    static int said;
    int pad = port_widescreen;
    if (pad <= 0 || !port_ws_cull || n < 1 || n > MAXR) return;
    char *first = NEXT_PRIM - (0xC + n * 0x20);
    int dx = td_dx;
    for (int i = 0; i < n; i++) if ((rd32((u8 *)first + 0x10 + i * 0x20) >> 24) != 0x65) return;
    for (int i = 0; i < n; i++)                     /* the frame inside the picture: the map moves with the objects */
        *(s16 *)(first + 0x14 + i * 0x20) = (s16)(*(s16 *)(first + 0x14 + i * 0x20) + dx);
    {   /* The 3DS (and PORT_WS_NO_WINDOWS=1 on a PC to see it): not the windows beside the game's.  Its wide frame
           is 40 columns a side, of which the game's window has 32 already; the two more windows were three map
           uploads into the same VRAM a frame, each a texture decode there (Baccus at 30 vblank/s, 22 ms a frame
           decoding, on an Old 3DS) */
#ifdef __3DS__
        return;
#else
        static int off = -1; if (off < 0) { const char *e = getenv("PORT_WS_NO_WINDOWS"); off = e && *e == '1'; }
        if (off) return;
#endif
    }
    port_ot_mark_fn = mark;
    const u8 *tim = *(const u8 **)port_ram_ptr(0x1F800394u), *map = *(const u8 **)port_ram_ptr(0x1F800390u);
    if (!tim || !map) return;
    if (rd32(tim) != 0x10 || (rd32(tim + 4) & 0xF) != 9 || rd32(tim + 8) != 0x20C) return;   /* 8 bpp TIM with CLUT */
    int pw = rd16(tim + 0x21C) * 2, ph = rd16(tim + 0x21E), wt = rd16(map) >> 2;
    int stride = wt * 32;
    if (pw < 64 || ph < 64 || stride < pw || stride > pw + 64 || (port_is_ram_ptr(tim) && !port_is_ram_ptr(tim + 0x220 + (size_t)stride * ph - 1))) return;
    const u8 *pic = tim + 0x220;

    const u8 *ob = (const u8 *)o;
    int d30 = *(const int *)(ob + 0x30), d34 = *(const int *)(ob + 0x34), b0f = (signed char)ob[0x0F];
    int xt = (s16)(d30 >> 5), yt = (s16)((unsigned)d34 >> 5);        /* topdownBuildVisibleTileList */
    if (xt > 0) xt--;
    if (yt > 0) yt--;

    Slot *s = &slots[slot_next];                    /* (a slot whose list was never drawn has saved nothing) */
    for (int i = 0; i < MAXR; i++) s->saved[i] = 0;
    memset(s->up, 0, sizeof s->up); s->nup = 0; s->n = n; s->used = 0;
    s->pic = pic; s->stride = stride; s->pw = pw; s->ph = ph;
    s->px0 = xt * 32; s->py0 = yt * 32 + row_off;
    const u8 *sp0 = (const u8 *)first + 0xC;
    int X = (s16)rd16(sp0 + 8), Y = (s16)rd16(sp0 + 10), ww = 0;
    for (int i = 0; i < n; i++) {
        const u8 *sp = (const u8 *)first + 0xC + i * 0x20, *dm = sp + 0x14;
        u32 e1 = rd32(dm + 4);
        if ((rd32(sp + 4) >> 24) != 0x65 || (e1 >> 24) != 0xE1 || ((e1 >> 7) & 3) != 1) return;   /* 8 bpp SPRTs */
        Rect *r = &s->r[i];
        int u = sp[12], v = sp[13], w = rd16(sp + 16), h = rd16(sp + 18);
        if ((u | w) & 1 || u + w > 256 || v + h > 256 || (w / 2) * h > (int)(sizeof s->save[0] / 2)) return;
        r->x = (e1 & 15) * 64 + u / 2; r->y = ((e1 >> 4) & 1) * 256 + v; r->w = w / 2; r->h = h;
        r->ex = (s16)rd16(sp + 8) - X; r->ey = (s16)rd16(sp + 10) - Y;
        if (r->ex < 0 || r->ey < 0 || r->y + h > 512) return;
        if (r->ex + w > ww) ww = r->ex + w;
    }
    s->ww = ww;
    /* the window the game uploaded must be the picture at (px0, py0): its first row in VRAM, where it lies inside */
    {
        const u16 *v = port_vram(); Rect *r = &s->r[0]; int ok = 1, row = s->py0;
        if (row >= 0 && row < ph && s->px0 >= 0 && s->px0 + r->w * 2 <= pw)
            ok = !memcmp(v + r->y * 1024 + r->x, pic + (size_t)row * stride + s->px0, (size_t)r->w * 2);
        if (!ok) { if (!said++) port_printf("[ws] top-down map: VRAM window is not picture (%d, %d); side strips off\n", s->px0, s->py0); return; }
    }
    td_K = X - dx - s->px0 + *(const s16 *)port_ram_ptr(0x1F800176u); td_pw = pw; td_hook_seq = td_seq;

    /* the game's own window past the picture's right / bottom edge holds the wrapped next row (or what follows the
       picture): never seen on 4:3, where the camera stops at the edge, but inside the wide strips.  Cut it there. */
    for (int i = 0; i < n; i++) {
        Rect *r = &s->r[i];
        u8 *sp = (u8 *)first + 0xC + i * 0x20;
        int over = s->px0 + r->ex + r->w * 2 - pw, pr = s->py0 + r->ey, h = rd16(sp + 18);
        if (over > 0) { int w = r->w * 2 - ((over + 1) & ~1); *(u16 *)(sp + 16) = (u16)(w > 0 ? w : 0); }
        if (pr + h > ph) *(u16 *)(sp + 18) = (u16)(ph - pr > 0 ? ph - pr : 0);
    }

    /* the batches: the window moved k widths, each SPRT clipped to the wide frame [-pad, 320 + pad) and the picture */
    char *pk[2 + 4 * (1 + 2 * MAXR)]; int np = 0, any = 0;
    for (int k = -2; k <= 2; k++) {
        if (!k) continue;
        int mark_at = -1;
        for (int i = 0; i < n; i++) {
            Rect *r = &s->r[i];
            int sx0 = X + r->ex + k * ww, a = sx0, b = sx0 + r->w * 2;
            int pc = s->px0 + r->ex + k * ww;                       /* picture column of sx0 */
            if (a < -pad) a = -pad;
            if (b > 320 + pad) b = 320 + pad;
            if (a < sx0 - pc) a = sx0 - pc;                          /* picture column 0 */
            if (b > sx0 - pc + pw) b = sx0 - pc + pw;
            a = sx0 + ((a - sx0) & ~1); b = sx0 + ((b - sx0 + 1) & ~1);
            if (b - a < 2) continue;
            int j0 = 0, j1 = r->h, pr = s->py0 + r->ey;
            if (pr + j0 < 0) j0 = -pr;
            if (pr + j1 > ph) j1 = ph - pr;
            if (j1 <= j0) continue;
            if (mark_at < 0) {
                char *m = prim(12);
                ((u32 *)m)[0] = 2u << 24; ((u32 *)m)[1] = OT_MARK; ((u32 *)m)[2] = (u32)(slot_next * 8 + k + 2);
                pk[np++] = m; mark_at = np;
            }
            int q = s->nup++;
            s->up[q].k = k; s->up[q].ri = i; s->up[q].du = a - sx0; s->up[q].dw = b - a; s->up[q].j0 = j0; s->up[q].j1 = j1;
            const char *gsp = first + 0xC + i * 0x20;
            char *dm = prim(12), *sp = prim(0x14);
            memcpy(dm, gsp + 0x14, 12); memcpy(sp, gsp, 0x14);
            *(s16 *)(sp + 8) = (s16)a; *(s16 *)(sp + 10) = (s16)(Y + r->ey + j0);
            sp[12] = (u8)(sp[12] + (a - sx0)); sp[13] = (u8)(sp[13] + j0);
            *(u16 *)(sp + 16) = (u16)(b - a); *(u16 *)(sp + 18) = (u16)(j1 - j0);
            pk[np++] = dm; pk[np++] = sp; any = 1;
        }
    }
    if (!any) return;
    char *m = prim(12);
    ((u32 *)m)[0] = 2u << 24; ((u32 *)m)[1] = OT_MARK; ((u32 *)m)[2] = (u32)(slot_next * 8 + 7);
    pk[np++] = m;
    s->used = 1;
    slot_next = (slot_next + 1) % SLOTS;
    /* link so that pk[0] runs first and the last marker runs just before the game's own map packets */
    u32 *ot = (u32 *)(CURRENT_OT + 0xC90) + b0f;
    for (int i = np - 1; i >= 0; i--) link_prim(ot, pk[i]);
}
