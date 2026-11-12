/*
 * --cut-content=funga (bit 64): AREA10 section 2 (Old Tree Hill), the unused Funga stage past the palisade.
 *
 * The "door" there (updateAreaMechanisms subtype 2, layer 4 type 7 at (1288, -187, 180)) is a small block, 16 wide,
 * 97 tall and 50 deep (x -8..7, z -25..25 around its origin), that fills the gap in the palisade across the path.  The
 * game never moves it, and the room model holds a copy of the same block at the same place, so turning the object
 * alone shows nothing.  Here the room's copy is flattened away (its upright faces in that box; the ground under it
 * stays) and the object swings open on its back edge (z 205) like a hinged gate: a quarter turn, its origin moved so
 * that the hinge stays put.  The camera bound that kept the stage out of reach: port/fixes.txt (initSectionCamera).
 */
#include <string.h>
typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32; typedef short s16; typedef int s32; typedef signed char s8;

extern int port_cut_content;
extern int rsin(int), rcos(int);

/* the room's copy of the gate block (WMD in end space = world here: no mirroring) */
void port_funga_room(const u8 *tbl, int area, int section)
{
    static const u8 *done;
    if (!(port_cut_content & 64) || area != 10 || section != 2 || !tbl || tbl == done) return;
    done = tbl;
    int n = tbl[0];
    const s32 *off = (const s32 *)(tbl + 4);
    static const int nv[4] = {3, 4, 3, 4}, cw[4] = {0, 0, 2, 3};   /* FT3 FT4 GT3 GT4: vertices, extra colour words */
    for (int s = 0; s < n; s++) {
        u8 *p = (u8 *)tbl + off[s];
        for (int g = 0; g < 4; g++) {
            u32 c; memcpy(&c, p, 4); p += 4;
            for (u32 k = 0; k < c; k++) {
                s16 *v = (s16 *)(p + 8 + 4 * cw[g]);
                int x0 = 32767, x1 = -32768, y0 = 32767, y1 = -32768, z0 = 32767, z1 = -32768;
                for (int j = 0; j < nv[g]; j++) {
                    s16 *w = v + 4 * j;
                    x0 = w[0] < x0 ? w[0] : x0; x1 = w[0] > x1 ? w[0] : x1;
                    y0 = w[1] < y0 ? w[1] : y0; y1 = w[1] > y1 ? w[1] : y1;
                    z0 = w[2] < z0 ? w[2] : z0; z1 = w[2] > z1 ? w[2] : z1;
                }
                if (x0 >= 1279 && x1 <= 1296 && y0 >= -230 && y1 <= -131 && z0 >= 155 && z1 <= 205 && y1 - y0 > 2)
                    for (int j = 1; j < nv[g]; j++) memcpy(v + 4 * j, v, 6);   /* all corners on the first: nothing drawn */
                p += 8 + 4 * cw[g] + 8 * nv[g];
            }
        }
    }
}

static void spawn_npcs(void);
/* the gate object, every update (after ObjCullRegister): opens once Tombi is past x 1088 (200 units before it) */
void port_funga_gate(u8 *o, int player_x)
{
    static int bx = 0x7fff, bz;
    if (!(port_cut_content & 64) || o[3] != 2) return;
    s16 *x = (s16 *)(o + 0x12), *z = (s16 *)(o + 0x1A);
    s32 *rot = (s32 *)(o + 0x88);
    if (bx == 0x7fff || (*rot == 0 && (*x != bx || *z != bz))) { bx = *x; bz = *z; }   /* (a new spawn: its own place) */
    if (!o[0x91]) { o[0x91] = 1; spawn_npcs(); }   /* once per gate object = once per section load (objects start zeroed) */
    if (o[0x0A] == 0x10) o[0x0A] = 0x11;           /* the model drawer that applies the rotation (+0x84/+0x88/+0x8C) */
    if (*rot < 0x400 && (*rot || player_x > 1088)) *rot += 0x20;
    if (*rot > 0x400) *rot = 0x400;
    /* hinge on the back edge (local z +25): origin = hinge - R(0, 0, 25) */
    int a = *rot;
    *x = (s16)(bx - 25 * rsin(a) / 4096);
    *z = (s16)(bz + 25 - 25 * rcos(a) / 4096);
}

/* ---- the Funga Chief (and later the Fungas): sprites of SPR table 0x37 (docs/CUT_CONTENT.md "Jungle (AREA10)").
   Only AREA10 load lists 1 and 5 bring that table; section 2 does not, so it is read here from the user's own extraction
   (AREA10/D005.GAM unpacked: list 1's LD0SYS pieces follow the room model, table 0x37 at +0x26954, 0x17FC bytes).  The
   pixels (SYSTEM/A05700.000 at VRAM 64,0) and section 2's palette (CLUT03: the chief comes out blue with a red cape)
   are in place already.  The objects are layer 3 updateAnimDecorSprite objects (the torches' handler: anim + cull)
   spawned with marker b3 = 0x5A and given this table, page offset 0 and their own frames (port_funga_npc_*). */
#include <stdio.h>
#include <stdlib.h>
extern const char *port_data_dir;
extern int port_printf(const char *, ...);
extern void lzDecompress(unsigned char *, unsigned char *);
extern void AnimLoadDuration(void *);
extern void spawnObjectFromRecord(void *, int);
#define TBL_OFF 0x26954u
#define TBL_LEN 0x17FCu
static u8 chief_tbl[TBL_LEN + 16];
static int chief_state;          /* 0 not tried, 1 loaded, -1 failed */
/* The game's rectangles for these never-used frames cut the drawings or take in pieces of their neighbours, so the
   sprites are drawn from our own table: one part per drawing, its rectangle around the whole drawing on the page
   (page coordinates measured on the pages; page 3 = VRAM 192,0 the chief, page 1 = VRAM 64,0 the Fungas). */
static const struct { u8 page, u, v, w, h; } OWN[] = {
    /* chief walk (bent forward), in order */
    {3,   2, 70, 49, 58}, {3,  58, 71, 43, 56}, {3, 108, 70, 35, 58}, {3, 149, 70, 33, 57}, {3, 188, 75, 51, 52}, {3, 170, 5, 52, 58},
    /* chief talk */
    {3,   4,  4, 48, 60}, {3, 113,  4, 49, 60},
};
#define NOWN (int)(sizeof OWN / sizeof OWN[0])
static u8 own_tbl[NOWN * 4 + NOWN * 16];
static void build_own(void)
{
    for (int f = 0; f < NOWN; f++) {
        u16 cnt = 1, off = (u16)(NOWN * 4 + f * 16);
        memcpy(own_tbl + 4 * f, &cnt, 2); memcpy(own_tbl + 4 * f + 2, &off, 2);
        u8 *q = own_tbl + off; u16 clut = 0x780A, tp = OWN[f].page;
        int u0 = OWN[f].u, v0 = OWN[f].v, u1 = u0 + OWN[f].w - 1, v1 = v0 + OWN[f].h - 1;
        q[0] = u0; q[1] = v0; memcpy(q + 2, &clut, 2); q[4] = u1; q[5] = v0; memcpy(q + 6, &tp, 2);
        q[8] = u0; q[9] = v1; q[10] = OWN[f].w; q[11] = OWN[f].h; q[12] = u1; q[13] = v1;
        q[14] = (u8)(s8)(-(OWN[f].w / 2)); q[15] = (u8)(s8)(8 - OWN[f].h);
    }
}
static int load_chief(void)
{
    if (chief_state) return chief_state > 0;
    chief_state = -1;
    char p[1024]; FILE *f; long len; u8 *gam, *pg;
    snprintf(p, sizeof p, "%s/AREA10/D005.GAM", port_data_dir ? port_data_dir : "data");
    if (!(f = fopen(p, "rb"))) { port_printf("funga: cannot open %s\n", p); return 0; }
    fseek(f, 0, SEEK_END); len = ftell(f); fseek(f, 0, SEEK_SET);
    gam = malloc(len + 16);
    if (!gam || fread(gam, 1, len, f) != (size_t)len) { fclose(f); free(gam); return 0; }
    fclose(f);
    u32 size = gam[4] | gam[5] << 8 | gam[6] << 16 | (u32)gam[7] << 24;
    if (len < 8 || memcmp(gam, "GAM", 3) || size < TBL_OFF + TBL_LEN || size > (16u << 20)) { free(gam); port_printf("funga: %s is not the expected GAM\n", p); return 0; }
    pg = calloc(1, size + 1024);
    lzDecompress(gam, pg);
    free(gam);
    memcpy(chief_tbl, pg + TBL_OFF, TBL_LEN);
    free(pg);
    u16 first; memcpy(&first, chief_tbl + 2, 2);
    if (first != 0x27C) { port_printf("funga: table 0x37 not where expected (first offset %x)\n", first); return 0; }
    build_own();
    chief_state = 1;
    return 1;
}

/* the spawns, once per section load (from the gate's first update) */
static void spawn_npcs(void)
{
    static u8 rec[16];
    if (!load_chief()) return;
    memset(rec, 0, sizeof rec);
    rec[0] = 2; rec[1] = 3; rec[2] = 0x3F; rec[3] = 0x5A; rec[4] = 0;
    s16 pos[3] = {1580, -160, 180};
    memcpy(rec + 6, pos, 6);
    spawnObjectFromRecord(rec, -1);
}

/* the chief: walks between x 1420 and 1720 (frames 50 51 52 51, 8 ticks each, 1 unit a tick), turns, and every second
   lap stops for about two seconds talking (47 49).  The frames face left: bit 0 of +2E set = as drawn. */
static const u16 WALK[] = {0, 1, 2, 3}, TALK[] = {6, 7};
static u8 script[8][8];
static void show(u8 *o, int k, u16 frame)
{
    u16 c = 0x8000 | 60;
    memset(script[k], 0, 8); memcpy(script[k], &frame, 2); memcpy(script[k] + 6, &c, 2);
    if (*(u32 *)(o + 0x24) != (u32)(unsigned long)script[k] || *(u16 *)script[k] != frame) {
        *(u32 *)(o + 0x24) = (u32)(unsigned long)script[k];
        AnimLoadDuration(o);
    }
}
void port_funga_npc_init(u8 *o)
{
    if (!(port_cut_content & 64) || o[0x0C] != 0x5A || chief_state <= 0) return;
    *(s16 *)(o + 0x1E) = 0;                 /* our parts hold the absolute page */
    o[0x0D] = 0;                            /* opaque */
    *(u32 *)(o + 0x3C) = (u32)(unsigned long)own_tbl;
    o[0x0F] = 0;
    *(s32 *)(o + 0x90) = 0;                 /* tick */
    *(s32 *)(o + 0x94) = 1;                 /* direction */
    *(s32 *)(o + 0x98) = 0;                 /* talk ticks left */
    *(s32 *)(o + 0x9C) = 0;                 /* laps */
    o[0x2E] &= ~1;
    show(o, 0, WALK[0]);
}
int port_funga_npc_step(u8 *o)
{
    if (!(port_cut_content & 64) || o[0x0C] != 0x5A || chief_state <= 0) return 0;
    s32 *t = (s32 *)(o + 0x90), *dir = (s32 *)(o + 0x94), *talk = (s32 *)(o + 0x98), *laps = (s32 *)(o + 0x9C);
    s16 *x = (s16 *)(o + 0x12);
    (*t)++;
    if (*talk > 0) {
        (*talk)--;
        show(o, 6 + (*t / 16 & 1), TALK[*t / 16 & 1]);
        return 1;
    }
    *x += *dir;
    if ((*dir > 0 && *x >= 1720) || (*dir < 0 && *x <= 1420)) {
        *dir = -*dir;
        if (++*laps % 2 == 0) *talk = 120;
    }
    if (*dir < 0) o[0x2E] |= 1; else o[0x2E] &= ~1;
    int k = *t / 8 % 4;
    show(o, k, WALK[k]);
    return 1;
}
