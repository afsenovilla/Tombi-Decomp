// VERSION SCUS_94236 dspsub2 800EAD70 876
#if defined(VERSION_SCUS_94236)
// USA version of PAL d0spsub2 800eb25c (memCardDrawMessage.c)
typedef struct { unsigned addr: 24; unsigned len: 8; } P_TAG;
typedef struct {
    unsigned addr: 24;
    unsigned len: 8;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    short w, h;
} TILE;
typedef struct { short x, y, w, h; } R;
typedef struct { short x, y; } XY;
typedef struct { short x0, x1, x2, x3; short a, b, c, d; short m; } L;
#define setaddr(p, _addr) (((P_TAG *)(p))->addr = (unsigned long)(_addr))
#define getaddr(p) (unsigned long)(((P_TAG *)(p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)
extern TILE *NEXT_PRIM;
extern unsigned long *CURRENT_OT;
extern R MEMCARD_MESSAGE_FRAMES[];
extern XY MEMCARD_YESNO_TILE_RECTS[], MEMCARD_YESNO_TILE_RECTS_2[], MEMCARD_YESNO_TILE_RECTS_3[];
extern int MEMCARD_YESNO_CURSOR;
extern int MEMCARD_MESSAGE_STRINGS[];
extern int MEMCARD_FOOTER_STRING;
typedef struct { short x, y, w, attr; int img, pC, str; void *p14; } Text;
extern Text MEMCARD_DRAW_PARAMS;
extern char MEMCARD_FONT[];
extern unsigned short GetClut(int x, int y);
extern void subScreenDrawString(int a, int b);
extern void drawBalloonFrame(L *l, int c, int tp);

void memCardDrawMessage(int n)
{
    TILE *T = (TILE *)0x1F800000;
    TILE *p;
    L l;
    R *e = &MEMCARD_MESSAGE_FRAMES[n];
    short x, y;
    x = e->x - (short)(e->w / 2 - 0xA0);
    y = e->y - (short)(e->h / 2 - 0x70);
    MEMCARD_DRAW_PARAMS.w = 4;
    MEMCARD_DRAW_PARAMS.x = x + 8;
    MEMCARD_DRAW_PARAMS.y = y + 8;
    MEMCARD_DRAW_PARAMS.attr = GetClut(0x150, 0x1F0);
    MEMCARD_DRAW_PARAMS.str = MEMCARD_MESSAGE_STRINGS[n];
    MEMCARD_DRAW_PARAMS.p14 = MEMCARD_FONT;
    subScreenDrawString(1, 1);
    if (n == 7 || n == 12 || n == 16) {
        T->len = 3;
        T->code = 0x60;
        T->g0 = 0xFF;
        T->r0 = 0;
        T->b0 = 0;
        T->code &= ~2;
        switch (n) {
        case 7:
            T->x0 = x + MEMCARD_YESNO_TILE_RECTS[MEMCARD_YESNO_CURSOR].x;
            T->y0 = y + MEMCARD_YESNO_TILE_RECTS[MEMCARD_YESNO_CURSOR].y;
            break;
        case 12:
            T->x0 = x + MEMCARD_YESNO_TILE_RECTS_2[MEMCARD_YESNO_CURSOR].x;
            T->y0 = y + MEMCARD_YESNO_TILE_RECTS_2[MEMCARD_YESNO_CURSOR].y;
            break;
        case 16:
            T->x0 = x + MEMCARD_YESNO_TILE_RECTS_3[MEMCARD_YESNO_CURSOR].x;
            T->y0 = y + MEMCARD_YESNO_TILE_RECTS_3[MEMCARD_YESNO_CURSOR].y;
            break;
        }
        T->w = 0x18;
        T->h = 0xC;
        p = NEXT_PRIM;
        *p = *T;
        addPrim(CURRENT_OT + 1, p);
        NEXT_PRIM++;
    }
    l.m = -1;
    l.a = x;
    l.b = y;
    l.c = e->w;
    l.d = e->h;
    drawBalloonFrame(&l, 0, 0x14);
    MEMCARD_DRAW_PARAMS.x = 0x28;
    MEMCARD_DRAW_PARAMS.y = 0xC4;
    MEMCARD_DRAW_PARAMS.w = 4;
    MEMCARD_DRAW_PARAMS.attr = GetClut(0x150, 0x1F0);
    MEMCARD_DRAW_PARAMS.p14 = MEMCARD_FONT;
    MEMCARD_DRAW_PARAMS.str = MEMCARD_FOOTER_STRING;
    subScreenDrawString(1, 1);
}
#endif
