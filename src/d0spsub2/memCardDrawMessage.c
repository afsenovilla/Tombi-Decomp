// FUNC 800eb25c 960 D0SPSUB2
// MATCHING 800eb25c 960
typedef struct { unsigned addr: 24; unsigned len: 8; } P_TAG;
typedef struct {
    unsigned addr: 24;
    unsigned len: 8;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    short w, h;
} TILE;
typedef struct { short x, y, w, h; } R;
typedef struct { short x0, x1, x2, x3; short a, b, c, d; short m; } L;
#define setaddr(p, _addr) (((P_TAG *)(p))->addr = (unsigned long)(_addr))
#define getaddr(p) (unsigned long)(((P_TAG *)(p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)
extern TILE *NEXT_PRIM;
extern unsigned long *CURRENT_OT;
extern R D_800F65D4[];
extern R D_800F6684[], D_800F6694[], D_800F66A4[];
extern int D_800F7148;
extern int D_800F6A80[];
extern int D_800F6AC8;
typedef struct { short x, y, w, attr; int img, pC, str; void *p14; } Text;
extern Text D_800F70D8;
extern char D_800EDC7C[];
extern unsigned short GetClut(int x, int y);
extern void memCardDrawString(int a, int b);
extern void drawBalloonFrame(L *l, int c, int tp);

void memCardDrawMessage(int n)
{
    TILE *T = (TILE *)0x1F800000;
    TILE *p;
    L l;
    R *e = &D_800F65D4[n];
    short x, y;
    x = 0x40;
    y = e->y - (short)(e->h / 2 - 0x80);
    D_800F70D8.x = 0x48;
    D_800F70D8.w = 4;
    D_800F70D8.y = y + 8;
    D_800F70D8.attr = GetClut(0x150, 0x1F0);
    D_800F70D8.str = D_800F6A80[n];
    D_800F70D8.p14 = D_800EDC7C;
    memCardDrawString(1, 1);
    if (n == 7 || n == 12 || n == 16) {
        T->len = 3;
        T->code = 0x60;
        T->g0 = 0xFF;
        T->r0 = 0;
        T->b0 = 0;
        T->code &= ~2;
        switch (n) {
        case 7:
            T->x0 = x + D_800F6684[D_800F7148].x;
            T->y0 = y + D_800F6684[D_800F7148].y;
            T->w = D_800F6684[D_800F7148].w;
            T->h = D_800F6684[D_800F7148].h;
            break;
        case 12:
            T->x0 = x + D_800F6694[D_800F7148].x;
            T->y0 = y + D_800F6694[D_800F7148].y;
            T->w = D_800F6694[D_800F7148].w;
            T->h = D_800F6694[D_800F7148].h;
            break;
        case 16:
            T->x0 = x + D_800F66A4[D_800F7148].x;
            T->y0 = y + D_800F66A4[D_800F7148].y;
            T->w = D_800F66A4[D_800F7148].w;
            T->h = D_800F66A4[D_800F7148].h;
            break;
        }
        p = NEXT_PRIM;
        *p = *T;
        addPrim(CURRENT_OT + 1, p);
        NEXT_PRIM++;
    }
    l.m = -1;
    l.a = x;
    l.b = y;
    l.c = *(unsigned short *)0x1F8003D4 + 0x10;
    l.d = *(unsigned short *)0x1F8003D6 + 0x10;
    drawBalloonFrame(&l, 0, 0x14);
    D_800F70D8.x = 0x50;
    D_800F70D8.y = 0xD4;
    D_800F70D8.w = 4;
    D_800F70D8.attr = GetClut(0x150, 0x1F0);
    D_800F70D8.p14 = D_800EDC7C;
    D_800F70D8.str = D_800F6AC8;
    memCardDrawString(1, 2);
}
