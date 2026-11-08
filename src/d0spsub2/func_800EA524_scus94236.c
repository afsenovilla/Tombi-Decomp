// VERSION SCUS_94236 dspsub2 800EA524 404
#if defined(VERSION_SCUS_94236)
// USA version of PAL d0spsub2 800eaa40 (memCardDrawParts.c)
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
extern SPRT *NEXT_PRIM;
extern unsigned long *CURRENT_OT;
extern unsigned short GetClut(int x, int y);

void memCardDrawParts(Part *s, short x, short y)
{
    SPRT *q = (SPRT *)0x1F800000;
    SPRT *p;
    do {
        q->code = 0x64;
        q->len = 4;
        q->r0 = 0x80;
        q->g0 = 0x80;
        q->b0 = 0x80;
        q->code &= ~2;
        q->x0 = x + s->x;
        q->y0 = y + s->y;
        q->u0 = s->u;
        q->v0 = s->v;
        q->w = s->w;
        q->h = s->h;
        q->clut = GetClut(s->cx, s->cy);
        s++;
        p = NEXT_PRIM;
        *p = *q;
        addPrim(CURRENT_OT + 1, p);
        NEXT_PRIM++;
    } while (s->x != -1);
}
#endif
