/* Controller-specific button prompts (docs/PORT.md "Button prompts"): the PS1 face-button glyphs the game draws in its
 * message boxes (tutorial signs, hints, the save prompt "Yes ✕ No ○", ...) replaced by glyphs of the controller in use.
 *
 * The message boxes draw every character through loadMessageGlyph (src/loadMessageGlyph.c, PORT2 block), which copies
 * a 4 bpp bitmap out of the area's WFM3 font bank into the glyph cache.  The four button glyphs are the same 12 x 16
 * bitmaps in all 369 WFM3 banks of the disc (every language), so they are recognised by the FNV-1a hash of their
 * pixels and swapped for one drawn here, by code, in the same size and with the same palette indices (the font CLUT:
 * 0 transparent, 6 the dark disc, 11..15 the ring's anti-aliased rim, 7 / 8 / 9 / 10 the PlayStation blue / green /
 * pink / red).  No game pixels are shipped: only the four hashes.
 *
 * What is drawn is the PHYSICAL button that produces that PS1 button with the current mapping:
 *   port_prompt_src[ps] (PROMPT_SRC_*: south / east / west / north face button, shoulders, triggers) is set by the host
 *   (runtime/sdl.c from its controller bindings on the PC; --prompt-map from TombiActivity's PadMap on Android), and
 *   port_prompt_style (PROMPT_*) picks the look: PlayStation shapes, Xbox coloured letters, Nintendo letters (cream on the dark disc, like
 *   the Switch Pro), Thor (the same letters, white on the SNES-coloured discs of the AYN Thor / Retroid), or a
 *   generic diamond that marks the button's position.  The letters are the controller's own button names as SDL /
 *   Android report them (SDL's A = the button labelled A on Xbox and, with SDL's default label mapping, on Nintendo
 *   pads; Android's KEYCODE_BUTTON_A = the button the AYN / Retroid handhelds label A).
 * Every style draws its own glyph (PlayStation too, so all sit at the same height): the disc on rows 3..14 of the 16-row
 * glyph, 1 px lower than the game's (rows 2..13), which centred it on the capitals; lowered, it sits on
 * the text line around it.
 *
 * Colours: the PlayStation and generic glyphs use the font CLUT.  The Xbox and Thor ones need colours it lacks
 * (yellow, deep blue, dark green, white), so the cache slots holding them are drawn with a port-only 16-colour CLUT
 * (port_prompt_clut below): the message box sprites of those slots get the CLUT id PORT_CLUT_ID (drawMessageBox.c,
 * drawMessageGlyphs.c PORT2 blocks), which the port GPU (gpu.c texel fetch, gpu_gl.c fragment shader) reads from this
 * table instead of VRAM.  No game VRAM is used: 0xFFFF is not a value GetClut can return (bit 15 set). */
#include <string.h>

typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32;

enum { PROMPT_AUTO = -1, PROMPT_PS = 0, PROMPT_XBOX = 1, PROMPT_NINTENDO = 2, PROMPT_GENERIC = 3, PROMPT_THOR = 4 };
enum { PS_CROSS, PS_CIRCLE, PS_SQUARE, PS_TRIANGLE };
enum { SRC_SOUTH, SRC_EAST, SRC_WEST, SRC_NORTH, SRC_L1, SRC_R1, SRC_L2, SRC_R2, SRC_NONE };

int port_prompt_option = PROMPT_AUTO;   /* --prompts auto|playstation|xbox|nintendo|generic|thor */
int port_prompt_style = PROMPT_PS;      /* the style in use (auto: runtime/sdl.c sets it from the controller type) */
/* the physical source of each PS1 face button; the default mapping (SDL / Android A = Cross, B = Circle, ...) */
int port_prompt_src[4] = { SRC_SOUTH, SRC_EAST, SRC_WEST, SRC_NORTH };
int port_prompt_map_fixed;              /* --prompt-map given: the host does not recompute port_prompt_src */

static const u32 ps_hash[4] = { 0xa50e3103u, 0x953416a3u, 0xbaeb4027u, 0x0557bd80u };   /* ✕ ○ □ △ (FNV-1a of 96 bytes) */

int port_prompt_parse_style(const char *s) {
    if (!strcmp(s, "auto")) return PROMPT_AUTO;
    if (!strcmp(s, "playstation") || !strcmp(s, "ps")) return PROMPT_PS;
    if (!strcmp(s, "xbox")) return PROMPT_XBOX;
    if (!strcmp(s, "nintendo") || !strcmp(s, "switch")) return PROMPT_NINTENDO;
    if (!strcmp(s, "generic")) return PROMPT_GENERIC;
    if (!strcmp(s, "thor") || !strcmp(s, "retroid")) return PROMPT_THOR;
    return -2;
}
static int src_parse(const char *s, int n) {
    static const char *nm[] = { "a", "b", "x", "y", "l1", "r1", "l2", "r2" };
    for (int i = 0; i < 8; i++) if ((int)strlen(nm[i]) == n && !strncmp(s, nm[i], n)) return i;
    return -1;
}
/* --prompt-map cross=a,circle=b,square=x,triangle=y (sources: a b x y l1 r1 l2 r2 none) */
void port_prompt_map(const char *s) {
    static const char *ps[] = { "cross", "circle", "square", "triangle" };
    while (*s) {
        const char *eq = strchr(s, '='), *end = strchr(s, ',');
        if (!end) end = s + strlen(s);
        if (eq && eq < end)
            for (int i = 0; i < 4; i++)
                if ((int)strlen(ps[i]) == eq - s && !strncmp(s, ps[i], eq - s)) {
                    int v = src_parse(eq + 1, (int)(end - eq - 1));
                    port_prompt_src[i] = v < 0 ? SRC_NONE : v;
                }
        s = *end ? end + 1 : end;
    }
    port_prompt_map_fixed = 1;
}

/* ---- drawing: a 12 x 16 index image ---- */
#define GW 12
#define GH 16
static u8 img[GH][GW];
/* palette indices of the font CLUT (measured: every index drawn in a box): 0 transparent, 1 the black ink, 2..5 its
   grey anti-aliasing (156 / 74 / 107 / 132), 6 dark grey (41), 7 blue (107,148,255), 8 green (0,255,0), 9 pink
   (255,132,132), 10 red (255,90,82), 11..13 the rim's khaki tones (206,198,165 / 165,156,140 / 140,132,115), 14 and 15
   the box's cream (255,239,222).  There is no yellow, dark blue / green or pure white: the Xbox and Thor glyphs use
   the port CLUT below. */
enum { C_T = 0, C_GREY = 3, C_DISC = 6, C_BLUE = 7, C_GREEN = 8, C_PINK = 9, C_RED = 10, C_KHAKI = 11, C_CREAM = 14 };
/* the port CLUT (Xbox / Thor glyphs): the font CLUT's entries 0, 3, 6, 11..14 kept (transparent, grey, dark disc,
   rim, cream), the others replaced: 1 / 2 / 4 / 5 the Xbox green / red / blue / yellow, 7 / 8 / 9 / 10 the SNES-coloured
   blue / green / yellow / red of the Nintendo-layout pads (AYN Thor, Retroid), 15 white.  BGR555. */
enum { P_XGREEN = 1, P_XRED = 2, P_XBLUE = 4, P_XYELLOW = 5, P_NBLUE = 7, P_NGREEN = 8, P_NYELLOW = 9, P_NRED = 10, P_WHITE = 15 };
#define BGR(r, g, b) (u16)(((r) >> 3) | ((g) >> 3) << 5 | ((b) >> 3) << 10)
const u16 port_prompt_clut[16] = {
    0, BGR(0x3C, 0xB0, 0x43), BGR(0xE0, 0x3C, 0x31), BGR(107, 107, 107), BGR(0x2F, 0x6F, 0xD6), BGR(0xF5, 0xC5, 0x18),
    BGR(41, 41, 41), BGR(0x1E, 0x3F, 0xB0), BGR(0x2E, 0x8B, 0x4E), BGR(0xF2, 0xC2, 0x30), BGR(0xC8, 0x28, 0x1E),
    BGR(206, 198, 165), BGR(165, 156, 140), BGR(140, 132, 115), BGR(255, 239, 222), BGR(255, 255, 255),
};
#define PORT_CLUT_ID 0xFFFF
#define NSLOTS 0x3c
static unsigned char slot_port_clut[NSLOTS];   /* glyph cache slot -> drawn with port_prompt_clut */
/* drawMessageBox / drawMessageGlyphs PORT2 hook: the CLUT id for a sprite of glyph cache slot `slot` */
unsigned short port_prompt_sprite_clut(int slot, unsigned short clut) {
    return slot >= 0 && slot < NSLOTS && slot_port_clut[slot] ? PORT_CLUT_ID : clut;
}

/* the disc the PlayStation glyphs sit on (rows 2..13): an anti-aliased ring, the same shape as the game's */
static const char *disc_rows[12] = {
    "...BCDDCB...", ".EC666666CE.", ".C66666666C.", "B6666666666B", "C6666666666C", "D6666666666D",
    "D6666666666D", "C6666666666C", "B6666666666B", ".C66666666C.", ".EC666666CE.", "...BCDDCB...",
};
static int hexv(char c) { return c >= '0' && c <= '9' ? c - '0' : c - 'A' + 10; }
static void draw_disc_col(int fill) {
    memset(img, 0, sizeof img);
    for (int y = 0; y < 12; y++) for (int x = 0; x < GW; x++) {
        char c = disc_rows[y][x];
        img[y + 2][x] = c == '.' ? C_T : c == '6' ? (u8)fill : (u8)hexv(c);
    }
}
static void draw_disc(void) { draw_disc_col(C_DISC); }
static void put(int x, int y, int c) { if (x >= 0 && x < GW && y >= 0 && y < GH) img[y][x] = (u8)c; }
/* 6 x 8 letters: even size, so they sit exactly in the middle of the 12 x 12 disc (x 3..8, rows 4..11) */
static const struct { char ch; u8 rows[8]; } font6[] = {
    { 'A', { 0x0C, 0x12, 0x21, 0x21, 0x3F, 0x21, 0x21, 0x21 } },
    { 'B', { 0x3E, 0x21, 0x21, 0x3E, 0x21, 0x21, 0x21, 0x3E } },
    { 'X', { 0x21, 0x33, 0x1E, 0x0C, 0x0C, 0x1E, 0x33, 0x21 } },
    { 'Y', { 0x21, 0x33, 0x1E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C } },
    { 'L', { 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x3F } },
    { 'R', { 0x3E, 0x21, 0x21, 0x3E, 0x28, 0x24, 0x22, 0x21 } },
};
/* 3 x 5 letters for two-character labels (LB, ZR, L1...) */
static const struct { char ch; u8 rows[5]; } font3[] = {
    { 'L', { 4, 4, 4, 4, 7 } }, { 'R', { 6, 5, 6, 5, 5 } }, { 'B', { 6, 5, 6, 5, 6 } }, { 'T', { 7, 2, 2, 2, 2 } },
    { 'Z', { 7, 1, 2, 4, 7 } }, { '1', { 2, 6, 2, 2, 7 } }, { '2', { 6, 1, 2, 4, 7 } },
};
static void letter6(char ch, int x0, int y0, int c) {
    for (unsigned i = 0; i < sizeof font6 / sizeof font6[0]; i++) if (font6[i].ch == ch)
        for (int y = 0; y < 8; y++) for (int x = 0; x < 6; x++) if (font6[i].rows[y] >> (5 - x) & 1) put(x0 + x, y0 + y, c);
}
static void letter3(char ch, int x0, int y0, int c) {
    for (unsigned i = 0; i < sizeof font3 / sizeof font3[0]; i++) if (font3[i].ch == ch)
        for (int y = 0; y < 5; y++) for (int x = 0; x < 3; x++) if (font3[i].rows[y] >> (2 - x) & 1) put(x0 + x, y0 + y, c);
}
static void label(const char *s, int c) {
    if (!s[1]) letter6(s[0], 3, 4, c);          /* one letter: 6 x 8, centred on the disc */
    else { letter3(s[0], 2, 6, c); letter3(s[1], 7, 6, c); }
}
/* the PlayStation symbols, in the game's colours */
static void ps_symbol(int which) {
    int x, y;
    switch (which) {
    case PS_CROSS:  for (int i = 0; i < 6; i++) { put(3 + i, 5 + i, C_BLUE); put(8 - i, 5 + i, C_BLUE); } break;
    case PS_CIRCLE:
        {   /* fallback only (the game's own circle is used once met): a clean 8 x 8 ring */
            static const u8 ring[8] = { 0x3C, 0x42, 0x81, 0x81, 0x81, 0x81, 0x42, 0x3C };
            for (y = 0; y < 8; y++) for (x = 0; x < 8; x++) if (ring[y] >> (7 - x) & 1) put(2 + x, 4 + y, C_RED);
        }
        break;
    case PS_SQUARE: for (int i = 0; i < 6; i++) { put(3 + i, 5, C_PINK); put(3 + i, 10, C_PINK); put(3, 5 + i, C_PINK); put(8, 5 + i, C_PINK); } break;
    case PS_TRIANGLE:
        for (y = 0; y <= 6; y++) { int l = 5 - y / 2, r = 6 + y / 2; put(l, 4 + y, C_GREEN); put(r, 4 + y, C_GREEN); }
        for (x = 2; x <= 9; x++) put(x, 10, C_GREEN);
        break;
    }
}
/* generic: four dots in a diamond, the button's position lit */
static void diamond(int pos) {
    static const int px[4] = { 5, 8, 2, 5 }, py[4] = { 9, 6, 6, 3 };   /* south, east, west, north (2 x 2 dots) */
    for (int i = 0; i < 4; i++) {
        int c = i == pos ? C_CREAM : C_GREY;
        put(px[i], py[i], c); put(px[i] + 1, py[i], c); put(px[i], py[i] + 1, c); put(px[i] + 1, py[i] + 1, c);
    }
}

static void draw(int ps, int src, int style) {
    static const char *ps_sh[] = { "L1", "R1", "L2", "R2" }, *xb_sh[] = { "LB", "RB", "LT", "RT" },
                      *nt_sh[] = { "L", "R", "ZL", "ZR" };
    static const char *letters = "ABXY";
    static const int xb_col[4] = { P_XGREEN, P_XRED, P_XBLUE, P_XYELLOW };   /* port CLUT */
    draw_disc();
    if (src == SRC_NONE) return;
    if (src >= SRC_L1) {
        const char *t = style == PROMPT_XBOX ? xb_sh[src - SRC_L1] : style == PROMPT_NINTENDO || style == PROMPT_THOR ? nt_sh[src - SRC_L1] : ps_sh[src - SRC_L1];
        label(t, C_CREAM);
        return;
    }
    switch (style) {
    case PROMPT_PS: ps_symbol(src); break;   /* SRC_SOUTH..NORTH are in PS_CROSS..TRIANGLE order */
    case PROMPT_XBOX: {
        char t[2] = { letters[src], 0 };
        label(t, xb_col[src]);
        break; }
    case PROMPT_NINTENDO: case PROMPT_THOR: {
        /* by position, as printed on Nintendo pads and the AYN Thor / Retroid: south B, east A, west Y, north X.
           Nintendo: cream letters on the dark disc (Switch Pro); Thor: white letters on the SNES colours (port CLUT:
           B yellow, A red, Y green, X blue).  src is the positional (Xbox-named) button: SDL with button labels off,
           Android's KEYCODE_BUTTON_A = south. */
        static const char nl[4] = { 'B', 'A', 'Y', 'X' };
        static const int ndisc[4] = { P_NYELLOW, P_NRED, P_NGREEN, P_NBLUE };
        char t[2] = { nl[src], 0 };
        if (style == PROMPT_THOR) { draw_disc_col(ndisc[src]); label(t, P_WHITE); }
        else label(t, C_CREAM);
        break; }
    default: diamond(src); break;
    }
    (void)ps;
}
/* 1 px lower: the disc from rows 2..13 to 3..14 (row 15 is empty before) */
static void lower1(void) {
    memmove(img[1], img[0], sizeof img[0] * (GH - 1));
    memset(img, 0, sizeof img[0]);
}

static u16 out[GW / 4 * GH];
static u8 orig[4][GH][GW], have_orig[4];   /* the game's PS glyphs, as met */
static void pack(void) {
    for (int y = 0; y < GH; y++) for (int w = 0; w < GW / 4; w++) {
        u16 v = 0;
        for (int k = 0; k < 4; k++) v |= (u16)(img[y][w * 4 + k] & 15) << (4 * k);
        out[y * (GW / 4) + w] = v;
    }
}

/* loadMessageGlyph's PORT2 hook: src = the glyph's pixels (after its 8-byte header), w in VRAM words, h rows; returns
   the pixels to load (ours for a button glyph, src otherwise); slot = the glyph cache slot it is loaded into */
const u16 *port_prompt_glyph(const u16 *src, int w, int h, int slot) {
    if (slot >= 0 && slot < NSLOTS) slot_port_clut[slot] = 0;
    if (w != GW / 4 || h != GH) return src;
    u32 hsh = 2166136261u;
    const u8 *b = (const u8 *)src;
    for (int i = 0; i < GW * GH / 2; i++) hsh = (hsh ^ b[i]) * 16777619u;
    for (int ps = 0; ps < 4; ps++) if (hsh == ps_hash[ps]) {
        /* keep the game's own glyph: PlayStation style shows the original art (only lowered), read here at run time */
        for (int y = 0; y < GH; y++) for (int x = 0; x < GW; x++) orig[ps][y][x] = src[y * (GW / 4) + x / 4] >> (4 * (x & 3)) & 15;
        have_orig[ps] = 1;
        int s2 = port_prompt_src[ps];
        if (port_prompt_style == PROMPT_PS && s2 < 4 && have_orig[s2]) memcpy(img, orig[s2], sizeof img);
        else draw(ps, s2, port_prompt_style);
        lower1();
        pack();
        if (slot >= 0 && slot < NSLOTS)
            slot_port_clut[slot] = port_prompt_style == PROMPT_XBOX || port_prompt_style == PROMPT_THOR;
        return out;
    }
    return src;
}

/* ---- the button icons of the menus (title, options, button config, save / load screens) ----
 * Outside the message boxes the game draws its button icons as sprites out of texture pages it uploads with LoadImage:
 * the title's "PRESS START or ✕ BUTTON" (page 384,0: a 16 x 16 ✕), the options screen's "✕ Button:CHANGE OPTION /
 * ○ Button:EXIT" and the button config pad (page 512,0: ○ □ △ ✕ in 16 x 16 at v 144, and in 12 x 16 next to them), the
 * save / load screens' "✕ Button:Select / ○ Button:Cancel" (page 256,0: ○ □ △ ✕ in 12 x 12 at v 232).  Each is a
 * fixed rect of its page, recognised by the FNV-1a of its texels when an upload covers it (gpu.c calls
 * port_prompt_vram_upload after every LoadImage / GP0 upload, with the host VRAM fresh), and overwritten in VRAM by the
 * glyph of the physical button, drawn here in the same rect (the disc in the original's bounding box).  The originals are
 * kept: PlayStation style copies the original icon of the source button (same page, same CLUT), so with the default
 * mapping nothing changes.  The other styles are drawn in the port CLUT: gpu.c gives any 4 bpp primitive whose texels
 * lie inside a patched rect the CLUT id PORT_CLUT_ID (port_prompt_tex_clut).  A change of style or mapping (a
 * controller plugged in) re-draws the patched rects at the next draw.  No game pixels are shipped: only the hashes. */
typedef struct { short x, y, w, h; u8 ps, grp; u32 hash; } MenuIcon;   /* VRAM words; ps = PS_*; grp: icons of one row */
static const MenuIcon menu_icons[] = {
    { 384, 128, 4, 16, PS_CROSS, 0, 0x9e815eb5 },   /* title (page 384,0 u0 v128) */
    { 512, 144, 4, 16, PS_CIRCLE, 1, 0x3195cb41 }, { 516, 144, 4, 16, PS_SQUARE, 1, 0xcc36ee81 },   /* page 512,0: 16 x 16 */
    { 520, 144, 4, 16, PS_TRIANGLE, 1, 0x055e57e0 }, { 524, 144, 4, 16, PS_CROSS, 1, 0x721212d1 },
    { 528, 144, 3, 16, PS_CIRCLE, 2, 0x0f32e06d }, { 531, 144, 3, 16, PS_SQUARE, 2, 0xe17028a1 },   /* page 512,0: 12 x 16 */
    { 534, 144, 3, 16, PS_TRIANGLE, 2, 0x695e1096 }, { 537, 144, 3, 16, PS_CROSS, 2, 0xa8c08971 },
    { 270, 232, 3, 12, PS_CIRCLE, 3, 0x03fef59d }, { 273, 232, 3, 12, PS_SQUARE, 3, 0xf4f0d4c5 },   /* page 256,0: 12 x 12 */
    { 276, 232, 3, 12, PS_TRIANGLE, 3, 0xa28f628e }, { 279, 232, 3, 12, PS_CROSS, 3, 0xf26f9aa1 },
};
#define NMI (int)(sizeof menu_icons / sizeof menu_icons[0])
static u16 mi_orig[NMI][16 * 4];
static u8 mi_have[NMI];        /* the original was met (mi_orig holds it) */
static u8 mi_on[NMI];          /* the rect holds our glyph now */
static u8 mi_pclut[NMI];       /* ... drawn with the port CLUT */
static int mi_gen[NMI];
int port_prompt_icons_on;      /* any mi_on: gpu.c's fast path */
static void (*mi_write)(int, int, int, int, const unsigned short *);
static int prompt_gen(void) { return port_prompt_style * 4096 + port_prompt_src[0] * 512 + port_prompt_src[1] * 64 + port_prompt_src[2] * 8 + port_prompt_src[3]; }

#define MW 16
static u8 mimg[16][MW];
static void mput(int x, int y, int c, int w, int h) { if (x >= 0 && x < w && y >= 0 && y < h) mimg[y][x] = (u8)c; }
/* the glyph of PS button `ps` for icon i (w x h texels), into mimg; returns 1 for the port CLUT */
static int mi_draw(int i, int w, int h) {
    int ps = menu_icons[i].ps, src = port_prompt_src[ps], st = port_prompt_style;
    int x0 = w, y0 = h, x1 = -1, y1 = -1;
    for (int y = 0; y < h; y++) for (int x = 0; x < w; x++)
        if (mi_orig[i][y * menu_icons[i].w + x / 4] >> (4 * (x & 3)) & 15) {
            if (x < x0) x0 = x; if (x > x1) x1 = x; if (y < y0) y0 = y; if (y > y1) y1 = y;
        }
    if (x1 < 0) { x0 = 0; y0 = 0; x1 = w - 1; y1 = h - 1; }
    if (st == PROMPT_PS && src < 4) {      /* the game's own icon of that button, from the same row (same size, CLUT) */
        for (int j = 0; j < NMI; j++)
            if (menu_icons[j].grp == menu_icons[i].grp && menu_icons[j].ps == src && mi_have[j]) {
                for (int y = 0; y < h; y++) for (int x = 0; x < w; x++)
                    mimg[y][x] = mi_orig[j][y * menu_icons[j].w + x / 4] >> (4 * (x & 3)) & 15;
                return 0;
            }
    }
    /* the disc in the original's bounding box (1 px lower when there is room, like the message glyphs) */
    if (y1 < h - 1 && h > 12) { y0++; y1++; }
    memset(mimg, 0, sizeof mimg);
    int d = x1 - x0 + 1 < y1 - y0 + 1 ? x1 - x0 + 1 : y1 - y0 + 1;
    int cx2 = x0 + x1 + 1, cy2 = y0 + y1 + 1;            /* centre * 2 */
    int fill = C_DISC;
    static const int ndisc[4] = { P_NYELLOW, P_NRED, P_NGREEN, P_NBLUE };
    if (st == PROMPT_THOR && src < 4) fill = ndisc[src];
    for (int y = 0; y < h; y++) for (int x = 0; x < w; x++) {
        int dx = 2 * x + 1 - cx2, dy = 2 * y + 1 - cy2, r2 = dx * dx + dy * dy;   /* in half pixels */
        if (r2 <= d * d - 2 * d) mimg[y][x] = (u8)fill;
        else if (r2 <= d * d + d / 2) mimg[y][x] = 12;                           /* the khaki rim */
    }
    int lx = cx2 / 2 - 3, ly = cy2 / 2 - 4;                /* a 6 x 8 letter centred */
    const char *t = 0; int col = C_CREAM; char one[2] = { 0, 0 };
    static const int xb_col[4] = { P_XGREEN, P_XRED, P_XBLUE, P_XYELLOW };
    static const char *xb_sh[] = { "LB", "RB", "LT", "RT" }, *nt_sh[] = { "L", "R", "ZL", "ZR" }, *ps_sh[] = { "L1", "R1", "L2", "R2" };
    if (src == SRC_NONE) return 1;
    if (src >= SRC_L1) t = st == PROMPT_XBOX ? xb_sh[src - SRC_L1] : st == PROMPT_NINTENDO || st == PROMPT_THOR ? nt_sh[src - SRC_L1] : ps_sh[src - SRC_L1];
    else if (st == PROMPT_XBOX) { one[0] = "ABXY"[src]; t = one; col = xb_col[src]; }
    else if (st == PROMPT_NINTENDO || st == PROMPT_THOR) { one[0] = "BAYX"[src]; t = one; col = st == PROMPT_THOR ? P_WHITE : C_CREAM; }
    if (t) {
        for (unsigned k = 0; k < sizeof font6 / sizeof font6[0] && !t[1]; k++) if (font6[k].ch == t[0])
            for (int y = 0; y < 8; y++) for (int x = 0; x < 6; x++) if (font6[k].rows[y] >> (5 - x) & 1) mput(lx + x, ly + y, col, w, h);
        if (t[1]) for (int c = 0; c < 2; c++) for (unsigned k = 0; k < sizeof font3 / sizeof font3[0]; k++) if (font3[k].ch == t[c])
            for (int y = 0; y < 5; y++) for (int x = 0; x < 3; x++) if (font3[k].rows[y] >> (2 - x) & 1) mput(lx + c * 4 + x, ly + 2 + y, C_CREAM, w, h);
        return 1;
    }
    if (st == PROMPT_PS) {   /* PS shapes where the source icon is not on the page (port CLUT: the nearest colours) */
        int s = src, c3 = cx2 / 2, c4 = cy2 / 2;
        if (s == PS_CROSS) for (int k = -3; k < 3; k++) { mput(c3 + k, c4 + k, P_XBLUE, w, h); mput(c3 - 1 - k, c4 + k, P_XBLUE, w, h); }
        else if (s == PS_CIRCLE) for (int y = -3; y < 3; y++) for (int x = -3; x < 3; x++) { int r = (2*x+1)*(2*x+1)+(2*y+1)*(2*y+1); if (r >= 14 && r <= 36) mput(c3 + x, c4 + y, P_XRED, w, h); }
        else if (s == PS_SQUARE) for (int k = -3; k < 3; k++) { mput(c3 + k, c4 - 3, P_NRED, w, h); mput(c3 + k, c4 + 2, P_NRED, w, h); mput(c3 - 3, c4 + k, P_NRED, w, h); mput(c3 + 2, c4 + k, P_NRED, w, h); }
        else { for (int y = 0; y < 6; y++) { mput(c3 - 1 - y / 2, c4 - 3 + y, P_XGREEN, w, h); mput(c3 + y / 2, c4 - 3 + y, P_XGREEN, w, h); } for (int x = -4; x < 4; x++) mput(c3 + x, c4 + 2, P_XGREEN, w, h); }
        return 1;
    }
    {   /* generic: the diamond, the button's position lit */
        static const int px[4] = { 0, 3, -3, 0 }, py[4] = { 3, 0, 0, -3 };
        for (int k = 0; k < 4; k++) { int c = k == src ? C_CREAM : C_GREY;
            mput(cx2 / 2 - 1 + px[k], cy2 / 2 - 1 + py[k], c, w, h); mput(cx2 / 2 + px[k], cy2 / 2 - 1 + py[k], c, w, h);
            mput(cx2 / 2 - 1 + px[k], cy2 / 2 + py[k], c, w, h); mput(cx2 / 2 + px[k], cy2 / 2 + py[k], c, w, h); }
    }
    return 1;
}
static void mi_apply(int i) {
    const MenuIcon *m = &menu_icons[i];
    static u16 o[16 * 4];
    int w = m->w * 4, h = m->h;
    mi_gen[i] = prompt_gen();
    int same = port_prompt_style == PROMPT_PS && port_prompt_src[m->ps] == m->ps;
    if (same) {                      /* the original: put it back if ours is there */
        if (mi_on[i]) { mi_write(m->x, m->y, m->w, m->h, mi_orig[i]); mi_on[i] = 0; }
        mi_pclut[i] = 0;
    } else {
        memset(mimg, 0, sizeof mimg);
        mi_pclut[i] = (u8)mi_draw(i, w, h);
        for (int y = 0; y < h; y++) for (int q = 0; q < m->w; q++) {
            u16 v = 0;
            for (int k = 0; k < 4; k++) v |= (u16)(mimg[y][q * 4 + k] & 15) << (4 * k);
            o[y * m->w + q] = v;
        }
        mi_on[i] = 1;
        mi_write(m->x, m->y, m->w, m->h, o);
    }
    port_prompt_icons_on = 0;
    for (int j = 0; j < NMI; j++) port_prompt_icons_on |= mi_on[j] | mi_have[j];
}
static u32 mi_hash(const u16 *vram, int i) {
    const MenuIcon *m = &menu_icons[i];
    u32 h = 2166136261u;
    for (int y = 0; y < m->h; y++) for (int q = 0; q < m->w; q++) {
        u16 v = vram[(m->y + y) * 1024 + m->x + q];
        h = (h ^ (v & 255)) * 16777619u; h = (h ^ (v >> 8)) * 16777619u;
    }
    return h;
}
/* gpu.c: VRAM (x, y, w, h) was just written by the game; vram = the host copy (1024 x 512), fresh in that rect */
void port_prompt_vram_upload(const unsigned short *vram, int x, int y, int w, int h,
                             void (*write)(int, int, int, int, const unsigned short *)) {
    static int logged, log = -1;
    int any = 0;
    if (log < 0) { extern char *getenv(const char *); log = getenv("PORT_PROMPT_LOG") != 0; }
    mi_write = write;
    for (int i = 0; i < NMI; i++) {
        const MenuIcon *m = &menu_icons[i];
        if (x >= m->x + m->w || x + w <= m->x || y >= m->y + m->h || y + h <= m->y) continue;
        mi_on[i] = 0; mi_have[i] = 0;
        int part = m->x < x || m->x + m->w > x + w || m->y < y || m->y + m->h > y + h;   /* not all of it */
        u32 hs = part ? 0 : mi_hash(vram, i);
        if (log && logged < 400) {
            extern void port_printf(const char *, ...); extern int port_frame; logged++;
            port_printf("[prompts] frame %d: upload %d,%d %dx%d: icon %d at %d,%d: hash 0x%08x\n", port_frame, x, y, w, h, i, m->x, m->y, hs);
        }
        if (part) continue;
        if (m->hash && hs != m->hash) continue;
        for (int yy = 0; yy < m->h; yy++) for (int q = 0; q < m->w; q++) mi_orig[i][yy * m->w + q] = vram[(m->y + yy) * 1024 + m->x + q];
        mi_have[i] = 1; any = 1;
    }
    if (any) for (int i = 0; i < NMI; i++) if (mi_have[i] && !mi_on[i]) mi_apply(i);
    port_prompt_icons_on = 0;
    for (int j = 0; j < NMI; j++) port_prompt_icons_on |= mi_on[j] | mi_have[j];
}
/* gpu.c, for a 4 bpp primitive of page (tx, ty) with texels u0..u1, v0..v1: 1 = give it the port CLUT */
int port_prompt_tex_clut(int tx, int ty, int u0, int v0, int u1, int v1) {
    int g = prompt_gen();
    for (int i = 0; i < NMI; i++) {
        if (!mi_have[i]) continue;
        const MenuIcon *m = &menu_icons[i];
        int ix = (m->x - tx) * 4, iy = m->y - ty;
        if (u0 < ix || u1 >= ix + m->w * 4 || v0 < iy || v1 >= iy + m->h) continue;
        if (mi_gen[i] != g && mi_write) mi_apply(i);
        return mi_on[i] && mi_pclut[i];
    }
    return 0;
}
