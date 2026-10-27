// FUNC 800ebe8c 1116 D0SPSUB3
// MATCHING 800ebe8c 1116
typedef struct { unsigned addr: 24; unsigned len: 8; } P_TAG;
typedef struct {
    unsigned addr: 24;
    unsigned len: 8;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    short w, h;
} TILE;
typedef struct { short x0, x1, x2, x3; short a, b, c, d; short m; short pad[3]; } L;
typedef struct { short x, y, w, attr; int img, pC, str; void *p14; } Text;
#define setaddr(p, _addr) (((P_TAG *)(p))->addr = (unsigned long)(_addr))
#define getaddr(p) (unsigned long)(((P_TAG *)(p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)
extern TILE *NEXT_PRIM;
extern unsigned long *CURRENT_OT;
extern Text SUBSCREEN_DRAW_PARAMS;
extern short SUBSCREEN_POPUP_CURSOR, SUBSCREEN_POPUP_COUNT, SUBSCREEN_POPUP_SUB_CURSOR, SUBSCREEN_POPUP_SUB_COUNT;
extern short SUBSCREEN_POPUP_ENTRIES[];
extern short SUBSCREEN_POPUP_SUB_ENTRIES[];
extern int SUBSCREEN_POPUP_STRINGS[];
extern int *SUBSCREEN_POPUP_SUB_STRINGS[];
extern char SUBSCREEN_FONT[];
extern void subScreenDrawString(int pri, int multiline);
extern void drawBalloonFrame(L *l, int c, int tp);

void subScreenDrawPopupMenu(void)
{
    TILE *T = (TILE *)0x1F800014;
    TILE *p;
    char env[0x60];
    L l;
    L *lp;
    int i;
    for (i = 0; i < SUBSCREEN_POPUP_COUNT; i++) {
        SUBSCREEN_DRAW_PARAMS.x = 0xC;
        SUBSCREEN_DRAW_PARAMS.y = (short)(i * 16 + 0x78) - SUBSCREEN_POPUP_COUNT * 8 + 6;
        SUBSCREEN_DRAW_PARAMS.w = 2;
        SUBSCREEN_DRAW_PARAMS.attr = 0x7C15;
        SUBSCREEN_DRAW_PARAMS.str = SUBSCREEN_POPUP_STRINGS[SUBSCREEN_POPUP_ENTRIES[i]];
        SUBSCREEN_DRAW_PARAMS.p14 = SUBSCREEN_FONT;
        subScreenDrawString(1, 1);
    }
    lp = &l;
    p = NEXT_PRIM;
    T->len = 3;
    T->code = 0x60;
    T->g0 = 0xFF;
    T->x0 = 0xC;
    T->w = 0x90;
    T->r0 = 0;
    T->b0 = 0;
    T->h = 0x10;
    T->code &= ~2;
    T->y0 = (short)(SUBSCREEN_POPUP_CURSOR * 16 + 0x78) - SUBSCREEN_POPUP_COUNT * 8 + 6;
    *p = *T;
    addPrim(CURRENT_OT + 2, p);
    NEXT_PRIM++;
    l.m = -1;
    l.a = 8;
    l.b = 0x78 - SUBSCREEN_POPUP_COUNT * 8;
    l.c = 0x98;
    l.d = SUBSCREEN_POPUP_COUNT * 16 + 0xC;
    drawBalloonFrame(lp, 0, 0x14);
    if (SUBSCREEN_POPUP_SUB_CURSOR >= 0) {
        for (i = 0; i < SUBSCREEN_POPUP_SUB_COUNT; i++) {
            SUBSCREEN_DRAW_PARAMS.x = 0xA4;
            SUBSCREEN_DRAW_PARAMS.y = (short)(i * 16 + 0x78) - SUBSCREEN_POPUP_SUB_COUNT * 8 + 6;
            SUBSCREEN_DRAW_PARAMS.w = 2;
            SUBSCREEN_DRAW_PARAMS.attr = 0x7C15;
            SUBSCREEN_DRAW_PARAMS.str = SUBSCREEN_POPUP_SUB_STRINGS[SUBSCREEN_POPUP_ENTRIES[SUBSCREEN_POPUP_CURSOR]][SUBSCREEN_POPUP_SUB_ENTRIES[i]];
            SUBSCREEN_DRAW_PARAMS.p14 = SUBSCREEN_FONT;
            subScreenDrawString(1, 1);
        }
        p = NEXT_PRIM;
        T->len = 3;
        T->code = 0x60;
        T->g0 = 0xFF;
        T->x0 = 0xA4;
        T->w = 0x90;
        T->r0 = 0;
        T->b0 = 0;
        T->h = 0x10;
        T->code &= ~2;
        T->y0 = (short)(SUBSCREEN_POPUP_SUB_CURSOR * 16 + 0x78) - SUBSCREEN_POPUP_SUB_COUNT * 8 + 6;
        *p = *T;
        addPrim(CURRENT_OT + 2, p);
        NEXT_PRIM++;
        lp->m = -1;
        lp->a = 0xA0;
        lp->b = 0x78 - SUBSCREEN_POPUP_SUB_COUNT * 8;
        lp->c = 0x98;
        lp->d = SUBSCREEN_POPUP_SUB_COUNT * 16 + 0xC;
        drawBalloonFrame(lp, 0, 0x14);
    }
}
