// FUNC 800ec50c 672 I0NFO3
#include "TOBJ.H"
extern void func_800EC3DC(TObj *o, int x, int y, int f);
extern unsigned short D_800A4670[];
extern unsigned char *D_800EECF8;

int func_800EC50C(TObj *o, short x, short y, short f)
{
    unsigned char c;
    short cx = x;
    short cy = y;
    short k;
    if (o->active & 4) return;
    c = D_800EECF8[(unsigned short)((x >> 3) + ((y + 0x540) >> 3) * D_800A4670[0])] & 0x7F;
    if (c == 0) return 0;
    if (c == 9) {
        func_800EC3DC(o, x, y, f);
        cy = *(int *)0x1F800074;
        cx = *(int *)0x1F800070;
        c = D_800EECF8[(unsigned short)((cx >> 3) + ((cy + 0x540) >> 3) * D_800A4670[0])] & 0x7F;
        if (c == 0) return 0;
    }
    cx &= 7;
    cy &= 7;
    switch (c) {
    case 1:
        k = ~cx & 7;
        if (cy < k) return 0;
        if (!(f & 2)) o->y.p.whole += k - cy;
        else o->a.p.whole -= cy - k;
        break;
    case 2:
        k = ~cx & 7;
        if (k < cy) return 0;
        if (f & 2) o->a.p.whole -= cy - k;
        else o->y.p.whole += k - cy;
        break;
    case 3:
        if (cx < cy) return 0;
        if (!(f & 2)) o->y.p.whole += cx - cy;
        else o->a.p.whole += cy - cx;
        break;
    case 4:
        if (cy < cx) return 0;
        if (f & 2) o->a.p.whole += cy - cx;
        else o->y.p.whole += cx - cy;
        break;
    case 5:
        o->a.p.whole -= cx;
        break;
    case 6:
        o->y.p.whole -= cy;
        break;
    case 7:
        o->a.p.whole = o->a.p.whole + 7 - cx;
        break;
    case 8:
        o->y.p.whole = o->y.p.whole + 7 - cy;
        break;
    }
    return 1;
}
