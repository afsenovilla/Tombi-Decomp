// FUNC 8011cc18 352 X006
// MATCHING 8011cc18 352
#include "gvars.h"
#include "TOBJ.H"

typedef struct { short x, y, w, h; } RECT;
typedef struct { char pad[0xd0]; unsigned short wd0; } G;
typedef struct { short d, v; } FR;

extern FR D_80120268[];
extern unsigned short *D_80122E18;
extern void drawSpriteFrameParts(TObj *, short, short, unsigned short);
extern void AddDrawMode(int, int);
extern void addTilePrim(RECT *, int, int, int);

void func_8011CC18(TObj *a, int b, short x, short y)
{
    G *g = (*(G **)&GVARS.p4c);
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
    drawSpriteFrameParts(a, x, y, v);
    AddDrawMode(7, 1);
    r.x = x - 0x40;
    r.y = y - 4;
    r.w = g->wd0;
    r.h = 8;
    addTilePrim(&r, 0xff, 0xff, 0);
    r.x = x - 0x40;
    r.y = y - 4;
    r.w = 0x80;
    r.h = 8;
    addTilePrim(&r, 0x20, 0x20, 0x20);
}
