// FUNC 800ec75c 304 I0NFO
#include "TOBJ.H"

void func_800EC75C(TObj *o, int x, int y, int flags)
{
    short nx = x, ny = y;
    int fx = x & 7;
    int fy = y & 7;
    int qx = fx >= 4;
    int q = qx;
    if (fy >= 4) q = qx | 2;
    switch (q) {
    case 0:
        if (flags & 2) {
            nx = x - 1 - fx;
            o->a.p.whole = o->a.p.whole - 1 - fx;
        } else {
            ny = y - 1 - fy;
            o->y.p.whole = o->y.p.whole - 1 - fy;
        }
        break;
    case 1:
        if (flags & 2) {
            nx = x + 8 - fx;
            o->a.p.whole = o->a.p.whole + 8 - fx;
        } else {
            ny = y - 1 - fy;
            o->y.p.whole = o->y.p.whole - 1 - fy;
        }
        break;
    case 2:
        if (flags & 2) {
            nx = x - 1 - fx;
            o->a.p.whole = o->a.p.whole - 1 - fx;
        } else {
            ny = y + 8 - fy;
            o->y.p.whole = o->y.p.whole + 8 - fy;
        }
        break;
    case 3:
        if (flags & 2) {
            nx = x + 8 - fx;
            o->a.p.whole = o->a.p.whole + 8 - fx;
        } else {
            ny = y + 8 - fy;
            o->y.p.whole = o->y.p.whole + 8 - fy;
        }
        break;
    }
    *(int *)0x1F800070 = nx;
    *(int *)0x1F800074 = ny;
}
