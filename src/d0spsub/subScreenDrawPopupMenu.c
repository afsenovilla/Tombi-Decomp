// FUNC 800ebf34 1116 D0SPSUB
// MATCHING 800ebf34 1116
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
extern TILE *D_1F800164;
extern unsigned long *D_1F8001E0;
extern Text D_8011113C;
extern short D_8010A058, D_8010A05C, D_8010A060, D_8010A064;
extern short D_8010A068[];
extern short D_8010A088[];
extern int D_80108FCC[];
extern int *D_80109168[];
extern char D_800FFD98[];
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
    for (i = 0; i < D_8010A05C; i++) {
        D_8011113C.x = 0xC;
        D_8011113C.y = (short)(i * 16 + 0x78) - D_8010A05C * 8 + 6;
        D_8011113C.w = 2;
        D_8011113C.attr = 0x7C15;
        D_8011113C.str = D_80108FCC[D_8010A068[i]];
        D_8011113C.p14 = D_800FFD98;
        subScreenDrawString(1, 1);
    }
    lp = &l;
    p = D_1F800164;
    T->len = 3;
    T->code = 0x60;
    T->g0 = 0xFF;
    T->x0 = 0xC;
    T->w = 0x90;
    T->r0 = 0;
    T->b0 = 0;
    T->h = 0x10;
    T->code &= ~2;
    T->y0 = (short)(D_8010A058 * 16 + 0x78) - D_8010A05C * 8 + 6;
    *p = *T;
    addPrim(D_1F8001E0 + 2, p);
    D_1F800164++;
    l.m = -1;
    l.a = 8;
    l.b = 0x78 - D_8010A05C * 8;
    l.c = 0x98;
    l.d = D_8010A05C * 16 + 0xC;
    drawBalloonFrame(lp, 0, 0x14);
    if (D_8010A060 >= 0) {
        for (i = 0; i < D_8010A064; i++) {
            D_8011113C.x = 0xA4;
            D_8011113C.y = (short)(i * 16 + 0x78) - D_8010A064 * 8 + 6;
            D_8011113C.w = 2;
            D_8011113C.attr = 0x7C15;
            D_8011113C.str = D_80109168[D_8010A068[D_8010A058]][D_8010A088[i]];
            D_8011113C.p14 = D_800FFD98;
            subScreenDrawString(1, 1);
        }
        p = D_1F800164;
        T->len = 3;
        T->code = 0x60;
        T->g0 = 0xFF;
        T->x0 = 0xA4;
        T->w = 0x90;
        T->r0 = 0;
        T->b0 = 0;
        T->h = 0x10;
        T->code &= ~2;
        T->y0 = (short)(D_8010A060 * 16 + 0x78) - D_8010A064 * 8 + 6;
        *p = *T;
        addPrim(D_1F8001E0 + 2, p);
        D_1F800164++;
        lp->m = -1;
        lp->a = 0xA0;
        lp->b = 0x78 - D_8010A064 * 8;
        lp->c = 0x98;
        lp->d = D_8010A064 * 16 + 0xC;
        drawBalloonFrame(lp, 0, 0x14);
    }
}
