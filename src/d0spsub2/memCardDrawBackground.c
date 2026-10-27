// FUNC 800ea160 540 D0SPSUB2
// MATCHING 800ea160 540
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
typedef struct { unsigned addr: 24; unsigned len: 8; unsigned long code[2]; } DR_MODE;
typedef struct { short x, y, u, v, w, h, pri, pad; } Part;
#define setaddr(p, _addr) (((P_TAG *)(p))->addr = (unsigned long)(_addr))
#define getaddr(p) (unsigned long)(((P_TAG *)(p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)
extern char *NEXT_PRIM;
extern unsigned long *CURRENT_OT;
extern Part MEMCARD_BACKGROUND_PARTS[];
extern unsigned short GetClut(int x, int y);
extern unsigned short GetTPage(int tp, int abr, int x, int y);
extern void SetDrawMode(DR_MODE *p, int dfe, int dtd, int tpage, void *tw);

void memCardDrawBackground(void)
{
    SPRT *S = (SPRT *)0x1F800000;
    SPRT *p;
    DR_MODE *m;
    Part *s = MEMCARD_BACKGROUND_PARTS;
    do {
        S->len = 4;
        S->code = 0x65;
        S->r0 = 0x80;
        S->g0 = 0x80;
        S->b0 = 0x80;
        S->code &= ~2;
        S->x0 = s->x;
        S->y0 = s->y;
        S->u0 = s->u;
        S->v0 = s->v;
        S->w = s->w;
        S->h = s->h;
        S->clut = GetClut(0x80, 0x1FF);
        p = (SPRT *)NEXT_PRIM;
        *p = *S;
        addPrim(CURRENT_OT + 4 + s->pri, p);
        m = (DR_MODE *)(NEXT_PRIM += sizeof(SPRT));
        SetDrawMode(m, 0, 0, GetTPage(1, 0, 0x80, 0), 0);
        addPrim(CURRENT_OT + 4 + s->pri, m);
        NEXT_PRIM += sizeof(DR_MODE);
        s++;
    } while (s->x != -1);
}
