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

void port_ntsc_apd_rec(void *rec) {
    short *r = (short *)rec;
    unsigned i;
    if (!port_ntsc || !r) return;
    for (i = 0; i < sizeof ntsc_apd / sizeof ntsc_apd[0]; i++)
        if (ntsc_apd[i].area == GAME[0] && r[4] == ntsc_apd[i].pal[4] && !memcmp(r, ntsc_apd[i].pal, 16)) {
            r[4] = ntsc_apd[i].us_y;
            if (port_verbose > 1) port_printf("[port] NTSC picture: area %d placement record (%d %d %d x %d) y %d -> %d (USA)\n",
                                              GAME[0], r[0], r[1], r[2], r[3], ntsc_apd[i].pal[4], r[4]);
            return;
        }
}
