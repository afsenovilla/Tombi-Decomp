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
extern Text D_80100B70;
extern short D_80100918, D_8010091C, D_80100920, D_80100924;
extern short D_80100928[];
extern short D_80100948[];
extern int D_800FFB30[];
extern int *D_800FFCCC[];
extern char D_800F68FC[];
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
    for (i = 0; i < D_8010091C; i++) {
        D_80100B70.x = 0xC;
        D_80100B70.y = (short)(i * 16 + 0x78) - D_8010091C * 8 + 6;
        D_80100B70.w = 2;
        D_80100B70.attr = 0x7C15;
        D_80100B70.str = D_800FFB30[D_80100928[i]];
        D_80100B70.p14 = D_800F68FC;
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
    T->y0 = (short)(D_80100918 * 16 + 0x78) - D_8010091C * 8 + 6;
    *p = *T;
    addPrim(CURRENT_OT + 2, p);
    NEXT_PRIM++;
    l.m = -1;
    l.a = 8;
    l.b = 0x78 - D_8010091C * 8;
    l.c = 0x98;
    l.d = D_8010091C * 16 + 0xC;
    drawBalloonFrame(lp, 0, 0x14);
    if (D_80100920 >= 0) {
        for (i = 0; i < D_80100924; i++) {
            D_80100B70.x = 0xA4;
            D_80100B70.y = (short)(i * 16 + 0x78) - D_80100924 * 8 + 6;
            D_80100B70.w = 2;
            D_80100B70.attr = 0x7C15;
            D_80100B70.str = D_800FFCCC[D_80100928[D_80100918]][D_80100948[i]];
            D_80100B70.p14 = D_800F68FC;
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
        T->y0 = (short)(D_80100920 * 16 + 0x78) - D_80100924 * 8 + 6;
        *p = *T;
        addPrim(CURRENT_OT + 2, p);
        NEXT_PRIM++;
        lp->m = -1;
        lp->a = 0xA0;
        lp->b = 0x78 - D_80100924 * 8;
        lp->c = 0x98;
        lp->d = D_80100924 * 16 + 0xC;
        drawBalloonFrame(lp, 0, 0x14);
    }
}
