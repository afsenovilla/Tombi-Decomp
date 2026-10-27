// FUNC 800eac90 684 D0SPSUB2
// MATCHING 800eac90 684
typedef struct { unsigned addr: 24; unsigned len: 8; } P_TAG;
typedef struct {
    unsigned addr: 24;
    unsigned len: 8;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    short w, h;
} SPRT;
typedef struct { short x, y, u, v, w, h, cx, cy; } Part;
typedef struct { short x, y; } Pos;
#define setaddr(p, _addr) (((P_TAG *)(p))->addr = (unsigned long)(_addr))
#define getaddr(p) (unsigned long)(((P_TAG *)(p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)
extern SPRT *NEXT_PRIM;
extern unsigned long *CURRENT_OT;
extern Part MEMCARD_FILE_ICON_PARTS[], MEMCARD_EMPTY_SLOT_PARTS[];
extern Pos MEMCARD_DRAW_PARAMS;
extern short MEMCARD_DRAW_Y, MEMCARD_DRAW_CLUT;
extern short MEMCARD_DRAW_TPAGE[];
extern int MEMCARD_DRAW_STR, MEMCARD_EMPTY_SLOT_STRING;
extern void *MEMCARD_DRAW_FONT;
extern char MEMCARD_FONT[];
extern unsigned short GetClut(int x, int y);
extern void AddDrawMode(int a, int b);
extern void memCardDrawString(int a, int b);

void memCardDrawFileIcon(int big, int pri, int pal, int lit)
{
    SPRT *S = (SPRT *)0x1F800000;
    SPRT *p;
    Part *s;
    if (big == 0) {
        s = MEMCARD_FILE_ICON_PARTS;
    } else {
        short x, y;
        MEMCARD_DRAW_TPAGE[0] = 4;
        x = MEMCARD_DRAW_PARAMS.x;
        y = MEMCARD_DRAW_Y;
        MEMCARD_DRAW_PARAMS.x = x + 0x10;
        MEMCARD_DRAW_Y = y + 0x10;
        MEMCARD_DRAW_CLUT = GetClut(0x150, 0x1F0);
        s = MEMCARD_EMPTY_SLOT_PARTS;
        MEMCARD_DRAW_FONT = MEMCARD_FONT;
        MEMCARD_DRAW_STR = MEMCARD_EMPTY_SLOT_STRING;
        memCardDrawString(6, 0);
        MEMCARD_DRAW_PARAMS.x = x;
        MEMCARD_DRAW_Y = y;
    }
    do {
        S->len = 4;
        S->code = 0x64;
        if (lit) {
            S->r0 = 0x80;
            S->g0 = 0x80;
            S->b0 = 0x80;
        } else {
            S->r0 = 0x50;
            S->g0 = 0x50;
            S->b0 = 0x50;
        }
        S->code &= ~2;
        S->x0 = MEMCARD_DRAW_PARAMS.x + s->x;
        S->y0 = MEMCARD_DRAW_PARAMS.y + s->y;
        S->u0 = s->u;
        S->v0 = s->v;
        S->w = s->w;
        S->h = s->h;
        if (s >= MEMCARD_EMPTY_SLOT_PARTS) S->clut = GetClut(0x140, pal + 0x1F0);
        else S->clut = GetClut(s->cx, s->cy);
        s++;
        p = NEXT_PRIM;
        *p = *S;
        addPrim(CURRENT_OT + pri + 4, p);
        NEXT_PRIM++;
    } while (s->x != -1);
    MEMCARD_DRAW_TPAGE[0] = 1;
    AddDrawMode(1, pri + 4);
}
