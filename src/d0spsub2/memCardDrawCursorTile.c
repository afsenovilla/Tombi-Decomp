// FUNC 800eaba4 236 D0SPSUB2
// MATCHING 800eaba4 236
typedef struct { unsigned addr: 24; unsigned len: 8; } P_TAG;
typedef struct {
    unsigned addr: 24;
    unsigned len: 8;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    short w, h;
} TILE;
#define setaddr(p, _addr) (((P_TAG *)(p))->addr = (unsigned long)(_addr))
#define getaddr(p) (unsigned long)(((P_TAG *)(p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)
#define S ((TILE *)0x1F800000)
extern TILE *D_1F800164;
extern unsigned long *D_1F8001E0;

void memCardDrawCursorTile(short x, short y)
{
    TILE *p;
    S->x0 = x;
    S->len = 3;
    S->code = 0x60;
    S->g0 = 0xFF;
    S->w = 0x30;
    S->r0 = 0;
    S->b0 = 0;
    S->code &= ~2;
    S->y0 = y;
    S->h = 0x10;
    p = D_1F800164;
    *p = *S;
    addPrim(D_1F8001E0 + 1, p);
    D_1F800164++;
}
