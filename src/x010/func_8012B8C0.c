// FUNC 8012b8c0 388 X010
// MATCHING 8012b8c0 388
#include "spad.h"
#include "gcd.h"
#include "TOBJ.H"

typedef struct { short x, y, w, h; } RECT;

extern int MoveImage(RECT *r, int x, int y);

void func_8012B8C0(TObj *o)
{
    RECT r;

    switch (o->b6b) {
    case 0:
        if (GCD.bcde3 == 0xff) {
            r.x = 0x120; r.y = 0x1e0; r.w = 0x10; r.h = 4;
            MoveImage(&r, 0x120, 0x1e8);
            o->wb4 = 0;
            o->b6b++;
        }
        break;
    case 1:
        r.x = 0x120; r.y = 0x1e4; r.w = 0x10; r.h = 4;
        MoveImage(&r, 0x120, 0x1e0);
        if ((unsigned short)o->wb4 >= 0xc) o->b6b = 5;
        else o->b6b++;
        break;
    case 2:
        if (!(SPAD.s1f8 & 7)) o->b6b++;
        break;
    case 3:
        r.x = 0x120; r.y = 0x1e8; r.w = 0x10; r.h = 4;
        MoveImage(&r, 0x120, 0x1e0);
        o->wb4++;
        o->b6b++;
        break;
    case 4:
        if (!(SPAD.s1f8 & 7)) o->b6b = 1;
        break;
    case 5:
        break;
    }
}
