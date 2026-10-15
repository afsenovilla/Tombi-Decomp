// FUNC 800ec7ac 116 I0NFO3
// MATCHING 800ec7ac 116
#include "TOBJ.H"
extern unsigned short D_800A4670;
extern unsigned char *D_800EECF8;

void func_800EC7AC(TObj *o, short x, short y)
{
    if (!(o->active & 4)) {
        unsigned char c = D_800EECF8[(short)((x >> 3) + ((y + 0x540) >> 3) * D_800A4670)];
        o->b68 = 0;
        if (c & 0x80) {
            o->b68 = 1;
        }
    }
}
