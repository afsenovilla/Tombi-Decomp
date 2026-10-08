// FUNC 8011d804 192 X004
// MATCHING 8011d804 192
#include "TOBJ.H"
typedef struct { short x, y, w, h; } RECT;
typedef struct { unsigned short c[80]; } CL;

extern unsigned char D_80130EFC[], D_80130F04[];
extern CL D_80130D1C[];
extern int LoadImage(RECT *, void *);

void func_8011D804(TObj *o)
{
    RECT r;
    int i;

    if (--o->w22 != -1) return;
    if (++o->b6b >= 7) o->b6b = 0;
    i = o->b6b;
    o->w22 = D_80130EFC[i];
    r.x = 0xd0;
    r.y = 0x1e0;
    r.w = 0x10;
    r.h = 5;
    LoadImage(&r, &D_80130D1C[D_80130F04[i]]);
}
