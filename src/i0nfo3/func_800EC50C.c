// FUNC 800ec50c 672 I0NFO3
// MATCHING 800ec50c 672
#include "TOBJ.H"
extern void func_800EC3DC(TObj *o, int x, int y, int f);
extern unsigned short D_800A4670[];
extern unsigned char *D_800EECF8;

int func_800EC50C(TObj *o, short x, short y, short f)
{
    unsigned char c;
    short k;
    int ix;
    if (o->active & 4) return;
    c = D_800EECF8[(unsigned short)(ix = (x >> 3) + ((y + 0x540) >> 3) * D_800A4670[0])]; c &= 0x7F;
    if (c == 0) return 0;
    if (c == 9) {
        func_800EC3DC(o, x, y, f);
        x = *(int *)0x1F800070;
        y = *(int *)0x1F800074;
        if ((c = D_800EECF8[(unsigned short)(ix = (x >> 3) + ((y + 0x540) >> 3) * D_800A4670[0])] & 0x7F) == 0) return 0;
    }
    x &= 7;
    y &= 7;
    switch (c) {
    case 1:
        k = ~x & 7;
        if (y < k) return 0;
        if (!(f & 2)) o->y.p.whole += k - y;
        else o->a.p.whole -= y - k;
        break;
    case 2:
        k = ~x & 7;
        if (k < y) return 0;
        if (f & 2) o->a.p.whole -= y - k;
        else o->y.p.whole += k - y;
        break;
    case 3:
        if (y > x) return 0;
        if (!(f & 2)) o->y.p.whole += x - y;
        else o->a.p.whole += y - x;
        break;
    case 4:
        if (y < x) return 0;
        if (f & 2) o->a.p.whole += y - x;
        else o->y.p.whole += x - y;
        break;
    case 5:
        o->a.p.whole -= x;
        break;
    case 6:
        o->y.p.whole -= y;
        break;
    case 7:
        o->a.p.whole = (short)(o->a.p.whole + 7) - x;
        break;
    case 8:
        o->y.p.whole = (short)(o->y.p.whole + 7) - y;
        break;
    }
    return 1;
}
