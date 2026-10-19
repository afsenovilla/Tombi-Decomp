// FUNC 800ebedc 424 I0NFO
// MATCHING 800ebedc 424
#include "TOBJ.H"
extern unsigned short D_800A4678[];
extern short D_800B1320[];

void func_800EBEDC(TObj *o)
{
    int xt, yt; short x;
    short y, r, k, t;
    unsigned int d;
    d = o->d34;
    yt = o->d30;
    d >>= 5;
    xt = (short)(yt >> 5);
    if (xt > 0) xt--;
    x = xt;
    do { } while (0);
    yt = (short)d;
    k = 0;
    if (yt > 0) yt--;
    y = yt;
    r = 0;
    do {
        t = x + (y + r) * D_800A4678[0];
        D_800B1320[k++] = t;
        D_800B1320[k++] = t + 1;
        D_800B1320[k++] = t + 2;
        D_800B1320[k++] = t + 3;
        D_800B1320[k++] = t + 4;
        D_800B1320[k++] = t + 5;
        D_800B1320[k++] = t + 6;
        D_800B1320[k++] = t + 7;
        D_800B1320[k++] = t + 8;
        D_800B1320[k++] = t + 9;
        D_800B1320[k++] = t + 10;
        D_800B1320[k++] = t + 11;
        r++;
    } while (r < 10);
}
