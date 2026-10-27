// FUNC 800ecb40 124 I0NFO
// MATCHING 800ecb40 124
#include "TOBJ.H"
extern unsigned short D_800A4674;
extern unsigned char *OVERHEAD_MAP_TILES;

void overheadCheckTileTrigger(TObj *o, short x, short y)
{
    if (!(o->active & 4)) {
        int r = ((y - 0x30) >> 3) * D_800A4674;
        o->b68 = 0;
        if (OVERHEAD_MAP_TILES[(short)(((x + 0x80) >> 3) + r)] & 0x80) {
            o->b68 = 1;
        }
    }
}
