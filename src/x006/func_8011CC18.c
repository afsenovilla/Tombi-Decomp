// FUNC 8011cc18 352 X006
// MATCHING 8011cc18 352
#include "TOBJ.H"

typedef struct { short x, y, w, h; } RECT;
typedef struct { char pad[0xd0]; unsigned short wd0; } G;
typedef struct { short d, v; } FR;

extern G *D_8009C94C;
extern FR D_80120268[];
extern unsigned short *D_80122E18;
extern void FUN_800595f4(TObj *, short, short, unsigned short);
extern void AddDrawMode(int, int);
extern void FUN_80059728(RECT *, int, int, int);

void func_8011CC18(TObj *a, int b, short x, short y)
{
    G *g = D_8009C94C;
    RECT r;
    unsigned short v;

    if (g->wd0 == 0) {
        if (--a->w4c == 0) {
            if (++a->w4a >= 3) {
                a->w4a = 0;
            }
            a->w4c = D_80120268[a->w4a].d;
        }
        v = D_80120268[a->w4a].v;
    } else {
        v = *D_80122E18;
    }
    FUN_800595f4(a, x, y, v);
    AddDrawMode(7, 1);
    r.x = x - 0x40;
    r.y = y - 4;
    r.w = g->wd0;
    r.h = 8;
    FUN_80059728(&r, 0xff, 0xff, 0);
    r.x = x - 0x40;
    r.y = y - 4;
    r.w = 0x80;
    r.h = 8;
    FUN_80059728(&r, 0x20, 0x20, 0x20);
}
