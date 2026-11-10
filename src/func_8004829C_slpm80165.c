// VERSION SLPM_80165 slpm80165 8004829C 156
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo-only: adds a flat TILE primitive (code 0x60) of colour (r, g, b) over the rectangle rc to the current
// ordering table (D_1F8001E0 entry 1) from the primitive pointer D_1F800164 (Psy-Q setTile / addPrim macros).
typedef struct { unsigned addr : 24; unsigned len : 8; unsigned char r0, g0, b0, code; } PTag;
typedef struct { unsigned int tag; unsigned char r0, g0, b0, code; short x0, y0, w, h; } Tile;
typedef struct { short x, y, w, h; } Rect;
extern Tile *D_1F800164;
extern unsigned int *D_1F8001E0;

void func_8004829C(Rect *rc, int r, int g, int b)
{
    Tile *p = D_1F800164;
    ((PTag *)p)->len = 3;
    ((PTag *)p)->code = 0x60;
    p->r0 = r;
    p->g0 = g;
    p->b0 = b;
    p->x0 = rc->x;
    p->y0 = rc->y;
    p->w = rc->w;
    p->h = rc->h;
    ((PTag *)p)->addr = ((PTag *)(D_1F8001E0 + 1))->addr;
    ((PTag *)(D_1F8001E0 + 1))->addr = (unsigned int)p;
    D_1F800164 = (Tile *)((char *)D_1F800164 + 0x10);
}
#endif
