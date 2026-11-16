/* The NTSC picture (--picture ntsc, the default with --hz 60; docs/PORT.md "NTSC picture"): the disc side.  The PAL
   disc moved some object placements down for its 256-line screen (APD placement records, load-record kinds
   0x80..0x8F: mostly backdrop / scenery objects 24 lines lower, a few AREA00 ones 44 / 52); the USA disc has them
   where its 224-line screen wants them.  As the game spawns an object from a record (spawnObjectFromRecord,
   port/fixes.txt) a record still holding the PAL words of one the USA disc has higher gets the USA y.  The table
   (tools/ntsc_apd.py) carries the area and all eight PAL words of each such record: only that exact record is
   patched (once: it no longer matches afterwards), whatever block or slot it sits in. */
#include <string.h>

extern int port_ntsc;
extern unsigned short GAME[];           /* GAME.selectedArea */
extern int port_printf(const char *, ...);
extern int port_verbose;

static const struct { unsigned char area, slot; unsigned short rec; short pal[8]; short us_y; } ntsc_apd[] = {
#include "ntsc_apd.inc"
};

/* The other way round, a slip of the PAL disc itself (always on with the PAL picture; docs/PORT.md "The gears room's
   wheels"): AREA16 section 3 (the gears room before the repair) kept the USA disc's y for its three layer-3 type 0x46
   pieces (the blue cog wheels drawn again over the player, and a third one not shown), while section 6 (the same
   room, the same picture, after the repair) and section 5 have theirs 24 lines lower like every other room.  So
   the PAL game draws section 3's wheels 24 rows above the picture's own: doubled rims and a cut where the pieces
   end.  The three records (all eight PAL words each) get section 6's y as the game spawns from them. */
static const struct { unsigned char area; short pal[8]; short y; } pal_apd_fix[] = {
    { 16, { 0x0303, 0x0046, 0x0000, 0x0048, 0x00d0, 0, 0, 0 }, 0x00e8 },
    { 16, { 0x0303, 0x0146, 0x0001, 0x011c, 0x00d0, 0, 0, 0 }, 0x00e8 },
    { 16, { 0x0303, 0x0246, 0x0002, 0x0096, 0x0096, 0, 0, 0 }, 0x00ae },
};

void port_ntsc_apd_rec(void *rec) {
    short *r = (short *)rec;
    unsigned i;
    if (!r) return;
    if (!port_ntsc) {
        for (i = 0; i < sizeof pal_apd_fix / sizeof pal_apd_fix[0]; i++)
            if (pal_apd_fix[i].area == GAME[0] && !memcmp(r, pal_apd_fix[i].pal, 16)) {
                r[4] = pal_apd_fix[i].y;
                if (port_verbose > 1) port_printf("[port] area %d placement record (%d %d %d x %d) y %d -> %d (PAL slip)\n",
                                                  GAME[0], r[0], r[1], r[2], r[3], pal_apd_fix[i].pal[4], r[4]);
                return;
            }
        return;
    }
    for (i = 0; i < sizeof ntsc_apd / sizeof ntsc_apd[0]; i++)
        if (ntsc_apd[i].area == GAME[0] && r[4] == ntsc_apd[i].pal[4] && !memcmp(r, ntsc_apd[i].pal, 16)) {
            r[4] = ntsc_apd[i].us_y;
            if (port_verbose > 1) port_printf("[port] NTSC picture: area %d placement record (%d %d %d x %d) y %d -> %d (USA)\n",
                                              GAME[0], r[0], r[1], r[2], r[3], ntsc_apd[i].pal[4], r[4]);
            return;
        }
}

/* The sub-screens (D0SPSUB Select / D0SPSUB3 Triangle item screen): the PAL overlays list 10 items / events a page,
   rows 16 lines apart from y 72 (80), so the 10th row sits at 216 (224) and the page footer / arrows at 248: the
   224-line picture cut the list's last row in half and hid the footer.  The USA overlays (SYSTEM/DSPSUB*.BIN) have the
   same tables with 8 rows (their slot lists end after the 8th), the footer and the arrows at 216, and the pink list
   backdrop at 112 instead of 128; the code pages by 8 (port/fixes.txt: page * PORT_NTSC(10, 8) ...).  Called from
   subScreenUpdate (port/fixes.txt) as the menu opens; each change only applies to the PAL value, so a
   second call (or another overlay's data) changes nothing.  Slots are { x, y, dw, idx }, ended by x == -1. */
void port_ntsc_subscreen(short *item, short *event, short *arrows, short *footer, short *bg) {
    short *t[2] = { item, event }, *f[2] = { arrows, footer };
    int k, i;
    if (!port_ntsc) return;
    for (k = 0; k < 2; k++)                     /* 8 rows: the 9th slot (idx 8) becomes the end marker */
        if (t[k] && t[k][8 * 4 + 3] == 8 && t[k][8 * 4] != -1 && t[k][9 * 4 + 3] == 9 && t[k][10 * 4] == -1) t[k][8 * 4] = -1;
    for (k = 0; k < 2; k++)                     /* footer line 248 -> 216 */
        for (i = 0; f[k] && i < 8 && f[k][i * 4] != -1; i++) if (f[k][i * 4 + 1] == 248) f[k][i * 4 + 1] = 216;
    if (bg && bg[0] == 80 && bg[1] == 128 && bg[3] == 0) bg[1] = 112;   /* the pink backdrop's centre */
}
