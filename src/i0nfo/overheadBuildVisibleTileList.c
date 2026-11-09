// FUNC 800ebedc 424 I0NFO
// MATCHING 800ebedc 424
#include "TOBJ.H"
extern unsigned short D_800A4678[];
extern short CAT3_OBJECT_LIST_BUF[];

void overheadBuildVisibleTileList(TObj *o)
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
        CAT3_OBJECT_LIST_BUF[k++] = t;
        CAT3_OBJECT_LIST_BUF[k++] = t + 1;
        CAT3_OBJECT_LIST_BUF[k++] = t + 2;
        CAT3_OBJECT_LIST_BUF[k++] = t + 3;
        CAT3_OBJECT_LIST_BUF[k++] = t + 4;
        CAT3_OBJECT_LIST_BUF[k++] = t + 5;
        CAT3_OBJECT_LIST_BUF[k++] = t + 6;
        CAT3_OBJECT_LIST_BUF[k++] = t + 7;
        CAT3_OBJECT_LIST_BUF[k++] = t + 8;
        CAT3_OBJECT_LIST_BUF[k++] = t + 9;
        CAT3_OBJECT_LIST_BUF[k++] = t + 10;
        CAT3_OBJECT_LIST_BUF[k++] = t + 11;
        r++;
    } while (r < 10);
}
