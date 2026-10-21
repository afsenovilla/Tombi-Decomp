// FUNC 800ec3dc 304 I0NFO3
// MATCHING 800ec3dc 304
#include "TOBJ.H"
#define X(k) { x += k; x -= dx; o->a.p.whole = (unsigned short)o->a.p.whole k - dx; }
#define Y(k) { y += k; y -= dy; o->y.p.whole = (unsigned short)o->y.p.whole k - dy; }

void tunnelResolveCornerTile(TObj *o, short x, short y, int f)
{
    int dx = x & 7;
    int dy = y & 7;
    int r = dx >= 4;
    int q = r;
    if (dy >= 4) q = r | 2;
    switch (q) {
    case 0:
        if (!(f & 2)) Y(-1) else X(-1)
        break;
    case 1:
        if (!(f & 2)) Y(-1) else { r = dx; x += 8; x -= dx; o->a.p.whole = (unsigned short)o->a.p.whole + 8 - r; }
        break;
    case 2:
        if (!(f & 2)) Y(+8) else X(-1)
        break;
    case 3:
        if (!(f & 2)) Y(+8) else X(+8)
        break;
    }
    *(int *)0x1F800070 = x;
    *(int *)0x1F800074 = y;
}
