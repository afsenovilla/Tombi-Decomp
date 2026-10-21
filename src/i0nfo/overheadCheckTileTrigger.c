// FUNC 800ecb40 124 I0NFO
// MATCHING 800ecb40 124
#include "TOBJ.H"
extern unsigned short D_800A4674;
extern unsigned char *D_800EE8C8;

void overheadCheckTileTrigger(TObj *o, short x, short y)
{
    if (!(o->active & 4)) {
        int r = ((y - 0x30) >> 3) * D_800A4674;
        o->b68 = 0;
        if (D_800EE8C8[(short)(((x + 0x80) >> 3) + r)] & 0x80) {
            o->b68 = 1;
        }
    }
}
