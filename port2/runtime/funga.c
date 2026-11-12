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
extern int port_printf(const char *, ...);
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
static void hold_fungas(void);
static void back_lane(void);
/* the gate object, every update (after ObjCullRegister): opens once Tombi is past x 1088 (200 units before it) */
void port_funga_gate(u8 *o, int player_x)
{
    static int bx = 0x7fff, bz;
    if (!(port_cut_content & 64) || o[3] != 2) return;
    s16 *x = (s16 *)(o + 0x12), *z = (s16 *)(o + 0x1A);
    s32 *rot = (s32 *)(o + 0x88);
    if (bx == 0x7fff || (*rot == 0 && (*x != bx || *z != bz))) { bx = *x; bz = *z; }   /* (a new spawn: its own place) */
    if (!o[0x91]) { o[0x91] = 1; spawn_npcs(); }
    back_lane();
    hold_fungas();   /* once per gate object = once per section load (objects start zeroed) */
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
static u8 *fungas[3];

static void spawn_npcs(void)
{
    static u8 rec[16];
    if (!load_chief()) return;
    memset(rec, 0, sizeof rec);
    rec[0] = 2; rec[1] = 3; rec[2] = 0x3F; rec[3] = 0x5A; rec[4] = 0;
    s16 pos[3] = {1580, -160, 180};
    memcpy(rec + 6, pos, 6);
    spawnObjectFromRecord(rec, -1);
    /* the jungle's Fungas: the spear tribesmen of section 0 (layer 2 type 0x3A, updateSpearPig), whose sprites are
       SPR table 0x37 = SPR_DATA[7]; section 2 leaves that slot empty, so it points at the table read above */
    extern char *SPR_DATA[];
    SPR_DATA[7] = (char *)chief_tbl;
    static const s16 FX[3] = {1353, 1741, 1788}, FZ[3] = {258, 271, 258};   /* on the outer back-row drums (the middle ones and the stand are the back lane's path) */
    static const u8 FST[3] = {0, 1, 0};
    for (int i = 0; i < 3; i++) {
        memset(rec, 0, sizeof rec);
        rec[0] = 2; rec[1] = 2; rec[2] = 0x3A; rec[4] = FST[i];
        s16 q[3] = {FX[i], -200, FZ[i]};
        memcpy(rec + 6, q, 6);
        spawnObjectFromRecord(rec, -1);
        { extern u8 *SPAWN_RECORD_OBJECT; fungas[i] = SPAWN_RECORD_OBJECT; }
    }
}
/* each frame (from the gate's update): every Funga stays on its drum (x fixed) and hops on it: a 40-unit jump every
   70 frames, the three out of step (their own jump only comes when they attack) */
static void hold_fungas(void)
{
    static const s16 FX[3] = {1353, 1741, 1788};
    for (int i = 0; i < 3; i++) {
        u8 *o = fungas[i];
        if (!o || o[0] == 0 || o[2] != 0x3A) { fungas[i] = 0; continue; }
        *(s16 *)(o + 0x12) = FX[i];
        /* a hop every 70 frames, 40 high, phases apart: y = top - parabola */
        extern int port_frame;
        int t = (port_frame + i * 23) % 70, h = t < 36 ? (40 * 4 * t * (36 - t)) / (36 * 36) : 0;
        *(s16 *)(o + 0x16) = (s16)(-186 - h); *(u16 *)(o + 0x14) = 0;
        /* their own pose is the walk (the AI keeps walking in place): standing on the drum (frame 62), crouched
           leap in the air (73), facing the path */
        static u8 sc[3][8];
        u16 fr = h > 4 ? 73 : 62, c = 0x8000 | 60;
        if (*(u16 *)sc[i] != fr || *(u32 *)(o + 0x24) != (u32)(unsigned long)sc[i]) {
            memset(sc[i], 0, 8); memcpy(sc[i], &fr, 2); memcpy(sc[i] + 6, &c, 2);
            *(u32 *)(o + 0x24) = (u32)(unsigned long)sc[i]; AnimLoadDuration(o);
        }
    }
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

/* ---- the back lane: the drums' row (z 258..286) and the straw-roofed stand (x 1526..1618, top y -232, z 212..298).
   The section's WHM (AREA10 load list 3, D204.GAM) has 3 planes; Tombi walks on plane 2 (z 180; plane = z / 90, origin 0,
   capped at 2 for this section, so anything deeper reads plane 2 too).  The game's own lane moves work off the floor's
   surface bits (applyCollisionTileResponse -> +A0): 0x2000 lets Up+Cross start playerStepLaneShiftFall (step 0x11,
   +90 depth while 1F8001C8 bit 0 is clear, as here: into the background), 0x1000 lets Down+Cross start
   playerStepLaneJump (step 0x12, -90: towards the camera); the section's own pier at x 352..384 uses both.  Here a copy
   of the WHM gets a fourth plane (z 270..359): the ground of plane 2 from the palisade (x 1336) on, a wall at its left
   end and the stand's top; its floors carry 0x1000 (back to the front with Down+Cross); the front floors in front of the
   stand (x 1528..1615) carry 0x2000.  The copy replaces WHM slot 0 (1F800308) whenever the slot holds this section's WHM (each load), and
   getCollisionPlaneAt lifts the plane cap to 3 only for this copy (port/fixes.txt).  The drums' solid box takes
   |dz| < 45, so the back-row drums are solid in the back lane only. */
u8 port_funga_whm[40000];
#define BACK_X0 1336
#define STAND_X0 1528
#define STAND_X1 1616
#define STAND_Y (-232)
static void back_lane(void)
{
    extern char *WHM_DATA[];
    const u8 *s = (const u8 *)WHM_DATA[0];
    u8 *d = port_funga_whm;
    if (!s || s == d) return;
    u16 w, o0, o1, o2;
    memcpy(&w, s + 4, 2); memcpy(&o0, s + 8, 2); memcpy(&o1, s + 10, 2); memcpy(&o2, s + 12, 2);
    if (s[0] != 0x11 || s[1] != 3 || w != 6400 || o0 != 14) return;
    /* size of the original: the end of the last list of plane 2 (the highest offset reached) */
    u32 end = 0;
    for (int pl = 0; pl < 3; pl++) {
        u16 po; memcpy(&po, s + 8 + 2 * pl, 2);
        for (int c = 0; c < w / 8; c++) {
            u16 co, n; memcpy(&co, s + po + 2 * c, 2); memcpy(&n, s + po + co, 2);
            u32 e = (u32)po + co + 2 + 8u * n;
            if (e > end) end = e;
        }
    }
    if (end + 2 + (w / 8) * 4u > sizeof port_funga_whm) { port_printf("funga: WHM copy too small\n"); return; }
    /* header with 4 plane offsets, planes 0..2 moved up by 2 bytes (their own offsets are relative to the plane) */
    memcpy(d, s, 8);
    d[1] = 4;
    memcpy(d + 16, s + 14, end - 14);
    u16 po[4] = {(u16)(o0 + 2), (u16)(o1 + 2), (u16)(o2 + 2), (u16)((end + 2 + 1) & ~1u)};
    memcpy(d + 8, po, 8);
    u8 *p2 = d + po[2], *p3 = d + po[3];
    /* the front: Up+Cross in front of the stand (the one stretch of the back row without drums) -> port_funga_up_lane */
    for (int c = STAND_X0 / 8; c < STAND_X1 / 8; c++) {
        u16 co, n; memcpy(&co, p2 + 2 * c, 2); memcpy(&n, p2 + co, 2);
        for (int k = 0; k < n; k++) {
            u8 *r = p2 + co + 2 + 8 * k;
            u16 a; memcpy(&a, r, 2);
            if (a == 1) { a = 0x2001; memcpy(r, &a, 2); }
        }
    }
    /* the back plane: column offsets, then one list per column */
    u32 q = (w / 8) * 2u;
    for (int c = 0; c < w / 8; c++) {
        u16 qq = (u16)q, n = 0;
        memcpy(p3 + 2 * c, &qq, 2);
        u8 *l = p3 + q + 2;
        if (c * 8 >= BACK_X0 - 8) {
            u16 co, m; memcpy(&co, p2 + 2 * c, 2); memcpy(&m, p2 + co, 2);
            if (l + 8 * (m + 3) > port_funga_whm + sizeof port_funga_whm) { port_printf("funga: WHM copy too small\n"); return; }
            if (c * 8 == BACK_X0 - 8) {   /* a wall that pushes right at x BACK_X0 - 1 */
                static const s16 wall[4] = {0x0008, -100, -300, 0x0007};
                memcpy(l + 8 * n++, wall, 8);
            }
            for (int k = 0; k < m; k++) {
                u8 *r = l + 8 * n++;
                memcpy(r, p2 + co + 2 + 8 * k, 8);
                u16 a; memcpy(&a, r, 2);
                if (a == 0x2001 || a == 1) { a = 0x1001; memcpy(r, &a, 2); }
            }
            if (c * 8 >= STAND_X0 && c * 8 < STAND_X1) {
                static const s16 top[4] = {0x1001, STAND_Y, 0, 0x0080};
                memcpy(l + 8 * n++, top, 8);
            }
        }
        memcpy(p3 + q, &n, 2);
        q += 2 + 8u * n;
    }
    WHM_DATA[0] = (char *)d;
    port_printf("funga: back lane WHM installed (%u bytes)\n", po[3] + q);
}

/* Up+Cross on the front floors in front of the stand (bit 0x2000, which would start step 0x11, a walk-level shift):
   instead the lane jump (step 0x12, velY -0x570, 2.5 depth a tick) with its depth direction turned to +90, so the arc
   goes up and back and lands on the stand's top (y -232) in the back plane */
int port_funga_lane_flip;
int port_funga_up_lane(void *po)
{
    extern char *WHM_DATA[];
    u8 *o = po;
    port_funga_lane_flip = 0;
    if (!(port_cut_content & 64) || WHM_DATA[0] != (char *)port_funga_whm) return 0;
    s16 px = (*(s16 **)(o + 0x40))[1], pz = (*(s16 **)(o + 0x44))[1];
    if (px < STAND_X0 || px >= STAND_X1 || pz != 180) return 0;
    port_funga_lane_flip = 1;
    return 1;
}
