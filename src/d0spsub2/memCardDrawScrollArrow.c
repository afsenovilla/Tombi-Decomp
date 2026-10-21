// FUNC 800ec5a4 472 D0SPSUB2
// MATCHING 800ec5a4 472
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
typedef struct { unsigned long tag; unsigned long code[2]; } DR_MODE;
typedef struct { unsigned short u, v; } UV;
#define setaddr(p, _addr) (((P_TAG *)(p))->addr = (unsigned long)(_addr))
#define getaddr(p) (unsigned long)(((P_TAG *)(p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)
extern SPRT *D_1F800164;
extern unsigned long *D_1F8001E0;
extern UV D_800F7040[], D_800F7042[];
extern unsigned short GetClut(int x, int y);
extern unsigned short GetTPage(int tp, int abr, int x, int y);
extern void SetDrawMode(DR_MODE *p, int dfe, int dtd, int tpage, void *tw);

void memCardDrawScrollArrow(int n, short x, short y)
{
    SPRT *S = (SPRT *)0x1F800000;
    SPRT *p;
    DR_MODE *d;
    S->len = 4;
    S->code = 0x64;
    S->code |= 1;
    S->r0 = 0x80;
    S->g0 = 0x80;
    S->b0 = 0x80;
    S->code &= ~2;
    S->x0 = x;
    S->y0 = y;
    S->u0 = D_800F7040[n].u;
    S->v0 = D_800F7042[n].u;
    S->w = 0x10;
    S->h = 0x10;
    S->clut = GetClut(0x130, 0x1F0);
    p = D_1F800164;
    *p = *S;
    addPrim(D_1F8001E0 + 2, p);
    D_1F800164++;
    d = (DR_MODE *)D_1F800164;
    SetDrawMode(d, 0, 0, GetTPage(0, 0, 0x40, 0), 0);
    addPrim(D_1F8001E0 + 2, d);
    D_1F800164 = (SPRT *)((DR_MODE *)D_1F800164 + 1);
}
