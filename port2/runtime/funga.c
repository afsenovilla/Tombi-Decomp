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

/* the gate object, every update (after ObjCullRegister): opens once Tombi is past x 1180 (about 100 units before it) */
void port_funga_gate(u8 *o, int player_x)
{
    static int bx = 0x7fff, bz;
    if (!(port_cut_content & 64) || o[3] != 2) return;
    s16 *x = (s16 *)(o + 0x12), *z = (s16 *)(o + 0x1A);
    s32 *rot = (s32 *)(o + 0x88);
    if (bx == 0x7fff || (*rot == 0 && (*x != bx || *z != bz))) { bx = *x; bz = *z; }   /* (a new spawn: its own place) */
    if (o[0x0A] == 0x10) o[0x0A] = 0x11;           /* the model drawer that applies the rotation (+0x84/+0x88/+0x8C) */
    if (*rot < 0x400 && (*rot || player_x > 1180)) *rot += 0x20;
    if (*rot > 0x400) *rot = 0x400;
    /* hinge on the back edge (local z +25): origin = hinge - R(0, 0, 25) */
    int a = *rot;
    *x = (s16)(bx - 25 * rsin(a) / 4096);
    *z = (s16)(bz + 25 - 25 * rcos(a) / 4096);
}
