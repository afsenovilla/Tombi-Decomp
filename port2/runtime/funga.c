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
typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32; typedef short s16; typedef int s32;

extern int port_cut_content;
extern int rsin(int), rcos(int);

/* The gate as the room model draws it: the palisade's stretch across the path (x 1268..1296, z 155..205, up to y -329,
   the Funga emblem on it), 21 polygons.  At load they are copied into a block of our own and flattened in the room;
   each frame the copy is turned about the hinge (the back post, x 1282 z 205) by the gate's angle and drawn with the
   room (port_funga_blocks).  The game's own door object (a 16 x 97 x 50 block inside that stretch) is hidden. */
#define GX0 1268
#define GX1 1296
#define GZ0 155
#define GZ1 205
#define HX 1282
#define HZ 205
static u8 src_blk[4096], cur_blk[4096];
static int blk_size, gate_angle;
void port_funga_room(const u8 *, int, int);
static const int nv[4] = {3, 4, 3, 4}, cw[4] = {0, 0, 2, 3};   /* FT3 FT4 GT3 GT4: vertices, extra colour words */

static int in_gate(const s16 *v, int n)
{
    int x0 = 32767, x1 = -32768, y0 = 32767, y1 = -32768, z0 = 32767, z1 = -32768;
    for (int j = 0; j < n; j++) {
        const s16 *w = v + 4 * j;
        x0 = w[0] < x0 ? w[0] : x0; x1 = w[0] > x1 ? w[0] : x1;
        y0 = w[1] < y0 ? w[1] : y0; y1 = w[1] > y1 ? w[1] : y1;
        z0 = w[2] < z0 ? w[2] : z0; z1 = w[2] > z1 ? w[2] : z1;
    }
    return x0 >= GX0 && x1 <= GX1 && z0 >= GZ0 && z1 <= GZ1 && y1 - y0 > 2;
}

void port_funga_room(const u8 *tbl, int area, int section)
{
    static const u8 *done; static s16 *probe;
    if (!(port_cut_content & 64) || area != 10 || section != 2 || !tbl) return;
    /* once per load (the same buffer can be loaded again: then the probe polygon is whole again) */
    if (tbl == done && (!probe || (probe[0] == probe[4] && probe[1] == probe[5] && probe[2] == probe[6]))) return;
    done = tbl; probe = 0; gate_angle = 0;
    int n = tbl[0], size = 0;
    const s32 *off = (const s32 *)(tbl + 4);
    u8 *grp[4]; int gsz[4] = {0, 0, 0, 0}; static u8 tmp[4][4096];
    for (int g = 0; g < 4; g++) grp[g] = tmp[g];
    for (int s = 0; s < n; s++) {
        u8 *p = (u8 *)tbl + off[s];
        for (int g = 0; g < 4; g++) {
            u32 c; memcpy(&c, p, 4); p += 4;
            for (u32 k = 0; k < c; k++) {
                int len = 8 + 4 * cw[g] + 8 * nv[g];
                s16 *v = (s16 *)(p + 8 + 4 * cw[g]);
                if (in_gate(v, nv[g]) && gsz[g] + len <= 4000) {
                    memcpy(grp[g] + gsz[g], p, len); gsz[g] += len;
                    if (!probe) probe = v;
                    for (int j = 1; j < nv[g]; j++) memcpy(v + 4 * j, v, 6);   /* all corners on the first: not drawn */
                }
                p += len;
            }
        }
    }
    u8 *d = src_blk;
    for (int g = 0; g < 4; g++) {
        int len = 8 + 4 * cw[g] + 8 * nv[g];
        u32 c = gsz[g] / len;
        if (size + 4 + gsz[g] + 16 > (int)sizeof src_blk) c = 0;
        memcpy(d, &c, 4); d += 4; memcpy(d, grp[g], c * len); d += c * len; size += 4 + c * len;
    }
    memset(d, 0, 16); size += 16;
    blk_size = size;
    memcpy(cur_blk, src_blk, size);
}

/* called from port_ws_area_model_link with the room's block list ([3] = count, u32 pointers from +4, room for 88) */
void port_funga_blocks(char *list, int area, int section)
{
    if (!(port_cut_content & 64) || area != 10 || section != 2 || !blk_size) return;
    int c = rcos(gate_angle), sn = rsin(gate_angle);
    const u8 *ps = src_blk; u8 *pd = cur_blk;
    for (int g = 0; g < 4; g++) {
        u32 cnt; memcpy(&cnt, ps, 4); ps += 4; pd += 4;
        int len = 8 + 4 * cw[g] + 8 * nv[g];
        for (u32 k = 0; k < cnt; k++) {
            const s16 *v = (const s16 *)(ps + 8 + 4 * cw[g]); s16 *w = (s16 *)(pd + 8 + 4 * cw[g]);
            for (int j = 0; j < nv[g]; j++) {       /* turn about the hinge: the stretch swings into the stage */
                int dx = v[4 * j] - HX, dz = v[4 * j + 2] - HZ;
                w[4 * j] = (s16)(HX + (dx * c - dz * sn) / 4096);
                w[4 * j + 2] = (s16)(HZ + (dx * sn + dz * c) / 4096);
            }
            ps += len; pd += len;
        }
    }
    int cnt = (u8)list[3];
    if (cnt < 88) { ((u32 *)list)[1 + cnt] = (u32)(unsigned long)cur_blk; list[3] = cnt + 1; }
}

/* the gate object, every update (after ObjCullRegister): hidden (the copy above is the gate now); opens once Tombi is
   past x 1000, a quarter turn in 32 frames */
void port_funga_gate(u8 *o, int player_x)
{
    if (!(port_cut_content & 64) || o[3] != 2) return;
    if (gate_angle < 0x400 && (gate_angle || player_x > 1000)) gate_angle += 0x20;
    *(s16 *)(o + 0x14 + 2) = -2000;                /* the object's own block out of sight */
}
