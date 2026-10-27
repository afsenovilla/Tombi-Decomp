// FUNC 800e8e44 404 O0PTSUB0
// MATCHING 800e8e44 404
typedef struct { unsigned addr: 24; unsigned len: 8; } P_TAG;
typedef struct {
    unsigned addr: 24;
    unsigned len: 8;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    short w, h;
} TILE;
typedef struct { short x, y, w, h; } RECT;
#define setaddr(p, _addr) (((P_TAG *)(p))->addr = (unsigned long)(_addr))
#define getaddr(p) (unsigned long)(((P_TAG *)(p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)
extern TILE D_1F800000;
extern char *NEXT_PRIM;
extern unsigned long *CURRENT_OT;
extern RECT OPTIONS_CURSOR_RECTS[];
extern RECT OPTIONS_CURSOR_RECTS_NO_ACTUATOR[];
extern short OPTIONS_PAD_HAS_ACTUATOR;

void optionsMenuDrawCursor(int sel)
{
    TILE *p;
    TILE *sp = (TILE *)0x1F800000;
    D_1F800000.len = 3;
    D_1F800000.code = 0x60;
    D_1F800000.g0 = 0xFF;
    D_1F800000.code |= 2;
    D_1F800000.r0 = 0;
    D_1F800000.b0 = 0;
    if (sel >= 4 && OPTIONS_PAD_HAS_ACTUATOR == 0) {
        D_1F800000.x0 = OPTIONS_CURSOR_RECTS_NO_ACTUATOR[sel - 4].x;
        D_1F800000.y0 = OPTIONS_CURSOR_RECTS_NO_ACTUATOR[sel - 4].y;
        D_1F800000.w = OPTIONS_CURSOR_RECTS_NO_ACTUATOR[sel - 4].w;
        D_1F800000.h = OPTIONS_CURSOR_RECTS_NO_ACTUATOR[sel - 4].h;
    } else {
        sp->x0 = OPTIONS_CURSOR_RECTS[sel].x;
        sp->y0 = OPTIONS_CURSOR_RECTS[sel].y;
        sp->w = OPTIONS_CURSOR_RECTS[sel].w;
        sp->h = OPTIONS_CURSOR_RECTS[sel].h;
    }
    p = (TILE *)NEXT_PRIM;
    *p = *sp;
    addPrim(CURRENT_OT + 6, p);
    NEXT_PRIM += sizeof(TILE);
}
