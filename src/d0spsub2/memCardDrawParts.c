// FUNC 800eaa40 356 D0SPSUB2
// MATCHING 800eaa40 356
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
#define setaddr(p, _addr) (((P_TAG *)(p))->addr = (unsigned long)(_addr))
#define getaddr(p) (unsigned long)(((P_TAG *)(p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)
extern SPRT *D_1F800164;
extern unsigned long *D_1F8001E0;
extern unsigned short GetClut(int x, int y);

void memCardDrawParts(Part *s, short x, short y)
{
    SPRT *p;
    do {
        p = D_1F800164;
        p->code = 0x64;
        p->len = 4;
        p->r0 = 0x80;
        p->g0 = 0x80;
        p->b0 = 0x80;
        p->code &= ~2;
        p->x0 = x + s->x;
        p->y0 = y + s->y;
        p->u0 = s->u;
        p->v0 = s->v;
        p->w = s->w;
        p->h = s->h;
        p->clut = GetClut(s->cx, s->cy);
        s++;
        addPrim(D_1F8001E0 + 1, p);
        D_1F800164++;
    } while (s->x != -1);
}
