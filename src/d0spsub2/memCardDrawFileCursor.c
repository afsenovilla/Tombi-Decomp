// FUNC 800eaf3c 432 D0SPSUB2
// MATCHING 800eaf3c 432
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
typedef struct { short x, y, u, v, w, h, p[2]; } Part;
#define setaddr(p, _addr) (((P_TAG *)(p))->addr = (unsigned long)(_addr))
#define getaddr(p) (unsigned long)(((P_TAG *)(p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)
extern SPRT *D_1F800164;
extern unsigned long *D_1F8001E0;
extern short D_800F70DC;
extern Part D_800F6CE8[];
extern unsigned short GetClut(int x, int y);
extern void AddDrawMode(int a, int b);

void memCardDrawFileCursor(int pri, short x, short y)
{
    SPRT *S = (SPRT *)0x1F800000;
    SPRT *p;
    Part *s = D_800F6CE8;
    do {
        S->code = 0x65;
        S->len = 4;
        S->r0 = 0x80;
        S->g0 = 0x80;
        S->b0 = 0x80;
        S->code &= ~2;
        S->x0 = x + s->x;
        S->y0 = y + s->y;
        S->u0 = s->u;
        S->v0 = s->v;
        S->w = s->w;
        S->h = s->h;
        s++;
        S->clut = GetClut(0x130, 0x1F2);
        p = D_1F800164;
        *p = *S;
        addPrim(D_1F8001E0 + pri, p);
        D_1F800164++;
    } while (s->x != -1);
    AddDrawMode(D_800F70DC, pri);
}
