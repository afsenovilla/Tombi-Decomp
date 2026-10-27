// FUNC 800ec7ac 116 I0NFO3
// MATCHING 800ec7ac 116
#include "TOBJ.H"
extern unsigned short D_800A4670;
extern unsigned char *TUNNEL_MAP_TILES;

void tunnelCheckTileTrigger(TObj *o, short x, short y)
{
    if (!(o->active & 4)) {
        unsigned char c = TUNNEL_MAP_TILES[(short)((x >> 3) + ((y + 0x540) >> 3) * D_800A4670)];
        o->b68 = 0;
        if (c & 0x80) {
            o->b68 = 1;
        }
    }
}
