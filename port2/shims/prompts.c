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
#include <stdlib.h>

typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32;

enum { PROMPT_AUTO = -1, PROMPT_PS = 0, PROMPT_XBOX = 1, PROMPT_NINTENDO = 2, PROMPT_GENERIC = 3, PROMPT_THOR = 4, PROMPT_THOR_BLACK = 5, PROMPT_THOR_WHITE = 6, PROMPT_CUSTOM = 7 };
/* Thor positions and labels: the AYN Thor's three button variants: SNES-coloured ("rainbow"), black, white */
#define THORISH(st) ((st) == PROMPT_THOR || (st) == PROMPT_THOR_BLACK || (st) == PROMPT_THOR_WHITE)
#define THOR_ROW_OF(st, src) ((st) == PROMPT_THOR_BLACK ? ROW_THOR_BLACK : (st) == PROMPT_THOR_WHITE ? ROW_THOR_WHITE : ROW_THOR + (src))
enum { PS_CROSS, PS_CIRCLE, PS_SQUARE, PS_TRIANGLE };
enum { SRC_SOUTH, SRC_EAST, SRC_WEST, SRC_NORTH, SRC_L1, SRC_R1, SRC_L2, SRC_R2, SRC_NONE };

int port_prompt_option = PROMPT_AUTO;   /* --prompts auto|playstation|xbox|nintendo|generic|thor|thor-black|thor-white|custom */
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
    if (!strcmp(s, "thor") || !strcmp(s, "retroid") || !strcmp(s, "thor-rainbow") || !strcmp(s, "thor_rainbow")) return PROMPT_THOR;
    if (!strcmp(s, "thor-white") || !strcmp(s, "thor_white")) return PROMPT_THOR_WHITE;
    if (!strcmp(s, "custom")) return PROMPT_CUSTOM;
    if (!strcmp(s, "thor-black") || !strcmp(s, "thor_black")) return PROMPT_THOR_BLACK;
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

/* --prompts custom: --prompt-colors a=RRGGBB:RRGGBB,b=...,x=...,y=...,sym=xbox|nintendo|ps.  a b x y are the face
   buttons by POSITION as in --prompt-map (a south, b east, x west, y north), each disc:letter colour; sym the symbols
   drawn (Xbox letters A B X Y, Nintendo / Thor letters B A Y X, PlayStation shapes ✕ ○ □ △, by position).  The shadow
   (the disc 30 % toward black) and the soft edge (the disc 35 % toward the letter) are computed here. */
enum { SYM_XBOX, SYM_NINTENDO, SYM_PS };
static int custom_sym = SYM_NINTENDO;
extern u16 port_prompt_clut[];
static u16 bgr_of(u32 rgb) { return (u16)((rgb >> 19 & 31) | (rgb >> 11 & 31) << 5 | (rgb >> 3 & 31) << 10); }
static u32 mix_rgb(u32 a, u32 b, int t) {
    u32 o = 0;
    for (int k = 0; k < 24; k += 8) { int x = a >> k & 255, y = b >> k & 255; o |= (u32)(x + (y - x) * t / 100) << k; }
    return o;
}
static void custom_row(int pos, u32 disc, u32 letter) {
    u16 *r = port_prompt_clut + (9 + pos) * 16;   /* ROW_CUSTOM + pos */
    for (int i = 0; i < 16; i++) r[i] = port_prompt_clut[3 * 16 + i];   /* the shared entries of a Thor row */
    r[7] = bgr_of(disc); r[8] = bgr_of(mix_rgb(disc, 0, 30)); r[9] = bgr_of(mix_rgb(disc, letter, 35)); r[15] = bgr_of(letter);
    if (!r[7]) r[7] = 0x8000; if (!r[8]) r[8] = 0x8000; if (!r[9]) r[9] = 0x8000; if (!r[15]) r[15] = 0x8000;   /* black, not transparent */
}
static void custom_defaults(void);
void port_prompt_colors(const char *s) {
    static const char *pos = "abxy";
    custom_defaults();   /* buttons not named keep the default */
    while (*s) {
        const char *end = strchr(s, ','), *eq = strchr(s, '=');
        if (!end) end = s + strlen(s);
        if (eq && eq < end) {
            if (eq - s == 3 && !strncmp(s, "sym", 3)) {
                custom_sym = !strncmp(eq + 1, "xbox", 4) ? SYM_XBOX : !strncmp(eq + 1, "ps", 2) ? SYM_PS : SYM_NINTENDO;
            } else if (eq - s == 1 && strchr(pos, *s)) {
                unsigned d = 0, l = 0xFFFFFF;
                char buf[32]; int n = (int)(end - eq - 1) < 31 ? (int)(end - eq - 1) : 31;
                memcpy(buf, eq + 1, n); buf[n] = 0;
                char *c = strchr(buf, ':');
                d = (unsigned)strtoul(buf, 0, 16);
                if (c) l = (unsigned)strtoul(c + 1, 0, 16);
                custom_row((int)(strchr(pos, *s) - pos), d, l);
            }
        }
        s = *end ? end + 1 : end;
    }
}
static void custom_defaults(void) {   /* before any --prompt-colors: black discs, white letters */
    static int done;
    if (done) return;
    done = 1;
    for (int i = 0; i < 4; i++) custom_row(i, 0x181818, 0xFFFFFF);
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
/* The lettered glyphs (Xbox, Nintendo, Thor) get a 1 px drop shadow down-right (the fill 30 % toward black) and a soft
   edge (the disc pixels 4-adjacent to the letter: the fill 35 % toward the letter's colour: white on Thor), so each needs
   a few more colours: the port CLUT has PORT_CLUT_ROWS rows (gpu_backend.h; CLUT id 0xFFFF - row), all sharing entries
   0, 3, 6, 11..14 (transparent, grey, dark disc, rim, cream):
     row 0: the table above (generic, the PlayStation shapes drawn where the game's icon is missing);
     row 1: Xbox: 1 / 2 / 4 / 5 the letters, 7 the shadow (disc * 0.7), 8 / 9 / 10 / 15 the soft edge per letter;
     row 2: Nintendo (and the shoulder labels of every lettered style): 14 the cream letters, 7 shadow, 8 soft edge;
     rows 3..6: Thor, one per button (south B yellow, east A red, west Y green, north X blue): 7 the disc, 8 its shadow,
     9 its soft edge, 15 the white letter;
     row 7: Thor with black buttons (thor-black): the same layout, a near-black disc;
     row 8: Thor with white buttons (thor-white): an off-white disc, 15 = the mid-grey letter, soft edge toward it;
     rows 9..12: custom (--prompts custom), one per position (south, east, west, north), the Thor layout built at run
     time from --prompt-colors. */
#define PORT_CLUT_ROWS 13   /* = gpu_backend.h */
#define MX(a, b, t) ((a) + ((b) - (a)) * (t) / 100)
#define MIX(r, g, b, r2, g2, b2, t) BGR(MX(r, r2, t), MX(g, g2, t), MX(b, b2, t))
#define RGB_(...) BGR(__VA_ARGS__)   /* the colour macros below hold commas: expand them first */
#define MIX_(...) MIX(__VA_ARGS__)
#define DISC 41, 41, 41
#define BASE0 0
#define CREAM RGB_(255, 239, 222)
#define KHAKI RGB_(206, 198, 165), RGB_(165, 156, 140), RGB_(140, 132, 115)
#define XG 0x3C, 0xB0, 0x43
#define XR 0xE0, 0x3C, 0x31
#define XB 0x2F, 0x6F, 0xD6
#define XY 0xF5, 0xC5, 0x18
#define THOR_ROWL(r, g, b, lr, lg, lb) 0, 0, 0, RGB_(107, 107, 107), 0, 0, RGB_(DISC), RGB_(r, g, b), MIX_(r, g, b, 0, 0, 0, 30), \
    MIX_(r, g, b, lr, lg, lb, 35), 0, KHAKI, CREAM, RGB_(lr, lg, lb)
#define THOR_ROW(r, g, b) THOR_ROWL(r, g, b, 255, 255, 255)
u16 port_prompt_clut[PORT_CLUT_ROWS * 16] = {   /* rows 9..12 (custom) are filled by --prompt-colors */
    0, RGB_(XG), RGB_(XR), RGB_(107, 107, 107), RGB_(XB), RGB_(XY),
    RGB_(DISC), RGB_(0x1E, 0x3F, 0xB0), RGB_(0x2E, 0x8B, 0x4E), RGB_(0xF2, 0xC2, 0x30), RGB_(0xC8, 0x28, 0x1E),
    KHAKI, CREAM, RGB_(255, 255, 255),
    /* 1 Xbox */
    0, RGB_(XG), RGB_(XR), RGB_(107, 107, 107), RGB_(XB), RGB_(XY), RGB_(DISC), MIX_(DISC, 0, 0, 0, 30),
    MIX_(DISC, XG, 35), MIX_(DISC, XR, 35), MIX_(DISC, XB, 35), KHAKI, CREAM, MIX_(DISC, XY, 35),
    /* 2 Nintendo */
    0, 0, 0, RGB_(107, 107, 107), 0, 0, RGB_(DISC), MIX_(DISC, 0, 0, 0, 30), MIX_(DISC, 255, 239, 222, 35), 0, 0,
    KHAKI, CREAM, RGB_(255, 255, 255),
    /* 3..6 Thor: B, A, Y, X */
    THOR_ROW(0xF2, 0xC2, 0x30), THOR_ROW(0xC8, 0x28, 0x1E), THOR_ROW(0x2E, 0x8B, 0x4E), THOR_ROW(0x1E, 0x3F, 0xB0),
    /* 7 Thor with black buttons: one near-black disc for all four */
    THOR_ROW(24, 24, 24),
    /* 8 Thor with white buttons: an off-white disc, mid-grey letters (index 15 holds the letter colour) */
    THOR_ROWL(236, 236, 232, 130, 130, 130),
};
enum { ROW_XBOX = 1, ROW_NINTENDO = 2, ROW_THOR = 3, ROW_THOR_BLACK = 7, ROW_THOR_WHITE = 8, ROW_CUSTOM = 9 };
#define PORT_CLUT_ID 0xFFFF
/* the letter effects: letter pixels carry 0x80 | colour; the shadow, then the soft edge (pixels still == fill), then the
   letters (soft_of[colour] = the soft edge index next to a letter of that colour) */
static void letter_fx(u8 *p, int stride, int w, int h, int fill, int shadow, const u8 *soft_of) {
    #define L(x, y) ((x) >= 0 && (x) < w && (y) >= 0 && (y) < h && (p[(y) * stride + (x)] & 0x80))
    for (int y = h - 2; y >= 0; y--) for (int x = w - 2; x >= 0; x--)
        if (L(x, y) && !L(x + 1, y + 1) && p[(y + 1) * stride + x + 1] == fill) p[(y + 1) * stride + x + 1] = (u8)shadow;
    for (int y = 0; y < h; y++) for (int x = 0; x < w; x++) {
        if (p[y * stride + x] != fill) continue;
        int c = L(x - 1, y) ? p[y * stride + x - 1] : L(x + 1, y) ? p[y * stride + x + 1] :
                L(x, y - 1) ? p[(y - 1) * stride + x] : L(x, y + 1) ? p[(y + 1) * stride + x] : 0;
        if (c) p[y * stride + x] = soft_of[c & 15];
    }
    for (int i = 0; i < h; i++) for (int x = 0; x < w; x++) p[i * stride + x] &= 15;
    #undef L
}
static const u8 soft_xbox[16] = { [P_XGREEN] = 8, [P_XRED] = 9, [P_XBLUE] = 10, [P_XYELLOW] = 15 };
static const u8 soft_cream[16] = { [C_CREAM] = 8 };
static const u8 soft_white[16] = { [P_WHITE] = 9 };
#define NSLOTS 0x3c
static unsigned char slot_port_clut[NSLOTS];   /* glyph cache slot -> 1 + the port CLUT row it is drawn with, 0 none */
/* drawMessageBox / drawMessageGlyphs PORT2 hook: the CLUT id for a sprite of glyph cache slot `slot` */
unsigned short port_prompt_sprite_clut(int slot, unsigned short clut) {
    return slot >= 0 && slot < NSLOTS && slot_port_clut[slot] ? PORT_CLUT_ID - (slot_port_clut[slot] - 1) : clut;
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
/* --prompt-big (the 3DS's default: its screen shows the game's 320 columns one to one, and a 6 x 8 letter on a
   disc could not be told from another): the letter alone, bold, 8 x 10, coloured and outlined (big_letter) */
int port_prompt_big;
static const struct { char ch; u8 rows[10]; } font8[] = {
    { 'A', { 0x3C, 0x7E, 0xE7, 0xC3, 0xC3, 0xFF, 0xFF, 0xC3, 0xC3, 0xC3 } },
    { 'B', { 0xFE, 0xFF, 0xC3, 0xC3, 0xFE, 0xFF, 0xC3, 0xC3, 0xFF, 0xFE } },
    { 'X', { 0xC3, 0xC3, 0x66, 0x3C, 0x18, 0x18, 0x3C, 0x66, 0xC3, 0xC3 } },
    { 'Y', { 0xC3, 0xC3, 0x66, 0x3C, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18 } },
    { 'L', { 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xFF, 0xFF } },
    { 'R', { 0xFE, 0xFF, 0xC3, 0xC3, 0xFF, 0xFE, 0xCC, 0xC6, 0xC3, 0xC3 } },
};
/* the letter alone, 8 x 10, in its button's colour with a black outline, centred on (cx2 / 2, cy2 / 2), into an index
   image of w x h (stride given): index 15 the letter, 7 the outline (the rows of custom_row); 1 + the port CLUT row */
static int big_letter(u8 *p, int stride, int w, int h, int cx2, int cy2, int src) {
    static const u32 colour[4] = { 0xFFD800, 0xF03040, 0x40C850, 0x4088FF };   /* B yellow, A red, Y green, X blue: the SNES's, the New 3DS's */
    static int rows_set;
    if (!rows_set) { rows_set = 1; for (int i = 0; i < 4; i++) custom_row(i, 0x000000, colour[i]); }
    const char ch = "BAYX"[src]; int x0 = cx2 / 2 - 4, y0 = cy2 / 2 - 5;
    if (x0 < 1) x0 = 1; if (y0 < 1) y0 = 1; if (y0 + 11 > h) y0 = h - 11; if (y0 < 0) y0 = 0;
    for (int y = 0; y < h; y++) for (int x = 0; x < w; x++) p[y * stride + x] = 0;
    for (unsigned i = 0; i < sizeof font8 / sizeof font8[0]; i++) if (font8[i].ch == ch)
        for (int y = 0; y < 10; y++) for (int x = 0; x < 8; x++)
            if ((font8[i].rows[y] >> (7 - x) & 1) && x0 + x < w && y0 + y < h) p[(y0 + y) * stride + x0 + x] = 15;
    for (int y = 0; y < h; y++) for (int x = 0; x < w; x++) {
        if (p[y * stride + x]) continue;
        int near = 0;
        for (int dy = -1; dy <= 1; dy++) for (int dx = -1; dx <= 1; dx++) {
            int xx = x + dx, yy = y + dy;
            if (xx >= 0 && xx < w && yy >= 0 && yy < h && p[yy * stride + xx] == 15) near = 1;
        }
        if (near) p[y * stride + x] = 7;
    }
    return 1 + ROW_CUSTOM + src;
}
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
/* the PlayStation shapes, one design for the message boxes and every menu icon: (ox, oy) = the top-left of the 12 x 12
   disc they sit on; small (a disc under 12 px, e.g. the save screens' 12 x 12 slots): the same shapes in a 6 x 6 box,
   hollow like the big ones */
static void ps_shape(u8 *buf, int stride, int w, int h, int ox, int oy, int which, int col, int small) {
    #define SP(x, y) do { int X_ = ox + (x), Y_ = oy + (y); if (X_ >= 0 && X_ < w && Y_ >= 0 && Y_ < h) buf[Y_ * stride + X_] = (u8)col; } while (0)
    static const u8 big[4][8] = { { 0 }, { 0x3C, 0x42, 0x81, 0x81, 0x81, 0x81, 0x42, 0x3C }, { 0 }, { 0 } };
    static const u8 sm[4][6] = { { 0x21, 0x12, 0x0C, 0x0C, 0x12, 0x21 }, { 0x1E, 0x21, 0x21, 0x21, 0x21, 0x1E },
                                 { 0x3F, 0x21, 0x21, 0x21, 0x21, 0x3F }, { 0x0C, 0x12, 0x12, 0x21, 0x21, 0x3F } };
    if (small) {   /* centred: the disc is (12 - 2 * shrink) wide, the caller passes its centre - 6 */
        for (int y = 0; y < 6; y++) for (int x = 0; x < 6; x++) if (sm[which][y] >> (5 - x) & 1) SP(3 + x, 3 + y);
    } else switch (which) {
    case PS_CROSS:  for (int i = 0; i < 6; i++) { SP(3 + i, 3 + i); SP(8 - i, 3 + i); } break;
    case PS_CIRCLE: for (int y = 0; y < 8; y++) for (int x = 0; x < 8; x++) if (big[1][y] >> (7 - x) & 1) SP(2 + x, 2 + y); break;
    case PS_SQUARE: for (int i = 0; i < 6; i++) { SP(3 + i, 3); SP(3 + i, 8); SP(3, 3 + i); SP(8, 3 + i); } break;
    default:
        for (int y = 0; y <= 6; y++) { SP(5 - y / 2, 2 + y); SP(6 + y / 2, 2 + y); }
        for (int x = 2; x <= 9; x++) SP(x, 8);
    }
    #undef SP
}
/* the PlayStation symbols, in the game's colours */
static void ps_symbol_c(int which, int col);
static void ps_symbol(int which) { ps_symbol_c(which, -1); }
/* col >= 0: every shape in that colour (the custom style) */
static void ps_symbol_c(int which, int col) {
    static const int pcol[4] = { C_BLUE, C_RED, C_PINK, C_GREEN };   /* the circle: fallback only (the game's own is used once met) */
    ps_shape(&img[0][0], GW, GW, GH, 0, 2, which, col >= 0 ? col : pcol[which], 0);
}
/* generic: four dots in a diamond, the button's position lit */
static void diamond(int pos) {
    static const int px[4] = { 5, 8, 2, 5 }, py[4] = { 9, 6, 6, 3 };   /* south, east, west, north (2 x 2 dots) */
    for (int i = 0; i < 4; i++) {
        int c = i == pos ? C_CREAM : C_GREY;
        put(px[i], py[i], c); put(px[i] + 1, py[i], c); put(px[i], py[i] + 1, c); put(px[i] + 1, py[i] + 1, c);
    }
}

static int draw(int ps, int src, int style) {   /* returns 1 + the port CLUT row, 0 = the font CLUT */
    static const char *ps_sh[] = { "L1", "R1", "L2", "R2" }, *xb_sh[] = { "LB", "RB", "LT", "RT" },
                      *nt_sh[] = { "L", "R", "ZL", "ZR" };
    static const char *letters = "ABXY";
    static const int xb_col[4] = { P_XGREEN, P_XRED, P_XBLUE, P_XYELLOW };   /* port CLUT */
    draw_disc();
    if (src == SRC_NONE) return 0;
    if (src >= SRC_L1) {
        const char *t = style == PROMPT_XBOX ? xb_sh[src - SRC_L1] : style == PROMPT_NINTENDO || THORISH(style) || style == PROMPT_CUSTOM ? nt_sh[src - SRC_L1] : ps_sh[src - SRC_L1];
        if (style == PROMPT_PS || style == PROMPT_GENERIC) { label(t, C_CREAM); return 0; }
        label(t, C_CREAM | 0x80);
        letter_fx(&img[0][0], GW, GW, GH, C_DISC, 7, soft_cream);
        return 1 + ROW_NINTENDO;
    }
    if (port_prompt_big && style != PROMPT_PS) return big_letter(&img[0][0], GW, GW, GH, 12, 16, src);   /* (the disc's centre: rows 2..13) */
    switch (style) {
    case PROMPT_PS: ps_symbol(src); break;   /* SRC_SOUTH..NORTH are in PS_CROSS..TRIANGLE order */
    case PROMPT_XBOX: {
        char t[2] = { letters[src], 0 };
        label(t, xb_col[src] | 0x80);
        letter_fx(&img[0][0], GW, GW, GH, C_DISC, 7, soft_xbox);
        return 1 + ROW_XBOX; }
    case PROMPT_NINTENDO: case PROMPT_THOR: case PROMPT_THOR_BLACK: case PROMPT_THOR_WHITE: {
        /* by position, as printed on Nintendo pads and the AYN Thor / Retroid: south B, east A, west Y, north X.
           Nintendo: cream letters on the dark disc (Switch Pro); Thor: white letters on the SNES colours (port CLUT:
           B yellow, A red, Y green, X blue).  src is the positional (Xbox-named) button: SDL with button labels off,
           Android's KEYCODE_BUTTON_A = south. */
        static const char nl[4] = { 'B', 'A', 'Y', 'X' };
        char t[2] = { nl[src], 0 };
        if (THORISH(style)) {
            draw_disc_col(7); label(t, P_WHITE | 0x80);
            letter_fx(&img[0][0], GW, GW, GH, 7, 8, soft_white);
            return 1 + THOR_ROW_OF(style, src);
        }
        label(t, C_CREAM | 0x80);
        letter_fx(&img[0][0], GW, GW, GH, C_DISC, 7, soft_cream);
        return 1 + ROW_NINTENDO; }
    case PROMPT_CUSTOM: {   /* the user's colours (row ROW_CUSTOM + position), the chosen symbols */
        static const u8 soft_c[16] = { [P_WHITE] = 9 };
        custom_defaults();
        draw_disc_col(7);
        if (custom_sym == SYM_PS) ps_symbol_c(src, P_WHITE | 0x80);
        else { char t[2] = { (custom_sym == SYM_XBOX ? "ABXY" : "BAYX")[src], 0 }; label(t, P_WHITE | 0x80); }
        letter_fx(&img[0][0], GW, GW, GH, 7, 8, soft_c);
        return 1 + ROW_CUSTOM + src; }
    default: diamond(src); break;
    }
    (void)ps;
    return 0;
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
        int row1 = 0;
        if (port_prompt_style == PROMPT_PS && s2 < 4 && have_orig[s2]) memcpy(img, orig[s2], sizeof img);
        else row1 = draw(ps, s2, port_prompt_style);
        lower1();
        pack();
        if (slot >= 0 && slot < NSLOTS)
            slot_port_clut[slot] = (unsigned char)row1;
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
static u8 mi_pclut[NMI];       /* ... drawn with port CLUT row mi_pclut - 1 (0: its own CLUT) */
static int mi_gen[NMI];
int port_prompt_icons_on;      /* any mi_on: gpu.c's fast path */
static void (*mi_write)(int, int, int, int, const unsigned short *);
static int prompt_gen(void) { return port_prompt_big * 65536 + port_prompt_style * 4096 + port_prompt_src[0] * 512 + port_prompt_src[1] * 64 + port_prompt_src[2] * 8 + port_prompt_src[3]; }

#define MW 16
static u8 mimg[16][MW];
static void mput(int x, int y, int c, int w, int h) { if (x >= 0 && x < w && y >= 0 && y < h) mimg[y][x] = (u8)c; }
/* the glyph of PS button `ps` for icon i (w x h texels), into mimg; returns 1 + its port CLUT row (0 = its own CLUT) */
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
    if ((THORISH(st) || st == PROMPT_CUSTOM) && src < 4) fill = 7;   /* THOR_ROW_OF / ROW_CUSTOM + src */
    if (st == PROMPT_CUSTOM) custom_defaults();
    for (int y = 0; y < h; y++) for (int x = 0; x < w; x++) {
        int dx = 2 * x + 1 - cx2, dy = 2 * y + 1 - cy2, r2 = dx * dx + dy * dy;   /* in half pixels */
        if (r2 <= d * d - 2 * d) mimg[y][x] = (u8)fill;
        else if (r2 <= d * d + d / 2) mimg[y][x] = 12;                           /* the khaki rim */
    }
    int lx = cx2 / 2 - 3, ly = cy2 / 2 - 4;                /* a 6 x 8 letter centred */
    const char *t = 0; int col = C_CREAM; char one[2] = { 0, 0 };
    static const int xb_col[4] = { P_XGREEN, P_XRED, P_XBLUE, P_XYELLOW };
    static const char *xb_sh[] = { "LB", "RB", "LT", "RT" }, *nt_sh[] = { "L", "R", "ZL", "ZR" }, *ps_sh[] = { "L1", "R1", "L2", "R2" };
    if (src == SRC_NONE) return 1;   /* the empty disc (row 0) */
    if (port_prompt_big && src < 4 && st != PROMPT_PS) return big_letter(&mimg[0][0], MW, w, h, cx2, cy2, src);
    if (src >= SRC_L1) t = st == PROMPT_XBOX ? xb_sh[src - SRC_L1] : st == PROMPT_NINTENDO || THORISH(st) || st == PROMPT_CUSTOM ? nt_sh[src - SRC_L1] : ps_sh[src - SRC_L1];
    else if (st == PROMPT_XBOX) { one[0] = "ABXY"[src]; t = one; col = xb_col[src]; }
    else if (st == PROMPT_CUSTOM && custom_sym != SYM_PS) { one[0] = (custom_sym == SYM_XBOX ? "ABXY" : "BAYX")[src]; t = one; col = P_WHITE; }
    else if (st == PROMPT_NINTENDO || THORISH(st)) { one[0] = "BAYX"[src]; t = one; col = THORISH(st) ? P_WHITE : C_CREAM; }
    if (t) {
        for (unsigned k = 0; k < sizeof font6 / sizeof font6[0] && !t[1]; k++) if (font6[k].ch == t[0])
            for (int y = 0; y < 8; y++) for (int x = 0; x < 6; x++) if (font6[k].rows[y] >> (5 - x) & 1) mput(lx + x, ly + y, col | 0x80, w, h);
        if (t[1]) for (int c = 0; c < 2; c++) for (unsigned k = 0; k < sizeof font3 / sizeof font3[0]; k++) if (font3[k].ch == t[c])
            for (int y = 0; y < 5; y++) for (int x = 0; x < 3; x++) if (font3[k].rows[y] >> (2 - x) & 1) mput(lx + c * 4 + x, ly + 2 + y, C_CREAM | 0x80, w, h);
        if (t[1]) { letter_fx(&mimg[0][0], MW, w, h, C_DISC, 7, soft_cream); return 1 + ROW_NINTENDO; }
        if (st == PROMPT_XBOX) { letter_fx(&mimg[0][0], MW, w, h, C_DISC, 7, soft_xbox); return 1 + ROW_XBOX; }
        if (THORISH(st)) { letter_fx(&mimg[0][0], MW, w, h, 7, 8, soft_white); return 1 + THOR_ROW_OF(st, src); }
        if (st == PROMPT_CUSTOM) { letter_fx(&mimg[0][0], MW, w, h, 7, 8, soft_white); return 1 + ROW_CUSTOM + src; }
        letter_fx(&mimg[0][0], MW, w, h, C_DISC, 7, soft_cream);
        return 1 + ROW_NINTENDO;
    }
    if (st == PROMPT_PS || st == PROMPT_CUSTOM) {   /* PS shapes where the source icon is not on the page (port CLUT: the nearest colours); custom: in its letter colour */
        int cu = st == PROMPT_CUSTOM;
        static const int pcl[4] = { 4, 2, 10, 1 };   /* port CLUT: the nearest blue / red / pink / green */
        ps_shape(&mimg[0][0], MW, w, h, cx2 / 2 - 6, cy2 / 2 - 6, src, cu ? P_WHITE | 0x80 : pcl[src], d < 12);
        if (cu) { letter_fx(&mimg[0][0], MW, w, h, 7, 8, soft_white); return 1 + ROW_CUSTOM + src; }
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
/* gpu.c, for a 4 bpp primitive of page (tx, ty) with texels u0..u1, v0..v1: 1 + the port CLUT row to give it, or 0 */
int port_prompt_tex_clut(int tx, int ty, int u0, int v0, int u1, int v1) {
    int g = prompt_gen();
    for (int i = 0; i < NMI; i++) {
        if (!mi_have[i]) continue;
        const MenuIcon *m = &menu_icons[i];
        int ix = (m->x - tx) * 4, iy = m->y - ty;
        if (u0 < ix || u1 >= ix + m->w * 4 || v0 < iy || v1 >= iy + m->h) continue;
        if (mi_gen[i] != g && mi_write) mi_apply(i);
        return mi_on[i] ? mi_pclut[i] : 0;
    }
    return 0;
}
